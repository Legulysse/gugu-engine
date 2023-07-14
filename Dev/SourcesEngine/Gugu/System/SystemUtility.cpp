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
#include <filesystem>

#if defined(GUGU_OS_WINDOWS)
    #include <windows.h>
    #include <shellapi.h>
#elif defined(GUGU_OS_LINUX)
    #include <dirent.h>
#endif

namespace fs = std::filesystem;

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

bool StringEquals(std::string_view left, std::string_view right)
{
    return left == right;
}

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

#if defined(GUGU_OS_WINDOWS) && defined(GUGU_ENV_VISUAL)
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

void StdStringReplaceSelf(std::string& _strValue, const char& from, const char& to)
{
    std::replace(_strValue.begin(), _strValue.end(), from, to);
}

void StdStringSplit(std::string_view _strValue, std::string_view _strDelimiter, std::vector<std::string>& _vecTokens)
{
    _vecTokens.clear();

    if (_strValue.empty())
        return;

    size_t iStrLengthDelimiter = _strDelimiter.length();

    size_t iPos = 0;
    size_t iPosTokenStart = iPos;
    while ((iPos = _strValue.find(_strDelimiter, iPos)) != std::string::npos)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart, iPos - iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));

        iPos += iStrLengthDelimiter;
        iPosTokenStart = iPos;
    }

    size_t iPosEnd = _strValue.length()-1;
    if (iPosTokenStart <= iPosEnd)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));
    }
}

void StdStringSplit(std::string_view _strValue, char _strDelimiter, std::vector<std::string>& _vecTokens)
{
    _vecTokens.clear();

    if (_strValue.empty())
        return;

    size_t iStrLengthDelimiter = 1;

    size_t iPos = 0;
    size_t iPosTokenStart = iPos;
    while ((iPos = _strValue.find(_strDelimiter, iPos)) != std::string::npos)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart, iPos - iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));

        iPos += iStrLengthDelimiter;
        iPosTokenStart = iPos;
    }

    size_t iPosEnd = _strValue.length() - 1;
    if (iPosTokenStart <= iPosEnd)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));
    }
}

