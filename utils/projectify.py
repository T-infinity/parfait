#! /usr/bin/env python

# This script will create a CMakeLists.txt file for the current
# working directory with each of the source files listed individually.
# It will create a static library (so that other projects can link to
# it using cmake's ``find_package()''

import os

def isHeaderFile(file):
    if(file.endswith('.h') or file.endswith('.hpp')):
        return 1
    return 0

def isCPlusPlusSourceFile(file):
    if(file.endswith('.cxx') or file.endswith('.cpp')):
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

def getProjectName():
    return input("Project name: ")

def printCMAKE(sources,headers):
    with open('CMakeLists.txt','w') as f:
        name = getProjectName()
        directory = os.getcwd()
        f.write("cmake_minimum_required(VERSION 2.8)\n\n")
        f.write("PROJECT("+name+")\n\n")
        f.write('set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")\n\n')
        f.write('set('+name+'_SRC "'+'"\n      "'.join(sources)+'")\n\n')
        f.write('add_library('+name+' STATIC ${'+name+'_SRC})\n\n')


sources = getCPlusPlusSourceFileNames("./")
headers = getHeaderFileNames("./")

printCMAKE(sources,headers)
