# -*- coding: utf-8 -*-
"""
Created on Fri Jun 26 17:59:11 2015

@author: thiago
"""
from os import listdir
from os.path import isfile, join


def parseFile(filePath):
    lines = []
    input_file = open(filePath)
    ##reads all lines and removes comments
    lines = [line for line in input_file.readlines() if line[0]!='c' and line[0]!='p']
    ##list of lists containing clauses [weight, var1, var2,....]
    clauses = []    
    
    #maximum weight found    
    top = 0
    variables = set()
    ##reads clauses
    for line in lines:
        clause = []
        tokens = line.split(' ')
        #updates top
        if(tokens[0]>=top):
            top = tokens[0]
            
        clause.append(tokens[0]) #weight
        
        #for each variable
        for var in tokens[1:-1]: #excludes first and last
            variables.add(abs(int(var)))
            clause.append(int(var))
        clauses.append(clause)
    variables = sorted(list(variables))
    return (variables,top,clauses)
    
def genereateGlpkFile(variables,top, clauses,outputFilename):
    #indicates the index of each variable on the matrix
    pos = {}
    for inx, var in enumerate(variables):
        pos[var]=inx

    num_var = len(variables)    
    
    hard_matrix = [] #list of lists. each sublist has length = to the number of
                     # variables
    weight = []
    soft_matrix = []
    negation_count_hard = [] #number of negated variables for each clause
    negation_count_soft = []
    for clause in clauses:
        line = [0]*num_var
        negated = 0
        for variable in clause[1:]:
            if(variable > 0):
                line[pos[variable]] = 1
            else:
                line[pos[-variable]] = -1
                negated += 1
        if(clause[0]==top): #hard
            hard_matrix.append(line)
            negation_count_hard.append(negated)
        else: #soft
            soft_matrix.append(line)
            negation_count_soft.append(negated)
            weight.append(clause[0])
            
            
    out_string = ''
    ##info for hard clauses
    out_string += formatGlpkMatrix("h",listToString(variables),matrixToString(hard_matrix))
    out_string += '\n'
    #info for soft clause    
    out_string += formatGlpkMatrix("s",listToString(variables),matrixToString(soft_matrix))
    out_string += formatGlpkVector("nh", listToString(negation_count_hard))
    out_string += formatGlpkVector("ns", listToString(negation_count_soft))
    out_string += formatGlpkVector("w", listToString(weight))
    outFile = open(outputFilename,"w")
    outFile.write(out_string)
    
def listToString(lista):
    return [str(x) for x in lista]    

def matrixToString(matrix):
    return [listToString(x) for x in matrix]

def formatGlpkMatrix(nameVar,colNames, matrix):
    out = 'param '+nameVar + " : " + " ".join(colNames)+" :="
    for rowinx in range(len(matrix)):
        out += "\n" + str(rowinx) + " " +  " ".join(matrix[rowinx])
    out +=';\n'
    return out
    
def formatGlpkVector(nameVar, vector):
    out = 'param '+nameVar + " := "
    for inx in range(len(vector)):
        out += "\n" + str(inx) + " " +  vector[inx]
    out +=';\n'
    return out

#files to covert
onlyfiles = [ f for f in listdir('original') if isfile(join('original',f)) ]
print onlyfiles
for filename in onlyfiles:
    (variables,top,clauses) = parseFile("original/"+filename)
    genereateGlpkFile(variables,top, clauses,"glpk/"+filename)

        
        