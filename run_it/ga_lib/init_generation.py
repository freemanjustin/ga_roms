import sys, os, posix, commands, math, string
from time import localtime, strftime, sleep, time, mktime
from math import floor, ceil
from run_cmd import run_cmd

def init_generation(exp_name, generation, population):
    # create the directory structure for this experiment
    for i in range(0,population):
      the_dir = "mkdir -p %s/%04d/%04d" % (exp_name, generation, i)
      run_cmd(the_dir)

    # copy the roms binary and input text files to each run directory
    #for i in range(0,population):
    #  cp_cmd = "cp -r %s/* %s/%04d/%04d/." % (roms_bin_data, exp_name, generation, i)
    #  run_cmd(cp_cmd)

    # softlink the static data into each directory
    #for i in range(0,population):
    #  ln_cmd = "ln -sf %s/* %s/%04d/%04d/" % (roms_static_data, exp_name, generation, i)
    #  run_cmd(ln_cmd)
