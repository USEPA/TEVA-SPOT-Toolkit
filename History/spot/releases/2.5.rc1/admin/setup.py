
import os
import sys
import subprocess
import signal

admin_dir = os.path.dirname(os.path.abspath(__file__))
packages=['-p', '%s/tpl/acro/admin/acro-admin', '-p', '%s/admin/spot-admin', '-p', '%s/tpl/acro/tpl/cxxtest/python']

def signal_handler(signum, frame):
    pid=os.getpid()
    pgid=os.getpgid(pid)
    if pgid == -1:
        print "  ERROR: invalid pid",pid
        sys.exit(1)
    os.killpg(pgid,signal.SIGTERM)
    sys.exit(1)

signal.signal(signal.SIGINT,signal_handler)
if sys.platform[0:3] != "win":
    signal.signal(signal.SIGHUP, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)


def run():
    trunk = (len(sys.argv) > 1 and '--trunk' in sys.argv)
    if '--site-packages' in sys.argv:
        print "Installing with Python site packages"
        site_packages=['--site-packages']
    else:
        site_packages=[]
    rc=0
    if not os.path.exists('python'):
        if trunk:
            print "Installing Python from trunk"
            rc = subprocess.call(['tpl/vpykit/bin/install_python','-c',admin_dir+'/vpy/dev.ini']+site_packages+packages+['--trunk','--logfile','tpl/python.log'])
        else:
            print "Installing Python from cached packages"
            rc = subprocess.call(['tpl/vpykit/bin/install_python','-c',admin_dir+'/vpy/dev.ini','-z',admin_dir+'/vpy/python.zip']+site_packages+packages+['--logfile','tpl/python.log'])
    if rc != 0:
        sys.exit(rc)
    #rc = subprocess.call(['./bootstrap/bootstrap','all'])
    #if rc != 0:
        #sys.exit(rc)
    if trunk:
        start=2
    else:
        start=1
    if len(site_packages) == 1:
        start += 1
    if len(sys.argv) > start:
        rc = subprocess.call(['python/bin/driver']+sys.argv[start:])
    sys.exit(rc)
    
