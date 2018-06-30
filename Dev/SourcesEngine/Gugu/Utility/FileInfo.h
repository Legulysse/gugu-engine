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

                FileInfo        ();
                FileInfo        (const std::string& _strPath, const std::string& _strName);
                FileInfo        (const std::string& _strPathName);

    void        SetPathName     (const std::string& _strPathName);
    void        SetPathName     (const std::string& _strPath, const std::string& _strName);
    void        SetPath         (const std::string& _strPath);
    void        SetName         (const std::string& _strName);
    
    std::string GetPath         () const;
    std::string GetName         () const;
    std::string GetPathName     () const;

    std::string GetPrettyName   () const;
    std::string GetExtension    () const;

    bool        IsPathEnd       (const std::string& _strPath) const;

    bool		operator <      (const FileInfo& _oRight) const;	//Used by std sorts
    bool		operator ==     (const FileInfo& _oRight) const;

private:

    std::string m_path;
    std::string m_name;
};

}	// namespace gugu
