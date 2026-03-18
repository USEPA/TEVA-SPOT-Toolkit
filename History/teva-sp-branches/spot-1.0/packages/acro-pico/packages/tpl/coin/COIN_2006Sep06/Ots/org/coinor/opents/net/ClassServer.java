package org.coinor.opents.net;

import java.io.*;
import java.net.*;

public class ClassServer
{
    
    public static void main( String[] args )
    {
        try{
            ClassServer cs = new ClassServer( 8081, "D:/java/projects/tabusearch/opents_10/cvs-source/coin/ots/" );
            //ClassServer cs = new ClassServer( 8081, "D:/temp" );
            cs.start();
            //Thread.sleep( 5000 );
            //System.out.println( "Stopping..." );
            //cs.stop();
            
        }   // end try
        catch( Exception e ){
            e.printStackTrace();
        }   // end catch
    }   // end main
    
    /** Default port: 8080. */
    public final static int DEFAULT_PORT = 8080;
    
    private int port = -1;
    
    private String codebase;
    
    private Thread serverThread;
    
    private ServerSocket serverSocket;
    
    private java.util.List codebases;
    
    public ClassServer()
    {
        super();
        codebases = new java.util.LinkedList();
    }   // end constructor
    
    
    public ClassServer( int port, String codebase ) throws java.io.IOException
    {   
        this();
        setPort( port );
        setCodebase( codebase );
    }   // end constructor
    
    
    public synchronized void setPort( int port ) throws java.io.IOException
    {
        if( this.port == port )
            return;
        
        boolean wasRunning = serverThread != null;
        
        if( wasRunning )
            stop();
        
        this.port = port;
        
        if( wasRunning )
            start();
    }   // end setPort
    
    
    public synchronized void setCodebase( String codebase ) throws java.io.IOException
    {
        // If no change, do nothing
        if( codebase != null && codebase.equals( this.codebase ) )
            return;
        
        // If null, that's fine
        if( codebase == null ){
            this.codebase = codebase;
            return;
        }   // end if
        
        // Verify that it's a directory
        java.io.File fCodebase = new java.io.File( codebase );
        if( !fCodebase.isDirectory() )
            throw new java.io.IOException( "Not a directory: " + fCodebase );
        
        // Update fields
        this.codebase = codebase;
        
    }   // end setCodebase
    
    public synchronized String getCodebase()
    {
        return this.codebase;
    }   // end getCodebase
    
    
    public synchronized void addCodebase( java.net.URL codebase )
    {
        codebases.add( codebase );
    }   // end addCodebase
    
