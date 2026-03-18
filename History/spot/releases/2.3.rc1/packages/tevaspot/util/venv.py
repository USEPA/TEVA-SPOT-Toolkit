import re
import urllib2
import shutil
import string
import textwrap
import sys
import glob

wrapper = textwrap.TextWrapper(subsequent_indent="    ")


def extend_parser(parser):
    parser.add_option('--trunk',
        help='Install trunk branches of Python software.',
        action='store_true',
        dest='trunk',
        default=False)

    parser.add_option('--stable',
        help='Install stable branches of Python software.',
        action='store_true',
        dest='stable',
        default=False)

    parser.add_option('--update',
        help='Update all Python packages in the SPOT installation.',
        action='store_true',
        dest='update',
        default=False)

    parser.add_option('--proxy',
        help='Set the HTTP_PROXY environment with this option.',
        action='store',
        dest='proxy',
        default=None)

    parser.add_option('--preinstall',
        help='Prepare an installation that will be used to build a MS Windows installer.',
        action='store_true',
        dest='preinstall',
        default=False)

    parser.add_option('--zip',
        help='Add ZIP files that are use define this installation.',
        action='append',
        dest='zip',
        default=[])

    parser.add_option('--forum-pkg',
        help='Use one or more packages from the Coopr Forum.  Multiple packages are specified with a comma-separated list.',
        action='store',
        dest='forum',
        default='')

    parser.add_option('--forum-dev',
        help="Explicitly indicate which of the Coopr Forum packages are treated as development packages.  By default, no Coopr Forum packages are treated this way; packages omited from this list are installed from their latest 'tags' branch.  Multiple packages are specified with a comma-separated list.",
        action='store',
        dest='forumdev',
        default='')

    parser.add_option('--use-pythonpath',
        help="By default, the PYTHONPATH is ignored when installing SPOT.  This option allows the 'easy_install' tool to search this path for related Python packages, which are then installed.",
        action='store_true',
        dest='use_pythonpath',
        default=False)

    parser.add_option(
        '--site-packages',
        dest='no_site_packages',
        action='store_false',
        help="Give access to the global site-packages dir to the virtual environment",
        default=True)

    #
    # Change the virtualenv options
    #
    parser.get_option("--python").help = "Specify the Python interpreter to use, e.g., --python=python2.5 will install with the python2.5 interpreter."
    parser.get_option("--clear").help = "Remove the existing SPOT installation and reinstall from scratch."
    parser.remove_option("--relocatable")
    parser.remove_option("--version")
    parser.remove_option("--unzip-setuptools")
    parser.remove_option("--no-site-packages")
    #
    # Add description of spot_install
    #
    parser.description="The spot_install script manages the installation of the TEVA-SPOT Toolkit package."
    parser.epilog="If DEST_DIR is not specified, then spot_install will search the user's PATH environment for the 'sp' command.  If found, the path of this command will be used to identify the path of the SPOT installation.  If not found, then a default installation path is used:  C:\\tevaspot on Windows and ./tevaspot on Linux.  This command uses the Python 'setuptools' package to install Python packages.  This package installs packages by downloading files from the internet.  If you are running this from within a firewall, you may need to set the HTTP_PROXY environment variable to a value like 'http://<proxyhost>:<port>.'"
        

