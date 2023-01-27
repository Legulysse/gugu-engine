import sys

sys.path.append('../../../SourcesEngine/Gugu')
from DataBindingTool import *


# Generates the C++ binding from the xml binding.
# > _pathBindingXml : the path to the source xml definition.
# > _pathBindingCpp : the destination folder for the generated C++ files ('.h' and '.cpp' files).
# > _generatedFileName : the base name for generated C++ files (If empty, defaults to DataBinding).
GenerateBindingCpp('../../../../Version/DemoGame/Editor/DataBinding.xml', '', '')


input('Press Enter to close.')
