////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/Path.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/String.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

std::string NormalizePath(std::string_view path)
{
    std::string resultPath(path);
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

std::string CombinePaths(std::string_view pathLeft, std::string_view pathRight)
{
    std::string resultPath;
    CombinePaths(pathLeft, pathRight, resultPath);
    return resultPath;
}

void CombinePaths(std::string_view pathLeft, std::string_view pathRight, std::string& resultPath)
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
        resultPath.reserve(pathLeft.size() + 1 + pathRight.size());
        resultPath = pathLeft;
        resultPath += system::PathSeparator;
        resultPath += pathRight;
        NormalizePathSelf(resultPath);
    }
}

std::string EnsureTrailingPathSeparator(std::string_view path)
{
    std::string resultPath(path);
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

std::string DirectoryPartFromPath(std::string_view path)
{
    std::string resultPath(path);
    DirectoryPartFromPathSelf(resultPath);
    return resultPath;
}

void DirectoryPartFromPathSelf(std::string& path)
{
    size_t indexSlash = path.rfind(system::PathSeparator);

    if (indexSlash != std::string::npos)
    {
        path.erase(indexSlash);
    }
    else
    {
        path = "";
    }
}

std::string NamePartFromPath(std::string_view path)
{
    std::string resultFile(path);
    NamePartFromPathSelf(resultFile);
    return resultFile;
}

void NamePartFromPathSelf(std::string& path)
{
    size_t indexSlash = path.rfind(system::PathSeparator);

    if (indexSlash != std::string::npos)
    {
        path.erase(0, indexSlash + 1);
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

}   // namespace gugu
