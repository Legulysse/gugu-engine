import sys

sys.path.append('../../../SourcesEngine/Gugu')
from DatasheetBindingTool import *


# Generates the C++ binding from the xml binding
# > _pathBindingXml : the path to the source xml definition
# > _pathBindingCpp : the destination folder for the C++ files (DatasheetBinding.h and DatasheetBinding.cpp)
GenerateBindingCpp('Binding.xml', '')


input('Press Enter to close.')
