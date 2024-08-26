
dirEngineBuildScripts = 'Build'

import sys

sys.path.append(dirEngineBuildScripts)
import GuguUtility


# Main
GuguUtility.CountLinesInDirectory('../Dev/SourcesEngine')
GuguUtility.CountLinesInDirectory('../Dev/SourcesEditor')
GuguUtility.CountLinesInDirectory('../Dev/SourcesEditorApp')
GuguUtility.CountLinesInDirectory('../Dev/SourcesDemos')
#GuguUtility.CountLinesInDirectory('../Externals/SFML/include')
#GuguUtility.CountLinesInDirectory('../Externals/SFML/src')
#GuguUtility.CountLinesInDirectory('../Externals/PugiXml/src')
#GuguUtility.CountLinesInDirectory('../Externals/ImGui')
#GuguUtility.CountLinesInDirectory('../Externals/ImGuiSFML')


print('')
input('Press Enter to close.')

