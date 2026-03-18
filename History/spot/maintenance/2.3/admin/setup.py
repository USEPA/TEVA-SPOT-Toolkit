
import os
import sys
import subprocess
import signal

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
    trunk = len(sys.argv) > 1 and sys.argv[1] == '--trunk'
    rc=0
    if not os.path.exists('python'):
        if trunk:
            print "Installing Python from trunk"
            rc = subprocess.call(['bin/install_python','--trunk','--logfile','tpl/python.log'])
        else:
            print "Installing Python from cached packages"
            rc = subprocess.call(['bin/install_python','--logfile','tpl/python.log'])
    if rc < 0:
        return
    #rc = subprocess.call(['./bootstrap/bootstrap','all'])
    #if rc < 0:
        #return
    if trunk:
        start=2
    else:
        start=1
    if len(sys.argv) > start:
        rc = subprocess.call(['python/bin/driver']+sys.argv[start:])
    
