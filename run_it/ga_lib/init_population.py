import sys, os, posix, commands, math, string
from time import localtime, strftime, sleep, time, mktime
from math import floor, ceil
from run_cmd import run_cmd

def init_population(exp_name, generation, population):
    print "Initialize population:"
    print "\texperiment = %s" % exp_name
    print "\tgeneration = %d (should be zero!)" % generation
    print "\tpopulation = %d\n" % population

    # create population size random bottom drag scenarios

    return
