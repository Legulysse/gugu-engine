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
    NormalizePath(path, m_path);
    UpdateFromPath();
}

void FileInfo::UpdateFromPath()
{
    m_indexSeparator = m_path.find_last_of(System::PathSeparator);

    if (m_indexSeparator != std::string::npos)
    {
        m_fileName = m_path.substr(m_indexSeparator + 1);
    }
    else
    {
        m_fileName = m_path;
    }
}

std::string FileInfo::GetDirectoryPath() const
{
    //TODO: optimize this (wait for string_view ?).
    return m_indexSeparator == std::string::npos ? std::string() : m_path.substr(0, m_indexSeparator);
}

const std::string& FileInfo::GetFileName() const
{
    return m_fileName;
}

const std::string& FileInfo::GetFilePath() const
{
    return m_path;
}

std::string FileInfo::GetPrettyName() const
{
    size_t pos = m_fileName.find_first_of(".");
    if (pos != std::string::npos)
    {
        return m_fileName.substr(0, pos);
    }

    return m_fileName;
}

std::string FileInfo::GetExtension() const
{
    size_t pos = m_fileName.find_last_of(".");
    if (pos != std::string::npos)
    {
        return m_fileName.substr(pos + 1);
    }

    return "";
}

std::string FileInfo::GetAllExtensions() const
{
    size_t pos = m_fileName.find_first_of(".");
    if (pos != std::string::npos)
    {
        return m_fileName.substr(pos + 1);
    }

    return "";
}

bool FileInfo::HasExtension(const std::string& extension) const
{
    return m_fileName.size() >= extension.size() + 1
        && m_fileName[m_fileName.size() - extension.size() - 1] == '.'
        && StdStringEndsWith(m_fileName, extension);
}

bool FileInfo::operator < (const FileInfo& other) const
{
    //TODO: optimize this (wait for string_view ?).
    std::string path = GetDirectoryPath();
    std::string otherPath = other.GetDirectoryPath();
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
