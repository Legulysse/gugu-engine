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
    FileInfo(const std::string& directoryPath, const std::string& fileName);
    FileInfo(const std::string& path);

    std::string_view GetDirectoryPath() const;
    const std::string& GetFileName() const;
    const std::string& GetFilePath() const;

    std::string_view GetPrettyName() const;
    std::string_view GetExtension() const;
    std::string_view GetAllExtensions() const;
    bool HasExtension(const std::string& extension) const;

    bool operator < (const FileInfo& other) const;    //Used by std sorts
    bool operator == (const FileInfo& other) const;
    bool operator != (const FileInfo& other) const;

private:

    void UpdateFromPath();

private:

    std::string m_path;
    std::string m_fileName;
    size_t m_indexSeparator;
};

}   // namespace gugu
