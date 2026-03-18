# This file was created automatically by SWIG.
import utilibc
class BitArrayPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            utilibc.delete_BitArray(self.this)
    def resize(self,arg0):
        val = utilibc.BitArray_resize(self.this,arg0)
        return val
    def len(self):
        val = utilibc.BitArray_len(self.this)
        return val
    def calc_nbits(self):
        val = utilibc.BitArray_calc_nbits(self.this)
        return val
    def nbits(self):
        val = utilibc.BitArray_nbits(self.this)
        return val
    def nrefs(self):
        val = utilibc.BitArray_nrefs(self.this)
        return val
    def data(self):
        val = utilibc.BitArray_data(self.this)
        return val
    def set(self,*args):
        val = apply(utilibc.BitArray_set,(self.this,)+args)
        return val
    def reset(self,*args):
        val = apply(utilibc.BitArray_reset,(self.this,)+args)
        return val
    def flip(self,*args):
        val = apply(utilibc.BitArray_flip,(self.this,)+args)
        return val
    def put(self,arg0,arg1):
        val = utilibc.BitArray_put(self.this,arg0,arg1)
        return val
    def compare(self,arg0):
        val = utilibc.BitArray_compare(self.this,arg0.this)
        return val
    def at(self,*args):
        val = apply(utilibc.BitArray_at,(self.this,)+args)
        val = BitArrayPtr(val)
        val.thisown = 1
        return val
    def write(self,arg0):
        val = utilibc.BitArray_write(self.this,arg0.this)
        return val
    def read(self,arg0):
        val = utilibc.BitArray_read(self.this,arg0.this)
        return val
    def __call__(self,arg0):
        val = utilibc.BitArray___call__(self.this,arg0)
        return val
    def __repr__(self):
        return "<C BitArray instance>"
class BitArray(BitArrayPtr):
    def __init__(self) :
        self.this = utilibc.new_BitArray()
        self.thisown = 1




class CharStringPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            utilibc.delete_CharString(self.this)
    def resize(self,arg0):
        val = utilibc.CharString_resize(self.this,arg0)
        return val
    def len(self):
        val = utilibc.CharString_len(self.this)
        return val
    def data(self):
        val = utilibc.CharString_data(self.this)
        return val
    def nrefs(self):
        val = utilibc.CharString_nrefs(self.this)
        return val
    def set_subvec(self,arg0,arg1,arg2,*args):
        val = apply(utilibc.CharString_set_subvec,(self.this,arg0,arg1,arg2.this,)+args)
        val = CharStringPtr(val)
        return val
    def write(self,arg0):
        val = utilibc.CharString_write(self.this,arg0.this)
        return val
    def read(self,arg0):
        val = utilibc.CharString_read(self.this,arg0.this)
        return val
    def __getitem__(self,arg0):
        val = utilibc.CharString___getitem__(self.this,arg0)
        return val
    def __setitem__(self,arg0,arg1):
        val = utilibc.CharString___setitem__(self.this,arg0,arg1)
        return val
    def __repr__(self):
        return "<C CharString instance>"
class CharString(CharStringPtr):
    def __init__(self) :
        self.this = utilibc.new_CharString()
        self.thisown = 1




class BasicArray_intPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            utilibc.delete_BasicArray_int(self.this)
    def resize(self,arg0):
        val = utilibc.BasicArray_int_resize(self.this,arg0)
        return val
    def len(self):
        val = utilibc.BasicArray_int_len(self.this)
        return val
    def data(self):
        val = utilibc.BasicArray_int_data(self.this)
        return val
    def nrefs(self):
        val = utilibc.BasicArray_int_nrefs(self.this)
        return val
    def set_data(self,arg0,arg1,*args):
        val = apply(utilibc.BasicArray_int_set_data,(self.this,arg0,arg1,)+args)
        val = BasicArray_intPtr(val)
        return val
    def set_subvec(self,arg0,arg1,arg2,*args):
        val = apply(utilibc.BasicArray_int_set_subvec,(self.this,arg0,arg1,arg2.this,)+args)
        val = BasicArray_intPtr(val)
        return val
    def __getitem__(self,arg0):
        val = utilibc.BasicArray_int___getitem__(self.this,arg0)
        return val
    def __setitem__(self,arg0,arg1):
        val = utilibc.BasicArray_int___setitem__(self.this,arg0,arg1)
        return val
    def __repr__(self):
        return "<C BasicArray_int instance>"