std::string StdStringToLower(const std::string& _strValue)
{
    std::string strResult = _strValue;
    StdStringToLowerSelf(strResult);
    return strResult;
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

void StdStringToUpperSelf(std::string& _strValue)
{
    std::locale loc;
    std::transform(_strValue.begin(), _strValue.end(), _strValue.begin(),
        [&loc](const unsigned char c) { return std::toupper(c, loc); });
}

bool StdStringStartsWith(std::string_view _strValue, std::string_view _strSub)
{
    return _strValue.size() >= _strSub.size()
        && _strValue.compare(0, _strSub.size(), _strSub) == 0;
}

bool StdStringEndsWith(std::string_view _strValue, std::string_view _strSub)
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

void NormalizePathSelf(std::string& path)
{
    // Normalize separator.
    StdStringReplaceSelf(path, '\\', system::PathSeparator);

    if (!path.empty())
    {
        size_t currentSlashOrZero = 0;
        while (currentSlashOrZero != std::string::npos && !path.empty())
        {
            size_t currentSegment = path[currentSlashOrZero] == system::PathSeparator ? currentSlashOrZero + 1 : currentSlashOrZero;
            bool parseNextSlash = true;

            // Strip leading '/'.
            if (currentSlashOrZero == 0 && path[currentSlashOrZero] == system::PathSeparator)
            {
                path.erase(currentSlashOrZero, 1);
                parseNextSlash = false;
            }
            // Strip "/xxx/../" parts.
            else if (path.size() > currentSegment + 1 && path[currentSegment] == '.' && path[currentSegment + 1] == '.'
                && (path.size() <= currentSegment + 2 || path[currentSegment + 2] == system::PathSeparator))
            {
                size_t previousSlash = currentSlashOrZero > 0 ? path.rfind(system::PathSeparator, currentSlashOrZero - 1) : std::string::npos;
                size_t previousSlashOrZero = previousSlash != std::string::npos ? previousSlash : 0;
                size_t previousSegment = previousSlash != std::string::npos ? previousSlash + 1 : 0;

                if (previousSlashOrZero != currentSlashOrZero)
                {
                    bool isPreviousSegmentUpperDirectory = path.size() > previousSegment + 1
                        && path[previousSegment] == '.' && path[previousSegment + 1] == '.'
                        && (path.size() <= previousSegment + 2 || path[previousSegment + 2] == system::PathSeparator);
                    
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
                && (path.size() <= currentSegment + 1 || path[currentSegment + 1] == system::PathSeparator))
            {
                path.erase(currentSegment, 2);
                parseNextSlash = false;
            }
            // Strip '//'.
            else if (path.size() > currentSegment && path[currentSegment] == system::PathSeparator)
            {
                path.erase(currentSegment, 1);
                parseNextSlash = false;
            }

            if (parseNextSlash)
            {
                currentSlashOrZero = path.find(system::PathSeparator, currentSlashOrZero + 1);
            }
        }
    }

    // Handle trailing separator.
    if (!path.empty() && path.back() == system::PathSeparator)
    {
        path.pop_back();
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
        resultPath = pathLeft + system::PathSeparator + pathRight;
        NormalizePathSelf(resultPath);
    }
}

std::string EnsureTrailingPathSeparator(const std::string& path)
{
    std::string resultPath = path;
    EnsureTrailingPathSeparatorSelf(resultPath);
    return resultPath;
}

void EnsureTrailingPathSeparatorSelf(std::string& path)
{
    if (path.empty() || path.back() != system::PathSeparator)
    {
        path.push_back(system::PathSeparator);
    }
}

std::string DirectoryPartFromPath(const std::string& pathFile)
{
    std::string resultPath = pathFile;
    DirectoryPartFromPathSelf(resultPath);
    return resultPath;
}

void DirectoryPartFromPathSelf(std::string& pathFile)
{
    size_t indexSlash = pathFile.rfind(system::PathSeparator);

    if (indexSlash != std::string::npos)
    {
        pathFile.erase(indexSlash);
    }
    else
    {
        pathFile = "";
    }
}

std::string NamePartFromPath(const std::string& pathFile)
{
    std::string resultFile = pathFile;
    NamePartFromPathSelf(resultFile);
    return resultFile;
}

void NamePartFromPathSelf(std::string& pathFile)
{
    size_t indexSlash = pathFile.rfind(system::PathSeparator);

    if (indexSlash != std::string::npos)
    {
        pathFile.erase(0, indexSlash + 1);
    }
}

bool PathStartsWith(std::string_view path, std::string_view subPath)
{
    if (!path.empty() && path.back() == system::PathSeparator)
    {
        path.remove_suffix(1);
    }

    if (!subPath.empty() && subPath.back() == system::PathSeparator)
    {
        subPath.remove_suffix(1);
    }

    if (path.empty() || subPath.empty() || path.size() < subPath.size())
        return false;

    // SubPath must represent a non-truncated part of Path, meaning it is either equal to path, or match the position of a separator.
    return (path.size() == subPath.size() || path.at(subPath.size()) == system::PathSeparator) && StdStringStartsWith(path, subPath);
}

void OpenFileExplorer(const std::string& path)
{
#if defined(GUGU_OS_WINDOWS)
    std::string normalizedPath = path;
    StdStringReplaceSelf(normalizedPath, system::PathSeparator, '\\');

    ShellExecuteA(nullptr, "open", normalizedPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);   //ShellExecuteA uses normal strings, ShellExecuteW uses wide strings (which needs a L prefix : L"...")
#endif
}

void OpenWebBrowser(const std::string& _strURL)
{
#if defined(GUGU_OS_WINDOWS)
    ShellExecuteA(nullptr, "open", _strURL.c_str(), nullptr, nullptr, SW_SHOWNORMAL);   //ShellExecuteA uses normal strings, ShellExecuteW uses wide strings (which needs a L prefix : L"...")
#endif
}

void GetFiles(const std::string& rootPath, std::vector<FileInfo>& files, bool recursive)
{
    std::string normalizedPath = NormalizePath(rootPath);
    if (normalizedPath.empty())
    {
        normalizedPath = ".";
    }

    if (recursive)
    {
        for (const auto& entry : fs::recursive_directory_iterator(fs::u8path(normalizedPath)))
        {
            if (entry.is_regular_file())
            {
                files.push_back(FileInfo::FromPath(entry.path()));
            }
        }
    }
    else
    {
        for (const auto& entry : fs::directory_iterator(fs::u8path(normalizedPath)))
        {
            if (entry.is_regular_file())
            {
                files.push_back(FileInfo::FromPath(entry.path()));
            }
        }
    }
}

void GetDirectories(const std::string& rootPath, std::vector<std::string>& directories, bool recursive)
{
    std::string normalizedPath = NormalizePath(rootPath);
    if (normalizedPath.empty())
    {
        normalizedPath = ".";
    }

    if (recursive)
    {
        for (const auto& entry : fs::recursive_directory_iterator(fs::u8path(normalizedPath)))
        {
            if (entry.is_directory())
            {
                directories.push_back(NormalizePath(entry.path().generic_u8string()));
            }
        }
    }
    else
    {
        for (const auto& entry : fs::directory_iterator(fs::u8path(normalizedPath)))
        {
            if (entry.is_directory())
            {
                directories.push_back(NormalizePath(entry.path().generic_u8string()));
            }
        }
    }
}

bool DirectoryExists(const std::string& path)
{
    return fs::is_directory(fs::u8path(path));
}

bool FileExists(const std::string& path)
{
    return fs::is_regular_file(fs::u8path(path));
}

bool EnsureDirectoryExists(const std::string& _strPath)
{
    if (_strPath.empty())
        return true;

#if defined(GUGU_OS_WINDOWS)

    std::vector<std::string> vecDirectories;
    StdStringSplit(_strPath, system::PathSeparator, vecDirectories);

    std::string strCombinedPath = "";
    for (auto strSubDirectory : vecDirectories)
    {
        strCombinedPath += strSubDirectory;

        if (!(CreateDirectoryA(strCombinedPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()))
        {
            return false;
        }

        strCombinedPath.push_back(system::PathSeparator);
    }

    return true;

#else

    return false;

#endif
}

bool RemoveFile(const std::string& path)
{
    fs::path convertedPath = fs::u8path(path);
    if (fs::is_regular_file(convertedPath))
    {
        std::error_code errorCode;
        return fs::remove(convertedPath, errorCode);
    }

    return false;
}

bool RemoveTargetDirectory(const std::string& path)
{
    fs::path convertedPath = fs::u8path(path);
    if (fs::is_directory(convertedPath))
    {
        std::error_code errorCode;
        return fs::remove(convertedPath, errorCode);
    }

    return false;
}

bool RemoveDirectoryTree(const std::string& path)
{
    fs::path convertedPath = fs::u8path(path);
    if (fs::is_directory(convertedPath))
    {
        std::error_code errorCode;
        return fs::remove_all(convertedPath, errorCode) > 0;
    }

    return false;
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