    public synchronized boolean removeCodebase( java.net.URL codebase )
    {
        return codebases.remove( codebase );
    }   // end removeCodebase
    
    
    public synchronized void start() throws java.io.IOException
    {
        if( serverThread != null )
            return; // Already running
        System.out.println( "Starting ClassServer on port " + port + " and codebase " + getCodebase() );
        setServerSocket( new java.net.ServerSocket( port ) );
        final Thread sThread = new Thread( new Runnable(){
            public void run(){
                try{
                    while( true ){
                        try{
                            System.out.print( "ClassServer listening..." );
                            final java.net.Socket socket = getServerSocket().accept();
                            System.out.println( "Got socket." );
                            Thread t = new Thread( new Runnable(){
                                public void run(){
                                    handleIncoming( socket );
                                }   // end run
                            }); // end runnable
                            t.setDaemon( true );
                            t.start();
                        }   // end try
                        catch( java.net.SocketException e ){
                            e.printStackTrace();
                        }   // end catch
                    }   // end while: indefinitely
                }   // end try
                catch( IOException e ){
                    e.printStackTrace();
                }   // end catch
            }   // end run
        });
        //sThread.setDaemon( true );
        setServerThread( sThread );
        serverThread.start();
    }   // end start
    
    
    public synchronized void stop() throws java.io.IOException
    {
        if( serverThread == null )
            return;
        System.out.print( "Stopping..." );
        try{
            serverSocket.close();
            System.out.println( "isBound: " + serverSocket.isBound() );
        }   // end try
        catch( Exception e){
            e.printStackTrace();
        }   // end catch
        
        serverSocket = null;
        serverThread = null;
        System.out.println( "Stopped." );
    }   // end stop
    
    
    public ServerSocket getServerSocket()
    {   
        return serverSocket;
    }   // end setServerSocket
    
    
    public void setServerSocket( ServerSocket socket )
    {
        this.serverSocket = socket;
    }   // end setServerSocket
    
    
    protected void handleIncoming( java.net.Socket socket )
    {
        java.io.InputStream  in  = null;
        java.io.OutputStream out = null;
        java.io.InputStream  fileIn = null;
        try{
            // Get I/O streams
            in  = new java.io.BufferedInputStream( socket.getInputStream() );
            out = new java.io.BufferedOutputStream( socket.getOutputStream() );
        
            // Read request
            String sRequest = readRequest( in );
            System.out.println( "Requested: " + sRequest );
            
            boolean success = false;
    /*            
            // New way
            java.util.Iterator iter = codebases.iterator();
            java.net.URL urlReq = null;
            while( iter.hasNext() ){ 
                java.net.URL base = (java.net.URL)iter.next();
                try{
                    // See if we can make a valid request of this.
                    urlReq = new java.net.URL( base, sRequest );
                    System.out.println( "Testing for URL " + urlReq );

                    // Read and send file
                    fileIn = new java.io.BufferedInputStream( urlReq.openStream() );
                    out.write(new String("HTTP/1.0 200 OK\r\n").getBytes());
                    if( urlReq.toString().endsWith(".html") )
                        out.write(new String("Content-Type: text/html\r\n\r\n").getBytes());
                    else
                        out.write(new String("Content-Type: application/java\r\n\r\n").getBytes());
                    byte[] buff = new byte[4096];
                    int numBytes = -1;
                    while( (numBytes = fileIn.read(buff)) >= 0 ){
                        out.write(buff, 0, numBytes );
                    }   // end while
                    success = true;
                    break; // Success. Leave outer while loop.
                }   // end try
                catch( Exception e ){
                    e.printStackTrace();
                }   // end catch
            }   // end while: through list

     */
            
            // How 'bout old way
            if( !success ){
                System.out.println( "Try old way instead." );
                // No base set yet?
                if( getCodebase() == null ){
                    out.write(new String("HTTP/1.0 400 Bad Request\r\n\r\n").getBytes());
                    throw new java.io.IOException( "No codebase set." );
                }   // end if: no codebase set

                // Valid file?
                java.io.File fRequest = new java.io.File( getCodebase(), sRequest );
                if( !fRequest.isFile() ){
                    out.write(new String("HTTP/1.0 400 Bad Request\r\n\r\n").getBytes());
                    throw new java.io.IOException( "Invalid request: " + fRequest );
                }   // end if: bad request

                // Read and send file
                out.write(new String("HTTP/1.0 200 OK\r\n").getBytes());
                out.write(new String("Content-Length: " + fRequest.length() + "\r\n").getBytes());
                out.write(new String("Content-Type: application/java\r\n\r\n").getBytes());
                fileIn = new java.io.BufferedInputStream( new java.io.FileInputStream( fRequest ) );
                byte[] buff = new byte[4096];
                int numBytes = -1;
                while( (numBytes = fileIn.read(buff)) >= 0 ){
                    out.write(buff, 0, numBytes );
                }   // end while
                success = true;
            }   // endif: old way
                
            if( !success ){
                out.write(new String("HTTP/1.0 400 Bad Request\r\n\r\n").getBytes());
                System.err.println( "Could not supply " + sRequest );
            }   // end if: no success
            
            
        }   // end try
        catch( IOException e ){
            System.err.println( "Very last line");
            e.printStackTrace();
        }   // end catch
        finally{
            try{ in.close();  }catch( Exception e ){}
            try{ out.close(); }catch( Exception e ){}
            try{ fileIn.close(); }catch( Exception e ){}
        }   // end finally
    }   // end handleIncoming
    
    
    
    protected String readRequest( java.io.InputStream in ) throws java.io.IOException
    {
        int c = -1;
        int MAX_SIZE = 500;
        StringBuffer b = new StringBuffer( MAX_SIZE );
        String file = null;
        while( (file == null) && (b.length() < MAX_SIZE ) && ((c = in.read()) >= 0) )
        {
            // Check for CRLF
            if( c == '\n' )
                throw new java.io.IOException( "Bad http request." );
            if( c == '\r' ){
                c = in.read();
                if( c != '\n' )
                    throw new java.io.IOException( "Bad http request." );
                String line = b.toString();
                java.util.StringTokenizer st = new java.util.StringTokenizer( line, " " );
                if( !st.hasMoreTokens() )
                    throw new java.io.IOException( "Bad http request." );
                String shouldBeGET = st.nextToken();
                if( !shouldBeGET.equals( "GET" ) )
                    throw new java.io.IOException( "Bad http request." );
                if( !st.hasMoreTokens() )
                    throw new java.io.IOException( "Bad http request." );
                file = st.nextToken();
            }   // end if: end of line
            b.append( (char)c );
        }   // end while
        
        if( file == null )
            throw new java.io.IOException( "Bad http request." );
        
        // Strip leading /
        if( file.startsWith("/") )
            file = file.substring(1);
            
        return file;
    }   // end readRequest
    
    
    protected void setServerThread( Thread sThread )
    {
        this.serverThread = sThread;
    }   // end setServerThread
    
}   // end class ClassServer

