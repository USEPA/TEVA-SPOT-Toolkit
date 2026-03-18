package org.coinor.opents.net;

import org.coinor.opents.*;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ClientTabuSearch extends Remote
{
    
    public abstract void setObjectiveFunctionRemote( ObjectiveFunction func ) throws RemoteException;
    public abstract void setTabuListRemote( TabuList list ) throws RemoteException;
    public abstract void setAspirationCriteriaRemote( AspirationCriteria asp ) throws RemoteException;
    public abstract void setCurrentSolutionRemote( Solution soln ) throws RemoteException;
    public abstract void setBestSolutionRemote( Solution soln ) throws RemoteException;
    public abstract Object[] getBestMoveRemote( Move[] moves ) throws RemoteException;
    public abstract void setMaximizingRemote( boolean maximizing ) throws RemoteException;
    public abstract void setChooseFirstImprovingMoveRemote( boolean choose ) throws RemoteException;
    public abstract void operateOnCurrentSolutionRemote( Move move, double[] newObjValue ) throws RemoteException;
    public abstract java.net.InetAddress getInetAddressRemote() throws RemoteException;
    
}   // end interface ClientTabuSearch
