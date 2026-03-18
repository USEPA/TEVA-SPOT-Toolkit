package org.coinor.opents.net;

import org.coinor.opents.*;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ClientTabuSearchFactory extends Remote
{
    
    public abstract ClientTabuSearch createClientTabuSearchRemote() throws RemoteException;
    
    
}   // end interface ClientTabuSearchFactory
