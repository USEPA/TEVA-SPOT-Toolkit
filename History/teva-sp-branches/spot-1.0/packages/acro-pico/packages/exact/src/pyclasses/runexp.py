#
# A python module of classes for reading in experimental study files
# and experimental results files.
#
# Note: experimental results are associated with the Experiment object.
#

import math
import os
import types
import sys
from xml.dom import minidom, Node

#
# BUGS/ISSUES
#
# 1. These classes parse an XML file.  Should we test the overall syntax
# 	of the file first?  I think there are XML files to do that if we
#	have an XML style sheet.
#
# 2. These class extract just a subset of the XML information that might
#	be in a file.  If 'extra' information is provided, is that an error?
#	Should we capture that 'extra' information in a standard manner?
#	If so, we should probably have these classes inherit from a base
#	class that defines generic XML information (attributes, text, etc)
#	
# 3. The semantics of how this class differs from the Pickled XML used in
#	EXACT may be unclear to the user.  In both cases, we refer to how
#	these classes are 'loaded' from XML.
#
# 4. I made a design decision to try to make these classes naturally follow
#	from the XML specification used with 'runexp'.  Thus, we need
#	to reconcile how these scripts relate to the EXACT EXIST database.
#
# 5. Should we rename 'analyze' to 'analysis'?  That seems a better naming
#	convention.  But perhaps the question is whether the 'analyze' block
#	contains multiple tests, or whether we should have a separate
#	'analysis' block for each test/analysis that we want to do... Hmmm.
#
# 6. Make I/O controlable at a global level, to enable a 'quiet' mode.
#
# 7. The XML specification of a 'results' file should be changed to make it
#	more specific to the results for a _single_ experiment.
#
# 8. Do we need to have the experimental trial store the type of the 
#    data elements?
#


def get_text(node):
  nodetext = ""
  for child in node.childNodes:
    if child.nodeType == Node.TEXT_NODE:
       nodetext = nodetext + child.nodeValue
  return nodetext.strip()



#
# This class illustrates how we can define objects to perform
# analysis.  The 'validation' analysis is the simplest form.  It verifies
# that the specified measurement has a given value (within a specified
# tolerance).
#
class ValidationAnalysis(object):
    def __init__(self):
	"""
	An object that can analyze an experiment to validate one
	or more values in the experiment.
	"""
	self.measurement=None
	self.value=None
	self.tolerance=None
	self.results={}
	#
	# sense = 1  if minimizing
	# sense = -1 if maximizing
	# sense = 0  if looking for an absolute tolerance level
	#
	self.sense=1.0

    def analyze(self, results):
	"""
	Analyze an experiment.
	"""
	for combination in results:
	  status = "Pass"
	  for trial in combination:
	    if not self.validate(trial[self.measurement]):
	       status = "Fail"
	       break
	  if status == "Pass":
	     sys.stdout.write(".")
	  else:
	     sys.stdout.write("F")
	  self.results[combination.name] = status

    def validate(self, tvalue):
	if (tvalue[1] == "ERROR") or\
	   (tvalue[1] == ""):
	   return False
	if (tvalue[0] == "numeric/double"):
	   if (self.sense == 0) and\
	      (math.abs(tvalue[1]-self.value) > self.tolerance):
	      return False
	   if (self.sense != 0) and\
	      ((self.sense*(tvalue[1]-self.value)) > self.tolerance):
	      return False
	if ((tvalue[0] == "numeric/boolean") or (tvalue[0] == "")) and\
	   (tvalue[1] != self.value):
	   return False
	return True



class ExperimentalTrial(object):
    def __init__(self, node=None):
	"""
	An object that contains experimental results for a single 
	trial
	"""
	self.reset()
	if node:
	   self.initialize(node)


    def reset(self):
	self.id = -1
	self.seed = -1
	self.value = {}


    def initialize(self, node):
	self.reset()
	for (name,value) in node.attributes.items():
	  if name == "id":
	     self.id = value
	  elif name == "seed":
	     self.seed = value
	for child in node.childNodes:
	  if child.nodeType == Node.ELEMENT_NODE and child.nodeName == "Value":
	     vname = "unknown"
	     vtype = "unknown"
	     for (name,value) in child.attributes.items():
	       if name == "name":
	          vname = value
	       elif name == "type":
	          vtype = value
	     if vtype == "numeric/double" or vtype == "numeric/integer" or\
		vtype == "numeric/boolean":
	        self.value[vname] = (vtype, eval(get_text(child)))
	     else:
	        self.value[vname] = (vtype, get_text(child))


    def pprint(self):
	print "      id=" + self.id + " seed=" + self.seed,
	vkeys = self.value.keys()
	vkeys.sort()
	for vname in vkeys:
	  print vname + "=" + (self.value[vname])[1],
        print ""


    def __getitem__(self,index):
	if index == "seed":
	   return self.seed
	elif index == "id":
	   return self.id
	return self.value[index]




