import sys, os, posix, commands, math, string
from time import localtime, strftime, sleep, time, mktime
from math import floor, ceil
from run_cmd import run_cmd


def evolve(exp_name, generation, population):
    print "\n***** evol:"
    print "\t***** experiment = %s" % exp_name
    print "\t***** generation = %d" % generation
    print "\t***** population = %d" % population

    evol_cmd = "./evol %s %d %d" % (exp_name, generation, population)
    run_cmd(evol_cmd)

    print "*** done evol"
    return
