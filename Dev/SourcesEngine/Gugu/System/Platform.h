#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/FileInfo.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

void WriteInConsole(const std::string& value);
void WriteInConsoleEndline(const std::string& value);
void WriteInIDEConsole(const std::string& value);
void WriteInIDEConsoleEndline(const std::string& value);
void WriteInFileEndline(const std::string& fileName, const std::string& value);

bool ExecuteCommand(std::string_view executablePath, std::string_view arguments);
void OpenFileExplorer(std::string_view path);
void OpenWebBrowser(std::string_view url);

void GetFiles(std::string_view rootPath_utf8, std::vector<FileInfo>& files, bool recursive);
void GetDirectories(std::string_view rootPath_utf8, std::vector<std::string>& directories, bool recursive);

bool DirectoryExists(std::string_view path_utf8);
bool FileExists(std::string_view path_utf8);

bool EnsureDirectoryExists(std::string_view path_utf8);

bool RemoveFile(std::string_view path_utf8);
bool RemoveTargetDirectory(std::string_view path_utf8);
bool RemoveDirectoryTree(std::string_view path_utf8);

}   // namespace gugu