class BasicArray_int(BasicArray_intPtr):
    def __init__(self) :
        self.this = utilibc.new_BasicArray_int()
        self.thisown = 1




class BasicArray_doublePtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            utilibc.delete_BasicArray_double(self.this)
    def resize(self,arg0):
        val = utilibc.BasicArray_double_resize(self.this,arg0)
        return val
    def len(self):
        val = utilibc.BasicArray_double_len(self.this)
        return val
    def data(self):
        val = utilibc.BasicArray_double_data(self.this)
        return val
    def nrefs(self):
        val = utilibc.BasicArray_double_nrefs(self.this)
        return val
    def set_data(self,arg0,arg1,*args):
        val = apply(utilibc.BasicArray_double_set_data,(self.this,arg0,arg1,)+args)
        val = BasicArray_doublePtr(val)
        return val
    def set_subvec(self,arg0,arg1,arg2,*args):
        val = apply(utilibc.BasicArray_double_set_subvec,(self.this,arg0,arg1,arg2.this,)+args)
        val = BasicArray_doublePtr(val)
        return val
    def __getitem__(self,arg0):
        val = utilibc.BasicArray_double___getitem__(self.this,arg0)
        return val
    def __setitem__(self,arg0,arg1):
        val = utilibc.BasicArray_double___setitem__(self.this,arg0,arg1)
        return val
    def __repr__(self):
        return "<C BasicArray_double instance>"
class BasicArray_double(BasicArray_doublePtr):
    def __init__(self) :
        self.this = utilibc.new_BasicArray_double()
        self.thisown = 1




