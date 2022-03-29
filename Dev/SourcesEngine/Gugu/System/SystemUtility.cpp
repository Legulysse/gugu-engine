////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <locale>

#if defined(GUGU_OS_WIN32)
    #include <windows.h>
    #include <shellapi.h>
#elif defined(GUGU_OS_LINUX)
    #include <dirent.h>
#endif

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

std::string ToString(int _iValue)
{
    return std::to_string(_iValue);
}

std::string ToString(const char* _strValue)
{
    return std::string(_strValue);
}

void WriteInConsole(const std::string& _strLine, bool printInIDE)
{
    std::cout << _strLine;

#if defined(GUGU_OS_WIN32) && defined(GUGU_ENV_VISUAL)
    if (printInIDE)
    {
        OutputDebugStringA(_strLine.c_str());
    }
#endif
}

void WriteInConsoleEndline(const std::string& _strLine, bool printInIDE)
{
    WriteInConsole(_strLine + "\n", printInIDE);
}

void WriteInFileEndline(const std::string& _strFileName, const std::string& _strLine)
{
    std::ofstream oFile;
    oFile.open(_strFileName.c_str(), std::ios::out | std::ios::app);

    if(oFile)
    {
        oFile << _strLine << std::endl;
        oFile.close();
    }
}

std::string StdStringReplace(const std::string& _strValue, const std::string& _strFrom, const std::string& _strTo, bool _bIgnoreCase /*=false*/)
{
    std::string strResult = _strValue;
    StdStringReplaceSelf(strResult, _strFrom, _strTo, _bIgnoreCase);
    return strResult;
}

void StdStringReplace(const std::string& _strValue, const std::string& _strFrom, const std::string& _strTo, std::string& _strResult, bool _bIgnoreCase /*=false*/)
{
    _strResult = _strValue;
    StdStringReplaceSelf(_strResult, _strFrom, _strTo, _bIgnoreCase);
}

void StdStringReplaceSelf(std::string& _strValue, const std::string& _strFrom, const std::string& _strTo, bool _bIgnoreCase /*=false*/)
 {
    size_t iStrLengthFrom  = _strFrom.length();
    size_t iStrLengthTo    = _strTo.length();

    if (!_bIgnoreCase)
    {
        size_t iPos = 0;
        while ((iPos = _strValue.find(_strFrom, iPos)) != std::string::npos)
        {
            _strValue.replace(iPos, iStrLengthFrom, _strTo);
            iPos += iStrLengthTo;
        }
    }
    else
    {
        size_t iStrLengthResult = _strValue.length();

        size_t iPos = 0;
        while ((iPos + iStrLengthFrom) <= iStrLengthResult)
        {
            bool bFound = true;
            for (size_t j = 0; j < iStrLengthFrom; ++j)
            {
                if (tolower(_strFrom[j]) != tolower(_strValue[iPos + j]))
                {
                    bFound = false;
                    break;
                }
            }

            if (bFound)
            {
                _strValue.replace(iPos, iStrLengthFrom, _strTo);
                iStrLengthResult = _strValue.length();

                iPos += iStrLengthTo;
            }
            else
                ++iPos;
        }
    }
}

