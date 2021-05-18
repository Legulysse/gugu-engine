////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Utility/FileInfo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/System.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

FileInfo::FileInfo()
: m_path("")
, m_name("")
{
}

FileInfo::FileInfo(const std::string& _strPath, const std::string& _strName)
: m_path("")
, m_name(_strName)
{
    SetPath(_strPath);
}

FileInfo::FileInfo(const std::string& _strPathName)
: m_path("")
, m_name("")
{
    SetPathName(_strPathName);
}

void FileInfo::SetPathName(const std::string& _strPathName)
{
    std::string strPathNameNormalized;
    NormalizePath(_strPathName, false, strPathNameNormalized);

    size_t iPos = strPathNameNormalized.find_last_of("/");
    if (iPos != std::string::npos)
    {
        m_path = strPathNameNormalized.substr(0, iPos + 1);
        m_name = strPathNameNormalized.substr(iPos + 1);
    }
    else
    {
        m_path = "";
        m_name = strPathNameNormalized;
    }
}

void FileInfo::SetPathName(const std::string& _strPath, const std::string& _strName)
{
    SetPath(_strPath);
    SetName(_strName);
}

void FileInfo::SetPath(const std::string& _strPath)
{
    NormalizePath(_strPath, true, m_path);
}

void FileInfo::SetName(const std::string& _strName)
{
    m_name = _strName;
}

std::string FileInfo::GetPath() const
{
    return m_path;
}

std::string FileInfo::GetName() const
{
    return m_name;
}

std::string FileInfo::GetPathName() const
{
    return m_path + m_name;
}

std::string FileInfo::GetPrettyName() const
{
    size_t iPos = m_name.find_first_of(".");
    if (iPos != std::string::npos)
    {
        return m_name.substr(0, iPos);
    }

    return m_name;
}

std::string FileInfo::GetExtension() const
{
    size_t iPos = m_name.find_first_of(".");
    if (iPos != std::string::npos)
    {
        return m_name.substr(iPos+1);
    }

    return "";
}

bool FileInfo::IsPathEnd(const std::string& _strPath) const
{
    return StdStringEndsWith(m_path, _strPath);
}

bool FileInfo::operator < (const FileInfo& _oRight) const
{
    return m_name < _oRight.m_name;
}

bool FileInfo::operator == (const FileInfo& _oRight) const
{
    return m_name == _oRight.m_name && m_path == _oRight.m_path;
}

}   // namespace gugu
