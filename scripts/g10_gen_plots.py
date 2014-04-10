#! /usr/bin/env python3
import csv
import sys
import re
import subprocess
import random
import numpy as np
#from scipy.interpolate import pchip
#import numpy as np
#from scipy import interpolate
def average(data,iteration,rerun):
	ans=[]
	for i in range(iteration):
		ans.append(float(sum(data[i*rerun:i*rerun+rerun])/float(rerun)))
	return ans
def uniq(input):
  output = []
  for x in input:
    if x not in output:
      output.append(x)
  return output
def stdev(data12):
	data1=data12
	avr=float(sum(data1)/(float(len(data1))))
	for i in range(len(data1)):
		data1[i]=(data12[i]-avr)*(data12[i]-avr)
		#print(data1[i])
	#print(sum(data1))
	var=sum(data1)/(float(len(data1)))
	#print(avr)
	return var**(.5)
def bar(dat,interval):
	ans=dat
	mini=min(dat)
	maxi=max(dat)
	for i in range(len(dat)):
		ans[i]=int((ans[i]-mini)*float(interval)/(maxi-mini))*((maxi-mini)/float(interval))+mini
	return ans

data=[[],[],[],[],[],[],[]]
iteration =500
rerun=10
highrol=53
nrand=5
interv=20
r=csv.reader(open('./data/lab09_g10_data.csv'))
for row in r:
	row_d=re.findall(r'[0-9]+(?:\.[0-9]+)?', ','.join(row))
	for i in range(2):
		data[i].append(int(row_d[i]))
	for i in range(7)[2:7]:
		data[i].append(float(row_d[i]))



average_step=average(data[2],iteration,rerun)
average_loop=average(data[6],iteration,rerun)
average_collision=average(data[3],iteration,rerun)
average_velocity=average(data[4],iteration,rerun)
average_position=average(data[5],iteration,rerun)
stddev=[]
for i in range(iteration):
	stddev.append(float(stdev(data[2][i*rerun:i*rerun+rerun])))
high_data=data[2][(highrol-1)*rerun:(highrol-1)*rerun+rerun]
high_d=bar(high_data,interv)
data2_rand=[]
r2=csv.reader(open('./data/lab09_g10_random.csv'))
for row in r2:
	row_d=re.findall(r'[0-9]+(?:\.[0-9]+)?', ','.join(row))
	data2_rand.append(float(row_d[2]))

#print(data2_rand)
average_rand=average(data2_rand,iteration,nrand)


import matplotlib.pyplot as plt
import pylab
from matplotlib.pyplot import *
from numpy import *

p1= plt.bar(range(iteration+1)[1:],average_step,color='r')#,label="average step time over reruns for iteration values")
p2,=plt.plot(range(iteration+1)[1:],average_loop,linestyle='-',color='b')#,label="average loop time over reruns for iteration values")
p3,=plt.plot(range(iteration+1)[1:],[min(average_loop)]*iteration,linestyle='-',color='g')
p4,=plt.plot(range(iteration+1)[1:],[max(average_loop)]*iteration,linestyle='-',color='c')
plt.ylabel("average time over reruns")
plt.xlabel("iteration values")
plt.ylim([0,1.4*max(average_loop)])
legend([p1,p2,p3,p4],["avg step time over reruns for itervalues","avg loop time over reruns for itervalues","minima","maxima"])
pylab.savefig('./plots/g10_lab09_plot01.png')
plt.clf()
#print(stddev)

p3, = plt.plot(range(iteration+1)[1:], average_step,linestyle='-',color='y')
p4, = plt.plot(range(iteration+1)[1:], average_collision,linestyle='-', color='g')
p5, = plt.plot(range(iteration+1)[1:], average_velocity,linestyle='-', color='b')
p6, = plt.plot(range(iteration+1)[1:], average_position,linestyle='-', color='r')
plt.ylabel('Various Timing Values')
plt.xlabel('Iteration Values')
plt.title('Second Plot: Step, collision, velocity and position time v/s iteration values')
l2 = plt.legend([p3,p4,p5,p6], ["Step Time", "Collision Time", "Velocity Time", "Position Time"], loc = 1)
plt.savefig('./plots/g10_lab09_plot02.png')
#plt.show()
plt.clf()
p7 = plt.errorbar(range(iteration+1)[1:], average_step,yerr=stddev)
l3 = plt.legend([p7], ["Step Time averaged over reruns with error bars"], loc = 1)
plt.xlabel('Iteration Values')
plt.ylabel('Step Time with error bars')
plt.title('Step time with error bars v/s iteration values')
plt.savefig('./plots/g10_lab09_plot03.png')
#plt.show()
plt.clf()

intervals = 50
#cumdata = cumulative(datalist[2][4400:4500], intervals)
#distlist = dist(min(datalist[2][4400:4500]), max(datalist[2][4400:4500]), intervals)''
dist=uniq(high_d)
cumdata=[]
dist.sort()
var=0
for i in range (len(dist)):
	for j in range(len(high_d)):
		if dist[i] == high_d[j]:
			var+=1
	cumdata.append(int(var))
	
#f= interpolate.interp1d(dist, cumdata)
p8, = plt.plot(dist, cumdata, '-')#,dist,f,'-')
#print(high_d)
p9 = plt.hist(high_d, bins=interv)
'''l4 = plt.'''
plt.legend([p8],["Cumulative Frequency Curve"])
''''''
plt.title("Frequency and cumulative distributions for step times v/s 53rd iterations")
plt.xlabel('Step Time')
plt.ylabel('Reruns')
plt.ylim([0,1.2*max(cumdata)])
plt.savefig('./plots/g10_lab09_plot04.png')
#plt.show()
plt.clf()

p3, = plt.plot(range(iteration+1)[1:],average_step,'x', color='r')
coefs = np.lib.polyfit(range(iteration+1)[1:], average_step, 1) #4
fit_y = np.lib.polyval(coefs, range(iteration+1)[1:]) #5
p4,=plt.plot(range(iteration+1)[1:], fit_y, 'b-') #6p4,=


p5, = plt.plot(range(iteration+1)[1:],average_rand,'x', color='c')
coefs1 = np.lib.polyfit(range(iteration+1)[1:], average_rand, 1) #4
fit_y1 = np.lib.polyval(coefs1, range(iteration+1)[1:]) #5
p6,=plt.plot(range(iteration+1)[1:], fit_y1, 'g-') #6p4,=

plt.ylabel('Various Timing Values')
plt.xlabel('Iteration Values')
plt.title('Step time of random data v/s normal data')
l2 = plt.legend([p3,p4,p5,p6], ["Step Time", "Fitted line","Step Time_random", "Fitted line_random"],loc = 1)#, "Velocity Time", "Position Time"], loc = 1)
plt.savefig('./plots/g10_lab09_plot05.png')
#plt.show()
plt.clf()
