#!/usr/bin/python
from subprocess import call
import itertools
import sys
#open the conf.txt file and write a new configuration in it
e = "-e 2"
t0 = 0
t1 = 0
t2 = 0
tN = 15
for occupied in range(3, 4):
	#print "hello"
	combo = itertools.combinations(range(15), occupied)
	for c in combo:
		
		out_file = open("conf.txt", 'w')
		#print "new combo"
		
		#split the chcombo entry
		ch = itertools.chain(c)
		z = ""
		i = 0
		for t in ch:
			if(i == 0):
				a = "-a {0}".format(t)
			if(i == 1):
				b = "-b {0}".format(t)
			if(i == 2):
				c = "-c {0}".format(t)
			#print t;
			#print "new element in chain"
			nc = t / 3;
			ns = t % 3;
			z += "S%i%i" % (ns, nc)
			
			#output of the form id\tnc\tns\n
			out_file.write("%i\t%i\t%i\n" % (0, nc, ns))
			#command = "./simulation -c 5 -l 5 -m 2 -s " + z
			#print command
		        i = i + 1	
		#a = "-a {0}".format(a)
		#b = "-b {0}".format(t2 - t1)
		#c = "-c {0}".format(tN - t2)
		i = 0
		out_file.close()
		command = "./simulation -c 5 -l 5 -m 2 -e 2 -s " + z
		#print command
		#command = "../evaluateModel/main {0} {1} {2} {3}" .format(a, b, c, e)
		#print command
		parameters = "-s " + z
		call(["./simulation", "-c 5", "-l 5", "-m 2", "-i 1000000", parameters])
		call(["../evaluateModel/main",a, b, c, e, parameters])
		#print "hello";

