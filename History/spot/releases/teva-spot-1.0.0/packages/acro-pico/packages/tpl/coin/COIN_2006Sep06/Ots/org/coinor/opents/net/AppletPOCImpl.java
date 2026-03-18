package org.coinor.opents.net;

import org.coinor.opents.*;
import java.rmi.server.*;
import java.rmi.*;


public class AppletPOCImpl extends UnicastRemoteObject implements AppletPOC
{
    private ClusterController controller;
    
    public AppletPOCImpl() throws RemoteException
    {
        super();
    }   // end constructor
    
    public AppletPOCImpl( ClusterController controller ) throws RemoteException
    {
        this.controller = controller;
    }
    
    
    public void registerClientTabuSearchFactoryRemote( ClientTabuSearchFactory factory ) throws RemoteException
    {
        ClientTabuSearch client = factory.createClientTabuSearchRemote();
        controller.addClientTabuSearch( client );
        //clusterApp.addClientTabuSearch( client );
    }
    
    
}   // end class ClientTabuSearchFactoryImpl
