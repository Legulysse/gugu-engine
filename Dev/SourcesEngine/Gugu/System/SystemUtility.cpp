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

std::string StdStringReplace(const std::string& value, const std::string& from, const std::string& to)
{
    std::string result = value;
    StdStringReplaceSelf(result, from, to);
    return result;
}

void StdStringReplace(const std::string& value, const std::string& from, const std::string& to, std::string& result)
{
    result = value;
    StdStringReplaceSelf(result, from, to);
}

void StdStringReplaceSelf(std::string& value, const std::string& from, const std::string& to)
 {
    size_t lengthFrom = from.length();
    size_t lengthTo = to.length();

    size_t pos = 0;
    while ((pos = value.find(from, pos)) != std::string::npos)
    {
        value.replace(pos, lengthFrom, to);
        pos += lengthTo;
    }
}

std::string StdStringReplace(const std::string& value, const char& from, const char& to)
{
    std::string result = value;
    StdStringReplaceSelf(result, from, to);
    return result;
}

void StdStringReplace(const std::string& value, const char& from, const char& to, std::string& result)
{
    result = value;
    StdStringReplaceSelf(result, from, to);
}

void StdStringReplaceSelf(std::string& _strValue, const char& from, const char& to)
{
    std::replace(_strValue.begin(), _strValue.end(), from, to);
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
    return _strValue.size() >= _strSub.size()
        && _strValue.compare(0, _strSub.size(), _strSub) == 0;
}

bool StdStringEndsWith(const std::string& _strValue, const std::string& _strSub)
{
    return _strValue.size() >= _strSub.size()
        && _strValue.compare(_strValue.size() - _strSub.size(), _strSub.size(), _strSub) == 0;
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
        StdStringReplaceSelf(strResult, kvp.first, kvp.second);
    }
    return strResult;
}

std::string NormalizePath(const std::string& _strPath, bool trailingSlash)
{
    std::string strResult = _strPath;
    NormalizePathSelf(strResult, trailingSlash);
    return strResult;
}

void NormalizePath(const std::string& _strPath, bool trailingSlash, std::string& _strNormalized)
{
    _strNormalized = _strPath;
    NormalizePathSelf(_strNormalized, trailingSlash);
}

void NormalizePathSelf(std::string& _strPath, bool trailingSlash)
{
    // Normalize slashes.
    StdStringReplaceSelf(_strPath, '\\', '/');

    if (!_strPath.empty())
    {
        size_t currentSlashOrZero = 0;
        while (currentSlashOrZero != std::string::npos && !_strPath.empty())
        {
            size_t currentSegment = _strPath[currentSlashOrZero] == '/' ? currentSlashOrZero + 1 : currentSlashOrZero;
            bool parseNextSlash = true;

            // Strip leading '/'.
            if (currentSlashOrZero == 0 && _strPath[currentSlashOrZero] == '/')
            {
                _strPath.erase(currentSlashOrZero, 1);
                parseNextSlash = false;
            }
            // Strip "/xxx/../" parts.
            else if (_strPath.size() > currentSegment + 1 && _strPath[currentSegment] == '.' && _strPath[currentSegment + 1] == '.'
                && (_strPath.size() <= currentSegment + 2 || _strPath[currentSegment + 2] == '/'))
            {
                size_t previousSlash = currentSlashOrZero > 0 ? _strPath.rfind('/', currentSlashOrZero - 1) : std::string::npos;
                size_t previousSlashOrZero = previousSlash != std::string::npos ? previousSlash : 0;
                size_t previousSegment = previousSlash != std::string::npos ? previousSlash + 1 : 0;

                if (previousSlashOrZero != currentSlashOrZero)
                {
                    bool isPreviousSegmentUpperDirectory = _strPath.size() > previousSegment + 1
                        && _strPath[previousSegment] == '.' && _strPath[previousSegment + 1] == '.'
                        && (_strPath.size() <= previousSegment + 2 || _strPath[previousSegment + 2] == '/');
                    
                    if (!isPreviousSegmentUpperDirectory)
                    {
                        _strPath.erase(previousSegment, (currentSegment - previousSegment) + 3);
                        currentSlashOrZero = previousSlashOrZero;
                        parseNextSlash = false;
                    }
                }
            }
            // Strip '/./'.
            else if (_strPath.size() > currentSegment && _strPath[currentSegment] == '.'
                && (_strPath.size() <= currentSegment + 1 || _strPath[currentSegment + 1] == '/'))
            {
                _strPath.erase(currentSegment, 2);
                parseNextSlash = false;
            }
            // Strip '//'.
            else if (_strPath.size() > currentSegment && _strPath[currentSegment] == '/')
            {
                _strPath.erase(currentSegment, 1);
                parseNextSlash = false;
            }

            if (parseNextSlash)
            {
                currentSlashOrZero = _strPath.find('/', currentSlashOrZero + 1);
            }
        }
    }

    // Handle trailing slash.
    if (trailingSlash && !_strPath.empty() && _strPath.back() != '/')
    {
        _strPath += '/';
    }
    else if (!trailingSlash && !_strPath.empty() && _strPath.back() == '/')
    {
        _strPath.pop_back();
    }
}

