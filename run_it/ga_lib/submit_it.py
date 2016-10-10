import sys
import os
import commands

import threading
import Queue
import time, random

class e:
    def __init__(self):
        self.jobpath = None


class Worker(threading.Thread):

	def __init__(self, queue):
		self.__queue = queue
		threading.Thread.__init__(self)

	def run(self):
		while 1:
			item = self.__queue.get()

			if item is None:
				break # reached end of queue

			#break up the input arguments
			#item[0] is the threadID (as entered from the main line)
			threadID   = item[0]
			#additional arguments are stored after it (filename, data structure)
			threadArgs = item[1:]
			filename   = threadArgs[0]
			#get the data structure that holds static information
			E          = threadArgs[1]
			print "filename = %s\n" % filename
			#here, we build the job script, submit to queue, wait for return
			script = []

			#initial script settings
			script.append("#!/bin/bash\n")
			script.append("#PBS -S /bin/bash\n")
			script.append("#PBS -q express\n")
			script.append("#PBS -l ncpus=32\n")
			script.append("#PBS -l wd\n")
			script.append("#PBS -l walltime=00:40:00\n")
			script.append("#PBS -l mem=60gb\n")
			script.append("#PBS -W block=true\n")
			script.append("#PBS -P ep4\n")

			script.append("cd ${PBS_O_WORKDIR}\n")
			script.append("source /home/548/jcf548/.bashrc\n")
			script.append("module load openmpi/1.10.0\n")
			script.append("time mpirun -np 32 ./oceanM ocean.in > romsout.txt\n"  )
			#script.append("time ./bodas.exe %s\n" % (filename) )

			#create the script filename
			script_name = "%sjROMS.pbs" % (filename)
			#write the script to the file
			run_script = open(script_name, 'w')
			run_script.writelines(script)
			run_script.close()
			#change the properties of the file so that it can be executed (?)
			os.chmod(script_name,0774)

			#queue the job
			CMD = "qsub %s" % (script_name)
			result = commands.getstatusoutput(CMD)
			status = result[0]
			message = result[1]
			if status != 0:
				print "Error running command: " + CMD
				print message
				sys.exit(1)


#
# run with limited queue
def submit_it(exp_name, generation, population):
	#set some global variables: number of workers is the maximum number of jobs
	#we want to submit to the queue at any one time (up to 20)
	global WORKERS, MAXAREAS
	WORKERS = 20

	E = e()
	jobs = []
	#define jobs
	#CMD = "ls input*.nml"
	#result = commands.getstatusoutput(CMD)
	#filenames = result[1].split('\n')
	#nfiles = len(filenames)
	for i in range(0,population):
		jobs.append("%s/%04d/%04d/" % (exp_name, generation, i))
		#print "from submit_it: " + jobs[i]


	#create the queue (not sure why the size is workers+1)
	queue = Queue.Queue(WORKERS+1)
	for i in range(WORKERS):
		Worker(queue).start() # start a worker

	for item in range(population):
	#for item in range(3):
		#put the item (file) on the queue for processing, when a slot is available
		queue.put( (item, jobs[item], E) )

	#once the above is finished, retire the workers
	for i in range(WORKERS):
		queue.put(None) # add end-of-queue markers