class TreatmentCombination(object):
    def __init__(self, node=None):
	"""
	An object that contains experimental results for a single 
	treatment combination (i.e. combination of factor levels)
	"""
	self.reset()
	if node:
	   self.initialize(node)


    def reset(self):
	self.name = "unknown"
	self.start_time = ""
	self.run_time = -1.0
	self.expparams = ""
	self.level = {}
	self.replicationlist = []
	self.key = None
	self.status = "Fail"


    def initialize(self, node):
	"""
	Initialize a treatment condition with a node of an XML parsed tree.
	"""
	self.reset()
	for child in node.childNodes:
	  if child.nodeType == Node.ELEMENT_NODE:
	     if child.nodeName == "StartTime":
		self.start_time = get_text(child)

	     elif child.nodeName == "Name":
		self.name = get_text(child)

	     elif child.nodeName == "RunTime":
		self.run_time = get_text(child)

	     elif child.nodeName == "ExecutionStatus":
		self.status = get_text(child)

	     elif child.nodeName == "Description":
		self.expparams = get_text(child)
		for fnode in child.childNodes:
		  if fnode.nodeType == Node.ELEMENT_NODE and\
		     fnode.nodeName == "Factor":
		     #
		     # Setup a diagnostic text string
		     #
		     self.expparams = self.expparams + " " + get_text(fnode)
		     #
		     # Get the factor levels
		     #
		     fname = ""
		     flevel = -1
		     for (name,value) in fnode.attributes.items():
		       if name == "name":
			  fname = value
		       elif name == "level":
			  flevel = value
		     self.level[fname] = flevel

	     elif child.nodeName == "Trial":
		self.replicationlist = self.replicationlist + [ExperimentalTrial(child)]


    def numReplications(self):
	return len(self.replications)


    def __iter__(self):
	"""
	Returns the iterator to the replication list.  This method allows
	a TreatmentCombinations() object to be treated as the generator for the 
	iterator of the list of ExperimentalTrial().
	"""
	return self.replicationlist.__iter__()


    def pprint(self):
	print "    Treatment Combination: " + self.name
	print "      Start Time:          " + self.start_time
	print "      Run Time:            " + self.run_time
	print "      Status:              " + self.status
	print "      Levels:"
	for name in self.level.keys():
	  print "        " + name + "\t" + self.level[name]
	print "      Exp Params:          " + self.expparams
	print "      Trials:"
	for trial in self.replicationlist:
	  trial.pprint()




class ExperimentalResults(object):
    def __init__(self, node=None):
        """
        An object that describes experimental results.
        """
	self.reset()
	if node:
	   self.initialize(node)


    def reset(self):
	self.combinations = []


    def initialize(self, node):
	"""
	Initialize the experimental results with a node of an XML parsed tree.
	"""
	self.reset()
	for child in node.childNodes:
	  if child.nodeType == Node.ELEMENT_NODE and child.nodeName == "Experiment":
	     self.combinations = self.combinations + [TreatmentCombination(child)]


    def __iter__(self):
	"""
	Returns the iterator to the combination list.  This method allows
	a ExperimentalResults() object to be treated as the generator for the 
	iterator of the list of ExperimentalCombination().
	"""
	return self.combinations.__iter__()


    def pprint(self):
	for combination in self.combinations:
	  combination.pprint()



