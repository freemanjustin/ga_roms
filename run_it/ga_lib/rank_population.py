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

    # and write it out to file in the current generation directory
    rmse_file_name = "%s/%04d/all_rmse.txt" % (exp_name, generation)
    rmse_file = open(rmse_file_name, 'w')
    for i in range(0,population):
        print rmse[i]
        rmse_file.write("%d %f\n" % (rmse[i][0], rmse[i][1]))
    rmse_file.close()

    print "*** done rank_population"
    return
