package org.coinor.opents;


/**
 * This version of the {@link TabuSearch} can distribute work among remote computers
 * (along with doing some work on its own). Like {@link MultiThreadedTabuSearch}, 
 * DistributedTabuSearch divides the neighborhood up among computers by distributing
 * the {@link Move}s that are returned by {@link MoveManager}. The quantity of work that
 * each computer gets is dynamic, changing according to how quickly the computer 
 * performed the previous iteration's work. It's possible that with slow computers or a
 * slow network, the work will entirely fall back on the local machine.
 * <p/>
 * <em><b>This is the recommended {@link TabuSearch}</b></em> for nearly all well-threaded
 * applications because it does not block on the {@link #startSolving} method. When done
 * solving a given set of iterations, the thread will die, freeing up your computer's resources.
 * If you add iterations with {@link SingleThreadedTabuSearch#setIterationsToGo setIterationsToGo()} and call 
 * {@link #startSolving} again,vthe thread(s) will restart.
 *
 *
 * <p><em>This code is licensed for public use under the Common Public License version 0.5.</em><br/>
 * The Common Public License, developed by IBM and modeled after their industry-friendly IBM Public License,
 * differs from other common open source licenses in several important ways:
 * <ul>
 *  <li>You may include this software with other software that uses a different (even non-open source) license.</li>
 *  <li>You may use this software to make for-profit software.</li>
 *  <li>Your patent rights, should you generate patents, are protected.</li>
 * </ul>
 * </p>
 * <p><em>Copyright © 2006 Robert Harder</em></p>
 *
 * @author  Robert Harder
 * @author  rob@iharder.net
 * @copyright 2006 Robert Harder
 * @version 1.1
 * @since 1.1
 */
public class DistributedTabuSearch extends MultiThreadedTabuSearch
{
    
    /**
	 * Identifier for serialization. Change this value whenever
	 * the structure significantly changes.
	 */
	private static final long serialVersionUID = -8891521226930084779L;
	
	
	private DistributedHelper[] distHelpers = {};
    private boolean destroyDiscoverer  = false;
    
    
    public DistributedTabuSearch() 
    {
        super();
        DistributedHelper[] distHelpers= new DistributedHelper[1];
        distHelpers[0] = new DistributedHelper();
        distHelpers[0].start();
        
        startDiscovery();
    }   // end empty constructor
    
    
    
    /**
     * Constructs a <tt>DistributedThreadedTabuSearch</tt> with all tabu objects set.
     * The initial solution is evaluated with the objective function,
     * becomes the <tt>currentSolution</tt> and a copy becomes the <tt>bestSolution</tt>.
     *
     * @param initialSolution The initial <tt>currentSolution</tt>
     * @param moveManager The move manager
     * @param objectiveFunction The objective function
     * @param tabuList The tabu list
     * @param aspirationCriteria The aspiration criteria or <tt>null</tt> if none is to be used
     * @param maximizing Whether or not the tabu search should be maximizing the objective function
     *
     * @see Solution
     * @see ObjectiveFunction
     * @see MoveManager
     * @see TabuList
     * @see AspirationCriteria
     *
     * @since 1.1
     */
    public DistributedTabuSearch(
        Solution initialSolution,
        MoveManager moveManager,
        ObjectiveFunction objectiveFunction,
        TabuList tabuList,
        AspirationCriteria aspirationCriteria,
        boolean maximizing)
    {
        super( initialSolution, moveManager, objectiveFunction, tabuList, aspirationCriteria, maximizing);
        System.out.println( "Full constructor");
        DistributedHelper[] distHelpers= new DistributedHelper[1];
        distHelpers[0] = new DistributedHelper();
        distHelpers[0].start();
        
        startDiscovery();
    }   // end constructor
    
    
    
    
    
    public void addRemoteHelper( String ipAddress, int port )
    {
        System.out.println( "Adding remote helper at " + ipAddress + ":" + port );
    }   // end addRemoteHelper
    
    
    
    private void startDiscovery()
    {
        destroyDiscoverer = false;
        Thread t = new Thread( new Runnable() {   
            public void run()
            {
                while( !destroyDiscoverer )
                {
                    // Look for new helpers on the network
                    System.out.println("Pretending to find remote helper...");
                    addRemoteHelper( "127.0.0.1", 5815 ); // IP, Port


                    try{ Thread.sleep(10000); } catch(InterruptedException ex ){}


                }   // end while
                // Okay, we're dying now.
            }   // end run
        }, "Discovery" );
        //t.start();
        
    }   // end startDiscovery
    
    
    
    
    
