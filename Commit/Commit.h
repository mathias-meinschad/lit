#ifndef LIT_COMMIT_H
#define LIT_COMMIT_H

#include "../Lit/LitStructs.h"
#include <list>
#include <string>

class Commit {
  public:
	Commit(const std::string &fileName);
	Commit(std::string revisionNumber, std::string parentCommit, std::string commitMsg, tm date,
	       std::list<LitDifference> differences, std::list<std::string> addedFiles, std::list<std::string> removedFiles);
	void toFile(std::string fileName);

	std::string revisionNumber;
	std::string parentCommit;
	std::string commitMsg;
	tm date{};
	std::list<LitDifference> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	void ParseStringToDifference(std::string basicString);
};

#endif // LIT_COMMIT_H
