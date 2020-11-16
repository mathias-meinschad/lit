#include "LitCommands.h"
#include "../Helpers/ShellCommands.h"
#include "LitStructs.h"
#include <Constants.h>
#include <FileSystemHelper.h>
#include <LitCommandsHelper.h>
#include <cstring>
#include <ctime>
#include <iostream>

void commit(const std::string &commitMsg, const std::string &parentCommit2);

void litClear()
{
	if (!repoCheck()) {
        std::cout << "repository has to be initialized..." << std::endl;
		return;
	}

	std::cout << "Are you sure you want to remove the repository. (Y/N) " << std::endl;
	char c;
	std::cin >> c;
	if (c == 'Y' || c == 'y') {
		system("rm -rf .lit");
		std::cout << "repository has been deleted" << std::endl;
		return;
	}
}

void litLog(int args, char **argv)
{
	std::cout << "log is not implemented..." << std::endl;
}

void litMerge(int args, char **argv)
{
    if (!repoCheck()) {
        std::cout << "repository has to be initialized..." << std::endl;
        return;
    }

	if (args < 3) {
        std::cout << "wrong usage of commit, use help to obtain more information about usage of lit merge..." << std::endl;
		return;
	}

	auto currentCommitNr = getCurrentCommit();
	std::string mergeCommitNr = argv[2];

	if (branchExists(mergeCommitNr)) {
		mergeCommitNr = getRevisionNumberOfBranch(mergeCommitNr);
	}

	if (!isRevisionNumberOk(mergeCommitNr)) {
		std::cout << "cannot find commit: " << mergeCommitNr << std::endl;
	}

	std::list<Differences> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	bool noDifference = obtainDifferenceToWorkspace(currentCommitNr, addedFiles, removedFiles, differences);
	if (!noDifference) {
		std::cout << "cannot merge when there are uncommitted changes..." << std::endl;
		return;
	}
	differences = {};
	obtainDifferenceToWorkspace(mergeCommitNr, addedFiles, removedFiles, differences);
	if (differences.empty()) {
		mergeWithoutConflicts(removedFiles);
		commit("Merge " + mergeCommitNr + " into " + currentCommitNr, mergeCommitNr);
        std::cout << "merge was successful..." << std::endl;
	} else {
		mergeWithConflicts(removedFiles, differences, currentCommitNr, mergeCommitNr);
		std::cout << "merge has conflicts; please resolve them and commit then..." << std::endl;
	}
}

void litShow(int args, char **argv)
{
    if (!repoCheck()) {
        std::cout << "repository has to be initialized..." << std::endl;
        return;
    }

	std::string commitNumber;
	if (args > 2) {
		commitNumber = argv[2];
	} else {
		commitNumber = getCurrentCommit();
	}

	Commit commit(COMMITS_PATH + commitNumber + ".commit");
	std::cout << "Commit: " << commit.revisionNumber << std::endl;
	std::cout << "Parent: " << commit.parentCommit << (!commit.parentCommit2.empty() ? ", " + commit.parentCommit2 : "")
	          << std::endl;
	char buffer[40];
	std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &commit.date);
	std::string dateTimeString(buffer);
	std::cout << "Date: " << dateTimeString << std::endl << std::endl;
	std::cout << commit.commitMsg << std::endl << std::endl;
	std::cout << "Added files:" << std::endl;
	std::cout << strFromStringList(commit.addedFiles) << std::endl;
	std::cout << "Removed files:" << std::endl;
	std::cout << strFromStringList(commit.removedFiles) << std::endl;
	std::cout << "Differences:" << std::endl;
	std::cout << strFromDifferenceList(commit.differences);
}

void litStatus(int args, char **argv)
{
    if (!repoCheck()) {
        std::cout << "repository has to be initialized..." << std::endl;
        return;
    }

	std::string currentCommit = getCurrentCommit();
	std::list<Differences> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	obtainDifferenceToWorkspace(currentCommit, addedFiles, removedFiles, differences);

	for (const auto &addedFile : addedFiles) {
		std::cout << "A\t" << addedFile << std::endl;
	}

	for (const auto &removedFile : removedFiles) {
		std::cout << "D\t" << removedFile << std::endl;
	}

	for (const auto &difference : differences) {
		std::cout << "M\t" << difference.destFile << std::endl;
	}
}

