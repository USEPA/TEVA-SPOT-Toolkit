package org.coinor.opents.net;

import org.coinor.opents.*;
import java.rmi.server.*;
import java.rmi.*;


public class ClientTabuSearchFactoryImpl extends UnicastRemoteObject implements ClientTabuSearchFactory
{
    
    public ClientTabuSearchFactoryImpl() throws RemoteException
    {
        super();
    }   // end constructor
    
    
    public ClientTabuSearch createClientTabuSearchRemote() throws RemoteException
    {
        return new ClientTabuSearchImpl();
    }
    
}   // end class ClientTabuSearchFactoryImpl
