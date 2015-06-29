# -*- coding: utf-8 -*-
"""
Created on Fri Jun 26 23:48:47 2015

@author: thiago
"""
from os import system 
from os import listdir
from os.path import isfile, join
from multiprocessing import Pool
model_filename = 'final.glpk'
time_limit = 3600#seconds
path_to_inputs = '../instances/glpk_no_top'
onlyfiles = [ f for f in listdir(path_to_inputs) if isfile(join(path_to_inputs,f)) ]
print onlyfiles
#onlyfiles = ['test2.wcnf']
commands  = []
for filename in onlyfiles:
    commands.append( "glpsol --model  %s -d %s --output %s --tmlim %d > outputs/notop%s" % (model_filename,path_to_inputs+'/'+filename , "solutions_no_top/"+filename+".sol",time_limit,filename))
    print commands[-1]


    #system(command)
pool= Pool()
pool.map(system, commands)
