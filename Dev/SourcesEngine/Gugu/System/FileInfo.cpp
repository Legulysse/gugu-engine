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

FileInfo::FileInfo(const std::filesystem::path& path)
{
    m_systemPath = NormalizePath(path.string());

    // Cache
    m_filePath_utf8 = NormalizePath(path.u8string());
    m_indexSeparator = m_filePath_utf8.find_last_of(system::PathSeparator);
}

FileInfo FileInfo::FromPath(const std::filesystem::path& path)
{
    return FileInfo(path);
}

FileInfo FileInfo::FromString_utf8(const std::string& path_utf8)
{
    return FileInfo(std::filesystem::u8path(path_utf8));
}

const std::string& FileInfo::GetFileSystemPath() const
{
    return m_systemPath;
}

std::string_view FileInfo::GetFilePath_utf8() const
{
    return m_filePath_utf8;
}

std::string_view FileInfo::GetDirectoryPath_utf8() const
{
    return m_indexSeparator == std::string::npos ? std::string_view() : std::string_view(m_filePath_utf8).substr(0, m_indexSeparator);
}

std::string_view FileInfo::GetFileName_utf8() const
{
    return m_indexSeparator == std::string::npos ? std::string_view(m_filePath_utf8) : std::string_view(m_filePath_utf8).substr(m_indexSeparator + 1);
}

std::string_view FileInfo::GetPrettyName() const
{
    size_t pos = GetFileName_utf8().find_first_of(system::ExtensionSeparator);
    if (pos != std::string::npos)
    {
        return GetFileName_utf8().substr(0, pos);
    }

    return GetFileName_utf8();
}

std::string_view FileInfo::GetExtension() const
{
    size_t pos = GetFileName_utf8().find_last_of(system::ExtensionSeparator);
    if (pos != std::string::npos)
    {
        return GetFileName_utf8().substr(pos + 1);
    }

    return std::string_view();
}

std::string_view FileInfo::GetAllExtensions() const
{
    size_t pos = GetFileName_utf8().find_first_of(system::ExtensionSeparator);
    if (pos != std::string::npos)
    {
        return GetFileName_utf8().substr(pos + 1);
    }

    return std::string_view();
}

bool FileInfo::HasExtension(const std::string& extension) const
{
    std::string_view fileName = GetFileName_utf8();
    return fileName.size() >= extension.size() + 1
        && fileName[fileName.size() - extension.size() - 1] == system::ExtensionSeparator
        && StdStringEndsWith(fileName, extension);
}

bool FileInfo::operator < (const FileInfo& other) const
{
    std::string_view path = GetDirectoryPath_utf8();
    std::string_view otherPath = other.GetDirectoryPath_utf8();
    return path < otherPath || (path == otherPath && GetFileName_utf8() < other.GetFileName_utf8());
}

bool FileInfo::operator == (const FileInfo& other) const
{
    return m_systemPath == other.m_systemPath;
}

bool FileInfo::operator != (const FileInfo& other) const
{
    return m_systemPath != other.m_systemPath;
}

}   // namespace gugu
