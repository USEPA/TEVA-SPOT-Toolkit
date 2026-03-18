
def print_memmon_log(cmd, logfile):
    try:
        f = open(logfile, "r")
    except IOError:
        print cmd+" failure"
        sys.exit(1)

    for line in f:
        if 'memmon:' in line:
            print cmd+" "+line
    f.close()