std::string PathFromPathFile(const std::string& pathFile, bool trailingSlash)
{
    std::string resultPath = pathFile;
    PathFromPathFileSelf(resultPath, trailingSlash);
    return resultPath;
}

void PathFromPathFile(const std::string& pathFile, bool trailingSlash, std::string& path)
{
    path = pathFile;
    PathFromPathFileSelf(path, trailingSlash);
}

void PathFromPathFileSelf(std::string& pathFile, bool trailingSlash)
{
    size_t indexDot = pathFile.rfind('.');
    size_t indexSlash = pathFile.rfind('/');
    size_t indexSlashOrZero = indexSlash != std::string::npos ? indexSlash : 0;

    // Check that the last '.' is not the last character, and that it is after the last '/'.
    if (indexDot != std::string::npos
        && indexDot != pathFile.size() - 1
        && indexDot > indexSlashOrZero)
    {
        pathFile.erase(indexSlashOrZero);
    }

    NormalizePathSelf(pathFile, trailingSlash);
}

std::string CombinePathFile(const std::string& pathLeft, const std::string& pathFileRight)
{
    std::string resultPath;
    CombinePaths(pathLeft, pathFileRight, false, resultPath);
    return resultPath;
}

void CombinePathFile(const std::string& pathLeft, const std::string& pathFileRight, std::string& resultPathFile)
{
    CombinePaths(pathLeft, pathFileRight, false, resultPathFile);
}

std::string CombinePaths(const std::string& pathLeft, const std::string& pathRight, bool trailingSlash)
{
    std::string resultPath;
    CombinePaths(pathLeft, pathRight, trailingSlash, resultPath);
    return resultPath;
}

void CombinePaths(const std::string& pathLeft, const std::string& pathRight, bool trailingSlash, std::string& resultPath)
{
    if (pathLeft.empty() && pathRight.empty())
    {
        resultPath = "";
    }
    else if (pathLeft.empty())
    {
        resultPath = pathRight;
        NormalizePathSelf(resultPath, trailingSlash);
    }
    else if (pathRight.empty())
    {
        resultPath = pathLeft;
        NormalizePathSelf(resultPath, trailingSlash);
    }
    else
    {
        resultPath = pathLeft + '/' + pathRight;
        NormalizePathSelf(resultPath, trailingSlash);
    }
}

void OpenFileExplorer(const std::string& path)
{
#if defined(GUGU_OS_WIN32)
    std::string normalizedPath = path;
    StdStringReplaceSelf(normalizedPath, '/', '\\');

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
    StdStringReplaceSelf(strRoot, '/', '\\');
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

    return std::string(buffer);
}

}   // namespace gugu
