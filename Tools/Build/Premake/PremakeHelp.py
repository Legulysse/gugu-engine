dirEngineBuildScripts = '../.'

import sys
sys.path.append(dirEngineBuildScripts)
import GuguUtility

GuguUtility.ShellExecute('"Premake5.exe" --help')
input('')

