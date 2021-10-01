import sys
import os, os.path


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

    acceptableFileExtensions = ['.h', '.cpp', '.tpp'] #sys.argv[1:]
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


# Main
CountLinesInDirectory('../Dev/SourcesEngine')
CountLinesInDirectory('../Dev/SourcesEditor')
CountLinesInDirectory('../Dev/SourcesEditorApp')
CountLinesInDirectory('../Dev/SourcesDemos')
CountLinesInDirectory('../Dev/SourcesSFML')
CountLinesInDirectory('../Dev/SourcesPugiXml')
CountLinesInDirectory('../Dev/SourcesImGui')


print('')
input('Press Enter to close.')

