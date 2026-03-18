package org.coinor.opents.net;
import  org.coinor.opents.*;
import  java.io.*;
import  java.net.*;
import  java.rmi.*;
import  java.rmi.server.*;

public class ClusterController implements ClusterConstants
{
    
    // Network variables
    private int             dgPort;
    private MulticastSocket dgSocket;
    private InetAddress     dgGroup;
    private ClassServer     classServer;
    private int             classServerPort;
    private Thread          shutdownHook;
    
    // Event-handling variables
    private ClusterControllerListener[] listenerList;
    
    // Tabu search variables
    private HostTabuSearch  tabuSearch;
    
    // Client variables
    private java.util.List clients;     // All known ClientTabuSearch instances
    private java.util.Map  clientProps; // Properties keyed on client object
    
    // Thread management
    private ThreadPool threadPool;
    
    
    public ClusterController()
    {
        super();
        
        dgPort = DEFAULT_DATAGRAM_PORT;
        classServerPort = DEFAULT_CLASS_SERVER_PORT;
        
        initSecurity();
        initComponents();
        
    }   // end constructor
    
    
    /**
     *
     **/
    private void initSecurity()
    {
        // What codebase should be:
        String codebase = null;
        try{
            codebase = "http://" + java.net.InetAddress.getLocalHost().getHostAddress() + ":" + getClassServerPort();
        }   // end try
        catch( java.net.UnknownHostException e ){
            e.printStackTrace();
        }   // end catch
        
        // What is current codebase?
        String currentCodebase = System.getProperty( "java.rmi.server.codebase" );
        
        // Is there a current codebase?
        if( currentCodebase == null ){
            
            if( codebase == null ){
                // Problem
            }   // end if: codebase also is null
            
                ClassLoader c = ClassLoader.getSystemClassLoader();
                java.net.URL codebase2 = c.getResource("org/coinor/opents/net/ClusterClientApp.class");
                String newUrl = codebase2.toString().substring(0,codebase2.toString().lastIndexOf("org/coinor/opents/net/ClusterClientApp.class") );
                 //codebase2 = new java.net.URL( newUrl );
                
            try{
                System.setProperty("java.rmi.server.codebase", codebase + " " + newUrl );
            }   // end try
            catch( java.security.AccessControlException e ){
                e.printStackTrace();
                // Problem
            }   // end catch
        }   // end if: no current codebase
        else{
            // If it doesn't already contain our codebase
            if( currentCodebase.indexOf( codebase ) < 0 ){
                try{
                    System.setProperty( "java.rmi.server.codebase", currentCodebase + " " + codebase );
                }   // end try
                catch( java.security.AccessControlException e ){
                    e.printStackTrace();
                    // Problem
                }   // end catch
            }   // end if
        }   // end else: current codebase exists
        
        
        if( System.getProperty("java.security.policy") == null ){
            try{
                ClassLoader c = ClassLoader.getSystemClassLoader();
                java.net.URL localCodebase = c.getResource("org/coinor/opents/net/ClusterController.class");
                String newUrl = localCodebase.toString().substring(0,localCodebase.toString().lastIndexOf("org/coinor/opents/net/ClusterController.class") );
                localCodebase = new java.net.URL( newUrl );
                System.setProperty("java.security.policy", new java.net.URL( localCodebase, "policy.all" ).toString() );
            }   // end try
            catch( Exception e ){
                e.printStackTrace();
            }   // end catch
        }   // end if: security policy
        
        
        if (System.getSecurityManager() == null){
            System.setSecurityManager(new RMISecurityManager());
        }   // end if: no security manager
        else 
            System.out.println("Security Manager already installed" );
        
        System.out.println( "RMI Codebase: " + System.getProperty( "java.rmi.server.codebase" ) );
    }   // end initSecurity
    
    
    
    
    private void initComponents()
    {
        // Event-handling mechanisms
        listenerList = new ClusterControllerListener[0];
        
        // Client variables
        clients     = new java.util.LinkedList();
        clientProps = new java.util.HashMap();
        
        // Thread management
        threadPool = new ThreadPool();
        
    }   // end initComponents
    
    
    public void start()
    {
        
        try{
            startClassServerServices();
        }   // end try
        catch( java.io.IOException e ){
            e.printStackTrace();
        }   // end catch
        
        
        try{
            startAppletPOCServices();
        }   // end try
        catch( java.rmi.RemoteException e ){
            e.printStackTrace();
        }   // end catch
        catch( java.net.MalformedURLException e ){
            e.printStackTrace();
        }   // end catch
        
        
        // Datagram
        try{
            startDatagramServices();
        }   // end try
        catch( java.io.IOException e ){
            e.printStackTrace();
        }   // end catch
        
    }   // end start
    
    
    
/* ********  N E T W O R K   S E R V I C E S  ******** */    
    
    
    
    
    private void startClassServerServices() throws java.io.IOException
    {
        
        ClassLoader c = ClassLoader.getSystemClassLoader();
        java.net.URL codebase = c.getResource("org/coinor/opents/net/ClusterController.class");
        String newUrl = codebase.toString().substring(0,codebase.toString().lastIndexOf("org/coinor/opents/net/ClusterController.class") );
        codebase = new java.net.URL( newUrl );
        
        
        classServer = new ClassServer( getClassServerPort(), null );
        classServer.addCodebase( codebase );
        classServer.start();
        System.out.println( "Class server started " + classServer + " with codebase " + codebase );
    }   // end startClassServerServices
    
    
    
