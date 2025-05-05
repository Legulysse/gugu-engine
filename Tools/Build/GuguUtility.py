import glob, os, shutil
import stat
import sys
import collections
from subprocess import Popen, PIPE, STDOUT

#import configparser
#from sys import platform


#---------------------------------------------------------------------------------------------------
# High Level Utility

def DefaultMenu(pathEngineBuildScripts, nameBuildScript):
    
    # Menu : Available actions
    #selectedActions = { 'premake', 'subdirectories' }
    
    actionChoices = collections.OrderedDict()
    actionChoices['premake'] = 'premake'
    actionChoices['subdirectories'] = 'update submodules'
    selectedActions = PromptMenu('Select Actions', actionChoices, True)
    
    # Menu : Compilers selection
    selectedCompilers = []
    if 'premake' in selectedActions:
        compilerChoices = collections.OrderedDict()
        compilerChoices['vs2022'] = 'Visual 2022'
        compilerChoices['vs2019'] = 'Visual 2019'
        compilerChoices['vs2017'] = 'Visual 2017'
        compilerChoices['vs2015'] = 'Visual 2015'
        compilerChoices['gmake'] = 'GNU Makefiles'
        compilerChoices['xcode4'] = 'XCode'
        compilerChoices['codelite'] = 'CodeLite'
        selectedCompilers = PromptMenu('Select Compilers', compilerChoices, True)

    # Apply SubDirectories action
    if 'subdirectories' in selectedActions:
        GitUpdateSubRepositories()
    
    # Apply Premake action
    if 'premake' in selectedActions:
        for compiler in selectedCompilers:
            Premake(pathEngineBuildScripts, nameBuildScript, compiler)


#---------------------------------------------------------------------------------------------------
# Basic os-related methods

# Execute a command, and ensure a print of the shell output.
def ShellExecute(cmd, bSilent=False, bPrintCmd=True):
    if bPrintCmd:
        print('CMD : '+ cmd)
    p = Popen(cmd,
        shell=True,
        universal_newlines=True,
        stdin=PIPE,
        stdout=PIPE,
        stderr=STDOUT)
    output = p.stdout.read()
    if (not bSilent) and output:
        for outputLine in output.splitlines():
            print(outputLine)

# Get the size of a folder in bytes. Use ConvertBytes for a pretty print.
def GetFolderSize(folder):
    total_size = 0
    for item in os.listdir(folder):
        itempath = os.path.join(folder, item)
        if os.path.isfile(itempath):
            total_size += os.path.getsize(itempath)
        elif os.path.isdir(itempath):
            total_size += GetFolderSize(itempath)
    return total_size

# Pretty print of a byte size.
def ConvertBytes(value):
    if value < 1024.0:
        return str(round(value,2))+'o'
    value = value / 1024.0
    if value < 1024.0:
        return str(round(value,2))+'ko'
    value = value / 1024.0
    if value < 1024.0:
        return str(round(value,2))+'mo'
    value = value / 1024.0
    return str(round(value,2))+'go'
    
    
#---------------------------------------------------------------------------------------------------
# Files and Folders handling

# Copy a specified file from "src" to "dst" with a new name. If move=True, the file will be moved instead of copied.
def CopyAndRenameFile(src, dst, srcFilename, dstFilename, move=False):
    if not os.path.isdir(src):
        return

    srcname = os.path.join(src, srcFilename)
    dstname = os.path.join(dst, dstFilename)
    if os.path.isfile(srcname):
        if not os.path.lexists(dst):
            os.makedirs(dst)

        if os.path.isfile(dstname):
            os.chmod(dstname, stat.S_IWUSR)
            os.remove(dstname)
            
        if move:
            shutil.move(srcname, dstname)
        else:
            shutil.copy2(srcname, dstname)

