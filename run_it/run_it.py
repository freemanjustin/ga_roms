#!/usr/bin/env python
#
#
# Usage: ./run_it.py [experiment_name - will make this directory] [generation as int] [number of members]
#
# example:
#  ./run_it.py test1 0 100
#
# will mkdir -p test1
# will mkdir -p test1/0
# will mkdir -p test1/0/0
#               test1/0/1
#                ...
#               test1/0/99
#

import sys, os, posix, commands, math, string
from time import localtime, strftime, sleep, time, mktime
from math import floor, ceil
sys.path.append('./ga_lib')
import config
from run_cmd import run_cmd
from init_generation import init_generation
from init_population import init_population
from submit_it import submit_it
from rank_population import rank_population
from evolve import evolve

# Argument variables passed

if len(sys.argv) < 4:
    sys.stderr.write('Usage: run_it [expriment_name (dir)] [generation] [population size]\n')
    sys.exit(1)

scriptname   = sys.argv[0]
exp_name     = sys.argv[1]
generation   = int( sys.argv[2] )
population   = int( sys.argv[3] )


if generation == 0:
    # init dirctory structure for this generation
    init_generation(exp_name, generation, population)
    # init directory structure for next generation
    init_generation(exp_name, generation+1, population)
    # initialize the bottom drag values for each member of the population
    init_population(exp_name, generation, population)
else:
    # init directory structure for next generation
    init_generation(exp_name, generation+1, population)


# run models
submit_it(exp_name, generation, population)

# evaluate cost function and run genetic algorithm
# calc_vrms for current generation
# rank current generation
rank_population(exp_name, generation, population)

# cross over and mutate to create next generation
evolve(exp_name, generation, populatoin)

# resubmit this script to run next generation
script = []
##initial script settings
script.append("#!/bin/bash\n")
script.append("#PBS -S /bin/bash\n")
script.append("#PBS -q express\n")
script.append("#PBS -l ncpus=1\n")
script.append("#PBS -l wd\n")
script.append("#PBS -l walltime=04:00:00\n")
script.append("#PBS -l mem=60gb\n")
script.append("#PBS -N reef%d\n" % (generation+1)) 
script.append("#PBS -P ep4\n")

script.append("cd ${PBS_O_WORKDIR}\n")
#script.append("source /home/548/jcf548/.bashrc\n")
#script.append("module load openmpi/1.10.0\n")
script.append("./run_it.py %s %d %d\n" % (exp_name, generation+1, population)  )

script_name = "ga_roms_%d.pbs" % (generation+1)
##write the script to the file
run_script = open(script_name, 'w')
run_script.writelines(script)
run_script.close()
##change the properties of the file so that it can be executed (?)
#os.chmod(script_name,0774)
# now qsub the job and exit
CMD = "qsub %s" % (script_name)
result = commands.getstatusoutput(CMD)
status = result[0]
message = result[1]
if status != 0:
	print "Error running command: " + CMD
	print message
	sys.exit(1)
else:
    print "Generation %d complete. qsub'ed generation %d..." % (generation, generation+1)
