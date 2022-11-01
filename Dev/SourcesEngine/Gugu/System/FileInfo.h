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

    std::string GetDirectoryPath() const;
    const std::string& GetFileName() const;
    const std::string& GetFilePath() const;

    std::string GetPrettyName() const;
    std::string GetExtension() const;
    std::string GetAllExtensions() const;
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