    /**
     * Gets the best move--one that should be used for this iteration.
     * The work load is split up among as many threads as the user requests.
     * The default is 1.
     * By setting <var>chooseFirstImprovingMove</var> to <tt>true</tt>
     * you tell the tabu search to return the first move it encounters
     * that is improving and non-tabu rather than search through all of
     * the moves. This is actually implemented by having each thread
     * return the first improving move it encounters and then choosing
     * the best among these moves.
     *
     * @since 1.0
     */
    protected Object[] getBestMove( 
    final Solution soln, 
    final Move[] moves, 
    final ObjectiveFunction objectiveFunction, 
    final TabuList tabuList, 
    final AspirationCriteria aspirationCriteria, 
    final boolean maximizing, 
    final boolean chooseFirstImprovingMove )
    {      
        // If there are no helpers, use regular getBestMove routine.
        if( this.distHelpers.length == 0 )
        {   
            System.out.println( "Fallback to super.getBestMove");
            return super.getBestMove( soln, moves, objectiveFunction,
                tabuList, aspirationCriteria, maximizing, chooseFirstImprovingMove, this );
        }   // end if: one thread requested
        
        // Else use as many threads as was requested.
        else
        {
            System.out.println( "Using helpers with " + distHelpers.length + " helpers.");
            // Set up variables
            Move      bestMove     = null;
            double[]  bestMoveVal  = null;
            boolean   bestMoveTabu = false;
            
            // Divide up the moves according to past performance of helpers
            // While developing, I'm just evenly dividing them up.'
            int numGroups = distHelpers.length;
            int nominalSize = moves.length / numGroups;
            Move[][] moveGroups= new Move[ numGroups ][];
            
            // Assign equal number to all but last group
            for( int i = 0; i < numGroups-1; i++ )
            {   
                moveGroups[i] = new Move[ nominalSize ];
                System.arraycopy( moves, i*nominalSize, 
                    moveGroups[i], 0, nominalSize );
            }   // end for: group except last
            
            // Last group gets at least as many as the first groups
            // but may also have some leftovers
            moveGroups[ numGroups-1 ] = new Move[ nominalSize + (moves.length % numGroups) ];
            System.arraycopy( moves, (numGroups-1)*nominalSize, 
                moveGroups[numGroups-1], 0, moveGroups[numGroups-1].length );
            
            // Hand out moves to the threads.
            for( int i = 0; i < numGroups; i++ )
                distHelpers[i].setWork( soln, moveGroups[i], objectiveFunction, 
                    tabuList, aspirationCriteria, maximizing, chooseFirstImprovingMove, this );

            // Wait for threads to finish neighborhood when helpers[i].getBestMove
            // is called. That is, the method blocks until the work is done.
            // As a thread finishes, immediately compare it with the existing
            // best known move.
            Object[][] bestMoves = new Object[ numGroups ][];
            for( int i = 0; i < numGroups; i++ )
            {
                // Get helper thread's best move data
                Object[] contender = distHelpers[i].getBestMove();
                
                // Make quick access variables to contender.
                Move newMove = (Move) contender[0];
                double[] newObjVal = (double[]) contender[1];
                boolean newMoveTabu = ((Boolean)contender[2]).booleanValue();
                
                // If we don't already have a best move, then this is the
                // best by default.
                if( bestMove == null )
                {
                    bestMove = newMove;
                    bestMoveVal = newObjVal;
                    bestMoveTabu = newMoveTabu;
                }   // end if: no existing best move
                
                // Else check to see if the contender is better than the existing best
                else
                {
                    if( isFirstBetterThanSecond( newObjVal, bestMoveVal, maximizing ) )
                    {   
                        // New one has a better objective value.

                        // Compare tabu status. The following, messy 'if' statement says
                        // Continue under the following conditions:
                        //  o  The new move is not tabu, or
                        //  o  The new move is tabu, but so is the old best move
                        if( !(!bestMoveTabu && newMoveTabu) )
                        {   bestMove = newMove;
                            bestMoveVal = newObjVal;
                            bestMoveTabu = newMoveTabu;
                        }   // end if: switch over
                    }   // end if: new one has better objective value
                    else
                    {   // New one does not have better objective value, but see if it
                        // has a better tabu status.
                        if( bestMoveTabu && newMoveTabu )
                        {   bestMove = newMove;
                            bestMoveVal = newObjVal;
                            bestMoveTabu = newMoveTabu; // <-- This should be false at this point.
                        }   // end if: old was tabu, new one isn't.
                    }   // end else: new one does not have better objective value
                    
                }   // end else
                
            }   // end for: each thread

            return new Object[]{ bestMove, bestMoveVal, new Boolean( bestMoveTabu) };
        }   // end else: multiple threads
    }   // end getBestMove
    
    
    
    
/* ********  D I S C O V E R E R  ******** */
    
    
    private static class Discoverer extends Thread
    {
        
