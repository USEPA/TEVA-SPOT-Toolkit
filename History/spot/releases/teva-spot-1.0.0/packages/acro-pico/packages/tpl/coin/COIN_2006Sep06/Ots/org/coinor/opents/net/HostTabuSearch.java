package org.coinor.opents.net;

import org.coinor.opents.*;
import java.rmi.RemoteException;
import java.util.*;

public class HostTabuSearch extends MultiThreadedTabuSearch 
{
    private ClientHelper[] clients = {};
    
    private Move lastBestMove;
    
    private double[] myMovesPerSecond = {};
    
    
/* ********  C O N S T R U C T O R S  ******** */
    
    
    /**
     * Constructs a <tt>HostTabuSearch</tt> with no tabu objects set.
     *
     * @since 1.0
     */
    public HostTabuSearch()
    {
        super();
    }   // end constructor
    
    
    
    /**
     * Constructs a <tt>MultiThreadedTabuSearch</tt> with all tabu objects set.
     * The initial solution is evaluated with the objective function,
     * becomes the <tt>currentSolution</tt> 
     * and a copy becomes the <tt>bestSolution</tt>.
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
     * @since 1.0
     */
    public HostTabuSearch(
    Solution initialSolution,
    MoveManager moveManager,
    ObjectiveFunction objectiveFunction,
    TabuList tabuList,
    AspirationCriteria aspirationCriteria,
    boolean maximizing)
    {
        super( initialSolution, moveManager, objectiveFunction, tabuList, aspirationCriteria, maximizing);
    }   // end constructor
    
    
    
    public void startSolving()
    {
        for( int i = 0; i < clients.length; i++ ){
            initClient( clients[i] );
        }   // end for
        super.startSolving();
    }   // end startSolving
    
    
    
    protected void performOneIteration() throws NoMovesGeneratedException, NoCurrentSolutionException
    {
        super.performOneIteration();
        
        // Some tabu objects must be forcibly updated in the clients.
        for( int i = 0; i < clients.length; i++ ){
            clients[i].setTabuList( getTabuList() );
        }   // end for: each client
    }   // end performOneIteration
    

    protected Object[] getBestMove( 
    final Solution soln, 
    final Move[] moves, 
    final ObjectiveFunction objectiveFunction, 
    final TabuList tabuList, 
    final AspirationCriteria aspirationCriteria, 
    final boolean maximizing, 
    final boolean chooseFirstImprovingMove )
    {
        // Simplest case: no clients
        if( clients.length == 0 ){
            Object[] bestMoveArr = super.getBestMove( soln, moves, objectiveFunction,
                tabuList, aspirationCriteria, maximizing, chooseFirstImprovingMove );
            lastBestMove = (Move)bestMoveArr[0];
            return bestMoveArr;
        }   // end if: no clients
        
        // Start divying up the work to each client and be sure
        // to save some work for this host computer, which would
        // otherwise be idle.
        // Eventually do load-balancing here, but for now, just split
        // up the workload evenly among the clients.
       
        // Set up variables
        Move      bestMove     = null;
        double[]  bestMoveVal  = null;
        boolean   bestMoveTabu = false;
        ClientHelper[] helpers = getHelpers();

        // Divide up the moves in equal-sized groups.
        int numHelpers     = helpers.length;
        int numGroups      = numHelpers + 1; // Plus one for local machine
        int nominalSize    = moves.length / numGroups;
        Move[][] moveGroups= new Move[ numGroups ][];

        // Assign equal number to all but last group
        for( int i = 0; i < numHelpers; i++ )
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

        // Hand out moves to the helpers.
        for( int i = 0; i < numHelpers; i++ )
            helpers[i].setMoves( moveGroups[i] );
        
        // Do local machine's part and store it.
        Object[] localBest = super.getBestMove( soln, moveGroups[numGroups-1], objectiveFunction,
                tabuList, aspirationCriteria, maximizing, chooseFirstImprovingMove );
        bestMove     = (Move)localBest[0];
        bestMoveVal  = (double[])localBest[1];
        bestMoveTabu = ((Boolean)localBest[2]).booleanValue();

        // Wait for threads to finish neighborhood when helpers[i].getBestMove
        // is called. That is, the method blocks until the work is done.
        // As a thread finishes, immediately compare it with the existing
        // best known move.
        for( int i = 0; i < numHelpers; i++ )
        {
            // Get helper thread's best move data
            Object[] contender = helpers[i].getBestMove();

            // Make quick access variables to contender.
            Move newMove        = (Move) contender[0];
            double[] newObjVal  = (double[]) contender[1];
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
            }   // end else: is contender better than existing best
        }   // end for: each helper
                    
