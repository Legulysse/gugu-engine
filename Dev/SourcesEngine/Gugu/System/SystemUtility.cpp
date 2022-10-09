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

std::string StringNumberFormat(const std::string& value, size_t leadingZeros, const std::string& delimiter)
{
    std::string formattedValue = value;
    StringNumberFormatSelf(formattedValue, leadingZeros, delimiter);
    return formattedValue;
}

void StringNumberFormatSelf(std::string& value, size_t leadingZeros, const std::string& delimiter)
{
    if (value.empty())
    {
        return;
    }

    bool hasSign = value[0] == '-';
    size_t decimalSeparatorIndex = value.find('.');
    decimalSeparatorIndex = (decimalSeparatorIndex == std::string::npos ? value.find(',') : decimalSeparatorIndex);
    size_t fractionalPartOffset = (decimalSeparatorIndex == std::string::npos ? 0 : value.length() - decimalSeparatorIndex);

    // Add leading zeros
    size_t baseLength = value.length() + (hasSign ? -1 : 0) - fractionalPartOffset;
    size_t insertOffset = fractionalPartOffset;

    if (leadingZeros > baseLength)
    {
        size_t missingZeros = leadingZeros - baseLength;

        for (size_t i = 0; i < missingZeros; ++i)
        {
            value.insert(value.length() - insertOffset - baseLength - i, "0");
        }
    }

    // Add delimiter
    baseLength = value.length() + (hasSign ? -1 : 0) - fractionalPartOffset;
    insertOffset = fractionalPartOffset;

    size_t charIndex = 3;
    size_t addedLength = 0;
    while (charIndex < baseLength)
    {
        value.insert(value.length() - insertOffset - charIndex - addedLength, delimiter);

        addedLength += delimiter.length();
        charIndex += 3;
    }
}

std::string NormalizePath(const std::string& path)
{
    std::string resultPath = path;
    NormalizePathSelf(resultPath);
    return resultPath;
}

void NormalizePath(const std::string& path, std::string& resultPath)
{
    resultPath = path;
    NormalizePathSelf(resultPath);
}

void NormalizePathSelf(std::string& path)
{
    // Normalize separator.
    StdStringReplaceSelf(path, '\\', '/');

    if (!path.empty())
    {
        size_t currentSlashOrZero = 0;
        while (currentSlashOrZero != std::string::npos && !path.empty())
        {
            size_t currentSegment = path[currentSlashOrZero] == '/' ? currentSlashOrZero + 1 : currentSlashOrZero;
            bool parseNextSlash = true;

            // Strip leading '/'.
            if (currentSlashOrZero == 0 && path[currentSlashOrZero] == '/')
            {
                path.erase(currentSlashOrZero, 1);
                parseNextSlash = false;
            }
            // Strip "/xxx/../" parts.
            else if (path.size() > currentSegment + 1 && path[currentSegment] == '.' && path[currentSegment + 1] == '.'
                && (path.size() <= currentSegment + 2 || path[currentSegment + 2] == '/'))
            {
                size_t previousSlash = currentSlashOrZero > 0 ? path.rfind('/', currentSlashOrZero - 1) : std::string::npos;
                size_t previousSlashOrZero = previousSlash != std::string::npos ? previousSlash : 0;
                size_t previousSegment = previousSlash != std::string::npos ? previousSlash + 1 : 0;

                if (previousSlashOrZero != currentSlashOrZero)
                {
                    bool isPreviousSegmentUpperDirectory = path.size() > previousSegment + 1
                        && path[previousSegment] == '.' && path[previousSegment + 1] == '.'
                        && (path.size() <= previousSegment + 2 || path[previousSegment + 2] == '/');
                    
                    if (!isPreviousSegmentUpperDirectory)
                    {
                        path.erase(previousSegment, (currentSegment - previousSegment) + 3);
                        currentSlashOrZero = previousSlashOrZero;
                        parseNextSlash = false;
                    }
                }
            }
            // Strip '/./'.
            else if (path.size() > currentSegment && path[currentSegment] == '.'
                && (path.size() <= currentSegment + 1 || path[currentSegment + 1] == '/'))
            {
                path.erase(currentSegment, 2);
                parseNextSlash = false;
            }
            // Strip '//'.
            else if (path.size() > currentSegment && path[currentSegment] == '/')
            {
                path.erase(currentSegment, 1);
                parseNextSlash = false;
            }

            if (parseNextSlash)
            {
                currentSlashOrZero = path.find('/', currentSlashOrZero + 1);
            }
        }
    }

    // Handle trailing separator.
    if (!path.empty() && path.back() == '/')
    {
        path.pop_back();
    }
}

std::string EnsureTrailingPathSeparator(const std::string& path)
{
    std::string resultPath = path;
    EnsureTrailingPathSeparatorSelf(resultPath);
    return resultPath;
}

void EnsureTrailingPathSeparator(const std::string& path, std::string& resultPath)
{
    resultPath = path;
    EnsureTrailingPathSeparatorSelf(resultPath);
}

