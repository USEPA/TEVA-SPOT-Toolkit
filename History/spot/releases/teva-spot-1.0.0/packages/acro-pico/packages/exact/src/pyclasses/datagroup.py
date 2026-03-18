import exact
import dataset
from data import DataWrapper, normalizeName
import exceptions

class DataGroup(DataWrapper):
    """
    This class is used to store information about the problem being
    investigated
    """
    def __init__(self, name, description=''):
        dbProxy           = exact.eXact().getDbProxy()
        coll              = dbProxy.getCollection()

        self.__name       = name
        self.__path       = dbProxy.getBaseCollectionName() + "/" + \
                           normalizeName(name)
        self.__description = description
        #self.__datasets    = None
        DataWrapper.__init__(self)
	#print "datagroup.py: __path: " + self.__path;
        #print "datagroup.py: coll" + `coll`;
        #print "datagroup.py: coll.collections"  + `coll.collections`;
        #print "datagroup.py: self.name"  + self.__name;

        # check to see if the datagroup exists in the database
        #if not self.__name in coll.collections:   JWB 1/3/05
        dirname = "/db/eXist/" + self.__name
        coll = dbProxy.getCollection(dirname)
        if coll == None:
	    #print "datagroup.py: adding collection: " + self.__path;
            dbProxy.addCollection(self.__path)
            self.save()
            coll = dbProxy.getCollection(dirname)
        #print "datagroup.py: " + `coll` +" "+ coll.name + " has documents "+`coll.documents`

    def __str__(self):
        return "name: " + self.__name + "\n" \
               "path: " + normalizeName(self.__name) + "/\n" \
               "descripton: " + self.__description + "\n"

    def getName(self):
	return self.__name
    
    def createDataSet(self, name, data):
        """
        Creates a DataSet instance in the current database
        data: Information to be stored in the DataSet. Ideally it should be
              a dictionay object. With keys being factors, and values being
              the settings for these factors
        """
        ds = dataset.DataSet(self, name, data)
        ds.save()
        return ds


    def getDataSet(self, name):
        """
        returns the dataset that is named name
        """
        dbProxy           = exact.eXact().getDbProxy()
        dirname = "/db/eXist/" + self.__name + "/" + name
        coll = dbProxy.getCollection(dirname)
        dirname = "/db/eXist/" + self.__name + "/" + name + "/dataset.xml"
        #print "datagroup.py: " + `self` + coll.name + " has documents "+`coll.documents`
        #print "datagroup.py: getDataSet: getting " + dirname
        #doc = dbProxy.getDocument(dirname)
        #print "datagroup.py: getDataSet: about to call load: self: " + `self`
        ds = dataset.DataSet.load(self, name)
        #print "datagroup.py: " + coll.name + " has documents "+`coll.documents`
        #print "datagroup.py: retrieved dataset: " + `ds`
	return ds

    def getDataSets(self, params={}):
        """
        returns a list of DataSets, which have certain factors and levels.
        The factor to level mapping to be used is stored in params.

        e.g.:
          dg = DataGroup("Sorting Experiment")
          ds1 = dg.createDataSet("intarray1", {"input type": "random", ...})

          ds3 = dg.getDataSet("random intarray1")
          ds4 = dg.getDataSet({"input type": "random"})
        """
        return dataset.DataSet.queryForKeyValue(self, params)

    def hasDataSet(self, name):
        """
        returns true if the DataSet with the given name exists.
        """
        return dataset.DataSet.exists(self, name)
        
    def getNormalizedName(self):
        return __name

    def save(self):
        """
        saves information associated with the DataGroup
        """
        self.saveToXml(self.__path + "/datagroup.xml")

    def getPath(self):
        """
        for internal use. Returns the DataBase path to in which the DataGroup
        is stored.
        """
        return self.__path

    def getDbProxy(self):
        """
        Gets the DataBase object used to manipulate this datagroup
        """
        return exact.eXact().getDbProxy()
