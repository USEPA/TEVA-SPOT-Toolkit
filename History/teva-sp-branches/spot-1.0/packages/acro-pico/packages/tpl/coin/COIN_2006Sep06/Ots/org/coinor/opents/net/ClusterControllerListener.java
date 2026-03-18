package org.coinor.opents.net;


public interface ClusterControllerListener 
extends java.io.Serializable, java.util.EventListener
{
    
    public abstract void clientAdded( ClusterControllerEvent evt );
    
    public abstract void clientRemoved( ClusterControllerEvent evt );
    
    public abstract void clientPropertyChanged( ClusterControllerEvent evt );
    
    public abstract void tabuSearchStarted( ClusterControllerEvent evt );
    
    public abstract void tabuSearchStopped( ClusterControllerEvent evt );
    
    
}   // end interface ClusterControllerListener
