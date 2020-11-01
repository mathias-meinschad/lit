#include <filesystem>
#include <list>
#ifndef LIT_FILESYSTEMHELPER_H
#define LIT_FILESYSTEMHELPER_H

#endif // LIT_FILESYSTEMHELPER_H

std::filesystem::path stripRoot(const std::filesystem::path& p);
void listFiles(const std::string &path, std::list<std::string> &files);
bool copyFileAndDirectories(const std::string &src, const std::string &dest);
void removeFileAndDirectories(const std::string &src);