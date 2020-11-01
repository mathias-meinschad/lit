#include <filesystem>
#include <list>
#ifndef LIT_FILESYSTEMHELPER_H
#define LIT_FILESYSTEMHELPER_H

#endif // LIT_FILESYSTEMHELPER_H

bool hasFileBeenRemoved(const std::string &file, std::list<std::string> &removedFiles);
void listFiles(const std::string &path, std::list<std::string> &files);
bool copyFileAndDirectories(const std::string &src, const std::string &dest);
void removeFileAndDirectories(const std::string &src);
void setRefs(const std::string &revisionNumber);
void setHead(const std::string &branch);
void CopyCommitToWorkingArea();
void saveContentToFile(const std::string &filename, const std::string &content, bool createMissingDirectories = false);