# Copy a specified list of files from "src" to "dst". If move=True, files will be moved instead of copied.
def CopyFiles(src, dst, filenames, move=False):
    if not os.path.isdir(src):
        return

    names = os.listdir(src)
    for name in names:
        if name in filenames:
            srcname = os.path.join(src, name)
            dstname = os.path.join(dst, name)
            if os.path.isfile(srcname):
                if not os.path.lexists(dst):
                    os.makedirs(dst)
                    
                if os.path.isfile(dstname):
                    os.chmod(dstname, stat.S_IWUSR)
                    os.remove(dstname)
                    
                if move:
                    shutil.move(srcname, dst)
                else:
                    shutil.copy2(srcname, dst)

# Copy each file from "src" to "dst". If move=True, files will be moved instead of copied.
def CopyAllFiles(src, dst, move=False):
    if not os.path.isdir(src):
        return
        
    names = os.listdir(src)
    for name in names:
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)
        if os.path.isfile(srcname):
            if not os.path.lexists(dst):
                os.makedirs(dst)
                
            if os.path.isfile(dstname):
                os.chmod(dstname, stat.S_IWUSR)
                os.remove(dstname)
                
            if move:
                shutil.move(srcname, dst)
            else:
                shutil.copy2(srcname, dst)

# Copy (recursively) each files from "src" to "dst", ignoring "ignore" folders (like shutil.ignore_patterns('*.svn')).
def RecursiveCopyAllFiles(src, dst, ignore=None):
    if not os.path.isdir(src):
        return
    if not os.path.lexists(dst):
        os.makedirs(dst)

    CopyAllFiles(src, dst)

    names = os.listdir(src)
    if ignore is not None:
        ignored_names = ignore(src, names)
    else:
        ignored_names = set()

    for name in names:
        if name in ignored_names:
            continue
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)

        if os.path.isdir(srcname):
            RecursiveCopyAllFiles(srcname, dstname, ignore)

# Copy each file with extension in "exts" from "src" to "dst" ("exts" can be like ['h','cpp']). If move=True, files will be moved instead of copied.
def CopyAllFilesWithExtensions(src, dst, exts, move=False):
    if not os.path.isdir(src):
        return
        
    names = os.listdir(src)
    for name in names:
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)
        if os.path.isfile(srcname):
            for ext in exts:
                if srcname.lower().endswith('.'+ext):
                
                    if not os.path.lexists(dst):
                        os.makedirs(dst)
                        
                    if os.path.isfile(dstname):
                        os.chmod(dstname, stat.S_IWUSR)
                        os.remove(dstname)
                        
                    if move:
                        shutil.move(srcname, dst)
                    else:
                        shutil.copy2(srcname, dst)

# Call CopyAllFilesWithExtensions with a single extension.
def CopyAllFilesWithExtension(src, dst, ext, bMove=False):
    CopyAllFilesWithExtensions(src, dst, [ext], bMove)

# Copy (recursively) each files with "ext" extension from "src" to "dst", ignoring "ignore" folders (like shutil.ignore_patterns('*.svn')).
def RecursiveCopyAllFilesWithExtensions(src, dst, exts, ignore=None):
    if not os.path.isdir(src):
        return
    if not os.path.lexists(dst):
        os.makedirs(dst)

    CopyAllFilesWithExtensions(src, dst, exts)

    names = os.listdir(src)
    if ignore is not None:
        ignored_names = ignore(src, names)
    else:
        ignored_names = set()

    for name in names:
        if name in ignored_names:
            continue
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)

        if os.path.isdir(srcname):
            RecursiveCopyAllFilesWithExtensions(srcname, dstname, exts, ignore)
            
# Call RecursiveCopyAllFilesWithExtensions with a single extension.
def RecursiveCopyAllFilesWithExtension(src, dst, ext, ignore=None):
    RecursiveCopyAllFilesWithExtensions(src, dst, [ext], ignore)


# Remove "srcFilename" file located into "src" directory.
def RemoveFile(src, srcFilename):
    if not os.path.isdir(src):
        return
        
    srcname = os.path.join(src, srcFilename)
    if os.path.isfile(srcname):
        os.chmod(srcname, stat.S_IWUSR)
        os.remove(srcname)

