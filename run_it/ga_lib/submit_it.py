# System modules
import sys
import os
import commands
from Queue import Queue
from threading import Thread
import time, random


# Set up some global variables
num_fetch_threads = 10
job_queue = Queue()
jobs = []

def run_model_HPC(i, q):
	"""This is the worker thread function.
	It processes items in the queue one after
	another.  These daemon threads go into an
	infinite loop, and only exit when
	the main thread ends.
	"""
	while True:
		print '%s: Looking for the next model' % i
		job = q.get()
		print ' %s: running:' % i, job
        #here, we build the job script, submit to queue, wait for return
		script = []
		##initial script settings
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

		##create the script filename
		script_name = "%sjROMS.pbs" % (job)
		##write the script to the file
		run_script = open(script_name, 'w')
		run_script.writelines(script)
		run_script.close()
		##change the properties of the file so that it can be executed (?)
		os.chmod(script_name,0774)

		#queue the job
		CMD = "cd %s ; qsub jROMS.pbs" % (job)
		result = commands.getstatusoutput(CMD)
		status = result[0]
		message = result[1]
		if status != 0:
			print "Error running command: " + CMD
			print message
			sys.exit(1)
		#time.sleep(random.randint(1000, 5000) / 1000.0)
		print "task", job, "finished"
		q.task_done()
		q.join()


def submit_it(exp_name, generation, population):
	#define jobs
	for i in range(0,population):
		jobs.append("%s/%04d/%04d/" % (exp_name, generation, i))
		print "from submit_it: " + jobs[i]

	# Set up some threads to fetch the enclosures
	for i in range(num_fetch_threads):
		worker = Thread(target=run_model_HPC, args=(i, job_queue,))
		worker.setDaemon(True)
		worker.start()

	# Download the feed(s) and put the enclosure URLs into
	# the queue.
	for job in jobs:
	    print 'Queuing:', job
	    job_queue.put(job)

	# Now wait for the queue to be empty, indicating that we have
	# processed all of the downloads.
	print '*** Main thread waiting'
	job_queue.join()
	print '*** Done'
