dirEngineBuildScripts = '../.'

import sys
sys.path.append(dirEngineBuildScripts)
import Utility

Utility.ShellExecute('"Premake5.exe" --help')
input('')

