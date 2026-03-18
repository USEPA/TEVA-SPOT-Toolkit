package org.coinor.opents.net;

import org.coinor.opents.*;
import java.rmi.server.*;
import java.rmi.*;


public class ClientTabuSearchImpl extends UnicastRemoteObject implements ClientTabuSearch
{
    InternalTabuSearch ts;
    
    
    
    public static void main(String[] args) {
        System.out.println( "Launching MultiThreadedClientTabuSearch" );
        
        String rsrc = "org/coinor/opents";
        java.net.URL temp = ClassLoader.getSystemResource(rsrc);
        String codebase = temp.toString().substring(0,temp.toString().length() - 17); 
        System.out.println( "Codebase: " + codebase );
        final java.io.FilePermission perm = new java.io.FilePermission( codebase + "-", "read" );
        
        if (System.getSecurityManager() == null) 
        {
            /*System.setSecurityManager( new RMISecurityManager(){
                public void checkRead(java.io.FileDescriptor filedescriptor) {}
                public void checkRead(String filename){}
                public void checkRead(String filename, Object executionContext) {}
            });*/
            
            //System.setSecurityManager(new SecurityManager() );
            System.setSecurityManager(new RMISecurityManager());
            //System.setSecurityManager (new RMISecurityManager() {
            //    public void checkConnect (String host, int port) {}
            //    public void checkConnect (String host, int port, Object context) {}
            //});

        }else System.out.println( "Security Manager already in place." );
        //String name = "//127.0.0.1/ClientTabuSearch";
        String name = "ClientTabuSearchFactory";
        try {
            System.out.print("Instantiating...");
            ClientTabuSearchFactory clientFactory = new ClientTabuSearchFactoryImpl();
            System.out.println("Done.");
            System.out.print( "Rebinding...");
            Naming.rebind(name, clientFactory);
            System.out.println("Done.");
            System.out.println("ClientTabuSearchFactory bound");
        } catch (Exception e) {
            System.err.println("ClientTabuSearchFactory exception: " + 
			       e.getMessage());
            e.printStackTrace();
        }
    }

    
    public ClientTabuSearchImpl() throws RemoteException
    {
        super();
        System.out.println( "New ClientTabuSearchImpl." );
        ts = new InternalTabuSearch();
    }   // end constructor
    
    
/* ******** R E M O T E   C L I E N T T A B U S E A R C H   M E T H O D S  ******** */    
    
    
    public void setChooseFirstImprovingMoveRemote(boolean choose) throws RemoteException
    {
        ts.setChooseFirstImprovingMove( choose );
    }   // end setChooseFirstImprovingMoveRemote
    
    
    public void setBestSolutionRemote(Solution soln) throws RemoteException
    {
        System.out.println( "Setting new best solution: " + soln );
        ts.setBestSolution( soln );
    }   // end setBestSolutionRemote
    
    
    public void setMaximizingRemote(boolean maximizing) throws RemoteException
    {
        ts.setMaximizing( maximizing );
    }   // end setMaximizingRemote
    
    
    public void setAspirationCriteriaRemote(AspirationCriteria asp) throws RemoteException
    {
        ts.setAspirationCriteria( asp );
    }   // end setAspirationCriteriaRemote
    
    
    public void setTabuListRemote(TabuList list) throws RemoteException
    {
        ts.setTabuList( list );
    }   // end setTabuListRemote
    
    
    public void setCurrentSolutionRemote(Solution soln) throws RemoteException
    {
        System.out.println( "Setting new current solution: " + soln );
        ts.setCurrentSolution( soln );
    }   // end setCurrentSolutionRemote
    
    
    public void setObjectiveFunctionRemote(ObjectiveFunction func) throws RemoteException
    {
        ts.setObjectiveFunction( func );
    }   // end setObjectiveFunctionRemote
    
    
    public Object[] getBestMoveRemote(Move[] moves) throws RemoteException
    { 
        System.out.println( "Working on " + moves.length + " moves (" + moves + ")" );
        return ts.getBestMove( moves );
    }   // end getBestMoveRemote
    
    
    public void operateOnCurrentSolutionRemote( Move move, double[] newObjVal ) throws RemoteException
    {
        ts.operateOnCurrentSolution( move, newObjVal );
    }   // end operateOnCurrentSolution
    
    
    public java.net.InetAddress getInetAddressRemote() throws RemoteException
    {
        try{
            return java.net.InetAddress.getLocalHost();
        }   // end try
        catch( /*java.net.UnknownHost*/Exception e ){
            e.printStackTrace();
            return null;
            //throw new RemoteException( e.getMessage() );
        }   // end catch
    }   // end getInetAddressRemote
    
    
/* ********  I N T E R N A L   C L A S S  ******** */    
    
    
    protected static class InternalTabuSearch extends MultiThreadedTabuSearch
    {
        
        public InternalTabuSearch()
        {
            //setThreads( Runtime.getRuntime().availableProcessors() );
        }   // end constructor
     
        
        protected Object[] getBestMove( Move[] moves )
        {
            return getBestMove(
                getCurrentSolution(),
                moves,
                getObjectiveFunction(),
                getTabuList(),
                getAspirationCriteria(),
                isMaximizing(),
                isChooseFirstImprovingMove() );
        }   // end getBestMove
        
        protected void operateOnCurrentSolution( Move move, double[] newObjVal )
        {
            Solution soln = getCurrentSolution();
            move.operateOn( soln );
            soln.setObjectiveValue( newObjVal );
        }   // end operateOnCurrentSolution
        
    }   // end inner class InternalTabuSearch
    
}   // end class MultiThreadedClientTabuSearch
