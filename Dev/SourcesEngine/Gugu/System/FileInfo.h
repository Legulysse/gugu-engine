#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class FileInfo
{
public:

    FileInfo();
    FileInfo(const std::string& path, const std::string& name);
    FileInfo(const std::string& pathName);

    std::string GetPath(bool trailingSlash) const;
    const std::string& GetName() const;
    const std::string& GetPathName() const;

    std::string GetPrettyName() const;
    std::string GetExtension() const;
    std::string GetAllExtensions() const;
    bool HasExtension(const std::string& extension) const;

    bool operator < (const FileInfo& other) const;    //Used by std sorts
    bool operator == (const FileInfo& other) const;
    bool operator != (const FileInfo& other) const;

private:

    std::string m_pathName;
    std::string m_name;
    size_t m_indexSeparator;
};

}   // namespace gugu
