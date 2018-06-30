import os, shutil, glob


def ListDirectory(pathFolder):
    aFiles = os.listdir(pathFolder)
    return aFiles

def ParseSourceFiles(pathRoot, pathCurrent, dicoFiles, aExcludes):
    aSourceFiles = ListDirectory(pathCurrent)
    
    for strFile in aSourceFiles:
        pathFile = os.path.join(pathCurrent, strFile)
        pathFile = pathFile.replace('\\', '/')
        
        bSkip = False
        for exclude in aExcludes:
            if exclude in pathFile:
                bSkip = True
        if bSkip:
            continue
            
        if os.path.isfile(pathFile):
            strCategory = ''
            if strFile.lower().endswith('.h') or strFile.lower().endswith('.hpp') or strFile.lower().endswith('.tpp'):
                strCategory = 'HEADERS'
            elif strFile.lower().endswith('.c') or strFile.lower().endswith('.cpp'):
                strCategory = 'SOURCES'

            if strCategory != '':
                pathFile = pathFile.replace(pathRoot, '')
                dicoFiles[strCategory].append(pathFile)
                dicoFiles['OTHER_FILES'].append(pathFile)
                
        elif os.path.isdir(pathFile):
            ParseSourceFiles(pathRoot, pathFile, dicoFiles, aExcludes)

def ProcessMakefile(pathMakefile, allPathSources, aExcludes):
    dicoFiles = {}
    dicoFiles['HEADERS'] = []
    dicoFiles['SOURCES'] = []
    dicoFiles['OTHER_FILES'] = []

    for pathSources in allPathSources:
        ParseSourceFiles(pathSources, pathSources, dicoFiles, aExcludes)

    fMakefile = open(pathMakefile, 'r')
    lines = fMakefile.readlines()
    fMakefile.close()

    bLineWrite = True

    fMakefile = open(pathMakefile, 'w')
    for line in lines:

        if line.startswith('#Auto end'):
            bLineWrite = True

        if bLineWrite:
            print(line)
            fMakefile.write(line)

        if line.startswith('#Auto begin'):
            bLineWrite = False
            
            fMakefile.write('\n')
            fMakefile.write('HEADERS += ')
            for strFile in dicoFiles['HEADERS']:
                fMakefile.write(' \\\n            '+strFile)
            fMakefile.write('\n')

            fMakefile.write('\n')
            fMakefile.write('SOURCES += ')
            for strFile in dicoFiles['SOURCES']:
                fMakefile.write(' \\\n            '+strFile)
            fMakefile.write('\n')
            
            fMakefile.write('\n')
            fMakefile.write('OTHER_FILES += ')
            for strFile in dicoFiles['OTHER_FILES']:
                fMakefile.write(' \\\n            '+strFile)
            fMakefile.write('\n')
            
            fMakefile.write('\n')


aSfmlExcludes = [   'SFML/Main/SFMLActivity.',
                    'SFML/Window/EGLCheck.',
                    'SFML/Window/EglContext.',
                    '/Android/',
                    '/FreeBSD/',
                    '/iOS/',
                    '/OSX/',
                    '/Unix/',
                ]

ProcessMakefile('Project/PugiXml/PugiXml.pro', ['../SourcesPugiXml/'], [])
ProcessMakefile('Project/Sfml/Sfml.pro', ['../SourcesSFML/include/', '../SourcesSFML/src/'], aSfmlExcludes)
ProcessMakefile('Project/Engine/Engine.pro', ['../SourcesEngine/'], [])

