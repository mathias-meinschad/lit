#include "LitStructs.h"
#include <list>
#include <string>

#ifndef LIT_LITCOMMANDSHELPER_H
#define LIT_LITCOMMANDSHELPER_H

#endif // LIT_LITCOMMANDSHELPER_H

std::string getCurrentRevisionNumber();

void increaseRevisionNumber();

std::string getCurrentCommit();

bool compareToCurrentCommit(const std::string &filePath, std::list<Differences> &differences,
                            std::list<std::string> &addedFiles);

std::string getParentCommit(const std::string &revisionNumber);

std::string getParentCommit(const std::string &revisionNumber);

void restoreAddedFiles(const std::list<std::string> &addedFiles, const std::string &revisionNrDirectory);

void removeRemovedFiles(const std::list<std::string> &removedFiles);

void applyChanges(const std::list<Differences> &differences);

void restoreCurrentlyCommittedFiles(std::string revisionNumber);

void copyAddedFiles(const std::string &revisionNrDirectory, const std::list<std::string> &addedFiles);

bool isRevisionNumberOk(const std::string &revisionNumber);

bool branchExists(const std::string &branchName);

std::string getRevisionNumberOfBranch(const std::string &branchName);

std::string strFromStringList(const std::list<std::string> &list);

std::string strFromDifferenceList(const std::list<Differences> &list);

bool obtainDifferenceToWorkspace(const std::string &revisionNumber, std::list<std::string> &addedFiles,
                                 std::list<std::string> &removedFiles, std::list<Differences> &differences);

void mergeWithoutConflicts(const std::list<std::string> &removedFiles);

void mergeWithConflicts(std::list<std::string> &removedFiles, std::list<Differences> &differences,
                        const std::string &currentCommit, const std::string &mergeCommit);