void StdStringSplit(const std::string& _strValue, const std::string& _strDelimiter, std::vector<std::string>& _vecTokens)
{
    _vecTokens.clear();

    if (_strValue.empty())
        return;

    size_t iStrLengthDelmimiter = _strDelimiter.length();

    size_t iPosTokenStart = 0;
    size_t iPos = 0;
    while ((iPos = _strValue.find(_strDelimiter, iPos)) != std::string::npos)
    {
        std::string strToken = _strValue.substr(iPosTokenStart, iPos - iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(strToken);

        iPosTokenStart = iPos + iStrLengthDelmimiter;
        iPos += iStrLengthDelmimiter;
    }

    size_t iPosEnd = _strValue.length()-1;
    if (iPosTokenStart <= iPosEnd)
    {
        std::string strToken = _strValue.substr(iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(strToken);
    }
}

std::string StdStringToLower(const std::string& _strValue)
{
    std::string strResult = _strValue;
    StdStringToLowerSelf(strResult);
    return strResult;
}

void StdStringToLower(const std::string& _strValue, std::string& _strResult)
{
    _strResult = _strValue;
    StdStringToLowerSelf(_strResult);
}

void StdStringToLowerSelf(std::string& _strValue)
{
    std::locale loc;
    std::transform(_strValue.begin(), _strValue.end(), _strValue.begin(),
        [&loc](const unsigned char c) { return std::tolower(c, loc); });
}

std::string StdStringToUpper(const std::string& _strValue)
{
    std::string strResult = _strValue;
    StdStringToUpperSelf(strResult);
    return strResult;
}

void StdStringToUpper(const std::string& _strValue, std::string& _strResult)
{
    _strResult = _strValue;
    StdStringToUpperSelf(_strResult);
}

void StdStringToUpperSelf(std::string& _strValue)
{
    std::locale loc;
    std::transform(_strValue.begin(), _strValue.end(), _strValue.begin(),
        [&loc](const unsigned char c) { return std::toupper(c, loc); });
}

bool StdStringStartsWith(const std::string& _strValue, const std::string& _strSub)
{
    return _strValue.find(_strSub) == 0;
}

bool StdStringEndsWith(const std::string& _strValue, const std::string& _strSub)
{
    size_t i = _strValue.rfind(_strSub);
    return (i != std::string::npos) && (i == (_strValue.length() - _strSub.length()));
}

std::string StringFormat(const std::string& _tValue)
{
    return _tValue;
}

const std::map<std::string, std::string>& FormatParameters::GetParameters() const
{
    return m_parameters;
}

std::string StringFormat(const std::string& _tValue, const FormatParameters& Params)
{
    std::string strResult = _tValue;
    for (auto kvp : Params.GetParameters())
    {
        StdStringReplaceSelf(strResult, kvp.first, kvp.second, false);
    }
    return strResult;
}

//TODO: rename all folder into directory
std::string NormalizePath(const std::string& _strPath, bool _bIsFolder)
{
    std::string strResult = _strPath;
    NormalizePathSelf(strResult, _bIsFolder);
    return strResult;
}

void NormalizePath(const std::string& _strPath, bool _bIsFolder, std::string& _strNormalized)
{
    _strNormalized = _strPath;
    NormalizePathSelf(_strNormalized, _bIsFolder);
}

void NormalizePathSelf(std::string& _strPath, bool _bIsFolder)
{
    StdStringReplaceSelf(_strPath, "\\", "/");

    //TODO: I must look for all "../" not preceded by "./" but preceded by "x/" !! The current version will cause bugs.
    //Strip all "/xxx/.." parts
    /*size_t iIndexUpperFolder = _strNormalized.find("/..");

    while (iIndexUpperFolder != std::string::npos && iIndexUpperFolder > 0)
    {
        size_t iIndexCut = _strNormalized.find_last_of("/", iIndexUpperFolder-1);
        iIndexCut = (iIndexCut == std::string::npos)? 0 : iIndexCut;

        _strNormalized = _strNormalized.substr(0, iIndexCut) + _strNormalized.substr(iIndexUpperFolder+3);

        iIndexUpperFolder = _strNormalized.find("/..");
    }*/

    //Strip leading '/'
    if (_strPath.size() > 0 && _strPath[0] == '/')
        _strPath = _strPath.substr(1);

    //Add trailing '/' if folder
    if (_bIsFolder && !_strPath.empty())
    {
        char strLastChar = *_strPath.rbegin();
        if (strLastChar != '/')
            _strPath += '/';
    }
}

void OpenFileExplorer(const std::string& path)
{
#if defined(GUGU_OS_WIN32)
    std::string normalizedPath = path;
    StdStringReplaceSelf(normalizedPath, "/", "\\");

    ShellExecuteA(nullptr, "open", normalizedPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);   //ShellExecuteA uses normal strings, ShellExecuteW uses wide strings (which needs a L prefix : L"...")
#endif
}

void OpenWebBrowser(const std::string& _strURL)
{
#if defined(GUGU_OS_WIN32)
    ShellExecuteA(nullptr, "open", _strURL.c_str(), nullptr, nullptr, SW_SHOWNORMAL);   //ShellExecuteA uses normal strings, ShellExecuteW uses wide strings (which needs a L prefix : L"...")
#endif
}

bool EnsureDirectoryExists(const std::string& _strPath)
{
    if (_strPath.empty())
        return true;

#if defined(GUGU_OS_WIN32)

    std::vector<std::string> vecDirectories;
    StdStringSplit(_strPath, "/", vecDirectories);

    std::string strCombinedPath = "";
    for (auto strSubDirectory : vecDirectories)
    {
        strCombinedPath += strSubDirectory;

        if (!(CreateDirectoryA(strCombinedPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()))
        {
            return false;
        }
        
        strCombinedPath += "/";
    }

    return true;

#else

    return false;

#endif
}

void GetFilesList(const std::string& _strPath, std::vector<FileInfo>& _vecFiles, bool _bRecursive)
{
#if defined(GUGU_OS_WIN32)

    std::string strPathNormalized;
    NormalizePath(_strPath, true, strPathNormalized);

    //Win32 path conversion and filter
    std::string strRoot = strPathNormalized;
    StdStringReplaceSelf(strRoot, "/", "\\");
    strRoot += "*";

    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFileA(strRoot.c_str(), &FindFileData);

    BOOL bContinue = (hFind != INVALID_HANDLE_VALUE);
    while (bContinue != 0)
    {
        std::string strFile(FindFileData.cFileName);
        if (strFile[0] != '.')
        {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (_bRecursive)
                    GetFilesList(strPathNormalized + strFile, _vecFiles, _bRecursive);
            }
            else
            {
                _vecFiles.push_back(FileInfo(strPathNormalized, strFile));
            }
        }

        bContinue = FindNextFileA(hFind, &FindFileData);
    }

    FindClose(hFind);

#elif defined(GUGU_OS_LINUX)

    std::string strPathNormalized;
    NormalizePath(_strPath, true, strPathNormalized);

    dirent* sdirent = nullptr;
    DIR* flux = nullptr;

    if ((flux = opendir(strPathNormalized.c_str())) != nullptr)
    {
        while ((sdirent = readdir(flux)) != nullptr)
        {
            std::string strFile(sdirent->d_name);
            if (strFile[0] != '.')
            {
                if (sdirent->d_type == DT_DIR)
                {
                    if (_bRecursive)
                        GetFilesList(strPathNormalized + strFile, _vecFiles, _bRecursive);
                }
                else
                {
                    _vecFiles.push_back(FileInfo(strPathNormalized, strFile));
                }
            }
        }
        closedir(flux);
    }

#endif
}

bool RemoveFile(const std::string& _strPathName)
{
#if defined(GUGU_OS_WIN32)
    return std::remove(_strPathName.c_str()) == 0;
#else
    return false;
#endif
}

int64 GetTimestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() );
    return ms.count();
}

std::string GetTimestampAsString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buffer[32] = { 0 };
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));  // "%Y-%m-%d %H:%M:%S"

    return buffer;
}

}   // namespace gugu
