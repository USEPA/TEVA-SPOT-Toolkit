import trial
from data import DataWrapper
import exceptions

class Experiment(DataWrapper):
    def __init__(self, ds, data, createNew=1):
        """

        if we are not creating a new object data is the experiment number
        """
        if createNew:
            DataWrapper.__init__(self, data)
            self.__ds       = ds
            self.__uid      = ds.getNewExperimentId()
            self.__path     = Experiment.__makepath(ds, self.__uid)
            self.__trialNum = -1
	    #print "experiment.py: adding collection: " + self.getTrialDir();
            ds.getDbProxy().addCollection(self.getTrialDir())
            self.save()  #HACK ???
        else:
            for name, value in data.__dict__.items():
                setattr(self, name, value)
            self.__ds = ds

    def __makepath(ds, expNum):
        return "%s/experiments/_%i_.xml" % (ds.getPath(), expNum)
    __makepath = staticmethod(__makepath)


    def load(ds, expNum):
        if type(expNum) is not int:
            raise exceptions.TypeError("data parameter should be integer")
        path   = Experiment.__makepath(ds, expNum)
        data = DataWrapper.loadFromFile(path)
        return Experiment(ds, data, 0)
    load = staticmethod(load)


    def queryForKeyValue(ds, params):
        objs = DataWrapper.findKeyValuePairs(ds.getPath() +"/experiments",
                                             'Experiment', params)
        if objs == None:
            return None
        exps = []
        for obj in objs:
            exps.append(Experiment(ds, obj, 0))
        return exps
    queryForKeyValue = staticmethod(queryForKeyValue)

    def __getattr__(self, name):
        if len(name) or name[0] == '_':
            raise exceptions.AttributeError("%s : not found" % (name))
        else:
            try:
                return getattr(self.getData(), name)
            except:
                return exceptions.AttributeError("%s : not found" % (name))


    def __setattr__(self, name, value):
        if  name[0] == '_':
            self.__dict__[name] = value
        else:
            self.getData().__dict__[name] = value

    def save(self):
        #HACK to remove some useless data this is saved to the database
        ds = self.__ds
        self.__ds = None
        self.saveToXml(self.__path)
        self.__ds = ds

    def createTrial(self, data):
        trialObj = trial.Trial(self, self.getNewTrialId(), data)
        trialObj.save()
        return trialObj

    def getNewTrialId(self):
        self.__trialNum = self.__trialNum + 1
        return self.__trialNum
    
    def getTrial(self, trialNum):
        return trial.Trial.load(self, trialNum)

    def getTrials(self):
        coll = []
        for i in range(self.__trialNum):
            col.append(self.getTrial(i))

    def getDataSet(self):
        return self.__ds

    def getTrialDir(self):
        return  self.__path.replace(".xml", "") + "/trials"

    def getDbProxy(self):
        return self.__ds.getDbProxy()

    def getId(self):
        return self.__uid