class SimpleArray_intPtr(BasicArray_intPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def compare(self,arg0):
        val = utilibc.SimpleArray_int_compare(self.this,arg0.this)
        return val
    def write(self,arg0):
        val = utilibc.SimpleArray_int_write(self.this,arg0.this)
        return val
    def read(self,arg0):
        val = utilibc.SimpleArray_int_read(self.this,arg0.this)
        return val
    def __repr__(self):
        return "<C SimpleArray_int instance>"
class SimpleArray_int(SimpleArray_intPtr):
    def __init__(self) :
        self.this = utilibc.new_SimpleArray_int()
        self.thisown = 1




class SimpleArray_doublePtr(BasicArray_doublePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def compare(self,arg0):
        val = utilibc.SimpleArray_double_compare(self.this,arg0.this)
        return val
    def write(self,arg0):
        val = utilibc.SimpleArray_double_write(self.this,arg0.this)
        return val
    def read(self,arg0):
        val = utilibc.SimpleArray_double_read(self.this,arg0.this)
        return val
    def __repr__(self):
        return "<C SimpleArray_double instance>"
class SimpleArray_double(SimpleArray_doublePtr):
    def __init__(self) :
        self.this = utilibc.new_SimpleArray_double()
        self.thisown = 1




class IntVectorPtr(SimpleArray_intPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def plus(self,arg0,arg1):
        val = utilibc.IntVector_plus(self.this,arg0.this,arg1.this)
        return val
    def minus(self,arg0,arg1):
        val = utilibc.IntVector_minus(self.this,arg0.this,arg1.this)
        return val
    def times(self,arg0,arg1):
        val = utilibc.IntVector_times(self.this,arg0.this,arg1.this)
        return val
    def divide(self,arg0,arg1):
        val = utilibc.IntVector_divide(self.this,arg0.this,arg1.this)
        return val
    def __repr__(self):
        return "<C IntVector instance>"
class IntVector(IntVectorPtr):
    def __init__(self) :
        self.this = utilibc.new_IntVector()
        self.thisown = 1




class RealVectorPtr(SimpleArray_doublePtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def plus(self,arg0,arg1):
        val = utilibc.RealVector_plus(self.this,arg0.this,arg1.this)
        return val
    def minus(self,arg0,arg1):
        val = utilibc.RealVector_minus(self.this,arg0.this,arg1.this)
        return val
    def times(self,arg0,arg1):
        val = utilibc.RealVector_times(self.this,arg0.this,arg1.this)
        return val
    def divide(self,arg0,arg1):
        val = utilibc.RealVector_divide(self.this,arg0.this,arg1.this)
        return val
    def __repr__(self):
        return "<C RealVector instance>"
class RealVector(RealVectorPtr):
    def __init__(self) :
        self.this = utilibc.new_RealVector()
        self.thisown = 1




class CommonIOPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            utilibc.delete_CommonIO(self.this)
    def verbosity(self,arg0):
        val = utilibc.CommonIO_verbosity(self.this,arg0)
        return val
    def setDebug(self,arg0):
        val = utilibc.CommonIO_setDebug(self.this,arg0)
        return val
    def __setattr__(self,name,value):
        if name == "debug" :
            utilibc.CommonIO_debug_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "debug" : 
            return utilibc.CommonIO_debug_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C CommonIO instance>"
class CommonIO(CommonIOPtr):
    def __init__(self) :
        self.this = utilibc.new_CommonIO()
        self.thisown = 1




class RNGPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def asLong(self):
        val = utilibc.RNG_asLong(self.this)
        return val
    def reset(self):
        val = utilibc.RNG_reset(self.this)
        return val
    def asFloat(self):
        val = utilibc.RNG_asFloat(self.this)
        return val
    def asDouble(self):
        val = utilibc.RNG_asDouble(self.this)
        return val
    def __repr__(self):
        return "<C RNG instance>"
class RNG(RNGPtr):
    def __init__(self,this):
        self.this = this




class CRandVarPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def generator(self):
        val = utilibc.CRandVar_generator(self.this)
        val = RNGPtr(val)
        return val
    def __call__(self):
        val = utilibc.CRandVar___call__(self.this)
        return val
    def set_generator(self,arg0):
        val = utilibc.CRandVar_set_generator(self.this,arg0.this)
        return val
    def __repr__(self):
        return "<C CRandVar instance>"
class CRandVar(CRandVarPtr):
    def __init__(self,this):
        self.this = this




class PM_LCGPtr(RNGPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def reseed(self,arg0):
        val = utilibc.PM_LCG_reseed(self.this,arg0)
        return val
    def getSeed(self):
        val = utilibc.PM_LCG_getSeed(self.this)
        return val
    def reset(self):
        val = utilibc.PM_LCG_reset(self.this)
        return val
    def asLong(self):
        val = utilibc.PM_LCG_asLong(self.this)
        return val
    def asDouble(self):
        val = utilibc.PM_LCG_asDouble(self.this)
        return val
    def write(self,arg0):
        val = utilibc.PM_LCG_write(self.this,arg0.this)
        return val
    def read(self,arg0):
        val = utilibc.PM_LCG_read(self.this,arg0.this)
        return val
    def __repr__(self):
        return "<C PM_LCG instance>"
class PM_LCG(PM_LCGPtr):
    def __init__(self,*args) :
        self.this = apply(utilibc.new_PM_LCG,()+args)
        self.thisown = 1




class UniformPtr(CRandVarPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def low(self):
        val = utilibc.Uniform_low(self.this)
        return val
    def high(self):
        val = utilibc.Uniform_high(self.this)
        return val
    def __repr__(self):
        return "<C Uniform instance>"
class Uniform(UniformPtr):
    def __init__(self,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        self.this = apply(utilibc.new_Uniform,()+args)
        self.thisown = 1




class NormalPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "mean" :
            utilibc.Normal_mean_set(self.this,value)
            return
        if name == "stddev" :
            utilibc.Normal_stddev_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "mean" : 
            return utilibc.Normal_mean_get(self.this)
        if name == "stddev" : 
            return utilibc.Normal_stddev_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C Normal instance>"
class Normal(NormalPtr):
    def __init__(self,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        self.this = apply(utilibc.new_Normal,()+args)
        self.thisown = 1




class TriangularPtr(CRandVarPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def generator(self):
        val = utilibc.Triangular_generator(self.this)
        val = RNGPtr(val)
        return val
    def truncation(self,arg0):
        val = utilibc.Triangular_truncation(self.this,arg0)
        return val
    def __repr__(self):
        return "<C Triangular instance>"
class Triangular(TriangularPtr):
    def __init__(self,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        self.this = apply(utilibc.new_Triangular,()+args)
        self.thisown = 1




class ostreamPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def flush(self):
        val = utilibc.ostream_flush(self.this)
        return val
    def write(self,arg0):
        val = utilibc.ostream_write(self.this,arg0)
        return val
    def __repr__(self):
        return "<C ostream instance>"
class ostream(ostreamPtr):
    def __init__(self,this):
        self.this = this




class ofstreamPtr(ostreamPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def open(self,arg0):
        val = utilibc.ofstream_open(self.this,arg0)
        return val
    def close(self):
        val = utilibc.ofstream_close(self.this)
        return val
    def __del__(self):
        if self.thisown == 1 :
            utilibc.delete_ofstream(self.this)
    def __repr__(self):
        return "<C ofstream instance>"
class ofstream(ofstreamPtr):
    def __init__(self,arg0) :
        self.this = utilibc.new_ofstream(arg0)
        self.thisown = 1




class istreamPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __repr__(self):
        return "<C istream instance>"
class istream(istreamPtr):
    def __init__(self,this):
        self.this = this




class ifstreamPtr(istreamPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def open(self,arg0):
        val = utilibc.ifstream_open(self.this,arg0)
        return val
    def close(self):
        val = utilibc.ifstream_close(self.this)
        return val
    def __repr__(self):
        return "<C ifstream instance>"
class ifstream(ifstreamPtr):
    def __init__(self,arg0) :
        self.this = utilibc.new_ifstream(arg0)
        self.thisown = 1






#-------------- FUNCTION WRAPPERS ------------------

def bitwise_or(arg0,arg1,arg2):
    val = utilibc.bitwise_or(arg0.this,arg1.this,arg2.this)
    return val

def bitwise_xor(arg0,arg1,arg2):
    val = utilibc.bitwise_xor(arg0.this,arg1.this,arg2.this)
    return val

calc_filesize = utilibc.calc_filesize

CPUSeconds = utilibc.CPUSeconds

WallClockSeconds = utilibc.WallClockSeconds

ElapsedCPUSeconds = utilibc.ElapsedCPUSeconds

ElapsedWallClockSeconds = utilibc.ElapsedWallClockSeconds

InitializeTiming = utilibc.InitializeTiming

round = utilibc.round

def shuffle(arg0,*args):
    val = apply(utilibc.shuffle,(arg0.this,)+args)
    return val

def subshuffle(arg0,arg1,arg2,arg3):
    val = utilibc.subshuffle(arg0.this,arg1,arg2,arg3)
    return val

def sum(arg0):
    val = utilibc.sum(arg0.this)
    return val

def length(arg0):
    val = utilibc.length(arg0.this)
    return val

def dist(arg0,arg1):
    val = utilibc.dist(arg0.this,arg1.this)
    return val

covariance = utilibc.covariance

argmedian = utilibc.argmedian

median = utilibc.median

def concat(arg0,arg1):
    val = utilibc.concat(arg0.this,arg1.this)
    val = BasicArray_doublePtr(val)
    val.thisown = 1
    return val

def trimmed_mean(arg0,*args):
    val = apply(utilibc.trimmed_mean,(arg0.this,)+args)
    return val

def mean(arg0):
    val = utilibc.mean(arg0.this)
    return val

def var(arg0,*args):
    val = apply(utilibc.var,(arg0.this,)+args)
    return val

def mad(arg0,arg1):
    val = utilibc.mad(arg0.this,arg1.this)
    return val

rowscale = utilibc.rowscale

def argmin(arg0):
    val = utilibc.argmin(arg0.this)
    return val

def min(arg0):
    val = utilibc.min(arg0.this)
    return val

def argmax(arg0):
    val = utilibc.argmax(arg0.this)
    return val

def max(arg0):
    val = utilibc.max(arg0.this)
    return val

cout_print = utilibc.cout_print

cerr_print = utilibc.cerr_print

Discretize = utilibc.Discretize

foo = utilibc.foo

CommonIO_reset = utilibc.CommonIO_reset

CommonIO_setIORank = utilibc.CommonIO_setIORank

CommonIO_setIOFlush = utilibc.CommonIO_setIOFlush

CommonIO_rank = utilibc.CommonIO_rank

CommonIO_io_rank = utilibc.CommonIO_io_rank

CommonIO_numProcs = utilibc.CommonIO_numProcs

def CommonIO_outs():
    val = utilibc.CommonIO_outs()
    val = ostreamPtr(val)
    return val

def CommonIO_errs():
    val = utilibc.CommonIO_errs()
    val = ostreamPtr(val)
    return val

def CommonIO_ins():
    val = utilibc.CommonIO_ins()
    val = istreamPtr(val)
    return val

def CommonIO_set_streams(arg0,arg1,arg2):
    val = utilibc.CommonIO_set_streams(arg0.this,arg1.this,arg2.this)
    return val

def CommonIO_set_cout(arg0):
    val = utilibc.CommonIO_set_cout(arg0.this)
    return val

def CommonIO_set_cerr(arg0):
    val = utilibc.CommonIO_set_cerr(arg0.this)
    return val

def CommonIO_set_cin(arg0):
    val = utilibc.CommonIO_set_cin(arg0.this)
    return val

CommonIO_set_ofile = utilibc.CommonIO_set_ofile

CommonIO_begin_tagging = utilibc.CommonIO_begin_tagging

CommonIO_end_tagging = utilibc.CommonIO_end_tagging

CommonIO_flush = utilibc.CommonIO_flush

CommonIO_sync_stdio = utilibc.CommonIO_sync_stdio



#-------------- VARIABLE WRAPPERS ------------------

DataNotOwned = utilibc.DataNotOwned
AcquireOwnership = utilibc.AcquireOwnership
AssumeOwnership = utilibc.AssumeOwnership
PROBABILITY_BALANCED_TRIANGULAR = utilibc.PROBABILITY_BALANCED_TRIANGULAR
EXPECTATION_BALANCED_TRIANGULAR = utilibc.EXPECTATION_BALANCED_TRIANGULAR
TRUNCATED_TRIANGULAR = utilibc.TRUNCATED_TRIANGULAR
uglbl = utilibc.uglbl
CommonIO_std_cout = ostreamPtr(utilibc.uglbl.CommonIO_std_cout)
CommonIO_std_cerr = ostreamPtr(utilibc.uglbl.CommonIO_std_cerr)
CommonIO_std_cin = istreamPtr(utilibc.uglbl.CommonIO_std_cin)
#
# Additional python declarations defined in the file utilib_extras.py
#
CommonIO_sync_stdio()

cout = CommonIO_outs()
cerr = CommonIO_errs()
cin  = CommonIO_ins()

##
## Classes used to tie the sys.std* streams to the common_c* C++ streams 
## defined within utilib
##
class CPPcout:
  def write(self,str):
	cout << str

class CPPcerr:
  def write(self,str):
	cerr << str

class CPPcin:
  def read(self,n):
	return read_common_cin(n)

def foo2():
        res = RealVector()
        res.this = utilibc.foo()
