
dirEngineBuildScripts = '../Tools/Build'
dirProjectBuildScripts = '.'

import sys, os

sys.path.append(dirEngineBuildScripts)
import GuguUtility


def Main():
    
    # Setup
    dirCurrent = os.getcwd()
    os.chdir(dirProjectBuildScripts)

    # Default Actions Menu
    GuguUtility.DefaultMenu(dirEngineBuildScripts, 'Build-premake.lua')

    # Finalize
    os.chdir(dirCurrent)

    print('')
    input('Press Enter to close.')


Main()
