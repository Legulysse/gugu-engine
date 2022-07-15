////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/FileInfo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

FileInfo::FileInfo()
    : m_indexSeparator(std::string::npos)
{
}

FileInfo::FileInfo(const std::string& path, const std::string& name)
{
    NormalizePath(path, true, m_pathName);

    m_indexSeparator = m_pathName.empty() ? std::string::npos : m_pathName.size() - 1;

    // TODO: I could check that name does not contain any slash (maybe just directly apply CombinePaths ?).
    m_pathName += name;
    m_name = name;
}

FileInfo::FileInfo(const std::string& pathName)
{
    NormalizePath(pathName, false, m_pathName);

    m_indexSeparator = m_pathName.find_last_of("/");
    if (m_indexSeparator != std::string::npos)
    {
        m_name = m_pathName.substr(m_indexSeparator + 1);
    }
}

std::string FileInfo::GetPath(bool trailingSlash) const
{
    //TODO: optimize this (wait for string_view ?).
    return m_indexSeparator == std::string::npos ? std::string() : m_pathName.substr(0, trailingSlash ? m_indexSeparator + 1 : m_indexSeparator);
}

const std::string& FileInfo::GetName() const
{
    return m_name;
}

const std::string& FileInfo::GetPathName() const
{
    return m_pathName;
}

std::string FileInfo::GetPrettyName() const
{
    size_t pos = m_name.find_first_of(".");
    if (pos != std::string::npos)
    {
        return m_name.substr(0, pos);
    }

    return m_name;
}

std::string FileInfo::GetExtension() const
{
    size_t pos = m_name.find_last_of(".");
    if (pos != std::string::npos)
    {
        return m_name.substr(pos + 1);
    }

    return "";
}

bool FileInfo::IsExtension(const std::string& extension) const
{
    return m_name.size() >= extension.size() + 1
        && m_name[m_name.size() - extension.size() - 1] == '.'
        && StdStringEndsWith(m_name, extension);
}

bool FileInfo::operator < (const FileInfo& other) const
{
    //TODO: optimize this (wait for string_view ?).
    std::string path = GetPath(false);
    std::string otherPath = other.GetPath(false);
    return path < otherPath || (path == otherPath && m_name < other.m_name);
}

bool FileInfo::operator == (const FileInfo& other) const
{
    return m_pathName == other.m_pathName;
}

bool FileInfo::operator != (const FileInfo& other) const
{
    return m_pathName != other.m_pathName;
}

}   // namespace gugu
