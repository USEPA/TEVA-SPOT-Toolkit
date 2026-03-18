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
