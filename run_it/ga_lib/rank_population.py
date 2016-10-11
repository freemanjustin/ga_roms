import sys, os, posix, commands, math, string
from time import localtime, strftime, sleep, time, mktime
from math import floor, ceil
from run_cmd import run_cmd


def rank_population(exp_name, generation, population):
    print "\n*** Rank population:"
    print "\t*** experiment = %s" % exp_name
    print "\t*** generation = %d" % generation
    print "\t*** population = %d" % population

    # calculate cost function for each model
    for i in range(0,population):
        rmse_cmd = "cd %s/%04d/%04d ; ../../../rmse ../../../vel_ref.nc ocean_his.nc" % (exp_name, generation, i)
        run_cmd(rmse_cmd)

    # and rank them
    # read in each rmse file
    rmse = []
    for i in range(0,population):
        rmse_filename = "%s/%04d/%04d/rmse.txt" % (exp_name, generation, i)
        rmse_file = open(rmse_filename, 'r')
        for line in rmse_file: # iterate over each line
            trash, rms = line.split() # split by whitespace
            rmse.append([i,float(rms)]) # convert from string to float

    # sort ascending by rmse value
    rmse.sort(key=lambda x: x[1])
    for i in range(0,population):
        print rmse[i]

    print "*** done rank_population"
    return
