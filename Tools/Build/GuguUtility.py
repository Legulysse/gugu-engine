import glob, os, shutil
import stat
import sys
from subprocess import Popen, PIPE, STDOUT

#import configparser
#from sys import platform



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

# Copy each file from "src" to "dst". If bMove, files will be moved instead of copied.
def CopyFiles(src, dst, bMove=False):
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
                
            if bMove:
                shutil.move(srcname, dst)
            else:
                shutil.copy2(srcname, dst)

# Copy (recursively) each files from "src" to "dst", ignoring "ignore" folders (like shutil.ignore_patterns('*.svn')).
def RecursiveCopyFiles(src, dst, ignore=None):
    if not os.path.isdir(src):
        return
    if not os.path.lexists(dst):
        os.makedirs(dst)

    CopyFiles(src, dst)

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
            RecursiveCopyFiles(srcname, dstname, ignore)

# Copy each file with extension in "exts" from "src" to "dst" ("exts" can be like ['h','cpp']). If bMove, files will be moved instead of copied.
def CopyFilesFromExtensions(src, dst, exts, bMove=False):
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
                        
                    if bMove:
                        shutil.move(srcname, dst)
                    else:
                        shutil.copy2(srcname, dst)

# Call CopyFilesFromExtensions with a single extension.
def CopyFilesFromExtension(src, dst, ext, bMove=False):
    CopyFilesFromExtensions(src, dst, [ext], bMove)

# Copy (recursively) each files with "ext" extension from "src" to "dst", ignoring "ignore" folders (like shutil.ignore_patterns('*.svn')).
def RecursiveCopyFilesFromExtensions(src, dst, exts, ignore=None):
    if not os.path.isdir(src):
        return
    if not os.path.lexists(dst):
        os.makedirs(dst)

    CopyFilesFromExtensions(src, dst, exts)

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
            RecursiveCopyFilesFromExtensions(srcname, dstname, exts, ignore)
            
# Call CopyFilesFromExtensionRecursive with a single extension.
def RecursiveCopyFilesFromExtension(src, dst, ext, ignore=None):
    RecursiveCopyFilesFromExtensions(src, dst, [ext], ignore)


# Remove each file with extension in "exts" from "src" ("exts" can be like ['h','cpp']).
def RemoveFilesFromExtensions(src, exts):
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

# Call RemoveFilesFromExtensions with a single extension.
def RemoveFilesFromExtension(src, ext):
    RemoveFilesFromExtensions(src, [ext])

# Remove (recursively) each files with "ext" extension from "src", ignoring "ignore" folders (like shutil.ignore_patterns('*.svn')).
def RecursiveRemoveFilesFromExtensions(src, exts, ignore=None):
    if not os.path.isdir(src):
        return

    RemoveFilesFromExtensions(src, exts)

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
            RecursiveRemoveFilesFromExtensions(srcname, exts, ignore)
            
# Call RecursiveRemoveFilesFromExtensions with a single extension.
def RecursiveRemoveFilesFromExtension(src, ext, ignore=None):
    RecursiveRemoveFilesFromExtensions(src, [ext], ignore)
    

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
