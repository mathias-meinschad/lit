#include "../Lit/LitStructs.h"
#include <list>
#include <string>

#ifndef LIT_LITCOMMANDSHELPER_H
#define LIT_LITCOMMANDSHELPER_H

#endif // LIT_LITCOMMANDSHELPER_H

std::string getCurrentRevisionNumber();

void increaseRevisionNumber();

std::string getCurrentCommit();

bool compareToCurrentCommit(const std::string &filePath, std::list<LitDifference> &differences,
                            std::list<std::string> &addedFiles);

std::string getParentCommit(const std::string &revisionNumber);

std::string getParentCommit(const std::string &revisionNumber);

void restoreAddedFiles(const std::list<std::string> &addedFiles, const std::string &revisionNrDirectory);

void removeRemovedFiles(const std::list<std::string> &removedFiles);

void applyChanges(const std::list<LitDifference> &differences);

void restoreCurrentlyCommittedFiles(std::string revisionNumber);

void CopyAddedFiles(const std::string &revisionNrDirectory, const std::list<std::string> &addedFiles);

bool isRevisionNumberOk(const std::string &revisionNumber);

bool BranchExists(const std::string &branchName);

std::string GetRevisionNumberOfBranch(const std::string &branchName);

std::string strFromStringList(const std::list<std::string> &list);

std::string strFromDifferenceList(const std::list<LitDifference> &list);

bool ObtainDifferenceToWorkspace(std::list<std::string> &addedFiles, std::list<std::string> &removedFiles,
                                 std::list<LitDifference> &differences);