        /** Kills thread when the thread is no longer needed. */
        private boolean dispose = false;
        
        /**
         * Constructs a <tt>NeighborhoodHelper</tt> and starts its thread.
         * 
         * @since 1.0
         */
        private Discoverer()
        {   
            super( "Discoverer" );
            //start();
        }   // end constructor
        
        
        /** Mark helper for destruction. Thread will complete and die gracefully. */
        protected synchronized void dispose()
        {   
            this.dispose = true;
            this.notifyAll();
        }   // end dispose
    }   // end class Discoverer
    
    
/* ********  R E M O T E   H E L P E R  ******** */
    
    
    private static class DistributedHelper extends Thread
    {
        /** Kills thread when the thread is no longer needed. */
        private boolean dispose = false;
        
        /** Tells helper that there's work waiting to be done. */
        private boolean workToDo = false;
        
        /** Stores the results of the last work that was done. */
        private Object[] bestMove;
      
        private String ipAddress;
        private int    port;
        private boolean isLocal;
        
        private static int instanceNum = 0;
      
        
        
        // The tabu search objects that the helper uses.
        private Solution            soln;
        private Move[]              moves;
        private ObjectiveFunction   objectiveFunction;
        private TabuList            tabuList;
        private AspirationCriteria  aspirationCriteria;
        private boolean             maximizing; 
        private boolean             chooseFirstImprovingMove;
        private TabuSearch          tabuSearch;
        
        
        /**
         * Empty constructor indicates this will run on the local machine.
         */
        private DistributedHelper()
        {
            super( "Distributed-Helper-" + (instanceNum++));
            isLocal = true;
        }   // end RemoteHelper
        
        
        private DistributedHelper( String ipAddress, int port )
        {
        
            super( "Distributed-Helper-" + (instanceNum++));
            isLocal = false;
        }   // end constructor
    
    
        
        /** Set the tabu objects that are to do work. */
        protected synchronized void setWork(
            final Solution soln, 
            final Move[] moves, 
            final ObjectiveFunction objectiveFunction, 
            final TabuList tabuList, 
            final AspirationCriteria aspirationCriteria, 
            final boolean maximizing, 
            final boolean chooseFirstImprovingMove,
            final TabuSearch tabuSearch )
        {         
            // Store the tabu objects
            this.soln                       = soln;
            this.moves                      = moves;
            this.objectiveFunction          = objectiveFunction;
            this.tabuList                   = tabuList;
            this.aspirationCriteria         = aspirationCriteria;
            this.maximizing                 = maximizing;
            this.chooseFirstImprovingMove   = chooseFirstImprovingMove;
            this.tabuSearch                 = tabuSearch;
            
            // Turn on the work flag
            workToDo = true;
            bestMove = null;
            
            // Wake up self.
            this.notifyAll();
            
        }   // end setWork
        
    
        /**
         * Returns the results of the last work that was done. 
         * If work is still happening, then block until the work is done.
         */
        protected synchronized Object[] getBestMove()
        {   
            while( workToDo )
                synchronized( this )
                {   
                    try{ this.wait(); }
                    catch( java.lang.InterruptedException e )
                    {   
                        e.printStackTrace( err );
                    }   // end catch
                }   // end sync
                
            return bestMove;
        }   // end getBestMove
        
    
        
    
        /** 
         * Keeps running until the tabu search determines it's time to quit
         * and free up resources.
         */
        public void run()
        {   
            while( !dispose )
            {
                // If there's nothing to do, just wait.
                if( !workToDo )
                {
                    synchronized( this )
                    {   try{ this.wait(); }
                        catch( java.lang.InterruptedException e )
                        {   e.printStackTrace( err );
                        }   // end catch
                    }   // end sync
                }   // end if: no work to do
                
                // Else there's work to do
                else
                {   
                    synchronized( this )
                    {   
                        if(isLocal)
                        {
                        // Use the "getBestMove" method in the simpler SingleThreadedTabuSearch
                        // class and pretend like we're just finding a "smalller" neighborhood.
                        bestMove = SingleThreadedTabuSearch.getBestMove( 
                            soln, 
                            moves, 
                            objectiveFunction, 
                            tabuList, 
                            aspirationCriteria, 
                            maximizing, 
                            chooseFirstImprovingMove, 
                            tabuSearch );
                        workToDo = false;
                        }   // end if: local
                        else
                        {
                            
                        }   // end else: remote
                        this.notifyAll();
                    }   // end sync
                }   // end else: work to do
            }   // end while
            // Okay, we're dying now.
        }   // end run
    
    }   // end class DistributedHelper
        
}   // end class DistributedTabuSearch
