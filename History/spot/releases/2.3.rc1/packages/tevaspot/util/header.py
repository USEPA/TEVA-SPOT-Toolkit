#  _________________________________________________________________________
#
#  TEVA-SPOT Toolkit: Tools for Designing Contaminant Warning Systems
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the LGPL License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the README.txt file in the top SPOT directory.
#  _________________________________________________________________________
#
# This script was created with the spot_install script.
#

import re
import urllib2
import zipfile

#
# The following taken from pkg_resources
#
component_re = re.compile(r'(\d+ | [a-z]+ | \.| -)', re.VERBOSE)
replace = {'pre':'c', 'preview':'c','-':'final-','rc':'c','dev':'@'}.get


def zip(filename,fdlist):
    zf = zipfile.ZipFile(filename, 'w')
    for file in fdlist:
        if os.path.isdir(file):
            for root, dirs, files in os.walk(file):
                for fname in files:
                    if fname.endswith('exe'):
                        zf.external_attr = (0777 << 16L) | (010 << 28L)
                    else:
                        zf.external_attr = (0660 << 16L) | (010 << 28L)
                    zf.write(join(root,fname))
        else:
            zf.write(file)
    zf.close()


def unzip(filename, dir=None):
    fname = os.path.abspath(filename)
    zf = zipfile.ZipFile(fname, 'r')
    if dir is None:
        dir = os.getcwd()
    #zf.extractall()
    for file in zf.infolist():
        name = file.filename
        #perm = ((file.external_attr >> 16L) & 0777)
        #print "HERE",perm,file.external_attr, file.external_attr >> 16L
        if name.endswith('/') or name.endswith('\\'):
            outfile = os.path.join(dir, name)
            if not os.path.exists(outfile):
                os.makedirs(outfile)
        else:
            outfile = os.path.join(dir, name)
            parent = os.path.dirname(outfile)
            if not os.path.exists(parent):
                os.makedirs(parent)
            OUTPUT = open(outfile, 'wb')
            OUTPUT.write(zf.read(name))
            OUTPUT.close()
    zf.close()


def svnjoin(*args):
    return '/'.join(args)

def _parse_version_parts(s):
    for part in component_re.split(s):
        part = replace(part,part)
        if not part or part=='.':
            continue
        if part[:1] in '0123456789':
            yield part.zfill(8)    # pad for numeric comparison
        else:
            yield '*'+part

    yield '*final'  # ensure that alpha/beta/candidate are before final

def parse_version(s):
    parts = []
    for part in _parse_version_parts(s.lower()):
        if part.startswith('*'):
            if part<'*final':   # remove '-' before a prerelease tag
                while parts and parts[-1]=='*final-': parts.pop()
            # remove trailing zeros from each series of numeric parts
            while parts and parts[-1]=='00000000':
                parts.pop()
        parts.append(part)
    return tuple(parts)
#
# pkg_resources Ends
#

#
# Use pkg_resources to guess version.
# This allows for parsing version with the syntax:
#   9.3.2
#   8.28.rc1
#
def guess_release(svndir):
    output = urllib2.urlopen(svndir).read()
    if output=="":
       return None
    links = re.findall('\<li>\<a href[^>]+>[^\<]+\</a>',output)
    versions = []
    for link in links:
        versions.append( re.split('>', link[:-5])[-1] )
    latest = None
    latest_str = None
    for version in versions:
        if version is '.':
            continue
        v = parse_version(version)
        if latest is None or latest < v:
            latest = v
            latest_str = version
    return svndir+"/"+latest_str


