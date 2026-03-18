from gnosis.xml.pickle import XML_Pickler
from gnosis.xml.pickle.util import setParanoia
import exact

setParanoia(0)

# we have to remove the DOCTYPE from the pickled string to prevent an
# xml database from trying to validate the document based on the DTD
pickDocStr = "<!DOCTYPE PyObject SYSTEM \"PyObjects.dtd\">\n"

class DataWrapper(object):
    """
    Handles an object which is automatically pickled.
    """
    __data = None

    def __init__(self, data=None):
        """
        creates the object and associates some information with it
        """
        if data:
            self.__data = data

    def setData(self, newData):
        """
        changes the data associated with a DataWrapper
        """
        self.__data = newData
        
    def getData(self):
        """
        returns the data associated with the object
        """
        return self.__data

    def toXml(self):
        """
        returns an XML pickled representation of the object
        """
	#print "data.py: toXML() called"
        pickObj = XML_Pickler(self)
        pickStr = pickObj.dumps()
	#print "data.py: toXML(): string len: " + `len(pickStr)`
        return pickStr.replace(pickDocStr, "")

    
    def saveToXml(self, filename):
        """
        Converts an object reference to XML, and saves it in the database.
        """
	#print "data.py: saveToXml() " + filename
        #if (DataWrapper.exists(filename)):
	#	print "removing " + filename
        #	exact.eXact.getDbProxy().remove(filename)
        exact.eXact.getDbProxy().addDocument(self.toXml(), filename, 1)
	#print "data.py: addDocument was called: " + filename

        
    def loadFromFile(filename):
        """
        unpickles an XML document from the database, that was previously saved
        with saveToXml
        """
        dbProxy           = exact.eXact().getDbProxy()
        xmlstr = exact.eXact.getDbProxy().getDocument(filename)
        if not xmlstr:
	    #print "data.py: loadFromFile " + filename + " failed"
            return None
        obj = objectifyString(xmlstr)
        return obj
    loadFromFile = staticmethod(loadFromFile)

    def exists(filename):
        """
        Checks if a file exists in the DataBase
        """
        x = exact.eXact.getDbProxy().hasDocument(filename)
        if x:
            return 1
        else:
            return 0
    exists = staticmethod(exists)

    def hasKeyValuePairs(collection, cls, params):
        """
        Queries the database 
        """
        lst = exact.eXact.getDbProxy().findKeyValuePairs(collection,cls,params)
        if lst == None:
            return 0
        else:
            return 1
    hasKeyValuePairs = staticmethod(hasKeyValuePairs)
                            
    def findKeyValuePairs(collection, cls, params):
        """
        Queries the database for a DataWrapper.
        collection: The collection in which to search
        cls : string representing what subclass of DataWrapper we are querying for
        params : A dictionary containing the subset of settings being sought after
        """
        lst = exact.eXact.getDbProxy().findKeyValuePairs(collection,cls,params)
        if lst == None:
            return None
        objs = []
        for elem in lst:
            objs.append(objectifyString(elem))
        return objs
    findKeyValuePairs = staticmethod(findKeyValuePairs)
    
def objectifyString(str):
    """
    unpickles the XML passed in as the first parameter
    """
    #HACK: the unpickler seems to require the tag of each xml element is on a
    #     new line, but eXist does not return this. We should probably move
    #     this to the eXist wrapper. Or use query instead of getDocument which 
    str = str.replace(">", ">\n")
    return XML_Pickler().loads(str)

def normalizeName(str):
    return str.replace(' ', '_').replace('/', '_')
