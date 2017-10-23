# -*- coding: utf-8 -*-
"""
Created on Mon Jul 10 16:43:41 2017

@author: user


This code only functions with the Arduino code "OLED_code5.ino".
"""

import numpy as np

#reading in arduino serial log and converting into an array.
#array would have a coordinate information for micro OLED.
x_width= 64
y_width= 48

#declaring the name of the log file.
log_file_name='arduino_log.txt'


#read in all the data in the txt file by lines.
with open(log_file_name) as f:
    string_array=f.readlines()



first_x=0 #flag variable for handling custom inputs.
custom_array=[] #again, array for handling custom inputs.
final_array=[] #array variable that would store all the stimulation of the experiment in time sequence.
group=[]
custom_flag=0

#Below are all the switches :) 
#as the for loop scans through the txt file, it checks what kind of input was given and stores it in "final_array" 
#for exception handling, included both strings with/ without "\n"
for ii in range(len(string_array)):
    if string_array[ii]=="lighting up all\n":
        for jj in range(x_width):
            for hh in range(y_width):
                final_array.append([jj+1,hh+1])
    elif string_array[ii]=="lighting up all":
        for jj in range(x_width):
            for hh in range(y_width):
                final_array.append([ii+1,jj+1])
    elif string_array[ii]=="lighting up columns\n":
        middle_prod=[] #declare middle_prod variable to store list of cooridnates stimulated simultaneously and then stores it in final_array.
        for jj in range(y_width):
            for xx in range(x_width): 
                middle_prod.append([jj+1, xx+1])
            final_array.append(middle_prod)
    elif string_array[ii]=="lighting up rows\n":
        middle_prod=[] #same logic with the columns.
        for jj in range(x_width):
            for yy in range(y_width):
                middle_prod.append([jj+1, yy+1])
            final_array.append(middle_prod)
    elif string_array[ii]=="lighting up columns":
        middle_prod=[]
        for jj in range(y_width):
            for xx in range(x_width):
                middle_prod.append([jj+1, xx+1])
            final_array.append(middle_prod)
    elif string_array[ii]=="lighting up rows":
        middle_prod=[]
        for jj in range(x_width):
            for yy in range(y_width):
                middle_prod.append([jj+1, yy+1])
            final_array.append(middle_prod)
    elif string_array[ii] == "type in the sequence of cycle with group numbers ex) 1324:\n":
        custom_flag=1
        stimulation_seq= string_array[ii+2].rstrip('\n')
        print(stimulation_seq)
        stimulation_seq=list(map(int, stimulation_seq)) #stimulation_seq stores group sequence
        stimulation_seq=np.array(stimulation_seq)-1
    elif string_array[ii] == "type in a number for iteration:\n":
        iteration=int(string_array[ii+2].rstrip('\n'))
    elif string_array[ii]=="x: \n": #checks that this is the first custom input
        if(not(first_x)):
            first_x=1 #flag is up.
            check_point=ii #check_point is saved. We will use this to scan throught the custom input lines once again.
    elif string_array[ii]=="breaking input\n": #now custom input session is finished.
        for hh in range(check_point,ii,1): #starting from check_point we scan it again and stores each custom input into custom_array
            if string_array[hh]=="x: \n" and string_array[hh+2].rstrip('\n')!="breaking input":
                    selected_coordinate=[string_array[hh+2].rstrip('\n'),string_array[hh+6].rstrip('\n')]
                    custom_array.append(selected_coordinate)
        group.append(custom_array)
        custom_array=[]
        first_x=0 #resetting the flag to false.
        
if custom_flag==1:        
    group = [ group[i] for i in stimulation_seq] # re-sorting group array according to sequence(group) input
    for jj in range(iteration): #counting iteration
        final_array.append(np.array(group)) #appending the group array into final_array
    
    iteration=0 #resetting the flag to false.
    custom_flag=0


