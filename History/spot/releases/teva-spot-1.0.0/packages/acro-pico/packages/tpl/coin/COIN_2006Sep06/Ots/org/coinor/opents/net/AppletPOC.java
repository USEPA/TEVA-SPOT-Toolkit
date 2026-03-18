package org.coinor.opents.net;

import org.coinor.opents.*;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface AppletPOC extends Remote
{
    
    public abstract void registerClientTabuSearchFactoryRemote( ClientTabuSearchFactory factory ) throws RemoteException;
    
    
}   // end interface AppletPOC
