
__all__ = ['Component']

from options import OptionParser


class Component(object):

    def __init__(self):
        self._parser_group = {}
        self._create_parser_groups()
        self._create_parser()

    def set_options(self, args):
        [self.options, args] = self.parser.parse_args(args)
        self._process_options()

    def _process_options(self):
        pass

    def _create_parser_groups(self):
        self.parser = OptionParser()

    def _create_parser(self):
        """
        Create the OptionParser object and populated it with option groups.
        """
        self.parser.ignore_invalid()
        for key in self._parser_group:
            self.parser.add_option_group(self._parser_group[key])
        