class FactorLevel(object):
    def __init__(self, node=None):
        """
        An object describes a factor level for an experiment.  This is
	simply a list of FactorLevel objects
        """
	self.reset()
	if node:
	   if type(node) == types.StringType:
		self.text = node
	   else:
		self.initialize(node)


    def reset(self):
	self.attr = {}
	self.name = "unknown"
	self.text = ""


    def initialize(self, node):
	"""
	Initialize the factor level with a node of an XML parsed tree.
	"""
	self.reset()
	for (name,value) in node.attributes.items():
	  self.attr[name] = value
	if self.attr.has_key("name"):
	   self.name = self.attr["name"]
	for cnode in node.childNodes:
          if cnode.nodeType == Node.TEXT_NODE:
             self.text = self.text + cnode.nodeValue
        self.text.strip()



class Factor(object):
    def __init__(self, node=None, number=None):
        """
        An object describes a factor for an experiment.  This is
	simply a list of FactorLevel objects
        """
	self.reset()
	if node and number:
	   self.initialize(node,number)


    def reset(self):
	self.levels = []
	self.attr = {}
	self.name = "unknown"
	self.number = -1


    def initialize(self, node, number):
	"""
	Initialize the factor with a node of an XML parsed tree.
	"""
	self.reset()
	self.number = number
	for (name,value) in node.attributes.items():
	  self.attr[name] = value
	if self.attr.has_key("name"):
	   self.name = self.attr["name"]
	if self.attr.has_key("filename"):
	   #
	   # Read factor levels from a file
	   #
	   read_factor_levels(self.attr["filename"])
  	   INPUT = open(self.attr["filename"])
  	   for line in INPUT.xreadlines():
    	     if line[0] == "#":
       	       continue
    	     self.levels = self.levels + [ FactorLevel(line.strip)] 
  	   INPUT.close()
	else:
	   #
	   # Initialize factor levels from nodes of an XML parsed tree.
	   #
  	   for cnode in node.childNodes:
    	     if cnode.nodeType == Node.ELEMENT_NODE:
       	        self.levels = self.levels + [ FactorLevel(cnode) ] 


    def pprint(self):
	print "    Factor " + self.name + " (" + `self.number` + ")"
	for level in self.levels:
	  print "      Level: ",
	  if level.name != "unknown":
	     print level.name
	  else:
	     print ""
	  print "        \"" + level.text + "\""



class Factors(object):
    def __init__(self, node=None):
        """
        An object that describes the factors for an experiment.  This is
	simply a list of Factor objects
        """
        self.debug = False
	self.reset()
	if node:
	   self.initialize(node)


    def reset(self):
	self.factorlist = []


    def initialize(self, node):
	"""
	Initialize the factor list with a node of an XML parsed tree.
	"""
	self.reset()
  	for cnode in node.childNodes:
    	  if cnode.nodeType == Node.ELEMENT_NODE:
       	     self.factorlist = self.factorlist + [ Factor(cnode,len(self.factorlist)+1) ] 


    def __iter__(self):
	"""
	Returns the iterator to the factor list.  This method allows
	a Factors() object to be treated as the generator for the 
	iterator of the list of Factors().
	"""
	return self.factorlist.__iter__()




class Controls(object):
    def __init__(self, node=None):
        """
        An object that describes the controls for an experiment.
        """
	self.reset()
	if node:
	   self.initialize(node)


    def reset(self):
	self.seeds = []


    def initialize(self, node):
	"""
	Initialize the experimental controls with a node of an XML parsed tree.
	"""
	self.reset()
	#
	# TODO: more stuff here later
	#



class Experiment(object):
    def __init__(self, node=None, expnum=None, path=None, expname=None):
	"""
	An experiment object, which contains the factors for an
	experiment as well as the experimental results.
	"""
	if node and expnum:
	   self.initialize(node,expnum)
	if path:
	   try:
	      self.readResults(path, expname)
	   except IOError:
	      #
	      # TODO: resolve error handling semantics.
	      #
	      # The default behavior is to try to read in an
	      # experimental results file.  It shouldn't be an error
	      # if this file doesn't exist, but we should be more careful
	      # with our error management here.  The file could have existed,
	      # but we had an error parsing/processing it!
	      #
	      pass


    def reset(self):
	self.name = "unknown"
	self.factors = None
	self.controls = None
	self.results = None


    def initialize(self, node,expnum):
	"""
	Initialize the experiment with a node of an XML parsed tree.
	"""
	self.reset()
	#
	# Setup experiment name
	#
	for (name,value) in node.attributes.items():
	  if name == "name":
	     self.name = value
	if self.name == "unknown":
	   self.name = "exp" + `expnum`
	#
	# Process factors and controls
	#
  	for cnode in node.childNodes:
    	  if cnode.nodeType == Node.ELEMENT_NODE:
	     if cnode.nodeName == "factors":
       	        self.factors = Factors(cnode)
	     elif cnode.nodeName == "controls":
		self.controls = Controls(cnode)


    def readResults(self, path, studyname):
	fname = path + studyname + "." + self.name + ".results.xml"
	if not os.path.exists(fname):
	   raise IOError, "Unable to load file " + fname
	print "  ... loading results file " + fname
        doc = minidom.parse(fname)
	self.results = ExperimentalResults(doc.documentElement)
	if self.results == None:
	   print "FOO"


    def pprint(self):
	print "  Factors: " + `len(self.factors.factorlist)`
	for factor in self.factors:
	  factor.pprint()
	print "  Results:"
	if self.results == None:
	   print "BOO"
	if self.results:
	   self.results.pprint()
	else:
	   print "    None"
	
	


