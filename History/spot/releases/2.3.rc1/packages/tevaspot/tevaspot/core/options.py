#
# This OptParse class provides an API that is similar to the 
# optparse.OptionParser class.  However, this class does not enforce
# strict parse semantics.  That is, it allows for parsing of 'invalid'
# options, which are ignored.  Unfortunately, this leads to ambiguities
# that require that all option with values use the --option=value syntax.
#
# TODO: when _ignore_invalid is True, then (a) disallow short options with
#       values and (b) require that long options with values employ the
#       syntax: --option=value.
#
__all__ = ['OptionParser', 'OptionGroup']

import optparse
from optparse import OptionGroup


class OptionParser(optparse.OptionParser):

    def __init__(self, *args, **kwds):
        optparse.OptionParser.__init__(self, *args, **kwds)
        self._ignore_invalid=False

    def ignore_invalid(self, val=True):
        """
        Declare that invalid options are ignored.  To work
        correctly, this option requires that all options that might
        be ignored use the syntax: --option=value.  However, this simple
        extension of OptionParser does not enforce this requirement.
        """
        self._ignore_invalid=val

    def parse_args(self, args=None, values=None):
        """
        This method provides a preprocessor for optparser.OptionParser,
        in which we eliminate invalid arguments.
        """
        tmp = self._get_args(args)
        _args = []
        self.invalid = []
        
        if self._ignore_invalid:
            for arg in tmp:
                if arg[0] is '-':
                    if self.has_option(arg.split('=')[0]):
                        _args.append(arg)
                    else:
                        self.invalid.append(arg)
                else:
                    _args.append(arg)
        else:
            _args=tmp

        return optparse.OptionParser.parse_args(self, args=_args,values=values)
                
