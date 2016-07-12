#! /usr/bin/env python
# Note: Requires Python 3.X for statistics
import os, math
import statistics

def isHeaderFile(file):
    if(file.endswith('.h') or file.endswith('.hpp')):
        return 1
    return 0

def isCPlusPlusSourceFile(file):
    if(file.endswith('.cxx') or file.endswith('.cpp')):
        return 1
    if(isHeaderFile(file)):
        return 1
    return 0

def getHeaderFileNames(dir):
    file_names = []
    for file in os.listdir(dir):
        if isHeaderFile(file):
            file_names.append(file)
    return file_names

def getCPlusPlusSourceFileNames(dir):
    file_names = []
    for file in os.listdir(dir):
        if isCPlusPlusSourceFile(file):
            file_names.append(file)
    return file_names

def findNextFunctionStart(lines,n):
    for i in range(n,len(lines)):
        if lineOpensScope(lines,i):
            function_begin = i
            return i
    return len(lines)

def findFunctionEnd(lines,n):
    opening_braces = 0
    closing_braces = 0
    for i in range(n,len(lines)):
        opening_braces += lines[i].count('{')
        closing_braces += lines[i].count('}')
        if(opening_braces == closing_braces):
            return i
    return n


def getFunctionLengths(file_name):
    with open(file_name,'r') as f:
        lines = f.readlines()
        function_begin = 0
        function_end = 0
        function_lengths = []
        while function_begin < len(lines):
            function_begin = findNextFunctionStart(lines,function_end+1)
            function_end = findFunctionEnd(lines,function_begin)
            function_length = function_end - function_begin
            if function_length > 0:
                function_lengths.append(function_length)
    if function_lengths == []:
        function_lengths.append(0)
    return function_lengths

def lineOpensScope(lines,i):
    if '{' in lines[i]:
        if ')' in lines[i] or ')' in lines[i-1]:
            return 1
    return 0

source_names = getCPlusPlusSourceFileNames("./")
for source in source_names:
    print("Analyzing file: ",source)
    function_lengths = getFunctionLengths(source)
    print("Number of functions in file: ", len(function_lengths))
    print("Longest function:            ",max(function_lengths))
    print("Shortest function:           ",min(function_lengths))
    print("Average function:            ",int(statistics.mean(function_lengths)))

