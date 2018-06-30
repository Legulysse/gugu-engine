#import os
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
        premakeExeName = 'premake5'

	#ShellExecute('"' + dirEngineBuildScripts + '/Premake/' + premakeExeName + '" --help')
    ShellExecute('"' + dirEngineBuildScripts + '/Premake/' + premakeExeName + '" --version')
    ShellExecute('"' + dirEngineBuildScripts + '/Premake/' + premakeExeName + '" --file="' + file + '" ' + compiler)
    
    
#---------------------------------------------------------------------------------------------------
# Zip

def Zip(src):
    ZipTo(src, src+'.7z')
    
def ZipTo(src, dst):
    ShellExecute('"7zip/7za.exe" a "'+ dst +'" "'+ src +'"')

def UnzipTo(src, dst):
    ShellExecute('"7zip/7za.exe" x "'+ src +'" -o"'+ dst +'" -y')

