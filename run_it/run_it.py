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
from run_cmd import run_cmd
from init_generation import init_generation
from init_population import init_population

# config variables for the ROMS runs
# roms bin data is where the input text files live
roms_bin_data = "/short/ep4/jcf548/ga_roms/roms_bin_data/"

# roms static data is where the forcing and bc data is
roms_static_data = "/short/ep4/jcf548/ga_roms/roms_static_data/"

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
# run_roms()

# evaluate cost function and run genetic algorithm
# calc_vrms for current generation
# rank current generation
# cross over and mutate to create next generation
# resubmit this script to run next generation
