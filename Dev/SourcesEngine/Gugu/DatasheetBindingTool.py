
#######################################################
# Public Interfaces


# Generates the C++ binding from the xml binding.
# > _pathBindingXml : the path to the source xml definition.
# > _pathBindingCpp : the destination folder for the generated C++ files ('.h' and '.cpp' files).
# > _generatedFileName : the base name for generated C++ files (If empty, defaults to DatasheetBinding).

def GenerateBindingCpp(_pathBindingXml, _pathBindingCpp, _generatedFileName):
    if _generatedFileName == '':
        _generatedFileName = 'DatasheetBinding'
    GenerateBindingCpp_Impl(_pathBindingXml, _pathBindingCpp, _generatedFileName)


    
#######################################################
# Implementation

from xml.dom import minidom
import os


#------------------------------------------------------
# Classes

class DefinitionBinding():
    def __init__(self):
        self.namespace = ''
        self.enums = []
        self.classes = []           # array of objects (TODO: clean and merge with self.dictClassNames)
        self.dictEnums = {}
        self.dictClassNames = {}    # dictionary of string (name) > string (class) (TODO: clean and merge with self.classes)
        
class DefinitionEnum():
    def __init__(self):
        self.name = ''
        self.code = ''
        self.values = []
        
    def GetDefaultCpp(self, _strDefault):
        if _strDefault == '':
            if len(self.values) == 0:
                return 'NoValuesDefined'
            return self.code +'::'+ self.values[0].name
        return self.code +'::'+ _strDefault
        
    def SaveDeclarationCpp(self, _file):
        _file.write('////////////////////////////////////////////////////////////////\n')
        
        _file.write('namespace '+ self.code +' {\n')
        
        _file.write('    enum Type\n')
        _file.write('    {\n')
        for enumValue in self.values:
            _file.write('        '+ enumValue.name +',\n')
        _file.write('    };\n')
        
        _file.write('\n')
        _file.write('    const gugu::DataEnumInfos* GetDataEnumInfos();\n')
        _file.write('    void GetEnumValues(std::vector<'+ self.code +'::Type>& enumValues);\n')
        _file.write('    size_t GetSize();\n')
        _file.write('\n')
        _file.write('    void Register();\n')
        
        _file.write('}\n')
        
    def SaveImplementationCpp(self, _file):
        _file.write('////////////////////////////////////////////////////////////////\n')
    
        _file.write('namespace '+ self.code +'\n')
        _file.write('{\n')
        
        _file.write('    void Register()\n')
        _file.write('    {\n')
        _file.write('        gugu::DataEnumInfos* enumInfos = new gugu::DataEnumInfos;\n')
        _file.write('        enumInfos->values.reserve('+ str(len(self.values)) +');\n')
        for enumValue in self.values:
            _file.write('        enumInfos->values.push_back("'+ enumValue.name +'");\n')
        _file.write('\n')
        _file.write('        gugu::GetResources()->RegisterDataEnumInfos("'+ self.name +'", enumInfos);\n')
        _file.write('    }\n')
        
        _file.write('\n')
        _file.write('    const gugu::DataEnumInfos* GetDataEnumInfos()\n')
        _file.write('    {\n')
        _file.write('        return gugu::GetResources()->GetDataEnumInfos("'+ self.name +'");\n')
        _file.write('    }\n')
        
        _file.write('\n')
        _file.write('    void GetEnumValues(std::vector<'+ self.code +'::Type>& enumValues)\n')
        _file.write('    {\n')
        _file.write('        enumValues.reserve('+ str(len(self.values)) +');\n')
        for enumValue in self.values:
            _file.write('        enumValues.push_back('+ self.code +'::'+ enumValue.name +');\n')
        _file.write('    }\n')
        
        _file.write('\n')
        _file.write('    size_t GetSize()\n')
        _file.write('    {\n')
        _file.write('        const gugu::DataEnumInfos* enumInfos = gugu::GetResources()->GetDataEnumInfos("'+ self.name +'");\n')
        _file.write('        if (enumInfos)\n')
        _file.write('            return enumInfos->values.size();\n')
        _file.write('        return 0;\n')
        _file.write('    }\n')
            
        _file.write('}\n')
        
    def SaveRegisterMethodCpp(self, _file):
        _file.write('    '+ self.code +'::Register();\n')
        