        return new Object[]{ bestMove, bestMoveVal, new Boolean(bestMoveTabu) };
    }   // end getBestMove
    
    
    
    protected void initClient( ClientHelper client )
    {
        client.setObjectiveFunction( getObjectiveFunction() );
        client.setCurrentSolution( getCurrentSolution() );
        client.setBestSolution( getBestSolution() );
        client.setTabuList( getTabuList() );
        client.setAspirationCriteria( getAspirationCriteria() );
        client.setMaximizing( isMaximizing() );
        client.setChooseFirstImprovingMove( isChooseFirstImprovingMove() );
    }   // end initClient

    
    public synchronized void addClient( ClientTabuSearch client )
    {
        ClientHelper[] list = new ClientHelper[ clients.length + 1 ];
        for( int i = 0; i < list.length - 1; i++ ){
            list[i] = clients[i];
        }
            
        ClientHelper newClient = new ClientHelper( client );
        newClient.start();
        list[ list.length-1 ] = newClient;
        clients = list;
        
        // If client is being added in the middle of a search,
        // be sure to initialize it with relevant objects.
        if( isSolving() ){
            initClient( newClient );
        }   // end if: currently solving
    }   // end addClient
    
    
    public synchronized void removeClient( ClientTabuSearch client )
    {
        // Find location of client to remove
        int index = -1;
        int j = 0;
        while( index < 0 && j < clients.length ){
            if( clients[j].client == client )
                index = j;
            else j++;
        }   // end while: through list
        
        // If index is less than zero then it wasn't in the list
        if( index >= 0 ){
            clients[index].dispose();
            ClientHelper[] list = new ClientHelper[ clients.length - 1 ];
            for( int i = 0; i < list.length; i++ ){
                list[i] = clients[ i < index ? i : i + 1 ];
            }   // end for: each item
            clients = list;
        }   // end if: client was in the list
    }   // end removeClient

    protected ClientHelper[] getHelpers()
    {
        return clients;
    }   // end getHelpers
    
    
    
/* ******** O V E R R I D E   T A B U S E A R C H   M E T H O D S  ******** */    
    
    
    
    protected void internalSetBestSolution( Solution soln )
    {
        super.internalSetBestSolution( soln );
        for( int i = 0; i < clients.length; i++ ){
            clients[i].setBestSolution( soln );
        }   // end for
    }   // end internalSetBestSolution
    
    
    protected void internalSetCurrentSolution( Solution soln )
    {
        super.internalSetCurrentSolution( soln );
        for( int i = 0; i < clients.length; i++ ){
                clients[i].setCurrentSolution( soln );
        }   // end for
    }   // end internalSetCurrentSolution
    
    
    public synchronized void setTabuList( TabuList tabuList )
    {
        super.setTabuList( tabuList );
        for( int i = 0; i < clients.length; i++ ){
                clients[i].setTabuList( tabuList );
        }   // end for
    }   // end setTabuList
    
    
    
    
    public synchronized void setObjectiveFunction( ObjectiveFunction func )
    {
        super.setObjectiveFunction( func );
        for( int i = 0; i < clients.length; i++ ){
            clients[i].setObjectiveFunction( func );
        }   // end for
    }   // end setObjectiveFunction
    
    
    public synchronized void setAspirationCriteria( AspirationCriteria asp )
    {
        super.setAspirationCriteria( asp );
        for( int i = 0; i < clients.length; i++ ){
            clients[i].setAspirationCriteria( asp );
        }   // end for
    }   // end setAspirationCriteria
    
    
    public synchronized void setMaximizing( boolean maximizing )
    {
        super.setMaximizing( maximizing );
        for( int i = 0; i < clients.length; i++ ){
            clients[i].setMaximizing( maximizing );
        }   // end for
    }   // end setMaximizing
    
    
    public synchronized void setChooseFirstImprovingMove( boolean choose )
    {
        super.setChooseFirstImprovingMove( choose );
        for( int i = 0; i < clients.length; i++ ){
            clients[i].setChooseFirstImprovingMove( choose );
        }   // end for
    }   // end setChooseFirstImprovingMove
    
    
    