void EnsureTrailingPathSeparatorSelf(std::string& path)
{
    if (path.empty() || path.back() != '/')
    {
        path.push_back('/');
    }
}

std::string PathFromPathFile(const std::string& pathFile)
{
    std::string resultPath = pathFile;
    PathFromPathFileSelf(resultPath);
    return resultPath;
}

void PathFromPathFile(const std::string& pathFile, std::string& path)
{
    path = pathFile;
    PathFromPathFileSelf(path);
}

void PathFromPathFileSelf(std::string& pathFile)
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

    NormalizePathSelf(pathFile);
}

std::string FileFromPathFile(const std::string& pathFile)
{
    std::string resultFile = pathFile;
    FileFromPathFileSelf(resultFile);
    return resultFile;
}

void FileFromPathFile(const std::string& pathFile, std::string& file)
{
    file = pathFile;
    FileFromPathFileSelf(file);
}

void FileFromPathFileSelf(std::string& pathFile)
{
    size_t indexSlash = pathFile.rfind('/');

    if (indexSlash != std::string::npos)
    {
        pathFile.erase(0, indexSlash + 1);
    }
}

std::string CombinePaths(const std::string& pathLeft, const std::string& pathRight)
{
    std::string resultPath;
    CombinePaths(pathLeft, pathRight, resultPath);
    return resultPath;
}

void CombinePaths(const std::string& pathLeft, const std::string& pathRight, std::string& resultPath)
{
    if (pathLeft.empty() && pathRight.empty())
    {
        resultPath = "";
    }
    else if (pathLeft.empty())
    {
        resultPath = pathRight;
        NormalizePathSelf(resultPath);
    }
    else if (pathRight.empty())
    {
        resultPath = pathLeft;
        NormalizePathSelf(resultPath);
    }
    else
    {
        resultPath = pathLeft + '/' + pathRight;
        NormalizePathSelf(resultPath);
    }
}

bool PathStartsWith(const std::string& path, const std::string& subPath)
{
    //TODO: I can probably avoid creating strings here and directly compare paths.
    return StdStringStartsWith(EnsureTrailingPathSeparator(path), EnsureTrailingPathSeparator(subPath));
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

void GetFiles(const std::string& rootPath, std::vector<FileInfo>& files, bool recursive)
{
#if defined(GUGU_OS_WIN32)

    std::string strPathNormalized;
    NormalizePath(rootPath, strPathNormalized);

    //Win32 path conversion and filter
    std::string strRoot = strPathNormalized;
    StdStringReplaceSelf(strRoot, '/', '\\');
    strRoot += "/*";

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
                if (recursive)
                {
                    // TODO: Should I provide some kind of CombinePathsUnsafe() ?
                    GetFiles(strPathNormalized + '/' + strFile, files, recursive);
                }
            }
            else
            {
                files.push_back(FileInfo(strPathNormalized, strFile));
            }
        }

        bContinue = FindNextFileA(hFind, &FindFileData);
    }

    FindClose(hFind);

#elif defined(GUGU_OS_LINUX)

    std::string strPathNormalized;
    NormalizePath(rootPath, strPathNormalized);

    dirent* sdirent = nullptr;
    DIR* flux = nullptr;

    // TODO: Check this works now that I removed the trailing '/'.
    if ((flux = opendir(strPathNormalized.c_str())) != nullptr)
    {
        while ((sdirent = readdir(flux)) != nullptr)
        {
            std::string strFile(sdirent->d_name);
            if (strFile[0] != '.')
            {
                if (sdirent->d_type == DT_DIR)
                {
                    if (recursive)
                    {
                        GetFiles(strPathNormalized + '/' + strFile, files, recursive);
                    }
                }
                else
                {
                    files.push_back(FileInfo(strPathNormalized, strFile));
                }
            }
        }

        closedir(flux);
    }

#endif
}

void GetDirectories(const std::string& rootPath, std::vector<std::string>& directories, bool recursive)
{
#if defined(GUGU_OS_WIN32)

    std::string strPathNormalized;
    NormalizePath(rootPath, strPathNormalized);

    //Win32 path conversion and filter
    std::string strRoot = strPathNormalized;
    StdStringReplaceSelf(strRoot, '/', '\\');
    strRoot += "/*";

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
                std::string directoryFullPath = strPathNormalized + '/' + strFile;

                directories.push_back(directoryFullPath);

                if (recursive)
                {
                    GetDirectories(directoryFullPath, directories, recursive);
                }
            }
        }

        bContinue = FindNextFileA(hFind, &FindFileData);
    }

    FindClose(hFind);

#endif
}

bool DirectoryExists(const std::string& _strPath)
{
    if (_strPath.empty())
        return true;

#if defined(GUGU_OS_WIN32)

    DWORD dwAttrib = GetFileAttributesA(_strPath.c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));

#else

    return false;

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
