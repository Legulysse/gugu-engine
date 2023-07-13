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

FileInfo::FileInfo(const std::string& directoryPath, const std::string& fileName)
{
    CombinePaths(directoryPath, fileName, m_path);
    UpdateFromPath();
}

FileInfo::FileInfo(const std::string& path)
{
    m_path = NormalizePath(path);
    UpdateFromPath();
}

void FileInfo::UpdateFromPath()
{
    m_indexSeparator = m_path.find_last_of(system::PathSeparator);

    if (m_indexSeparator != std::string::npos)
    {
        m_fileName = m_path.substr(m_indexSeparator + 1);
    }
    else
    {
        m_fileName = m_path;
    }
}

std::string_view FileInfo::GetDirectoryPath() const
{
    return m_indexSeparator == std::string::npos ? std::string_view() : std::string_view(m_path).substr(0, m_indexSeparator);
}

const std::string& FileInfo::GetFileName() const
{
    return m_fileName;
}

const std::string& FileInfo::GetFilePath() const
{
    return m_path;
}

std::string_view FileInfo::GetPrettyName() const
{
    size_t pos = m_fileName.find_first_of(system::ExtensionSeparator);
    if (pos != std::string::npos)
    {
        return std::string_view(m_fileName).substr(0, pos);
    }

    return std::string_view(m_fileName);
}

std::string_view FileInfo::GetExtension() const
{
    size_t pos = m_fileName.find_last_of(system::ExtensionSeparator);
    if (pos != std::string::npos)
    {
        return std::string_view(m_fileName).substr(pos + 1);
    }

    return std::string_view();
}

std::string_view FileInfo::GetAllExtensions() const
{
    size_t pos = m_fileName.find_first_of(system::ExtensionSeparator);
    if (pos != std::string::npos)
    {
        return std::string_view(m_fileName).substr(pos + 1);
    }

    return std::string_view();
}

bool FileInfo::HasExtension(const std::string& extension) const
{
    return m_fileName.size() >= extension.size() + 1
        && m_fileName[m_fileName.size() - extension.size() - 1] == system::ExtensionSeparator
        && StdStringEndsWith(m_fileName, extension);
}

bool FileInfo::operator < (const FileInfo& other) const
{
    std::string_view path = GetDirectoryPath();
    std::string_view otherPath = other.GetDirectoryPath();
    return path < otherPath || (path == otherPath && m_fileName < other.m_fileName);
}

bool FileInfo::operator == (const FileInfo& other) const
{
    return m_path == other.m_path;
}

bool FileInfo::operator != (const FileInfo& other) const
{
    return m_path != other.m_path;
}

}   // namespace gugu