class Repository(object):

    easy_install = "easy_install"
    python = "python"
    svn = "svn"
    dev = []

    def __init__(self, name, root=None, trunk=None, stable=None, release=None, tag=None, pyname=None, pypi=None, dev=False):
        self.name = name
        if not root is None:
            try:
                self.trunk = root+'/trunk'
                self.trunk_root = self.trunk
            except urllib2.HTTPError:
                self.trunk = None
                self.trunk_root = None
            try:
                self.stable = guess_release(root+'/stable')
                self.stable_root = self.stable
            except urllib2.HTTPError:
                self.stable = None
                self.stable_root = None
            try:
                self.release = guess_release(root+'/releases')
                self.release_root = self.release
            except urllib2.HTTPError:
                self.release = None
                self.release_root = None
        else:
            self.trunk = ""
            self.trunk_root = None
            self.stable = ""
            self.stable_root = None
            self.release = ""
            self.release_root = None
        if not trunk is None:
            self.trunk += trunk
        if not stable is None:
            self.stable += stable
        if not release is None:
            self.release += release
        if not tag is None:
            self.release += tag
        self.pypi = pypi
        if not pypi is None:
            self.pyname=pypi
        else:
            self.pyname=pyname
        self.dev = dev
        if dev:
            Repository.dev.append(name)
        self.pkgdir = None
        self.pkgroot = None

    def install_trunk(self, dir=None, setup=True):
        if self.trunk is '':
            if not self.stable is '':
                self.install_stable(dir=dir, setup=setup)
            elif self.pypi is '':
                self.install_release(dir=dir, setup=setup)
            elif setup:
                self.easy_install()
        else:
            self.pkgdir=self.trunk
            self.pkgroot=self.trunk_root
            print "-----------------------------------------------------------------"
            print "  Checking out source for package",self.name
            print "     Subversion dir: "+self.trunk
            if os.path.exists(dir):
                print "     No checkout required"
                print "-----------------------------------------------------------------"
            else:
                print "-----------------------------------------------------------------"
                self.run([self.svn,'checkout','-q',self.trunk, dir])
            if setup:
                self.run([self.python, 'setup.py', 'develop'], dir=dir)

    def install_stable(self, dir=None, setup=True):
        if self.stable is '':
            if not self.release is '':
                self.install_release(dir=dir, setup=setup)
            elif self.pypi is '':
                self.install_trunk(dir=dir, setup=setup)
            elif setup:
                self.easy_install()
        else:
            self.pkgdir=self.stable
            self.pkgroot=self.stable_root
            print "-----------------------------------------------------------------"
            print "  Checking out source for package",self.name
            print "     Subversion dir: "+self.stable
            print "     Source dir:     "+dir
            if os.path.exists(dir):
                print "     No checkout required"
                print "-----------------------------------------------------------------"
            else:
                print "-----------------------------------------------------------------"
                self.run([self.svn,'checkout','-q',self.stable, dir])
            if setup:
                self.run([self.python, 'setup.py', 'develop'], dir=dir)

    def install_release(self, dir=None, setup=True):
        if self.release is '':
            if not self.stable is '':
                self.install_stable(dir=dir, setup=setup)
            elif self.pypi is '':
                self.install_trunk(dir=dir, setup=setup)
            elif setup:
                self.easy_install()
        else:
            self.pkgdir=self.release
            self.pkgroot=self.release_root
            print "-----------------------------------------------------------------"
            print "  Checking out source for package",self.name
            print "     Subversion dir: "+self.release
            print "     Source dir:     "+dir
            if os.path.exists(dir):
                print "     No checkout required"
                print "-----------------------------------------------------------------"
            else:
                print "-----------------------------------------------------------------"
                self.run([self.svn,'checkout','-q',self.release, dir])
            if setup:
                self.run([self.python, 'setup.py', 'install'], dir=dir)

    def update_trunk(self, dir=None):
        if self.trunk is None:
            if not self.pypi is None:
                self.easy_upgrade()
            elif not self.stable is None:
                self.update_stable()
            else:
                self.update_release()
        else:
            self.pkgdir=self.trunk
            self.pkgroot=self.trunk_root
            print "-----------------------------------------------------------------"
            print "  Updating source for package",self.name
            print "     Subversion dir: "+self.trunk
            print "     Source dir:     "+dir
            print "-----------------------------------------------------------------"
            self.run([self.svn,'update','-q',dir])
            self.run([self.python, 'setup.py', 'develop'], dir=dir)

    def update_stable(self, dir=None):
        if self.stable is None:
            if not self.pypi is None:
                self.easy_upgrade()
            elif not self.release is None:
                self.update_release()
            elif not self.trunk is None:
                self.update_trunk()
        else:
            self.pkgdir=self.stable
            self.pkgroot=self.stable_root
            print "-----------------------------------------------------------------"
            print "  Updating source for package",self.name
            print "     Subversion dir: "+self.stable
            print "     Source dir:     "+dir
            print "-----------------------------------------------------------------"
            self.run([self.svn,'update','-q',dir])
            self.run([self.python, 'setup.py', 'develop'], dir=dir)

    def update_release(self, dir=None):
        if self.release is None:
            if not self.pypi is None:
                self.easy_upgrade()
            elif not self.stable is None:
                self.update_stable()
            elif not self.trunk is None:
                self.update_trunk()
        else:
            self.pkgdir=self.release
            self.pkgroot=self.release_root
            print "-----------------------------------------------------------------"
            print "  Updating source for package",self.name
            print "     Subversion dir: "+self.release
            print "     Source dir:     "+dir
            print "-----------------------------------------------------------------"
            self.run([self.svn,'update','-q',dir])
            self.run([self.python, 'setup.py', 'install'], dir=dir)

    def sdist_trunk(self, format='zip'):
        if self.trunk is None:
            if not self.pypi is None:
                self.easy_install()
            elif not self.stable is None:
                self.sdist_stable(format=format)
            else:
                self.sdist_release(format=format)
        else:
            self.pkgdir=self.trunk
            self.pkgroot=self.trunk_root
            print "-----------------------------------------------------------------"
            print "  Checking out source for package",self.name
            print "     Subversion dir: "+self.trunk
            print "-----------------------------------------------------------------"
            self.run([self.svn,'export','-q',self.trunk, 'pkg'+self.name])
            self.run([self.python, 'setup.py', 'sdist','-q','--dist-dir=..', '--formats='+format], dir='pkg'+self.name)
            os.chdir('..')
            rmtree('pkg'+self.name)

    def sdist_stable(self, format='zip'):
        if self.stable is None:
            if not self.pypi is None:
                self.easy_install()
            elif not self.release is None:
                self.install_release()
            elif not self.trunk is None:
                self.install_trunk()
        else:
            self.pkgdir=self.stable
            self.pkgroot=self.stable_root
            print "-----------------------------------------------------------------"
            print "  Checking out source for package",self.name
            print "     Subversion dir: "+self.stable
            print "     Source dir:     "+dir
            print "-----------------------------------------------------------------"
            self.run([self.svn,'checkout','-q',self.stable, dir])
            self.run([self.python, 'setup.py', 'develop'], dir=dir)

    def sdist_release(self, dir=None):
        if self.release is None:
            if not self.pypi is None:
                self.easy_install()
            elif not self.stable is None:
                self.install_stable()
            elif not self.trunk is None:
                self.install_trunk()
        else:
            self.pkgdir=self.release
            self.pkgroot=self.release_root
            print "-----------------------------------------------------------------"
            print "  Checking out source for package",self.name
            print "     Subversion dir: "+self.release
            print "     Source dir:     "+dir
            print "-----------------------------------------------------------------"
            self.run([self.svn,'checkout','-q',self.release, dir])
            self.run([self.python, 'setup.py', 'install'], dir=dir)

    def easy_install(self):
        self.run([self.easy_install_path, '-q', self.pypi])

    def easy_upgrade(self):
        self.run([self.easy_install_path, '-q', '--upgrade', self.pypi])

    def run(self, cmd, dir=None):
        if not dir is None:
            os.chdir(dir)
        print "Running command '%s'" % " ".join(cmd)
        call_subprocess(cmd, filter_stdout=filter_python_develop, show_stdout=True)