void litInit()
{
	if (repoCheck()) {
		std::cout << " repository has already been initialized; use 'lit clear' if you want to remove it..." << std::endl;
		return;
	}

	std::cout << "Initializing repository..." << std::endl;

	int successful;
	successful = std::filesystem::create_directories(LIT_PATH);
	if (!successful) {
		std::cout << "problem while trying to initialize repository" << std::endl;
	}

	successful = std::filesystem::create_directories(REFS) && successful;
	successful = std::filesystem::create_directories(COMMITS_PATH) && successful;
	successful = std::filesystem::create_directories(PATCH_PATH) && successful;
	successful = std::filesystem::create_directories(CURRENT_COMMIT) && successful;
	if (!successful) {
		std::cout << "problem while trying to initialize repository" << std::endl;
		system("rm -rf .lit");
		return;
	}

	saveContentToFile(COMMITS_PATH + "revisionNumber", "0");
	saveContentToFile(LIT_PATH + "HEAD", "refs/heads/master");
	saveContentToFile(REFS + "master", "");

	std::cout << "initializing repository was successful..." << std::endl;
}

void litHelp(int args, char **argv)
{
    std::cout << "'lit init': is used to initialize the repository" << std::endl;
    std::cout << "'lit clear': is used to remove the .lit folder" << std::endl;
    std::cout << "'lit status': lists changes in the working area" << std::endl;
    std::cout << "'lit commit': is used to commit the current changes" << std::endl;
    std::cout << "'lit show [commitNr]': is used to inspect a given commit" << std::endl;
    std::cout << "'lit checkout [-b] [branch|commit]': is used to create a branch (-b flag) or to checkout another commit or branch" << std::endl;
    std::cout << "'lit merge [branch|commitNr]': merges a commit or a branch into the currently checked out commit" << std::endl;

}

void createNewBranch(const std::string &branchName, const std::string &currentRevisionNumber)
{
	saveContentToFile(REFS + branchName, currentRevisionNumber, true);
	setHead(branchName);
}

void litCheckout(int args, char **argv)
{
    if (!repoCheck()) {
        std::cout << "repository has to be initialized..." << std::endl;
        return;
    }

	std::string currentCommit = getCurrentCommit();
	std::string branchName;
	if (args < 3) {
		branchName = getCurrentCommit();
	} else if (std::strcmp(argv[2], "-b") == 0) {
		if (args < 4) {
			std::cout << "please specify a branch name..." << std::endl;
			return;
		}
		createNewBranch(argv[3], currentCommit);
		std::cout << "branch " << argv[3] << " has been created successfully..." << std::endl;
		return;
	} else {
		branchName = argv[2];
	}

	std::string revisionNumber;
	// First I check if the branch exists
	if (branchExists(branchName)) {
		revisionNumber = getRevisionNumberOfBranch(branchName);
		setHead(branchName);
	} else {
		// if the branch does not exists I will try to check out a commit (revisionNumber)
		revisionNumber = branchName;
	}

	if (!isRevisionNumberOk(revisionNumber)) {
		std::cout << "cannot find commit/branch: " << revisionNumber << std::endl;
		return;
	}

	restoreCurrentlyCommittedFiles(revisionNumber);
	copyCommitToWorkingArea();
	std::cout << "successfully checked out branch: " << branchName << std::endl;
}

// pulled this function out of litCommit so i can reuse this in the litMerge function
void commit(const std::string &commitMsg, const std::string &parentCommit2)
{
	std::string revisionNumber = getCurrentRevisionNumber();
	if (revisionNumber.empty()) {
		std::cout << "problem occurred: please reinitialize the repo with 'lit clear' and 'lit init'..." << std::endl;
		return;
	}

	std::string currentCommit = getCurrentCommit();
	std::list<Differences> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	bool noDifference = obtainDifferenceToWorkspace(currentCommit, addedFiles, removedFiles, differences);

	if (noDifference) {
		std::cout << "nothing to commit..." << std::endl;
		return;
	}

	auto revisionNrDirectory = COMMITS_PATH + revisionNumber;
	if (!std::filesystem::create_directory(revisionNrDirectory)) {
		std::cout << "Problem while adding a directory. Please check the access rights." << std::endl;
		return;
	}
	copyAddedFiles(revisionNrDirectory + "/", addedFiles);

	time_t t = time(nullptr); // get time now
	tm *now = localtime(&t);
	Commit commit(revisionNumber, getCurrentCommit(), parentCommit2, commitMsg, *now, differences, addedFiles,
	              removedFiles);
	setRefs(revisionNumber);

	commit.toFile(COMMITS_PATH + revisionNumber);
	increaseRevisionNumber();

	char buffer[40];
	std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", now);
	std::string dateTimeString(buffer);
	std::cout << "Commit: " << revisionNumber << "\n"
	          << "Date: " << dateTimeString << std::endl;
}

void litCommit(int args, char **argv)
{
    if (!repoCheck()) {
        std::cout << "repository has to be initialized..." << std::endl;
        return;
    }

	if (args < 3) {
		std::cout << "wrong usage of commit, use help to obtain more information about usage of lit commit..." << std::endl;
		return;
	}

	commit(argv[2], "");
}