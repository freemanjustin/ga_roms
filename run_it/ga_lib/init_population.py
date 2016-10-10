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
    min_drag = 0.0001
    max_drag = 0.1
    gen_drag_cmd = "./init_drag %f %f %s %d %d" % (min_drag, max_drag, exp_name, generation, population)
    run_cmd(gen_drag_cmd)
    return
