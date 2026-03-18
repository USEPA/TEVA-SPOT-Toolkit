import socket
import utils.eXist

class eXact(object):
    """
    Used to initialize the program. For now it initializes a proxy for the
    database. This is done so that the proxy is stored in only one place.
    """
    
    def __init__(self, db=None):
        """
        a database proxy which supports all the methods that utils.eXist
        suppports,
        """
        if db == None:
            try:
                db = utils.eXist.eXist(port=8080)
                #store db in class so it can be called from class methods
                self.__class__.dbProxy = db
            except socket.error,e:
                #HACK to print out an error message when using bindings
                print "Database not started try running eXist.sh"
                raise "Database not started try running eXist.sh"
            
    def getDbProxy(cls):
        """returns an instance of the database proxy"""
        try:
	    #print "exact.py: getDbProxy(cls) "
            return cls.dbProxy
        except AttributeError:
            "this means __init__ has not been called so lets force one"
            x = eXact()
            return x.__class__.dbProxy
        
    getDbProxy = classmethod(getDbProxy)
