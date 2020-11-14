#include <list>
#include <string>
#ifndef LIT_LITSTRUCTS_H

struct Differences {
	Differences(std::string basicString, std::string basicString1, std::string basicString2);
	std::string srcFile;
	std::string destFile;
	std::string diffOutput;
};

struct Commit {
    explicit Commit(const std::string &fileName);
    Commit(std::string revisionNumber, std::string parentCommit, std::string parentCommit2, std::string commitMsg, tm date,
           std::list<Differences> differences, std::list<std::string> addedFiles,
           std::list<std::string> removedFiles);
    void toFile(const std::string &fileName);

    std::string revisionNumber;
    std::string parentCommit;
    std::string parentCommit2;
    std::string commitMsg;
    tm date{};
    std::list<Differences> differences;
    std::list<std::string> addedFiles;
    std::list<std::string> removedFiles;
};

#define LIT_LITSTRUCTS_H

#endif // LIT_LITSTRUCTS_H