# Remove each file with extension in "exts" from "src" ("exts" can be like ['h','cpp']).
def RemoveAllFilesWithExtensions(src, exts):
    if not os.path.isdir(src):
        return
        
    names = os.listdir(src)
    for name in names:
        srcname = os.path.join(src, name)
        if os.path.isfile(srcname):
            for ext in exts:
                if srcname.lower().endswith('.'+ext):
                    if os.path.isfile(srcname):
                        os.chmod(srcname, stat.S_IWUSR)
                        os.remove(srcname)

# Call RemoveAllFilesWithExtensions with a single extension.
def RemoveAllFilesWithExtension(src, ext):
    RemoveAllFilesWithExtensions(src, [ext])

# Remove (recursively) each files with "ext" extension from "src", ignoring "ignore" folders (like shutil.ignore_patterns('*.svn')).
def RecursiveRemoveAllFilesWithExtensions(src, exts, ignore=None):
    if not os.path.isdir(src):
        return

    RemoveAllFilesWithExtensions(src, exts)

    names = os.listdir(src)
    if ignore is not None:
        ignored_names = ignore(src, names)
    else:
        ignored_names = set()

    for name in names:
        if name in ignored_names:
            continue
        srcname = os.path.join(src, name)

        if os.path.isdir(srcname):
            RecursiveRemoveAllFilesWithExtensions(srcname, exts, ignore)
            
# Call RecursiveRemoveAllFilesWithExtensions with a single extension.
def RecursiveRemoveAllFilesWithExtension(src, ext, ignore=None):
    RecursiveRemoveAllFilesWithExtensions(src, [ext], ignore)
    

# Delete a directory recursively. Will delete itself.
def ClearDirectory(src):

    if not os.path.isdir(src):
        return

    names = os.listdir(src)
    for name in names:
        srcname = os.path.join(src, name)

        if os.path.isdir(srcname):
            ClearDirectory(srcname)
        elif os.path.isfile(srcname):
            os.chmod(srcname, stat.S_IWUSR)
            os.remove(srcname)

    os.chmod(src, stat.S_IWUSR)
    os.rmdir(src)
    
    
def ClearEmptyFolders(src):
    if not os.path.isdir(src):
        return
        
    names = os.listdir(src)
    for name in names:
        srcname = os.path.join(src, name)
        if os.path.isdir(srcname):
            ClearEmptyFolders(srcname)
            
    if not os.listdir(src): 
        os.chmod(src, stat.S_IWUSR)
        os.rmdir(src)

    
# If a path doesn't exist, create it.
def EnsurePathExists(path):
    if not os.path.exists(path):
        os.makedirs(path)
    
    
# Ensure a specified folder and all its content is uppercase.
def EnsureUpperCase(src):
    if not os.path.isdir(src):
        return
        
    names = os.listdir(src)
    for name in names:
        srcname = os.path.join(src, name)
        
        if os.path.isdir(srcname):
            EnsureUpperCase(srcname)
            
        if os.path.isfile(srcname):
            os.rename(srcname, srcname.upper())
        
    os.rename(src, src.upper())
        
    
#---------------------------------------------------------------------------------------------------
# Menus and navigation utilities
      
def PromptMenu(strMenu, dictChoices, bNoneMeansAll):
    i = 0
    aKeys = []
    aReturn = []
    
    print('#---- '+ strMenu +' ----#')
    for key, value in dictChoices.items():
        print(str(i) +' - '+ value)
        aKeys.append(key)
        i += 1
        
    answer = input("> Your Choice : ")
    
    if answer == '':
        if bNoneMeansAll:
            aReturn = aKeys
            print('> Choose All')
        else:
            print('> No Choice')
    else:
        for choice in answer.split():
            if choice.isdigit():
                index = int(choice)
                if index >= 0 and index < len(aKeys):
                    value = aKeys[index]
                    if not value in aReturn:
                        aReturn.append(value)
                        print('> Choice : '+ value)
                else:
                    print('> Choice is out of range : '+ choice)
            else:
                print('> Choice is not a number : '+ choice)
            
    print('')
    return aReturn


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
# Git

