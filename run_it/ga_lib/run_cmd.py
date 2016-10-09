import sys, os, posix, commands, math, string

# Define function to execute shell commands
def run_cmd(cmd):
    print cmd
    ret = commands.getstatusoutput(cmd)
    status = ret[0]
    message = ret[1]
    if status != 0:
        print "Error running command: " + cmd
        print message
        sys.exit(1)
