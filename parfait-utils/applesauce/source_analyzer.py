#! /usr/bin/env python
# Note: Requires Python 3.X for statistics
import os, math
import statistics

def isSourceFile(file):
    if(file.endswith('.h') or file.endswith('.hpp') 
        or file.endswith('.cpp')):
        return 1
    return 0

def isLineOfCode(line):
    if('}' in line or ';' in line 
            or ')' in line or '(' in line
            or '>' in line or ',' in line
            or '#' in line):
        return 1
    return 0
    
def countLinesInFile(filename):
    f = open(filename,'r')
    sum = 0
    for line in f:
        if isLineOfCode(line):
            sum += 1
    return sum

def getLineCountsForFiles(files):
    lines_per_file = []
    for file in files:
        lines_per_file.append(countLinesInFile(file))
    return lines_per_file

def getSourceFileNamesInDirectory(dir):
    file_names = []
    for file in os.listdir(dir):
        if isSourceFile(file):
            file_names.append(file)
    return file_names

source_names = getSourceFileNamesInDirectory("./")
lines_per_file = getLineCountsForFiles(source_names)
index_of_max = lines_per_file.index(max(lines_per_file))
index_of_min = lines_per_file.index(min(lines_per_file))
min_length = lines_per_file[index_of_min]
max_length = lines_per_file[index_of_max]
shortest_file = source_names[index_of_min]
longest_file = source_names[index_of_max]

number_of_files = len(lines_per_file)
average = statistics.mean(lines_per_file)
lines_in_directory = sum(lines_per_file)

standard_deviation = statistics.pstdev(lines_per_file)

print (number_of_files,'  Number of source files')
print (lines_in_directory,'  Lines in directory')
print (min_length,' lines in shortest file (' + shortest_file +')')
print (max_length,' lines in longest file (' + longest_file + ')')
print (int(average),' average lines in file')
print (int(standard_deviation),' standard deviation')