def GitUpdateSubRepositories():
    ShellExecute('git submodule update --init --recursive')
    print('')
    

#---------------------------------------------------------------------------------------------------
# Zip

def Zip(dirEngineBuildScripts, srcPath):
    ZipTo(dirEngineBuildScripts, srcPath, srcPath+'.zip')

# Using './sourcePath/*' as source instead of './sourcePath' will include all files inside the directory without adding the directory itself (useful for zipping contents, instead of zipping the directory).
def ZipTo(dirEngineBuildScripts, srcPath, dstPath):
    ShellExecute('"' + dirEngineBuildScripts + '/7zip/7za.exe" a "'+ dstPath +'" "'+ srcPath +'"')

def UnzipTo(dirEngineBuildScripts, srcPath, dstPath):
    ShellExecute('"' + dirEngineBuildScripts + '/7zip/7za.exe" x "'+ srcPath +'" -o"'+ dstPath +'" -y')

    
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


#---------------------------------------------------------------------------------------------------
# Count Lines In Directory
#
# call should look like this : CountLinesInDirectory('../Dev/SourcesEngine')

def CountLinesInDirectory(directory, verbose=False):

    # Script inspired by https://github.com/rrwick/LinesOfCodeCounter

    
    #!/usr/bin/env python

    # This Python script counts the lines of code in the directory in which it is
    # run.  It only looks at files which end in the file extensions passed to the
    # script as arguments.

    # It outputs counts for total lines, blank lines, comment lines and code lines
    # (total lines minus blank lines and comment lines).

    # Example usage and output:
    # > lines_of_code_counter.py .h .cpp
    # Total lines:   15378
    # Blank lines:   2945
    # Comment lines: 1770
    # Code lines:    10663

    # Change this value based on the comment symbol used in your programming
    # language.
    commentSymbol = "//"

    acceptableFileExtensions = ['.h', '.hpp', '.cpp', '.tpp'] #sys.argv[1:]
    if not acceptableFileExtensions:
        print ('Please pass at least one file extension as an argument.')
        return

    print ('--------------------')
    print ('Directory:     ' + directory)
    
    #currentDir = os.getcwd()
    currentDir = directory

    filesToCheck = []
    for root, _, files in os.walk(currentDir):
        for f in files:
            fullpath = os.path.join(root, f)
            if '.git' not in fullpath:
                for extension in acceptableFileExtensions:
                    if fullpath.endswith(extension):
                        filesToCheck.append(fullpath)

    if not filesToCheck:
        print ('No files found.')
        print ('')
        return

    lineCount = 0
    totalBlankLineCount = 0
    totalCommentLineCount = 0

    if verbose:
        print ('Filename\tlines\tblank lines\tcomment lines\tcode lines')

    for fileToCheck in filesToCheck:
        with open(fileToCheck, encoding="utf-8") as file:

            fileLineCount = 0
            fileBlankLineCount = 0
            fileCommentLineCount = 0

            for line in file:
                lineCount += 1
                fileLineCount += 1

                lineWithoutWhitespace = line.strip()
                if not lineWithoutWhitespace:
                    totalBlankLineCount += 1
                    fileBlankLineCount += 1
                elif lineWithoutWhitespace.startswith(commentSymbol):
                    totalCommentLineCount += 1
                    fileCommentLineCount += 1

            if verbose:
                print (os.path.basename(fileToCheck) + \
                      "\t" + str(fileLineCount) + \
                      "\t" + str(fileBlankLineCount) + \
                      "\t" + str(fileCommentLineCount) + \
                      "\t" + str(fileLineCount - fileBlankLineCount - fileCommentLineCount))

    if verbose:
        print ('')
        print ('> Totals :')
        
    print ('Lines:         ' + str(lineCount))
    print ('Blank lines:   ' + str(totalBlankLineCount))
    print ('Comment lines: ' + str(totalCommentLineCount))
    print ('Code lines:    ' + str(lineCount - totalBlankLineCount - totalCommentLineCount))
    print ('')
