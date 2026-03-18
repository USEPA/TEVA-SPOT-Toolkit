package org.coinor.opents.net;

import org.coinor.opents.*;
import javax.swing.table.*;
import javax.swing.*;
import java.util.*;
import java.rmi.*;

public class ClientTabuSearchTableModel 
extends AbstractTableModel 
implements ClusterControllerListener, ClusterConstants
{
    private transient java.util.List clients;
    private transient Map clientProperties;
    private transient ClusterController controller;
    
    private final static int COLUMN_COUNT = 3;
    private final static Integer[] COLUMN_IDS = {USE_PROP, INET_ADDR_PROP, STATUS_PROP};
    private final static String[] COLUMN_NAMES = {
        ResourceBundle.getBundle("org/coinor/opents/net/strings").getString("panel.client.table.columns.use.title"),
        ResourceBundle.getBundle("org/coinor/opents/net/strings").getString("panel.client.table.columns.ip.title"),
        ResourceBundle.getBundle("org/coinor/opents/net/strings").getString("panel.client.table.columns.status.title")
    };
    private final static Class[] COLUMN_CLASSES = {
        java.lang.Boolean.class,
        java.lang.String.class,
        java.lang.String.class
    };
    
    public ClientTabuSearchTableModel()
    {
        super();
        
        this.clients = new ArrayList();
        this.clientProperties = new HashMap();
    }   // end constructor
    
    
    public void setClusterController( ClusterController controller )
    {
        if( controller != null )
            controller.removeClusterControllerListener( this );
        
        this.controller = controller;
        controller.addClusterControllerListener( this );
    }   // end if
    
    
    
    private ClientTabuSearch[] getUsableClientsZZZ()
    {/*
        java.util.List usable = new java.util.LinkedList();
        java.util.Iterator iter = clients.iterator();
        while( iter.hasNext() ){
            Object client = iter.next();
            //java.util.Map props = (java.util.Map)clientProperties.get( client );
            //Object oUse = props.get( USE_PROP );
            Object oUse = controller.getClientProperty( client, USE_PROP );
            if( oUse != null && ((Boolean)oUse).booleanValue() )
                usable.add( client );
        }   // end while
        return (ClientTabuSearch[])usable.toArray(new ClientTabuSearch[0]);
      */
        return controller.getClients( USE_PROP, new Boolean(true) );
    }   // end getUsableClients
    
    
    /**
     * Recommend calling this on non-event-dispatching thread.
     */
    public void addClientTabuSearch( ClientTabuSearch client )
    {
        if( clients.contains( client ) )
            return;

        Map props = new HashMap();
        
        // Add use
        //props.put( USE, new Boolean( true ) );
        
        // Add IP Address
        try{
            //props.put( INET_ADDR_PROP, client.getInetAddressRemote() );
            controller.setClientProperty( client, INET_ADDR_PROP, client.getInetAddressRemote() );
        }   // end try
        catch( RemoteException e ){
            e.printStackTrace();
            controller.reportRemoteException( client, e );
        }   // end catch
      
        
        // Update on main thread
        final ClientTabuSearch Client  = client;
        final java.util.List   Clients = clients;
        final java.util.Map    CProps  = clientProperties;
        final java.util.Map    Props   = props;
        SwingUtilities.invokeLater( new Runnable(){
            public void run(){
                Clients.add( Client );
                CProps.put( Client, Props );
                int row = Clients.indexOf( Client );
                if( row < 0 )
                    return;
                fireTableRowsInserted( row, row );
            }   // end run
        }); // end runnable
    }   // end addClientTabuSearch
    
    
    public void removeClientTabuSearch( ClientTabuSearch client )
    {
        final ClientTabuSearch Client  = client;
        final java.util.List   Clients = clients;
        //final java.util.Map    CProps  = clientProperties;
        //SwingUtilities.invokeLater( new Runnable(){
        //    public void run(){
                int row = Clients.indexOf( Client );
                if( row < 0 )
                    return;
                Clients.remove( Client );
                //CProps.remove( Client );
                fireTableRowsDeleted( row, row );
        //    }   // end run
        //}); // end runnable
    }   // end removeClientTabuSearch
    
    
    private void setStatus( ClientTabuSearch client, String status )
    {
        int row = clients.indexOf( client );
        if( row < 0 ){
            System.err.println( "Client not found: " + client );
            return;
        }   // end if: no client
        
        java.util.Map props = (java.util.Map)clientProperties.get( client );
        props.put( STATUS_PROP, status );
        
        
        final int Row = row;
        //SwingUtilities.invokeLater( new Runnable(){
        //    public void run(){
                fireTableRowsUpdated( Row, Row );
        //    }   // end run
        //}); // end runnable
    }   // end setStatus
    
    
/* ********  T A B L E   M O D E L   M E T H O D S  ******** */
    
    
    public java.lang.Object getValueAt(int row, int col)
    {
        if( row < 0 || row >= getRowCount() )
            return null;
        
        if( col < 0 || col >= getColumnCount() )
            return null;
        
        // Status is maintained locally
        if( STATUS_PROP.equals( COLUMN_IDS[col] ) ){
            Map props = (Map)clientProperties.get( clients.get(row) );
            return props == null ? null : props.get( COLUMN_IDS[col] );
        }   // end if: status
        else{
            return controller.getClientProperty( (ClientTabuSearch)clients.get(row), COLUMN_IDS[col] );
        }   // end else
    }   // end getValueAt
    
    
    public void setValueAt( Object newVal, int row, int col )
    {
        ClientTabuSearch client = (ClientTabuSearch)clients.get( row );
        //Map    props  = (Map)clientProperties.get( client );
        //props.put( COLUMN_IDS[col], newVal );
        
        controller.setClientProperty( client, USE_PROP, newVal );
        
        final int Row = row;
        //SwingUtilities.invokeLater( new Runnable(){
        //    public void run(){
                fireTableRowsUpdated( Row, Row );
        //    }   // end run
        //}); // end runnable
    }   // end setValueAt
    
    
    
    public int getRowCount()
    {
        return clients.size();
    }   // end getRowCount()
    
    
    
    public int getColumnCount()
    {
        return COLUMN_COUNT;
    }   // getColumnCount
    
    
    public boolean isCellEditable( int row, int col )
    {
        if( COLUMN_IDS[col] == USE_PROP )
            return true;
        
        return false;
    }   // end isCellEditable
    
    public Class getColumnClass( int col )
    {
        if( col < 0 || col >= getColumnCount() )
            return null;
        
        return COLUMN_CLASSES[ col ];
    }   // end getColumnClass
    
    
    public String getColumnName( int col )
    {
        if( col < 0 || col >= getColumnCount() )
            return null;
        
        return COLUMN_NAMES[ col ];
    }   // end getColumnName
    
    
    
/* ********  C L U S T E R   C O N T R O L L E R   L I S T E N E R   M E T H O D S  ******** */    
    
    
    public void tabuSearchStarted(ClusterControllerEvent evt)
    {
        System.out.println( "TS Started" );
        TabuSearch ts = evt.getTabuSearch();
        ts.addTabuSearchListener( new TabuSearchAdapter(){
            
            public void newCurrentSolutionFound( TabuSearchEvent evt ){
                handleNewCurrentSolutionFound( evt );
            }   // end newCurrentSolutionFound
            
        }); // end adapter
    }
    
    public void clientRemoved(ClusterControllerEvent evt)
    {
        removeClientTabuSearch( evt.getClientTabuSearch() );
    }   // end clientRemoved
    
    
    public void clientAdded(ClusterControllerEvent evt)
    {
        addClientTabuSearch( evt.getClientTabuSearch() );
    }   // end clientAdded
    
    
    public void clientPropertyChanged(ClusterControllerEvent evt)
    {
        Object prop = evt.getProperty();
        if( prop == null )
            return;
        
        // Status handled internally
        if( STATUS_PROP.equals( prop ) ){
            final ClientTabuSearch Client = evt.getClientTabuSearch();
            final java.util.List Clients  = clients;
            final Object NewVal           = evt.getNewValue();
            SwingUtilities.invokeLater( new Runnable(){
                public void run(){
                    int row = Clients.indexOf( Client );
                    if( row < 0 )
                        return;
                    setStatus( Client, NewVal == null ? null : NewVal.toString() );
                }   // end run
            }); // end runnable
        }   // end if: status
        else if( USE_PROP.equals( prop ) || INET_ADDR_PROP.equals( prop ) ){
            final ClientTabuSearch Client = evt.getClientTabuSearch();
            final java.util.List Clients  = clients;
            SwingUtilities.invokeLater( new Runnable(){
                public void run(){
                    int row = Clients.indexOf( Client );
                    if( row < 0 )
                        return;
                    fireTableRowsUpdated( row, row );
                }   // end run
            }); // end runnable
        }   // end if: something was updated that we care about
        
        
    }   // end clientPropertyChanged
    
    public void tabuSearchStopped(ClusterControllerEvent evt)
    {
    }
    
    
/* ********  T A B U   S E A R C H   L I S T E N E R   M E T H O D S  ******** */    
    
    
    public void handleNewCurrentSolutionFound( TabuSearchEvent evt )
    {
        HostTabuSearch ts = (HostTabuSearch)evt.getTabuSearch();
        // Check status of clients
        final HostTabuSearch.ClientHelper[] Helpers = ts.getHelpers();
        final String[] Statuses = new String[ Helpers.length ];
        String message = java.util.ResourceBundle.getBundle("org/coinor/opents/net/strings").getString("panel.client.table.columns.status.messageformat");
        Object[] args  = new Object[2];
        for( int i = 0; i < Helpers.length; i++ ){
            int moves = Helpers[i].getLastNumberOfMoves();
            double rate = Helpers[i].getMovesPerSecond();
            args[0] = new Integer(moves);
            args[1] = new Double(rate);
            Statuses[i]  = java.text.MessageFormat.format( message, args );
System.out.println( Statuses[i] );
        }   // end for
        
        // Things to do on the event-dispatching thread
        SwingUtilities.invokeLater( new Runnable(){
            public void run(){
                
                for( int i = 0; i < Helpers.length; i++ ){
                    setStatus( Helpers[i].getClientTabuSearch(), Statuses[i] );
                }   // end for
                
            }   // end run
        }); // end runnable
        
    }   // end handleNewCurrentSolution
    
   
    
    
}   // end class ClientTabuSearchFactoryTableModel
