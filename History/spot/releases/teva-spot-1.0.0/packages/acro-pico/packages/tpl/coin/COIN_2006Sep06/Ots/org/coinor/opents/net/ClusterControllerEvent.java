package org.coinor.opents.net;
import  org.coinor.opents.TabuSearch;

public class ClusterControllerEvent extends java.util.EventObject
{
    protected ClientTabuSearch client;
    
    protected Object property;
    
    protected Object oldVal;
    
    protected Object newVal;
    
    protected TabuSearch tabuSearch;
    
    public ClusterControllerEvent(Object source)
    {   
        super( source );
    }   // end constructor
    
    
    public ClusterControllerEvent( Object source, ClientTabuSearch client )
    {   
        super( source );
        this.client = client;
    }   // end constructor
    
    
    public ClusterControllerEvent( Object source, ClientTabuSearch client, Object property, Object oldVal, Object newVal )
    {   
        super( source );
        this.client = client;
        this.property = property;
        this.oldVal = oldVal;
        this.newVal = newVal;
    }   // end constructor
    
    
    public ClusterControllerEvent( Object source, TabuSearch tabuSearch )
    {   
        super( source );
        this.tabuSearch = tabuSearch;
    }   // end constructor
    
    


    public ClusterController getClusterController()
    {   
        return (ClusterController) source;
    }   // end getClusterController

    
    public ClientTabuSearch getClientTabuSearch()
    {
        return client;
    }   // end getClientTabuSearch
    
    public Object getProperty()
    {
        return property;
    }   // end getProperty
    
    public Object getOldValue()
    {
        return oldVal;
    }   // end getOldValue
    
    public Object getNewValue()
    {
        return newVal;
    }   // end getNewValue
    
    public TabuSearch getTabuSearch()
    {
        return tabuSearch;
    }   // end getTabuSearch

}   // end class ClusterControllerEvent



