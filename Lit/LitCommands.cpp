#include "LitCommands.h"
#include "../Commit/Commit.h"
#include "../Helpers/ShellCommands.h"
#include "LitStructs.h"
#include <Constants.h>
#include <FileSystemHelper.h>
#include <LitCommandsHelper.h>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

bool checkIfRepoIsInitialized()
{
	DIR *dir = opendir(".lit");
	if (dir) {
		return true;
	}

	return false;
}

void executeClear()
{
	std::cout << "Are you sure you want to remove the repository. (Y/N) " << std::endl;
	char c;
	std::cin >> c;
	if (c == 'Y' || c == 'y') {
		system("rm -rf .lit");
		std::cout << "repository has been deleted" << std::endl;
		return;
	}
}

void executeLog(int args, char **argv)
{
	std::cout << "not implemented..." << std::endl;
}

void executeMerge(int args, char **argv)
{
	std::cout << "inside merge" << std::endl;

	if (!checkIfRepoIsInitialized()) {
		std::cout << "repository has to be initialized" << std::endl;

		return;
	}
}

void executeShow(int args, char **argv)
{
	if (!checkIfRepoIsInitialized()) {
		std::cout << "repository has to be initialized" << std::endl;

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
	std::cout << "Parent: " << commit.parentCommit << std::endl;
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

void executeStatus(int args, char **argv)
{
	if (!checkIfRepoIsInitialized()) {
		std::cout << "repository has to be initialized" << std::endl;
		return;
	}

	std::list<LitDifference> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	ObtainDifferenceToWorkspace(addedFiles, removedFiles, differences);

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

void executeInit(int args, char **argv)
{
	if (checkIfRepoIsInitialized()) {
		std::cout << "repository has been initialized already" << std::endl;
		return;
	}

	std::cout << "Initializing repository..." << std::endl;

	// todo: improve this.

	if (mkdir(".lit", 0777)) {
		std::cout << "problem while trying to initialize repository" << std::endl;
	}
	if (mkdir(".lit/refs", 0777)) {
		std::cout << "problem while trying to initialize repository" << std::endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/refs/heads", 0777)) {
		std::cout << "problem while trying to initialize repository" << std::endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/commits", 0777)) {
		std::cout << "problem while trying to initialize repository" << std::endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/branches", 0777)) {
		std::cout << "problem while trying to initialize repository" << std::endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/patches", 0777)) {
		std::cout << "problem while trying to initialize repository" << std::endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/currentCommit", 0777)) {
		std::cout << "problem while trying to initialize repository" << std::endl;
		system("rm -rf .lit");
	}

	saveContentToFile(".lit/commits/revisionNumber", "1");
	saveContentToFile(".lit/HEAD", "refs/heads/master");
	saveContentToFile(".lit/refs/heads/master", "r0");
	time_t t = time(nullptr); // get time now
	tm *now = localtime(&t);
	std::list<LitDifference> emptyDifference;
	std::list<std::string> emptyList;
	Commit commit("r0", "r0", "lit init", *now, emptyDifference, emptyList, emptyList);
	commit.toFile(".lit/commits/r0");

	std::cout << "initializing repository was successful..." << std::endl;
}

void executeHelp(int args, char **argv)
{
	std::cout << "inside help" << std::endl;

	if (!checkIfRepoIsInitialized()) {
		std::cout << "repository has to be initialized" << std::endl;
		return;
	}
}

void createNewBranch(const std::string &branchName, const std::string &currentRevisionNumber)
{
	saveContentToFile(REFS + branchName, currentRevisionNumber, true);
	setHead(branchName);
}

void executeCheckout(int args, char **argv)
{
	if (!checkIfRepoIsInitialized()) {
		std::cout << "repository has to be initialized" << std::endl;
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
	if (BranchExists(branchName)) {
		revisionNumber = GetRevisionNumberOfBranch(branchName);
		setHead(branchName);
	} else {
		// if the branch does not exists I will try to check out a commit (revisionNumber)
		revisionNumber = branchName;
	}

	if (!isRevisionNumberOk(revisionNumber)) {
		std::cout << "commit or branch cannot be found..." << std::endl;
		return;
	}

	restoreCurrentlyCommittedFiles(revisionNumber);
	CopyCommitToWorkingArea();
	std::cout << "successfully checked out branch: " << branchName << std::endl;
}

void executeCommit(int args, char **argv)
{
	if (!checkIfRepoIsInitialized()) {
		std::cout << "repository has to be initialized" << std::endl;
		return;
	}

	if (args < 3) {
		// todo: display usage
		std::cout << "Wrong usage of commit..." << std::endl;
		return;
	}

	std::string revisionNumber = getCurrentRevisionNumber();
	if (revisionNumber.empty()) {
		std::cout << "repo has not been initialized properly\n  please reinitialize the repo with 'lit clear' and 'lit "
		             "init'..."
		          << std::endl;
		return;
	}

	std::list<LitDifference> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	bool noDifference = ObtainDifferenceToWorkspace(addedFiles, removedFiles, differences);

	if (noDifference) {
		std::cout << "nothing to commit..." << std::endl;
		return;
	}

	// todo improve this
	auto revisionNrDirectory = COMMITS_PATH + revisionNumber;
	if (mkdir(revisionNrDirectory.c_str(), 0777)) {
		std::cout << "Problem while adding a directory. Please check the access rights." << std::endl;
		return;
	}
	CopyAddedFiles(revisionNrDirectory + "/", addedFiles);

	time_t t = time(nullptr); // get time now
	tm *now = localtime(&t);
	std::string commitMsg = std::string(argv[2]);
	Commit commit(revisionNumber, currentCommit, commitMsg, *now, differences, addedFiles, removedFiles);
	setRefs(revisionNumber);

	commit.toFile(COMMITS_PATH + revisionNumber);
	increaseRevisionNumber();

	char buffer[40];
	std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", now);
	std::string dateTimeString(buffer);
	std::cout << "Commit: " << revisionNumber << "\n"
	          << "Date: " << dateTimeString << std::endl;
}