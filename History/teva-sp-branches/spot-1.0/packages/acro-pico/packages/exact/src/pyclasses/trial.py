from data import DataWrapper
#from datetime import datetime
import exceptions

class Trial(DataWrapper):
    def __init__(self, experiment, trialNum, data, createNew=1):
        """
        Initializes a new Trial object.

        N.B.: It should not be called from user code. It meant to be called by
        Experiment::createTrial or any code that can provide a valid trialNum
        """
        def __makepath(experiment, trialNum):
            return "%s/%i.xml" % (experiment.getTrialDir(), trialNum)

        if type(trialNum) is not int:
            raise exceptions.TypeError("trialNum parameter should be integer")
        
        if createNew:
            self.__trialId = trialNum
            self.__exp     = experiment        
            self.__path    = __makepath(experiment, trialNum)
#            self.__time    = str(datetime.now())
            DataWrapper.__init__(self, data)
        else:
            path   = __makepath(experiment, trialNum)
            data = DataWrapper.loadFromFile(path)
            for name, value in data.__dict__.items():
                setattr(self, name, value)
            self.__exp = experiment


    def __getattr__(self, name):
        """
        JWB TODO: document this
        """
        if len(name) or name[0] == '_':
            raise exceptions.AttributeError("%s : not found" % (name))
        else:
            try:
                return getattr(self.__data, name)
            except:
                return exceptions.AttributeError("%s : not found" % (name))

    def __setattr__(self, name, value):
        """
        JWB TODO: document this
        """
        if name[0] == '_':
            self.__dict__[name] = value
        else:
            self.__data.__dict[name] = value

    def getExperiment(self):
        """
        Return the experiment to which this trial is associated
        """
        return self.__exp

    def save(self):
	"""
	JWB TODO: Rename this to make it clear that we're saving to a DB?
        """
        exp = self.__exp
	#
	# WEH - commented out until we can resurrect the DB functionality
	#
        #self.__exp = None
        #self.saveToXml(self.__path)
        self.__exp = exp

    def load(experiment, trialNum=-1):
        """
        Returns a Trial object that has been saved on a file
        """
        return Trial(experiment, trialNum, None, 0)
    load = staticmethod(load)


    def getId(self):
	"""
	Returns the ID of the current Trial object
	"""
        return self.__trialId


 #   def getTime(self):
 #       return self.__time
