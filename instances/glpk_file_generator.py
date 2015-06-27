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
    clauses_names_hard = []
    clauses_names_soft = []
    for clauseinx, clause in enumerate(clauses):
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
            clauses_names_hard.append(clauseinx)
        else: #soft
            soft_matrix.append(line)
            negation_count_soft.append(negated)
            weight.append(clause[0])
            clauses_names_soft.append(clauseinx)    
    clauses_names_hard = listToString(clauses_names_hard)
    clauses_names_soft = listToString(clauses_names_soft)
    
    out_string = ''
    out_string += formatGlpkSet('CS', clauses_names_soft)
    out_string += formatGlpkSet('CH', clauses_names_hard)
    out_string += formatGlpkSet('V', listToString(variables))
    ##info for hard clauses
    out_string += formatGlpkMatrix("h",clauses_names_hard,listToString(variables),matrixToString(hard_matrix))
    out_string += '\n'
    #info for soft clause
    out_string += formatGlpkMatrix("s",clauses_names_soft,listToString(variables),matrixToString(soft_matrix))
    out_string += formatGlpkVector("nh",clauses_names_hard, listToString(negation_count_hard))
    out_string += formatGlpkVector("ns",clauses_names_soft, listToString(negation_count_soft))
    out_string += formatGlpkVector("w",clauses_names_soft, listToString(weight))
    
    outFile = open(outputFilename,"w")
    outFile.write(out_string)

def listToString(lista):
    return [str(x) for x in lista]

def matrixToString(matrix):
    return [listToString(x) for x in matrix]

def formatGlpkMatrix(nameVar,rowNames,colNames, matrix):
    out = 'param '+nameVar + " : " + " ".join(colNames)+" :="
    for rowinx, row in enumerate(rowNames):
        out += "\n" + str(row) + " " +  " ".join(matrix[rowinx])
    out +=';\n'
    return out

def formatGlpkVector(nameVar,names, vector):
    out = 'param '+nameVar + " := "
    for inx, name in enumerate(names):
        out += "\n" + str(name) + " " +  vector[inx]
    out +=';\n'
    return out
    
def formatGlpkSet(name,values):
    return 'set '+name+" := " + " ".join(values)+";\n"

#files to covert
onlyfiles = [ f for f in listdir('original') if isfile(join('original',f)) ]
print onlyfiles
for filename in onlyfiles:
    (variables,top,clauses) = parseFile("original/"+filename)
    genereateGlpkFile(variables,top, clauses,"glpk/"+filename)