class Analysis(object):
    def __init__(self, node=None):
	"""
	An experimental analysis object.
	"""
	if node:
	   self.initialize(node)


    def reset(self):
	self.name = "unknown"


    def initialize(self, node):
	"""
	Initialize the analysis with a node of an XML parsed tree.
	"""
	self.reset()
	for (name,value) in node.attributes.items():
	  if name == "name":
	     self.name = value
	#
	# TODO: more here later
	#


    def pprint(self):
	print "  TODO"
		


#
# Note: this object could be initialize with a node from an XML tree,
# or with a filename, or with no argument at all
#
# Note: when an experimental study is created, results are loaded 
# if they are found.  Otherwise, an explicit 'readResults()' needs to
# be called when results are generated after this object is constructed.
#
class ExperimentalStudy(object):
    def __init__(self, node=None):
	"""
	An experimental study object, which contains one or more 
	experiments and experimental analyses.
	"""
	if node:
	   self.initialize(node)


    def reset(self):
	self.name = "study"
	self.experiments = {}
	self.analyses = {}
	self.path = "./"



    def initialize(self, node, performReset=True):
	"""
	Initialize the study with either a filename or a node of an 
	XML parsed tree.

	This method calls itself recursively, so a flag is passed to indicate
	whether the object will be reset.
	"""
	if performReset:
	   self.reset()
	if type(node) == types.StringType:
	   #
	   # Load in from a file
	   #
	   if not os.path.exists(node):
              print "ERROR: missing file " + node
              sys.exit(1)
	   self.path = os.path.dirname(node)
	   if self.path == "":
	      self.path = "./"
	   else:
	      self.path = self.path + "/"
           doc = minidom.parse(node)
	   self.initialize(doc.documentElement,False)
	else:
	   #
	   # Load in from an XML node
	   #
	   for (name,value) in node.attributes.items():
	     if name == "name":
	        self.name = value
  	   for cnode in node.childNodes:
    	     if cnode.nodeType == Node.ELEMENT_NODE:
	        if cnode.nodeName == "experiment":
 		   exp = Experiment(cnode, len(self.experiments)+1, self.path, self.name)
       	           self.experiments[exp.name] = exp
	        elif cnode.nodeName == "analyze":
		   analysis = Analysis(cnode)
		   self.analyses[analysis.name] = analysis


    def readResults(self):
	"""
	Read results from an XML file.
	"""
	for exp in experiments:
	  try: 
	     exp.readResults(self.path,self.name)
	  except IOError:
	     dummy=1 


    def pprint(self):
	"""
	Print out the information in the experimental study.
	"""
	print "Experimental Study: " + self.name
	print "  Experiments: " + `len(self.experiments)`
	print "  Analyses:    " + `len(self.analyses)`
	print ""
	expkeys = self.experiments.keys()
	expkeys.sort()
	for name in expkeys:
	  print "Experiment: " + name
	  self.experiments[name].pprint()
	  print ""
	for (name,analysis) in self.analyses.iteritems():
	  print "Analysis:   " + name
	  analysis.pprint()



#
# Load the UTILIB experimental results and print out the information
#
study = ExperimentalStudy("../../../utilib/test/utilib.exp.xml")
#study.pprint()
#
# Perform an analysis
#
analysis = ValidationAnalysis()
analysis.measurement = "diffs"
analysis.value = 0
analysis.analyze(study.experiments["exp1"].results)
