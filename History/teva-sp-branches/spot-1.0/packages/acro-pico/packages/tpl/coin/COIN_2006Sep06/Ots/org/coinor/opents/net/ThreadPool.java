package org.coinor.opents.net;


/**
 * This class provides a resizable pool of reusable threads.
 * This can be useful when threads are a limited resource, or
 * if you wish to limit the number of working threads to the number
 * of processors on a computer.
 * <p/>
 * To use this class, make a new <tt>ThreadPool</tt> and add jobs
 * to the queue by calling the <tt>run</tt> method. Here is an example:
 * <p/>
 * <code><pre>
 *     ThreadPool pool = new ThreadPool();
 *     pool.run( new Runnable()
 *     {   public void run()
 *         {   // Your work here
 *             // ...
 *         }   // end run
 *     }); // end Runnable
 * </pre></code>
 *
 * @author Robert Harder
 * @author rharder@usa.net
 * @version 1.0
 */
public class ThreadPool
{
    private Queue jobQueue      = new Queue();
    private Queue runningJobs   = new Queue();
    
    private java.util.List threadList = new java.util.ArrayList();

    public static final int MAX_THREADS = 100;
    
    private boolean growable = true;
    
    /**
     * Constructs a new <tt>ThreadPool</tt> with two threads.
     */
    public ThreadPool()
    {   
        this( 2 );
    }   // end constructor
    
    /**
     * Constructs a new <tt>ThreadPool</tt> with a given number of threads.
     * @param threads The number of threads in the <tt>ThreadPool</tt>.
     */
    public ThreadPool( int threads )
    {   
        for( int i = 0; i < threads; i++ ){
            addThread();
        }   // end for: each thread
    }   // end constructor

    /**
     * Add a job to the queue.
     * @param r The <tt>Runnable</tt> that will execute with the first available thread.
     */
    public void run( Runnable r )
    {   
        jobQueue.put( r );
        
        if( isGrowable() && jobQueue.size() > 0 && getThreads() < MAX_THREADS )
            addThread();
    }   // end run

    /**
     * Tries to withdraw the job from the queue. If the job is already being worked on,
     * then the job is left alone and <tt>false</tt> is returned.
     * @param r The <tt>Runnable</tt> job to return.
     * @return <tt>true</tt> if the job was withdrawn before work began, <tt>false</tt> otherwise.
     */
    public boolean withdraw( Runnable r )
    {   
        return jobQueue.remove( r );
    }   // end run
    
    
    /**
     * Blocks and does not return control until the queue is empty. This does not
     * guarantee that all jobs are finished executing.
     */
    public void waitUntilEmpty()
    {   
        jobQueue.waitUntilEmpty();
    }   // end waitToFinish

    /**
     * Blocks and does not return control until all jobs are finished.
     * This can be dangerous since it does not prevent other threads from
     * adding jobs <em>ad nauseum</em>, never releasing this calling thread.
     */
    public void waitToFinish()
    {   
        synchronized( jobQueue ){   
            jobQueue.waitUntilEmpty();
            runningJobs.waitUntilEmpty();
        }   // end sync: jobQueue
    }   // end waitToFinish

    
    /**
     * Set the number of threads that the <tt>ThreadPool</tt> utilizes. This
     * can be changed "on the fly." If reducing the number of threads, it will
     * remove an idle thread or an arbitrary thread if all threads are working.
     * @param threads The number of threads in the <tt>ThreadPool</tt>.
     */
    public synchronized void setThreads( int threads )
    {
        if( threads > threadList.size() )
            for( int i = threadList.size(); i <= threads; i++ )
               addThread();
        else if( threads < threadList.size() )
            for( int i = threads; threads < threadList.size(); i++ )
               removeThread();
    }   // end setThreads
    
    
    /**
     * Returns the number of threads in the <tt>ThreadPool</tt>.
     * @return The number of threads in the <tt>ThreadPool</tt>.
     */
    public synchronized int getThreads()
    {   
        return threadList.size();
    }   // end getThreads
    
    
    public synchronized boolean isGrowable()
    {
        return growable;
    }   // end isGrowable
    
    
    public synchronized void setGrowable( boolean state )
    {
        growable = state;
        notifyAll();
    }   // end setGrowable
    
    
    