    private void startAppletPOCServices() throws java.rmi.RemoteException, java.net.MalformedURLException
    {
        
        String name = "AppletPOC";
        
        System.out.print("Instantiating AppletPOCImpl...");
        AppletPOC poc = new AppletPOCImpl( this );
        System.out.println("Done.");
        System.out.print( "Rebinding...");
        Naming.rebind(name, poc);
        System.out.println("Done.");
        System.out.println("AppletPOCImpl bound");
        
    }   // end startAppletPOCServices
    
    
    private void startDatagramServices() throws java.io.IOException
    {
        dgGroup  = java.net.InetAddress.getByName("230.0.0.1");
        dgSocket = new MulticastSocket( getDatagramPort() );
        dgSocket.joinGroup( dgGroup );
        dgSocket.send( new DatagramPacket( new byte[]{ REQUEST_CLIENT_RESPONSE }, 1, dgGroup, getDatagramPort() ) );

        final MulticastSocket DgSocket = dgSocket;
        Thread t = new Thread( new Runnable(){
            public void run(){
                byte[] buffer = new byte[32];
                DatagramPacket dg = new DatagramPacket( buffer, buffer.length );
                try{
                    while( true ){
                        DgSocket.receive( dg );
                        final MulticastSocket Sock = DgSocket;
                        final DatagramPacket  Pack = dg;
                        //queueRunnable( new Runnable(){
                        //    public void run(){
                                handleIncomingDatagram( Sock, Pack );
                        //    }   // end run
                        //}); // end runnable
                    }   // end while
                }   // end try
                catch( IOException e ){
                    e.printStackTrace();
                }   // end catch
                finally{
                    stopDatagramServices();
                }   // end finally
            }   // end run
        });
        t.start();        
        
        shutdownHook = new Thread( new Runnable(){
            public void run(){
                System.out.println( "Shutdown hook started." );
                try{
                    DgSocket.send( new DatagramPacket( new byte[]{ I_AM_A_HOST_LEAVING }, 1, dgGroup, getDatagramPort() ) );
                }   // end try
                catch( Exception e){
                    e.printStackTrace();
                }
            }   // end run
        });
        Runtime.getRuntime().addShutdownHook( shutdownHook );
    }   // end startDatagramServices
    
    
    private void stopDatagramServices()
    {
        if( dgSocket == null )
            return;
        
        try{    
            dgSocket.leaveGroup( dgGroup );
        }   // end try
        catch( IOException e ){
            e.printStackTrace(); 
        }   // end catch
        
        dgSocket.close();
        
        this.dgSocket = null;
    }   // end stopDatagramServices
    
    
    private void handleIncomingDatagram( DatagramSocket socket, DatagramPacket packet )
    {
        int off    = packet.getOffset();
        int len    = packet.getLength();
        byte[] dat = packet.getData();   
        
        if( len == 0 )
            return;
        
        final InetAddress originator = packet.getAddress();
        byte firstByte = dat[off];
        
        switch( firstByte ){
            case REQUEST_CLIENT_RESPONSE:
                System.out.println( "Got REQUEST_CLIENT_RESPONSE from " + packet.getAddress());
                break;
                
            case I_AM_A_CLIENT:
                queueRunnable( new Runnable(){
                    public void run(){
                        System.out.println( "Got I_AM_A_CLIENT from " + originator );

                        String name = "/" + originator.toString() + "/ClientTabuSearchFactory";
                        try{
                            System.out.println( "  Looking up " + name );
                            ClientTabuSearchFactory clientFactory = (ClientTabuSearchFactory) Naming.lookup(name);
                            System.out.println( "  Got factory: " + clientFactory );
                            System.out.print( "  Creating client..." );
                            ClientTabuSearch client = clientFactory.createClientTabuSearchRemote();
                            System.out.println( "Got client: " + client );
                            addClientTabuSearch( client );
                            System.out.println( originator + ": Added client: " + client );
                            System.out.println( client );
                        }   // end try
                        catch( Exception e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end run
                }); // end runnable
                break;
                
            case I_AM_A_CLIENT_LEAVING:
                System.out.println( "Got I_AM_A_CLIENT_LEAVING from " + originator );
           /*     
                ClientTabuSearch client = (ClientTabuSearch)inetToClientMap.get( originator );
                System.out.println( originator + ": Client leaving: " + client );
                removeClientTabuSearch( client );
            */
                break;
                
            case I_AM_A_HOST:
                System.out.println( "Got I_AM_A_HOST from " + packet.getAddress());
                break;
                
            case I_AM_A_HOST_LEAVING:
                System.out.println( "Got I_AM_A_HOST_LEAVING from " + packet.getAddress());
                break;
                
            default:
                System.out.println( "Unhandled request: " + firstByte );
                break;
        }   // end switch
    }   // end handleIncomingDatagram
    
    
    
/* ********  C L I E N T   M A N A G E M E N T   M E T H O D S  ******** */    
    
    
    protected synchronized void addClientTabuSearch( ClientTabuSearch client )
    {
        if( clients.contains( client ) )
            return;
        
        clients.add( client );
        java.util.Map props = new java.util.HashMap();
        props.put( USE_PROP, new Boolean( true ) );
        clientProps.put( client, props );
        
        final ClientTabuSearch Client = client;
        queueRunnable( new Runnable(){
            public void run(){
                java.net.InetAddress source = null;
                try{
                    source = Client.getInetAddressRemote();
                    setClientProperty( Client, INET_ADDR_PROP, source );
                }   // end try
                catch( RemoteException e ){
                    e.printStackTrace();
                    reportRemoteException( Client, e ); 
                }   // end catch
            }   // end run
        }); // end runnable
        
        fireClientAdded( client );
    }   // end addClientTabuSearch
    
    
    public void setClientProperty( ClientTabuSearch client, Object prop, Object newVal )
    {
        if( !clients.contains( client ) )
            return;
        
        java.util.Map props = (java.util.Map)clientProps.get( client );
        Object oldVal = props.get( prop );
        props.put( prop, newVal );
        
        fireClientPropertyChanged( client, prop, oldVal, newVal );
    }   // end setClientProperty
    
    
    public Object getClientProperty( ClientTabuSearch client, Object prop )
    {
        if( !clients.contains( client ) )
            return null;
        
        java.util.Map props = (java.util.Map)clientProps.get( client );
        return props.get( prop );
    }   // end getClientProperty
    
    
    
    protected synchronized void reportRemoteException( ClientTabuSearch client, RemoteException e )
    {
        System.out.println( "Remote Exception reported to ClusterController." );
        // For now, just remove clients with exceptions. We'll get more
        // sophisticated later.
        clients.remove( client );
        clientProps.remove( client );
        
        fireClientRemoved( client );
    }   // end reportRemoteException
    
    
    
    protected synchronized ClientTabuSearch[] getClients()
    {
        return (ClientTabuSearch[])clients.toArray( new ClientTabuSearch[0] );
    }   // end getClients
    
    
    
    protected synchronized ClientTabuSearch[] getClients( Object property, Object value )
    {
        java.util.List list = new java.util.LinkedList();
        java.util.Iterator iter = clients.iterator();
        while( iter.hasNext() ){
            Object client = iter.next();
            java.util.Map props = (java.util.Map)clientProps.get( client );
            if( value == null ){
                if( props.get( property ) == null ){
                    list.add( client );
                }   // end if: prop == null
            }   // end if: value == null
            else if( value.equals( props.get( property ) ) ){
                list.add(client );
            }   // end else if: val == prop
        }   // end while
        return (ClientTabuSearch[])list.toArray( new ClientTabuSearch[0] );
    }   // end getClients
    
    
    
/* ********  T A B U   S E A R C H   M G M T   M E T H O D S  ******** */    
    
    
    
    
    
    public void setTabuSearchCodebase( String codebase ) throws java.io.IOException
    {
        classServer.setCodebase( codebase );
        
    }   // end setCodebase
    
    
    public String getTabuSearchCodebase()
    {
        return classServer.getCodebase();
    }   // end getTabuSearchCodebase
    
    
    public void startSolving( Initializer initializer, int iterations )
    {
        // Make tabu search
        tabuSearch = new HostTabuSearch();
        tabuSearch.setCurrentSolution( initializer.getStartingSolution() );
        tabuSearch.setMoveManager( initializer.getMoveManager() );
        tabuSearch.setTabuList( initializer.getTabuList() );
        tabuSearch.setObjectiveFunction( initializer.getObjectiveFunction() );
        tabuSearch.setIterationsToGo( iterations );

        // Add clients
        ClientTabuSearch[] clients = getClients( USE_PROP, new Boolean( true ) );
        for( int i = 0; i < clients.length; i++ ){
            tabuSearch.addClient( clients[i] );
        }   // end for: each client
    
        // Listeners
        tabuSearch.addTabuSearchListener( new TabuSearchAdapter(){

            public void tabuSearchStarted( TabuSearchEvent evt ){
                handleTabuSearchStarted( evt );
            }   // end tabuSearchStarted

            public void tabuSearchStopped( TabuSearchEvent evt ){
                handleTabuSearchStopped( evt );
            }   // end tabuSearchStopped

        });

        // Start
        tabuSearch.startSolving();
    }   // end startSolving
    
    
    
    public void stopSolving()
    {
        if( tabuSearch == null )
            return;
        else
            tabuSearch.stopSolving();
    }   // end stopSolving
    
    
    
    public boolean isSolving()
    {
        if( tabuSearch == null )
            return false;
        else
            return tabuSearch.isSolving();
    }   // end isSolving
    
    
    private void handleTabuSearchStopped( TabuSearchEvent evt )
    {
        fireTabuSearchStopped( evt.getTabuSearch() );
        tabuSearch = null;
    }   // end handleTabuSearchStopped
    
    
    private void handleTabuSearchStarted( TabuSearchEvent evt )
    {
        fireTabuSearchStarted( evt.getTabuSearch() );
    }   // end handleTabuSearchStarted
    
    
/* ********  T H R E A D   M A N A G E M E N T   M E T H O D S  ******** */    
    
    
    protected void queueRunnable( Runnable runnable )
    {
        threadPool.run( runnable );
    }   // end queueRunnable
    
    
    
    
/* ********  P U B L I C   G E T   M E T H O D S  ******** */    
    
    
    public int getDatagramPort()
    {
        return dgPort;
    }   // end getDatagramPort
    
    
    public int getClassServerPort()
    {
        return classServerPort;
    }   // end getClassServerPort
    
    
/* ********  C L U S T E R   C O N T R O L L E R   L I S T E N E R   M E T H O D S  ******** */
        
    
    public void addClusterControllerListener( ClusterControllerListener listener )
    {
        ClusterControllerListener[] list = new ClusterControllerListener[
            listenerList.length + 1 ];
        
        for( int i = 0; i < list.length - 1; i++ )
            list[i] = listenerList[i];

        list[ list.length-1 ] = listener;
        listenerList = list;
    }   // end addClusterControllerListener
    
    
     public final synchronized void removeClusterControllerListener( ClusterControllerListener listener )
     {  
         // Find location of listener to remove
        int index = -1;
        int j = 0;
        while( index < 0 && j < listenerList.length ){
            if( listenerList[j] == listener )
                index = j;
            else j++;
        }   // end while: through list
        
        // If index is less than zero then it wasn't in the list
        if( index >= 0 )
        {   ClusterControllerListener[] list = new ClusterControllerListener[
                listenerList.length - 1 ];
            for( int i = 0; i < list.length; i++ )
                list[i] = listenerList[ i < index ? i : i + 1 ];
            listenerList = list;
        }   // end if: listener was in the list
     }  // end removeClusterControllerListener

     
     protected void fireClientAdded( ClientTabuSearch client )
     {
        ClusterControllerListener[] list = null;
        ClusterControllerEvent evt = new ClusterControllerEvent( this, client );
        
        synchronized( this ){
            list = (ClusterControllerListener[])listenerList.clone();
        }  // end sync
        
        for( int i = 0; i < list.length; i++ )
            list[i].clientAdded( evt );
     }  // end fireClientAdded
     
     
     protected void fireClientRemoved( ClientTabuSearch client )
     {
        ClusterControllerListener[] list = null;
        ClusterControllerEvent evt = new ClusterControllerEvent( this, client );
        
        synchronized( this ){
            list = (ClusterControllerListener[])listenerList.clone();
        }  // end sync
        
        for( int i = 0; i < list.length; i++ )
            list[i].clientRemoved( evt );
     }  // end fireClientRemoved
     
     
     protected void fireClientPropertyChanged( ClientTabuSearch client, Object property, Object oldVal, Object newVal )
     {
        ClusterControllerListener[] list = null;
        ClusterControllerEvent evt = new ClusterControllerEvent( this, client, property, oldVal, newVal );
        
        synchronized( this ){
            list = (ClusterControllerListener[])listenerList.clone();
        }  // end sync
        
        for( int i = 0; i < list.length; i++ )
            list[i].clientPropertyChanged( evt );
     }  // end fireClientPropertyChanged
     
     
     protected void fireTabuSearchStarted( TabuSearch tabuSearch )
     {
        ClusterControllerListener[] list = null;
        ClusterControllerEvent evt = new ClusterControllerEvent( this, tabuSearch );
        
        synchronized( this ){
            list = (ClusterControllerListener[])listenerList.clone();
        }  // end sync
        
        for( int i = 0; i < list.length; i++ )
            list[i].tabuSearchStarted( evt );
     }  // end fireTabuSearchStarted
     
     
     
     protected void fireTabuSearchStopped( TabuSearch tabuSearch )
     {
        ClusterControllerListener[] list = null;
        ClusterControllerEvent evt = new ClusterControllerEvent( this, tabuSearch );
        
        synchronized( this ){
            list = (ClusterControllerListener[])listenerList.clone();
        }  // end sync
        
        for( int i = 0; i < list.length; i++ )
            list[i].tabuSearchStopped( evt );
     }  // end fireTabuSearchStopped
     
     
    
}   // end class ClusterController