if sys.platform.startswith('win'):
   Repository.python += '.exe'
   Repository.svn += '.exe'


#
# The following taken from PyUtilib
#
#  _________________________________________________________________________
#
#  PyUtilib: A Python utility library.
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the PyUtilib README.txt file.
#  _________________________________________________________________________
#
if (sys.platform[0:3] == "win"): #pragma:nocover
   executable_extension=".exe"
else:                            #pragma:nocover
   executable_extension=""


def search_file(filename, search_path=None, implicitExt=executable_extension, executable=False,         isfile=True):
    if search_path is None:
        #
        # Use the PATH environment if it is defined and not empty
        #
        if "PATH" in os.environ and os.environ["PATH"] != "":
            search_path = string.split(os.environ["PATH"], os.pathsep)
        else:
            search_path = os.defpath.split(os.pathsep)
    for path in search_path:
      if os.path.exists(os.path.join(path, filename)) and \
         (not isfile or os.path.isfile(os.path.join(path, filename))):
         if not executable or os.access(os.path.join(path,filename),os.X_OK):
            return os.path.abspath(os.path.join(path, filename))
      if os.path.exists(os.path.join(path, filename+implicitExt)) and \
         (not isfile or os.path.isfile(os.path.join(path, filename+implicitExt))):
         if not executable or os.access(os.path.join(path,filename+implicitExt),os.X_OK):
            return os.path.abspath(os.path.join(path, filename+implicitExt))
    return None

#
# PyUtilib Ends
#


def filter_python_develop(line):
    if not line.strip():
        return Logger.DEBUG
    for prefix in ['Searching for', 'Reading ', 'Best match: ', 'Processing ',
                   'Moving ', 'Adding ', 'running ', 'writing ', 'Creating ',
                   'creating ', 'Copying ']:
        if line.startswith(prefix):
            return Logger.DEBUG
    return Logger.NOTIFY


def localize_cmd_files(dir):
    if not sys.platform.startswith('win'):
        return
    for file in cmd_files:
        INPUT = open(join(dir,'bin',file), 'r')
        content = "".join(INPUT.readlines())
        INPUT.close()
        content = content.replace('__VIRTUAL_ENV__',dir)
        OUTPUT = open(join(dir,'bin',file), 'w')
        OUTPUT.write(content)
        OUTPUT.close()

