#import copy
import exact
import experiment
import exceptions
from data import DataWrapper, normalizeName

# TODO : Impove this
#-------------------
# Inheriting from DataWrapper gives us access to the facilities provided by
#    DataWrapper. Also the DataSet proper is saved in the db. So when we 
#    unpickle the saved DataSet the mangled private names are the same as
#    what we want our new DataSet object to have.
#    N.B.: when we pickle then unpickle un object it's __class__ attribute is
#    different from the __class__ of the original object. see
#    datatest.DataTests.testClassChanged
#
# After we create an experiment with a DataSet, we have to resave the DataSet
#    so that the dataset recognizes the changes that are made.
#
# warnings when we try to create a DataSet with a name that already exists
class  DataSet(DataWrapper):

    def __init__(self, dg, name, data=None, createNew=1):
        def __makename(self):
            return "_%i_" %(self.__dg.getNewDataSetId())
        
        if (createNew):
            self.__dg     = dg

            if name:
                self.__name = name
            else:
                self.__name = self.__makename()

            self.__path   = dg.getPath() + "/" + normalizeName(self.__name)
            DataWrapper.__init__(self, data)
            self.__expNum = -1
            dbProxy = self.getDbProxy()
	    #print "dataset constructor: created instance " + `self`
	    #print "dataset.py: adding collection: " + self.__path;
            dbProxy.addCollection(self.__path)
	    coll = dbProxy.getCollection(self.__path)
	    #print "dataset.py: it is object: " + `coll`
            dbProxy.addCollection(self.__path + "/experiments")
        else:
            for name, value in data.__dict__.items():
                setattr(self, name, value)
            self.__dg = dg

    def load(dg, name):
        if not name:
            raise "name expected"
        path  = "%s/%s/dataset.xml" % (dg.getPath(), normalizeName(name))
        #print "dataset.py: load: " + `dg`
        #print "dataset.py: attempting to load " + path
        #print "dataset.py: dg is " + `dg`
        dbProxy = exact.eXact.getDbProxy()
        data = DataWrapper.loadFromFile(path)
        return DataSet(dg, None, data, 0)
    load = staticmethod(load)

    def exists(dg, name):
        return DataWrapper.exists(dg.getPath() + "/" + normalizeName(name) \
                                  + "/dataset.xml");
    exists = staticmethod(exists)

    def queryForKeyValue(dg, params):
        objs = DataWrapper.findKeyValuePairs(dg.getPath(), 'DataSet', params)
        if objs == None:
            return None
        dsets = []
        for obj in objs:
            dsets.append(DataSet(dg, None, obj, 0))
        return dsets
    queryForKeyValue = staticmethod(queryForKeyValue)
    
    def getExperiment(self, expNum):
        return experiment.Experiment.load(self, expNum)

    def getExperiments(self, params={}):
        return experiment.Experiment.queryForKeyValue(self, params)

    def hasExperiment(self, params):
        if DataWrapper.hasKeyValuePairs(self.getPath()+"/experiments",
                                        'Experiment', params):
            return 1
        else:
            return 0
        
    def getDbProxy(self):
        return self.__dg.getDbProxy()


    def getNewExperimentId(self):
        self.__expNum = self.__expNum + 1
        return self.__expNum


    #BUG calling save somehow causes cascading __del__. pickling problem
    #def __del__(self):
        #self.save()
        
    def save(self):
        #HACK to remove some useless data this is saved to the database
        #dg = self.__dg			JWB
        #self.__dg = None		JWB
        #print "dataset.py: about to save " + `self`
	dg = self.__dg
        dbProxy = self.getDbProxy()
        dirname = "/db/eXist/" + dg.getName()
        coll = dbProxy.getCollection(dirname)
        self.saveToXml(self.__path + "/dataset.xml")
	#print "dataset.py: save() " + self.__path + "/dataset.xml" + " called!"
        #print "dataset.py: after save: "
        #self.__dg = dg			JWB
        dbProxy = self.getDbProxy()
        dirname = "/db/eXist/" + dg.getName()
        coll = dbProxy.getCollection(dirname)


    def createExperiment(self, params):
        """
        creates a new Experiment in this DataSet
        ALWAYS call save after creating a DataSet.
        """
        exp = experiment.Experiment(self, params)
        exp.save()
        return exp


    def getNormalizedName(self):
        return self.__name

    def getPath(self):
        return self.__path

    def getFactors(self):
        return []
