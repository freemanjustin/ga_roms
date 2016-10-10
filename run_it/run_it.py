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



# Argument variables passed

if len(sys.argv) < 4:
    sys.stderr.write('Usage: run_it [expriment_name (dir)] [generation] [population size]\n')
    sys.exit(1)

scriptname   = sys.argv[0]
exp_name     = sys.argv[1]
generation   = int( sys.argv[2] )
population   = int( sys.argv[3] )

# init dirctory structure for this generation
init_generation(exp_name, generation, population)
# init directory structure for next generation
init_generation(exp_name, generation+1, population)


if generation == 0:
    init_population(exp_name, generation, population)

# run models
submit_it(exp_name, generation, population)

# evaluate cost function and run genetic algorithm
# calc_vrms for current generation
# rank current generation
# cross over and mutate to create next generation
# resubmit this script to run next generation