/* ******** I N N E R   C L A S S  ******** */    
    
    
    
    
    protected static class ClientHelper extends java.lang.Thread
    {
        /** Kills thread when the thread is no longer needed. */
        private boolean dispose = false;
        
        /** Stores the results of the last work that was done. */
        private Object[] bestMove;
        
        private ClientTabuSearch client;
        
        private static int instanceNum = 0;
                
        private double[] movesPerSecond = {};
        
        private int numMovesLast;
        
        
        private java.util.List queue = new java.util.LinkedList();
        
        /**
         * Constructs a <tt>ClientHelper</tt> and starts its thread.
         * 
         * @since 1.0
         */
        private ClientHelper( ClientTabuSearch client )
        {   
            super( "ClientHelper-" + (instanceNum++));
            this.client = client;
        }   // end constructor
        
        
        /**
         * Loops through the queue running the objects it finds there
         * which, by design, are all of type Runnable.
         */
        public void run()
        {
            while( !dispose ){
                Object queuedObject = null;
                synchronized( queue ){
                    if( !queue.isEmpty() ){
                        queuedObject = queue.remove(0);
                        queue.notifyAll();
                    }   // end if: queue is not empty
                    else{
                        try{
                            queue.wait();
                        }   // end try
                        catch( InterruptedException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end else: not empty
                }   // end sync: queue
                
                if( queuedObject != null ){
                    ((Runnable)queuedObject).run();
                }   // end if: got something to do
            }   // end while: not dispose            
        }   // end run
        
        
        
        /**
         * Returns the results of the last work that was done. 
         * If work is still happening, then block until the work is done.
         */
        private synchronized Object[] getBestMove()
        {    
            while( bestMove == null ){
                    try{ 
                        this.wait(); 
                    }   // end try
                    catch( java.lang.InterruptedException e ){
                        e.printStackTrace();
                    }   // end catch
            }   // end while
                
            return bestMove;
        }   // end getBestMove
         
        
        
        /** Mark helper for destruction. Thread will complete and die gracefully. */
        private void dispose()
        {   
            synchronized( queue ){
                this.dispose = true;
                queue.notifyAll();
            }   // end sync
        }   // end dispose
        
        
        private void flushQueue()
        {
            synchronized( queue ){
                queue.clear();
                queue.notifyAll();
            }   // end sync: queue
        }   // end flushQueue
        
        
        private void setCurrentSolution( final Solution soln )
        {
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            client.setCurrentSolutionRemote( soln );
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setCurrentSolution
        
        
        private void setBestSolution( final Solution soln )
        {
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            client.setBestSolutionRemote( soln );
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setBestSolution
        
        
        private void setTabuList( final TabuList tabuList )
        {
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            client.setTabuListRemote( tabuList );
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setTabuList
        
        
        private void setObjectiveFunction( final ObjectiveFunction func )
        {
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            client.setObjectiveFunctionRemote( func );
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setObjectiveFunction
        
        
        private void setAspirationCriteria( final AspirationCriteria asp )
        {
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            client.setAspirationCriteriaRemote( asp );
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setAspirationCriteria
        
        
        private void setMaximizing( final boolean maximizing )
        {
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            client.setMaximizingRemote( maximizing );
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setMaximizing
        
        
        private void setChooseFirstImprovingMove( final boolean choose )
        {
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            client.setChooseFirstImprovingMoveRemote( choose );
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setChooseFirstImprovingMove
        
        
        private void setMoves( final Move[] moves )
        {
            setBestMove( null );
            this.numMovesLast = moves.length;
            final ClientHelper This = this;
            synchronized( queue ){
                queue.add( new Runnable(){
                    public void run(){
                        try{
                            // Get best move and time it.
                            long start = System.currentTimeMillis();
                            Object[] bestMove = client.getBestMoveRemote( moves );
                            double rate = 1000.0 * moves.length / (System.currentTimeMillis() - start);
                            addMovesPerSecondData( rate );
                            
                            // Record best move and notify listeners.
                            setBestMove( bestMove );
                            
                        }   // end try
                        catch( RemoteException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                queue.notifyAll();
            }   // end sync: queue
        }   // end setMoves
        
        private synchronized void setBestMove( Object[] bestMove )
        {
            this.bestMove = bestMove;
            notifyAll();
        }   // end setBestMove
        

        private synchronized void addMovesPerSecondData( double rate )
        {
            if( rate < 0.0 || rate > Double.MAX_VALUE )
                return;

            int maxData = 3;
            if( movesPerSecond.length < maxData )
            {
                double[] temp = new double[ movesPerSecond.length + 1 ];
                for( int i = 0; i < movesPerSecond.length; i++ ){
                    temp[i] = movesPerSecond[i];
                }   // end for
                temp[ temp.length - 1 ] = rate;
                movesPerSecond = temp;
            }   // end if: need to add elements
            else
            {
                for( int i = 0; i < movesPerSecond.length-1; i++ ){
                    movesPerSecond[i] = movesPerSecond[i+1];
                }   // end for
                movesPerSecond[movesPerSecond.length - 1] = rate;
            }   // end else: all elements present
        }   // end addMovesPerSecondData

        

        protected synchronized double getMovesPerSecond()
        {
            double rateSum = 0.0;

            if( movesPerSecond.length == 0 )
                return 0.0;

            for( int i = 0; i < movesPerSecond.length; i++ ){
                rateSum += movesPerSecond[i];
            }   // end for
            return rateSum / movesPerSecond.length;
        }   // end getMovesPerSecond
        
        
        protected synchronized int getLastNumberOfMoves()
        {   
            return numMovesLast;
        }   // end getLastNumberOfMoves
        
        protected synchronized ClientTabuSearch getClientTabuSearch()
        {
            return client;
        }   // end getClientTabuSearch
        
        
    }   // end inner class ClientHelper
    
}   // end class HostTabuSearch
