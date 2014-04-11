#! /usr/bin/env python3
import csv
import sys
import re
import subprocess
import random
import linecache


def shuffle(x):
    x = list(x)
    random.shuffle(x)
    return x
csv1 = open('./data/lab09_g10_data.csv', 'w')
csvw = open('./data/lab09_g10_random.csv', 'w')
iteration=500
rerun=10
nrand=5
for i in range(iteration):
	for j in range(rerun):
		argument=i+1
		process=subprocess.Popen(['./mybins/cs296_10_exe',str(i+1)], stdout = subprocess.PIPE)
		output=process.communicate()[0]
		#print(output)
		listofnum=re.findall(r'[0-9]+(?:[.][0-9]+)?', str(output))
		csv1.write(listofnum[0]+','+str(j+1)+','+listofnum[1]+','+listofnum[2]+','+listofnum[3]+','+listofnum[4]+','+listofnum[5]+'\n')
csv1.close()

#csvr=csv = open('./data/lab09_g10_data.csv', 'r')
#data_random=csv.read()

lines=[]
r=csv.reader(open('./data/lab09_g10_data.csv'))
#	r=csv.reader(f,delimiter=',')
for row in r:
	lines.append(','.join(row))
#print(len(lines))
for i in range(iteration):
	x=range(rerun)
	y=shuffle(x)
	for j in range(nrand):
		n=int(y[j])+i*rerun
		#print(n)
		csvw.write(lines[n]+'\n')
