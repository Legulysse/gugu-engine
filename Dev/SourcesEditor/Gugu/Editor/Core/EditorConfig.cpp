////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Core/EditorConfig.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

#include <PugiXml/pugixml.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

bool EditorConfig::LoadFromFile(const std::string& pathFile)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(pathFile.c_str());
    if (!result)
        return false;

    pugi::xml_node rootNode = document.child("EditorConfig");
    if (!rootNode)
        return false;

    defaultProjectPathFile = rootNode.child("DefaultProjectPathFile").attribute("value").as_string(defaultProjectPathFile.c_str());
    maxUndoStateCount = Max<size_t>(1, rootNode.child("MaxUndoStateCount").attribute("value").as_uint(maxUndoStateCount));

    return true;
}

}   //namespace gugu
