#ifndef LIT_COMMIT_H
#define LIT_COMMIT_H

#include <string>
class Commit {
  public:
	std::string revisionNumber;
	std::string Tree;
	std::string parentCommit;
	std::string commitMsg;
    tm date;
};

#endif // LIT_COMMIT_H