    /**
     * Returns the number of jobs being worked on.
     * @ return the number of jobs being worked on.
     */
    public int getRunningJobs()
    {   
        return runningJobs.size();
    }   // end countRunningJobs

    
    private synchronized void addThread()
    {   
        ThreadPoolThread t = new ThreadPoolThread();
        threadList.add( t );
        t.setDaemon( true ); // Don't keep alive after program should exit
        t.start();
        notifyAll();
    }   // end addThread

    
    private synchronized void removeThread()
    {   
        java.util.Iterator iterator = threadList.iterator();
    
        // Lock job queue too.
        synchronized( jobQueue ){   
            Object obj = null;
            while( obj == null && iterator.hasNext() ){   
                ThreadPoolThread t = (ThreadPoolThread)iterator.next();
                if( t.isIdle() ){   
                    t.setKeepWorking( false );
                    threadList.remove( t );
                    obj = t;
                }   // end if: idle thread
            }   // end while: each thread
        
            // If there were no idle threads, just kill the first one.
            if( obj == null ){   
                ThreadPoolThread t = (ThreadPoolThread)threadList.remove(0);
                t.setKeepWorking( false );
            }   // end if: no idle thread
            jobQueue.notifyAll();
        }   // end sync: jobQueue
        notifyAll();
    }   // end removeThread


    private Runnable getNextRunnable()
    {   
        return jobQueue.get();
    }   // end getNextRunnable

    
    private void putRunningJob( Runnable r )
    {   
        runningJobs.put( r );
    }   // end putRunningJob
    
    private void removeRunningJob( Runnable r )
    {   
        runningJobs.remove( r );
    }   // end removeRunningJob

    /**
     * Small test program that runs from the command line.
     */
    public static void main( String[] args )
    {
        ThreadPool pool = new ThreadPool( 3 );
        for( int i = 0; i < 10; i++ ){   
            final int number = i;
            pool.run( new Runnable(){   
                public void run(){   
                    for( int j = 0; j < 5; j++ ){   
                        System.out.print( new StringBuffer( " " ).append( number ).toString() );
                        try{ Thread.sleep( 10 ); } catch( InterruptedException e ){}
                    }   // end for: print number a few times
                }   // end run
            }); // end runnable
        }   // end for
        pool.setThreads(1);
        pool.waitToFinish();
        System.out.println( "Empty." );
    }   // end main



/* ********  I N N E R   C L A S S   Q U E U E  ******** */

    private class Queue
    {   
        private java.util.List list = new java.util.LinkedList();

        private synchronized void put( Runnable r )
        {   
            list.add( r );
            notifyAll();
        }   // end put

        private synchronized Runnable get()
        {   
            while( list.isEmpty() )
               try{ wait(); } catch( InterruptedException e ){}
            notifyAll();
            return (Runnable)list.remove(0);
        }   // end get

        private synchronized void waitUntilEmpty()
        {   
            while( !list.isEmpty() )
               try{ wait(); } catch( InterruptedException e ){}
        }   // end waitUntilEmpty
        
        private synchronized int size()
        {   
            return list.size();
        }   // end size
        
        private synchronized boolean remove( Runnable r )
        {   
            boolean success = list.remove( r );
            notifyAll();
            return success;
        }   // end remove

    }   // end inner class Queue


/* ********  I N N E R   C L A S S   T H R E A D   P O O L   T H R E A D  ******** */

    private class ThreadPoolThread extends Thread
    {
        private boolean keepWorking = true;
        private boolean idle = true;
        

        private synchronized boolean isIdle()
        {   
            return idle;
        }   // end isIdle
        
        private boolean keepWorking()
        {   
            return keepWorking;
        }   // end keepWorking

        private void setKeepWorking( boolean keepWorking )
        {   
            this.keepWorking = keepWorking;
        }   // end setKeepWorking

        public void run()
        {   
            while( keepWorking ){   
                Runnable r = null;
                synchronized( this ){   
                    idle = false;
                    synchronized( jobQueue ){   
                        r = getNextRunnable();
                        putRunningJob( r );
                    }   // end sync: jobQueue
                    notifyAll();
                }   // end sync: this

                try{ r.run(); } catch( Exception e ){}

                synchronized( this ){   
                    idle = true;
                    notifyAll();
                }   // end sync: this
                removeRunningJob( r );

            }   // end while: keep working
        }   // end run
        
    }   // end inner class ThreadPoolThread

}   // end class ThreadPool
