import os
import re
#import configparser
from sys import platform

from Utility import *


#---------------------------------------------------------------------------------------------------
# Premake

def Premake(dirEngineBuildScripts, file, compiler):
    premakeExeName = ''
    if sys.platform.startswith('win'):
        premakeExeName = 'Premake5.exe'
    elif sys.platform.startswith('linux'):
        premakeExeName = 'premake5_linux'
    elif sys.platform.startswith('darwin'):
        premakeExeName = 'premake5_mac'

	#ShellExecute('"' + dirEngineBuildScripts + '/Premake/' + premakeExeName + '" --help')
    ShellExecute('"' + dirEngineBuildScripts + '/Premake/' + premakeExeName + '" --version')
    print('')
    ShellExecute('"' + dirEngineBuildScripts + '/Premake/' + premakeExeName + '" --file="' + file + '" ' + compiler)
    print('')
    
    
#---------------------------------------------------------------------------------------------------
# Zip

def Zip(src):
    ZipTo(src, src+'.7z')
    
def ZipTo(src, dst):
    ShellExecute('"7zip/7za.exe" a "'+ dst +'" "'+ src +'"')

def UnzipTo(src, dst):
    ShellExecute('"7zip/7za.exe" x "'+ src +'" -o"'+ dst +'" -y')

    
#---------------------------------------------------------------------------------------------------
# Tabs to Spaces Converter
#
# call should look like this : ConvertTabsToSpaces('../../Dev/SourcesEngine/', 4, ('.cpp', '.h', '.tpp'))

def ConvertTabsToSpaces(directory, spaces_per_tab, extensions):
    found_files = []
    for path, subdirs, files in os.walk(directory):
        for name in files:
            found_files.append(os.path.join(path, name));
    matched_files = [name for name in found_files if name.endswith(extensions)]

    for file_path in matched_files:
        file_contents = ''
        lines = []
        with open(file_path) as f:
            lines = f.readlines()
        
        for line in lines:
            newLine = str()
            for c in line:
                if c == '\t':
                    newLine += ' ';     # force at least one space
                    while (len(newLine) % spaces_per_tab != 0):
                        newLine += ' ';
                else:
                    newLine += c
            #newLine = newLine.rstrip() + '\n'
            file_contents += newLine
            
        print('Replacing tabs in {0}'.format(file_path))
        with open(file_path, "w") as f:
            f.write(file_contents)

        
        
        
        