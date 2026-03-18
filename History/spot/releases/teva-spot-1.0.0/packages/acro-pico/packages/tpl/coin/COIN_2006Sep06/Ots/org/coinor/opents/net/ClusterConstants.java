package org.coinor.opents.net;


public interface ClusterConstants
{
    
    public static final byte REQUEST_CLIENT_RESPONSE    = 1;
    public static final byte I_AM_A_CLIENT              = 2;
    public static final byte I_AM_A_CLIENT_LEAVING      = 3;
    public static final byte I_AM_A_HOST                = 4;
    public static final byte I_AM_A_HOST_LEAVING        = 5;
    
    public static final int DEFAULT_DATAGRAM_PORT       = 5815;
    public static final int DEFAULT_CLASS_SERVER_PORT   = 5816;
    
    public final static Integer USE_PROP                = new Integer(1);
    public final static Integer INET_ADDR_PROP          = new Integer(2);
    public final static Integer STATUS_PROP             = new Integer(4);
    
}   // end interface ClusterConstants
