# -*- coding: utf-8 -*-
"""
Created on Sun Jul 16 05:59:11 2017

@author: user
"""

import serial
import numpy as np
import time
import sys

f=open("arduino_log.txt","w",encoding='utf-8') #opening text file to write all the printed lines
orig_stdout = sys.stdout #backing up sys.stdout
sys.stdout = f #sys.stdout writes in the opened text file.


ser = serial.Serial('COM5', baudrate=115200, timeout=None)#setting up serial, timeout must be None or long otherwise the input gets corrupted
#ser.open() #opening the serial
if(ser.is_open): #checks whether it is opened.
    print("serial is now open")
print(ser.readline().decode("utf-8"))


def initiator(number_of_groups, group_sequence): #writes 's' , number of groups and group sequence
    ser.write('s'.encode(encoding='ascii'))
    time.sleep(1)
    ser.write(str(number_of_groups).encode(encoding='ascii'))
    time.sleep(1)
    ser.write(str(group_sequence).encode(encoding='ascii'))
    time.sleep(1)


def group_declare(num_input, x_coord,y_coord):
    ser.write(str(num_input).encode(encoding='ascii'))
    time.sleep(1)
    if(type(x_coord) is not int): #if x_coord is a list
        for jj in range(int(len(x_coord))):
            ser.write(str(x_coord[jj]).encode(encoding='ascii'))
            time.sleep(1)
            ser.write(str(y_coord[jj]).encode(encoding='ascii'))
            time.sleep(1)
        time.sleep(2)



    else:
        ser.write(str(x_coord).encode(encoding='ascii'))
        time.sleep(1)
        ser.write(str(x_coord).encode(encoding='ascii'))
        time.sleep(1)
        ser.write("99".encode(encoding='ascii'))
        time.sleep(2)





#function for custom input
def stimulation_generator(delay, pulse_width, iteration): #straight forward. feeds in delay, pulse_width, iteration
    ser.write(str(iteration).encode(encoding='ascii'))
    time.sleep(1)
    ser.write(str(delay).encode(encoding='ascii'))
    time.sleep(1)
    ser.write(str(pulse_width).encode(encoding='ascii'))
    time.sleep(1)


def reader():
    print((ser.read_all()).decode("utf-8"))



#declaring control variables

num_of_groups=2 #how many groups are there
sequence=221 # ex) 21 = group2 -> group1

group=np.zeros((num_of_groups,2),dtype=object) #just declaring a placeholder
#below please declare "group" array
#group[group number-1]["0" for x, "1" for y]
group[0][0]=[10,20]
group[0][1]=[10,20]
group[1][0]=[30,20,10]
group[1][1]=[30,10,20]

num_input=[] #declaring list as a placeholder
for jj in range(num_of_groups):
    num_input.append(len(group[jj][0]))

#main function is below.

#below commented area is the lines for controlled input
"""
initiator(number_of_groups=num_of_groups,group_sequence=sequence)
for ii in range(num_of_groups):
    group_declare(num_input=num_input[ii],x_coord=group[ii][0],y_coord=group[ii][1])
stimulation_generator(delay=500, pulse_width=300,iteration=4) #(delay between blinks(ms), width of pulse (ms),number of inputs, iteration, x coordinates, y coordinates)
"""
#below two lines shows how to use built-in led lighting up scheme. ('a' equals light it up all)
ser.write('a'.encode(encoding='ascii'))
time.sleep(3)


#below reads the printed output to the txt file
reader()

sys.stdout = orig_stdout #turn it back to normal
f.close() #closing opened text file

ser.close()#close the serial
if(not(ser.is_open)): #checks whether its closed. 
    print("serial is now closed")


