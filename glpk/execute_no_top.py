# -*- coding: utf-8 -*-
"""
Created on Fri Jun 26 23:48:47 2015

@author: thiago
"""
from os import system 
from os import listdir
from os.path import isfile, join

model_filename = 'final.glpk'
time_limit = 3600#seconds
path_to_inputs = '../instances/glpk_no_top'
onlyfiles = [ f for f in listdir(path_to_inputs) if isfile(join(path_to_inputs,f)) ]
print onlyfiles
#onlyfiles = ['test2.wcnf']
for filename in onlyfiles:
    command = "glpsol --model  %s -d %s --output %s --tmlim %d " % (model_filename,path_to_inputs+'/'+filename , "solutions_no_top/"+filename+".sol",time_limit)
    print command    
    system(command)