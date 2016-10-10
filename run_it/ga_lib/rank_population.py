import sys, os, posix, commands, math, string
from time import localtime, strftime, sleep, time, mktime
from math import floor, ceil
from run_cmd import run_cmd


def rank_population(exp_name, generation, population):
    print "\nRank population:"
    print "\texperiment = %s" % exp_name
    print "\tgeneration = %d" % generation
    print "\tpopulation = %d\n" % population

    # calculate cost function for each model
    for i in range(0,population):
        rmse_cmd = "cd %s/%04d/%04d ; ../../../rmse ../../../vel_ref.nc ocean_his.nc" % (exp_name, generation, i)
        run_cmd(rmse_cmd)

    # and rank them
    #rank_pop_cmd = "./rank_pop %s %d %d" % (exp_name, generation, population)
    #run_cmd(gen_drag_cmd)
    return