class DefinitionEnumValue():
    def __init__(self):
        self.name = ''

class DefinitionMember():
    def __init__(self):
        self.name = ''
        self.code = ''
        self.type = ''
        self.isArray = False
        self.isReference = False
        self.isInstance = False
        self.default = ''
        
    def ParseType(self, _strType):
        aFlags = _strType.split(':')
        if len(aFlags) > 0:
            self.isArray        = 'array' in aFlags
            self.isReference    = 'reference' in aFlags
            self.isInstance     = 'instance' in aFlags
            self.type           = aFlags[-1]
        
class DefinitionClass():
    def __init__(self):
        self.name = ''
        self.code = ''
        self.type = 'datasheet'
        self.baseClassName = ''
        self.isAbstract = False
        self.members = []
        self.methods = []

    def SaveForwardDeclarationCpp(self, _file):
        _file.write('class '+ self.code +';\n')

    def SaveInstanciationCpp(self, _file):
        if not self.isAbstract:
            _file.write('    if (classType == "'+ self.name +'")\n')
            _file.write('    {\n')
            _file.write('        return new '+ self.code +';\n')
            _file.write('    }\n')

    def SaveDeclarationCpp(self, _file, _definitionBinding):
        _file.write('////////////////////////////////////////////////////////////////\n')
        
        parentClassName = 'gugu::DatasheetObject'
        if self.type == 'datasave':
            parentClassName = 'gugu::DatasaveObject'
        
        if self.baseClassName != '' and self.baseClassName in _definitionBinding.dictClassNames:
            parentClassName = _definitionBinding.dictClassNames[self.baseClassName]
            
        # Declaration
        _file.write('class '+ self.code +' : public '+ parentClassName +'\n')
        _file.write('{\n')
        _file.write('public:\n')
        
        # Constructor / Destructor
        _file.write('\n')
        _file.write('    '+ self.code +'();\n')
        _file.write('    virtual ~'+ self.code +'();\n')
        
        # Methods
        if len(self.methods) > 0:
            _file.write('\n')
            _file.write('public:\n')
            _file.write('\n')        
            for method in self.methods:
                _file.write('    '+ method +'\n')
        
        # Members
        if len(self.members) > 0:
            _file.write('\n')
            _file.write('public:\n')
            _file.write('\n')        
            for member in self.members:
                strType = ''
                    
                if member.isReference:
                    strType = 'const '
                    strType += _definitionBinding.dictClassNames[member.type]
                    strType += '*'
                elif member.isInstance:
                    if self.type == 'datasheet':
                        strType = 'const '
                    strType += _definitionBinding.dictClassNames[member.type]
                    strType += '*'
                elif member.type in _definitionBinding.dictEnums:
                    strType = _definitionBinding.dictEnums[member.type].code
                    strType += '::Type'
                else:
                    if member.type == 'string':
                        strType = 'std::string'
                    elif member.type == 'int':
                        strType = 'int'
                    elif member.type == 'float':
                        strType = 'float'
                    elif member.type == 'bool':
                        strType = 'bool'
                        
                    else:
                        print('Error : Unkown type "'+member.type+'" for member "'+member.name+'", skipping member declaration.')
                        continue
                    
                if member.isArray:
                    strType = 'std::vector<'+ strType +'>'
                    
                _file.write('    '+ strType +' '+ member.code +';\n')
        
        # Parser
        _file.write('\n')
        _file.write('protected:\n')
        _file.write('\n')
        _file.write('    virtual void ParseMembers(gugu::DatasheetParserContext& context) override;\n')

        # Serializer
        if self.type == 'datasave':
            _file.write('    virtual void SerializeMembers(gugu::DataSaveContext& context) const override;\n')
            _file.write('\n')
            _file.write('    virtual const std::string& GetDataInstanceType() const override;\n')

        # Finalize
        _file.write('};\n')
        _file.write('\n')

    def SaveImplementationCpp(self, _file, _definitionBinding):
        _file.write('////////////////////////////////////////////////////////////////\n')
        
        parentClassName = 'gugu::DatasheetObject'
        hasConcreteParentClass = False;
        
        if self.baseClassName != '' and self.baseClassName in _definitionBinding.dictClassNames:
            parentClassName = _definitionBinding.dictClassNames[self.baseClassName]
            hasConcreteParentClass = True;
        
        # Constructor
        _file.write(''+ self.code +'::'+ self.code +'()\n')
        _file.write('{\n')
        for member in self.members:
            if member.type != '' and member.name != '' and member.code != '':
                if not member.isArray:
                    if member.isReference:
                        _file.write('    '+ member.code +' = nullptr;\n')
                    elif member.isInstance:
                        _file.write('    '+ member.code +' = nullptr;\n')
                    elif member.type in _definitionBinding.dictEnums:
                        strDefault = _definitionBinding.dictEnums[member.type].GetDefaultCpp(member.default)
                        _file.write('    '+ member.code +' = '+ strDefault +';\n')
                    else:
                        strDefault = member.default
                        
                        if member.type == 'string':
                            if strDefault == '':
                                strDefault = '""'
                            else:
                                strDefault = '"'+ strDefault +'"'
                        elif member.type == 'int':
                            if strDefault == '':
                                strDefault = '0'
                        elif member.type == 'float':
                            if strDefault == '':
                                strDefault = '0.f'
                        elif member.type == 'bool':
                            if strDefault == '':
                                strDefault = 'false'
                                
                        else:
                            #print('Error : Unkown type "'+member.type+'" for member "'+member.name+'", skipping member implementation.')
                            continue
                                
                        _file.write('    '+ member.code +' = '+ strDefault +';\n')
                    
        _file.write('}\n')
        
        # Destructor
        _file.write('\n')
        _file.write(''+ self.code +'::~'+ self.code +'()\n')
        _file.write('{\n')
        for member in self.members:
            if member.type != '' and member.name != '' and member.code != '':
                if member.isReference:
                    if not member.isArray:
                        _file.write('    '+ member.code +' = nullptr;\n')
                    else:
                        _file.write('    '+ member.code +'.clear();\n')
                elif member.isInstance:
                    if not member.isArray:
                        _file.write('    SafeDelete('+ member.code +');\n')
                    else:
                        _file.write('    ClearStdVector('+ member.code +');\n')
                else:
                    pass
                    
        _file.write('}\n')
        
        # Parser
        _file.write('\n')
        _file.write('void '+ self.code +'::ParseMembers(gugu::DatasheetParserContext& context)\n')
        _file.write('{\n')
        
        if hasConcreteParentClass:
            _file.write('    '+ parentClassName +'::ParseMembers(context);\n')
            _file.write('\n')
        else:
            _file.write('    //'+ parentClassName +'::ParseMembers(context);\n')
            _file.write('\n')
        
        for member in self.members:
            if member.type != '' and member.name != '' and member.code != '':
                if member.isReference:
                    if not member.isArray:
                        _file.write('    gugu::DataBinding::ReadDatasheetReference(context, "'+member.name +'", '+ member.code +');\n')
                    else:
                        _file.write('    gugu::DataBinding::ReadDatasheetReferenceArray(context, "'+member.name +'", '+ member.code +');\n')
                elif member.isInstance:
                    if self.type == 'datasheet':
                        if not member.isArray:
                            _file.write('    gugu::DataBinding::ReadDatasheetInstance(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                        else:
                            _file.write('    gugu::DataBinding::ReadDatasheetInstanceArray(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                    else:
                        if not member.isArray:
                            _file.write('    gugu::DataBinding::ReadDatasaveInstance(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                        else:
                            _file.write('    gugu::DataBinding::ReadDatasaveInstanceArray(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                elif member.type in _definitionBinding.dictEnums:
                    if not member.isArray:
                        _file.write('    gugu::DataBinding::ReadEnum(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                    else:
                        _file.write('    gugu::DataBinding::ReadEnumArray(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                else:
                    strMethod = 'gugu::DataBinding::Read'
                    
                    if member.type == 'string':
                        strMethod += 'String'
                    elif member.type == 'int':
                        strMethod += 'Int'
                    elif member.type == 'float':
                        strMethod += 'Float'
                    elif member.type == 'bool':
                        strMethod += 'Bool'
                    else:
                        #print('Error : Unkown type "'+member.type+'" for member "'+member.name+'", skipping parsing method declaration.')
                        continue
                        
                    if member.isArray:
                        strMethod += 'Array'
                            
                    _file.write('    '+ strMethod +'(context, "'+member.name +'", '+ member.code +');\n')
                
        _file.write('}\n')
        
        # Serializer
        if self.type == 'datasave':
            _file.write('\n')
            _file.write('void '+ self.code +'::SerializeMembers(gugu::DataSaveContext& context) const\n')
            _file.write('{\n')
            
            if hasConcreteParentClass:
                _file.write('    '+ parentClassName +'::SerializeMembers(context);\n')
                _file.write('\n')
            else:
                _file.write('    //'+ parentClassName +'::SerializeMembers(context);\n')
                _file.write('\n')
                
            for member in self.members:
                if member.type != '' and member.name != '' and member.code != '':
                    if member.isReference:
                        if not member.isArray:
                            _file.write('    gugu::DataBinding::WriteDatasheetReference(context, "'+member.name +'", '+ member.code +');\n')
                        else:
                            _file.write('    gugu::DataBinding::WriteDatasheetReferenceArray(context, "'+member.name +'", '+ member.code +');\n')
                    elif member.isInstance:
                        if not member.isArray:
                            _file.write('    gugu::DataBinding::WriteDatasaveInstance(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                        else:
                            _file.write('    gugu::DataBinding::WriteDatasaveInstanceArray(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                    elif member.type in _definitionBinding.dictEnums:
                        if not member.isArray:
                            _file.write('    gugu::DataBinding::WriteEnum(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                        else:
                            _file.write('    gugu::DataBinding::WriteEnumArray(context, "'+member.name +'", "'+ member.type +'", '+ member.code +');\n')
                    else:
                        strMethod = 'gugu::DataBinding::Write'
                        
                        if member.type == 'string':
                            strMethod += 'String'
                        elif member.type == 'int':
                            strMethod += 'Int'
                        elif member.type == 'float':
                            strMethod += 'Float'
                        elif member.type == 'bool':
                            strMethod += 'Bool'
                        else:
                            #print('Error : Unkown type "'+member.type+'" for member "'+member.name+'", skipping parsing method declaration.')
                            continue

                        if member.isArray:
                            strMethod += 'Array'
                        
                        _file.write('    '+ strMethod +'(context, "'+member.name +'", '+ member.code +');\n')
                    
            _file.write('}\n')
            
            _file.write('\n')
            _file.write('const std::string& '+ self.code +'::GetDataInstanceType() const\n')
            _file.write('{\n')
            _file.write('    static std::string dataInstanceType = "'+ self.name +'";\n')
            _file.write('    return dataInstanceType;\n')
            _file.write('}\n')
        
        # Finalize
        _file.write('\n')


#------------------------------------------------------
# Generators

def GenerateBindingCpp_Impl(_pathBindingXml, _pathBindingCpp, _generatedFileName):

    pathBindingCpp = os.path.normpath(_pathBindingCpp)
    pathBindingCpp += '/'+ _generatedFileName
    
    xmlBinding = minidom.parse(_pathBindingXml).documentElement
    definitionBinding = ParseXmlBinding(xmlBinding)
    
    #-- Header File --#
    filePathHPP = pathBindingCpp +'.h'
    print('> Generating '+ filePathHPP)
    fileHeader = open(filePathHPP, 'w')
    fileHeader.write('#pragma once\n')
    fileHeader.write('\n')
    fileHeader.write('////////////////////////////////////////////////////////////////\n')
    fileHeader.write('// Includes\n')
    fileHeader.write('\n')
    fileHeader.write('#include "Gugu/Data/DataBindingUtility.h"\n') # This could be replaced by forward declarations, but it simplifies user integration.
    fileHeader.write('#include "Gugu/Data/DatasheetObject.h"\n')
    fileHeader.write('#include "Gugu/Data/DatasaveObject.h"\n')
    fileHeader.write('\n')
    fileHeader.write('////////////////////////////////////////////////////////////////\n')
    fileHeader.write('// File Declarations\n')
    fileHeader.write('\n')

    # Namespace
    if definitionBinding.namespace != '':
        fileHeader.write('namespace '+ definitionBinding.namespace +' {\n')
        fileHeader.write('\n')
        
    # Forward Declarations
    fileHeader.write('////////////////////////////////////////////////////////////////\n')
    for newClass in definitionBinding.classes:
        newClass.SaveForwardDeclarationCpp(fileHeader)
    
    # Enum Declarations
    fileHeader.write('\n')
    for newEnum in definitionBinding.dictEnums.values():
        newEnum.SaveDeclarationCpp(fileHeader)
        
    # Class Declarations
    fileHeader.write('\n')
    for newClass in definitionBinding.classes:
        newClass.SaveDeclarationCpp(fileHeader, definitionBinding)
        
    # Methods Declarations
    fileHeader.write('////////////////////////////////////////////////////////////////\n')
    fileHeader.write('void DatasheetBinding_Register();\n')
    fileHeader.write('\n')
    
    fileHeader.write('////////////////////////////////////////////////////////////////\n')
    fileHeader.write('gugu::DataObject* DatasheetBinding_InstanciateDatasheetObject(std::string_view classType);\n')
    
    # Namespace
    if definitionBinding.namespace != '':
        fileHeader.write('\n')
        fileHeader.write('} // namespace '+ definitionBinding.namespace +'\n')
        
    fileHeader.close()
    print('> Done')
    print('')

    
    #-- Source File --#
    filePathCPP = pathBindingCpp +'.cpp'
    print('> Generating '+ filePathCPP)
    fileSource = open(filePathCPP, 'w')
    
    fileSource.write('////////////////////////////////////////////////////////////////\n')
    fileSource.write('// Header\n')
    fileSource.write('\n')
    fileSource.write('#include "'+ _generatedFileName +'.h"\n')
    fileSource.write('\n')
    fileSource.write('////////////////////////////////////////////////////////////////\n')
    fileSource.write('// Includes\n')
    fileSource.write('\n')
    fileSource.write('#include "Gugu/Resources/ManagerResources.h"\n')
    fileSource.write('#include "Gugu/System/SystemUtility.h"\n')
    fileSource.write('\n')
    
    # Namespace
    if definitionBinding.namespace != '':
        fileSource.write('namespace '+ definitionBinding.namespace +' {\n')
        fileSource.write('\n')
        
    # Enum Implementations
    for newEnum in definitionBinding.dictEnums.values():
        newEnum.SaveImplementationCpp(fileSource)
    
    # Class Implementations
    fileSource.write('\n')
    for newClass in definitionBinding.classes:
        newClass.SaveImplementationCpp(fileSource, definitionBinding)
        
    # Methods Implementations
    fileSource.write('////////////////////////////////////////////////////////////////\n')
    fileSource.write('void DatasheetBinding_Register()\n')
    fileSource.write('{\n')
    
    if len(definitionBinding.dictEnums) > 0:
        for newEnum in definitionBinding.dictEnums.values():
            newEnum.SaveRegisterMethodCpp(fileSource)
        fileSource.write('\n')
        
    fileSource.write('    gugu::GetResources()->RegisterDatasheetObjectFactory(DatasheetBinding_InstanciateDatasheetObject);\n')
    fileSource.write('}\n')
    
    fileSource.write('\n')
    fileSource.write('////////////////////////////////////////////////////////////////\n')
    fileSource.write('gugu::DataObject* DatasheetBinding_InstanciateDatasheetObject(std::string_view classType)\n')
    fileSource.write('{\n')
    for newClass in definitionBinding.classes:
        newClass.SaveInstanciationCpp(fileSource)
    fileSource.write('    return nullptr;\n')
    fileSource.write('}\n')
    
    # Namespace
    if definitionBinding.namespace != '':
        fileSource.write('\n')
        fileSource.write('} // namespace '+ definitionBinding.namespace +'\n')
        
    fileSource.close()
    print('> Done')
    print('')
    
    print('> Finished C++ Generation')
    print('')


#------------------------------------------------------
# Parsers

def ParseXmlBinding(_xmlBinding):
    definitionBinding = DefinitionBinding()
    
    print('> Parsing Binding File')
    
    if 'namespace' in _xmlBinding.attributes:
        definitionBinding.namespace = _xmlBinding.attributes['namespace'].value
    
    xmlNodeListEnums = _xmlBinding.getElementsByTagName('Enum')
    for xmlEnum in xmlNodeListEnums :
        newEnum = ParseXmlEnum(xmlEnum)
    
        if newEnum.name == '':
            print('Error : An Enum has been declared without a name.')
            continue
        if newEnum.name in definitionBinding.dictClassNames or newEnum.name in definitionBinding.dictEnums:
            print('Error : Type "'+ newEnum.name +'" has already been declared.')
            continue
            
        definitionBinding.dictEnums[newEnum.name] = newEnum
        definitionBinding.enums.append(newEnum)
    
    xmlNodeListClasses = _xmlBinding.getElementsByTagName('Class')
    for xmlClass in xmlNodeListClasses :
        newClass = ParseXmlClass(xmlClass)
        
        if newClass.name == '':
            print('Error : A Class has been declared without a name.')
            continue
        if newClass.name in definitionBinding.dictClassNames or newClass.name in definitionBinding.dictEnums:
            print('Error : Type "'+ newClass.name +'" has already been declared.')
            continue
            
        definitionBinding.dictClassNames[newClass.name] = newClass.code
        definitionBinding.classes.append(newClass)
        
    print('> Done')
    print('')
    
    return definitionBinding
    
def ParseXmlEnum(_xmlEnum):
    newEnum = DefinitionEnum()
    
    if 'name' in _xmlEnum.attributes:
        newEnum.name = _xmlEnum.attributes['name'].value
        
    if 'code' in _xmlEnum.attributes:
        newEnum.code = _xmlEnum.attributes['code'].value
    
    xmlNodeListEnumValues = _xmlEnum.getElementsByTagName('Value')
    for xmlEnumValue in xmlNodeListEnumValues :
        newEnumValue = DefinitionEnumValue()
        if 'name' in xmlEnumValue.attributes:
            newEnumValue.name = xmlEnumValue.attributes['name'].value
        newEnum.values.append(newEnumValue)
            
    return newEnum

def ParseXmlClass(_xmlClass):
    newClass = DefinitionClass()
    
    if 'type' in _xmlClass.attributes:
        newClass.type = _xmlClass.attributes['type'].value
        
    if 'name' in _xmlClass.attributes:
        newClass.name = _xmlClass.attributes['name'].value
        
    if 'code' in _xmlClass.attributes:
        newClass.code = _xmlClass.attributes['code'].value
        
    if 'base' in _xmlClass.attributes:
        newClass.baseClassName = _xmlClass.attributes['base'].value
        
    if 'abstract' in _xmlClass.attributes:
        newClass.isAbstract = _xmlClass.attributes['abstract'].value
    
    # Methods
    xmlNodeListMethods = _xmlClass.getElementsByTagName('Method')
    for xmlMethod in xmlNodeListMethods :
        if 'declaration' in xmlMethod.attributes:
            newClass.methods.append(xmlMethod.attributes['declaration'].value.strip())
    
    # Members
    xmlNodeListMembers = _xmlClass.getElementsByTagName('Data')
    for xmlMember in xmlNodeListMembers :
        newMember = DefinitionMember()
        
        if 'name' in xmlMember.attributes:
            newMember.name = xmlMember.attributes['name'].value
        else:
            print('Error : A Class Member has been declared without a name (class '+ newClass.name +').')
            
        if 'code' in xmlMember.attributes:
            newMember.code = xmlMember.attributes['code'].value
        else:
            newMember.code = newMember.name
            
        if 'type' in xmlMember.attributes:
            newMember.ParseType(xmlMember.attributes['type'].value)
        else:
            print('Error : A Class Member has been declared without a type (class '+ newClass.name +').')
            
        if 'default' in xmlMember.attributes:
            newMember.default = xmlMember.attributes['default'].value
            
        newClass.members.append(newMember)
        
    return newClass