def adjust_options(options, args):
    global logger
    verbosity = options.verbose - options.quiet
    logger = Logger([(Logger.level_for_integer(2-verbosity), sys.stdout)])
    #
    # Indicate that updates are occuring, even if it's just specific packages
    #
    #if len(options.update_packages) > 0:
        #options.update = True
    #
    # The default installation directory is 'tevaspot'
    #
    if options.update and (options.stable or options.trunk):
        logger.fatal("ERROR: cannot specify --stable or --trunk when specifying the --update option.")
        sys.exit(1000)
    if len(args) > 2:
        logger.fatal("ERROR: spot_install can only have one argument")
        sys.exit(1000)
    #
    # Figure out the installation directory
    #
    if len(args) == 0:
        path = search_file('sp')
        if path is None or options.preinstall:
            #
            # The 'sp' script is not on the user's path, so install SPOT in a
            # default location.
            #
            if sys.platform == 'win32':
                home_dir = default_w32dir
            else:
                home_dir = default_unixdir
        else:
            home_dir = os.path.dirname(os.path.dirname(path))
        args.append( home_dir )
    else:
        home_dir = args[0]
    home_dir = os.path.abspath(home_dir)
    #
    # Error checking
    #
    if not options.preinstall and (os.path.exists(args[0]) ^ options.update):
        if options.update:
            logger.fatal(wrapper.fill("ERROR: The 'update' option is specified, but the installation path '%s' does not exist!" % args[0]))
            sys.exit(1000)
        elif options.clear is None and os.path.exists(join(args[0],'python')):
                logger.fatal(wrapper.fill("ERROR: The installation path '%s' already exists!  Use the --update option if you wish to update this SPOT installation, or use --clear to create a fresh installation." % args[0]))
                sys.exit(1000)
    if options.update:
        print "Updating existing SPOT installation in '%s'" % args[0]
    else:
        print "Starting fresh SPOT installation in '%s'" % args[0]
    #
    # Open up zip files
    #
    for file in options.zip:
        unzip(file, dir=home_dir)
    #
    # Setup HTTP proxy
    #
    if not options.proxy is None:
        os.environ['HTTP_PROXY'] = options.proxy
    print "  using the HTTP_PROXY environment: %s" % os.environ.get('HTTP_PROXY',"''")
    print ""
    #
    # Disable the PYTHONPATH, to isolate this installation from 
    # other Python installations that a user may be working with.
    #
    if not options.use_pythonpath:
        try:
            del os.environ["PYTHONPATH"]
        except:
            pass
    global sw_packages
    sw_packages = get_repositories()
    dev = options.forumdev.split(',')
    for pkg in options.forum.split(','):
        if pkg is '':
            continue
        if pkg in dev:
            if (options.trunk or options.stable):
                sw_packages[pkg] = 'http://coopr-forum.googlecode.com/svn/'+pkg
            else:
                sw_packages[pkg] = '-f http://coopr-forum.googlecode.com/svn/'+pkg+'/trunk '+pkg
            dev_packages.append(pkg)
        else:
            try:
                sw_packages[pkg] = guess_release('http://coopr-forum.googlecode.com/svn/'+pkg+'/dev/')
            except Exception, err:
                print "-----------------------------------------------------------------"
                print "ERROR Finding 'tags' branch for Coopr Forum Package %s: %s" % (pkg,str(err))
                print "-----------------------------------------------------------------"
    #
    # If --preinstall is declared, then we remove the directory, and prepare a ZIP file
    # that contains the full installation.
    #
    if options.preinstall:
        print "-----------------------------------------------------------------"
        print " STARTING preinstall in directory %s" % home_dir
        print "-----------------------------------------------------------------"
        rmtree(home_dir)
        os.mkdir(home_dir)
    #
    # If we're clearing the current installation, then remove a bunch of
    # directories
    #
    elif options.clear:
        path = join(home_dir,'src')
        if os.path.exists(path):
            rmtree(path)
    #
    # Setup the 'admin' directory
    #
    if not os.path.exists(home_dir):
        os.mkdir(home_dir)
    if not os.path.exists(join(home_dir,'admin')):
        os.mkdir(join(home_dir,'admin'))
    if options.update:
        INPUT=open(join(home_dir,'admin',"virtualenv.cfg"),'r')
        options.trunk = INPUT.readline().strip() != 'False'
        options.stable = INPUT.readline().strip() != 'False'
        INPUT.close()
    else:
        OUTPUT=open(join(home_dir,'admin',"virtualenv.cfg"),'w')
        print >>OUTPUT, options.trunk
        print >>OUTPUT, options.stable
        OUTPUT.close()
    #
    # Setup package directories
    #
    if not os.path.exists(join(home_dir,'dist')):
        os.mkdir(join(home_dir,'dist'))
    if not os.path.exists(home_dir+os.sep+"src"):
        os.mkdir(home_dir+os.sep+"src")
    if not os.path.exists(home_dir+os.sep+"bin"):
        os.mkdir(home_dir+os.sep+"bin")
    #
    # Get source packages
    #
    for pkg in sw_packages:
        if pkg.dev:
            tmp = join(home_dir,'src',pkg.name)
        else:
            tmp = join(home_dir,'dist',pkg.name)
        if options.trunk:
            if not options.update:
                pkg.install_trunk(dir=tmp, setup=False)
        elif options.stable:
            if not options.update:
                pkg.install_stable(dir=tmp, setup=False)
        else:
            if not options.update:
                pkg.install_release(dir=tmp, setup=False)
    if options.update or not os.path.exists(join(args[0],'doc')):
        install_dirs(home_dir, options)
    #
    # Create a README.txt file
    #
    OUTPUT=open(join(home_dir,"README.txt"),"w")
    print >>OUTPUT, README
    OUTPUT.close()
    #
    # Finalize preinstall
    #
    if options.preinstall:
        print "-----------------------------------------------------------------"
        print " FINISHED preinstall in directory %s" % home_dir
        print "-----------------------------------------------------------------"
        os.chdir(home_dir)
        zip('tevaspot.zip', ['.'])
        sys.exit(0)

        
def after_install(options, home_dir):
    home_dir = os.path.abspath(home_dir)
    #
    # Set the bin directory
    #
    if os.path.exists(home_dir+os.sep+"Scripts"):
        bindir = join(home_dir,"Scripts")
    else:
        bindir = join(home_dir,"bin")
    Repository.easy_install_path = os.path.abspath(join(bindir, 'easy_install'))
    Repository.python = os.path.abspath(join(bindir, 'python'))
    #
    # Install the related packages
    #
    for pkg in sw_packages:
        if pkg.dev:
            srcdir = join(home_dir,'src',pkg.name)
        else:
            srcdir = join(home_dir,'dist',pkg.name)
        if options.trunk:
            if options.update:
                pkg.update_trunk(dir=srcdir)
            else:
                pkg.install_trunk(dir=srcdir)
        elif options.stable:
            if options.update:
                pkg.update_stable(dir=srcdir)
            else:
                pkg.install_stable(dir=srcdir)
        else:
            if options.update:
                pkg.update_release(dir=srcdir)
            else:
                pkg.install_release(dir=srcdir)
    #
    # Copy the <env>/Scripts/* files into <env>/bin
    #
    if os.path.exists(home_dir+os.sep+"Scripts"):
        for file in glob.glob(home_dir+os.sep+"Scripts"+os.sep+"*"):
            shutil.copy(file, home_dir+os.sep+"bin")
    localize_cmd_files(home_dir)
    #
    # Misc notifications
    #
    if not options.update:
        print ""
        print "-----------------------------------------------------------------"
        print "  Add %s to the PATH environment variable" % (home_dir+os.sep+"bin")
        print "-----------------------------------------------------------------"
    print ""
    print "Finished SPOT installation in '%s'" % home_dir
