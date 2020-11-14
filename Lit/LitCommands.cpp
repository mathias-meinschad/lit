#include "LitCommands.h"
#include "../Helpers/ShellCommands.h"
#include "LitStructs.h"
#include <Constants.h>
#include <FileSystemHelper.h>
#include <LitCommandsHelper.h>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>

void commit(const std::string &commitMsg, const std::string &parentCommit2);

void RepoCheck()
{
	DIR *dir = opendir(".lit");
	if (!dir) {
		std::cout << "repository has to be initialized" << std::endl;
	}
}

void executeClear()
{
	RepoCheck();

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

void MergeWithoutConflicts(const std::list<std::string> &removedFiles)
{
	for (const std::string &removedFile : removedFiles) {
		copyFileAndDirectories(CURRENT_COMMIT + removedFile.substr(2, removedFile.length() - 2), "./" + removedFile);
	}
}

// I know this method is not performant at all because it is O(n^2)
// If I have more time I would use a tree structure and search with this structure for the common base
std::string CommonBaseOfBranches(std::string currentCommitNr, std::string mergeCommitNr)
{
	auto tmp = mergeCommitNr;
	while (!currentCommitNr.empty()) {
		mergeCommitNr = tmp;
		while (!mergeCommitNr.empty()) {
			if (currentCommitNr == mergeCommitNr) {
				return currentCommitNr;
			}
			mergeCommitNr = getParentCommit(mergeCommitNr);
		}
		currentCommitNr = getParentCommit(currentCommitNr);
	}
	return {};
}

void MergeWithConflicts(std::list<std::string> &removedFiles, std::list<Differences> &differences,
                        const std::string &currentCommit, const std::string &mergeCommit)
{
	std::string commonBaseBranch = CommonBaseOfBranches(currentCommit, mergeCommit);
	for (const std::string &removedFile : removedFiles) {
        copyFileAndDirectories(CURRENT_COMMIT + removedFile.substr(2, removedFile.length() - 2), "./" + removedFile);
	}
	for (const Differences &difference : differences) {
		copyFileAndDirectories(difference.srcFile, "./" + difference.destFile + "." + mergeCommit);
	}
	if (!commonBaseBranch.empty()) {
		restoreCurrentlyCommittedFiles(commonBaseBranch);

		for (const Differences &difference : differences) {
			copyFileAndDirectories(difference.srcFile,
			                       "./" + difference.destFile + "." + commonBaseBranch);
		}
	}
}

void executeMerge(int args, char **argv)
{
	RepoCheck();

	if (args < 3) {
		// todo: display usage
		std::cout << "wrong usage of merge..." << std::endl;
		return;
	}

	auto currentCommitNr = getCurrentCommit();
	std::string mergeCommitNr = argv[2];

	if (BranchExists(mergeCommitNr)) {
		mergeCommitNr = GetRevisionNumberOfBranch(mergeCommitNr);
	}

	if (!isRevisionNumberOk(mergeCommitNr)) {
		std::cout << "cannot find commit: " << mergeCommitNr << std::endl;
	}

	std::list<Differences> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	bool noDifference = ObtainDifferenceToWorkspace(currentCommitNr, addedFiles, removedFiles, differences);
	if (!noDifference) {
		std::cout << "cannot merge when there are uncommitted changes..." << std::endl;
		return;
	}
	differences = {};
	ObtainDifferenceToWorkspace(mergeCommitNr, addedFiles, removedFiles, differences);
	if (differences.empty()) {
		MergeWithoutConflicts(removedFiles);
		commit("Merge " + mergeCommitNr + " into " + currentCommitNr, mergeCommitNr);
        std::cout << "merge was successful..." << std::endl;
	} else {
		MergeWithConflicts(removedFiles, differences, currentCommitNr, mergeCommitNr);
		std::cout << "merge has conflicts; please resolve them and commit then..." << std::endl;
	}
}

void executeShow(int args, char **argv)
{
	RepoCheck();

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

void executeStatus(int args, char **argv)
{
	RepoCheck();

	std::string currentCommit = getCurrentCommit();
	std::list<Differences> differences;
	std::list<std::string> addedFiles;
	std::list<std::string> removedFiles;
	ObtainDifferenceToWorkspace(currentCommit, addedFiles, removedFiles, differences);

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

void executeInit()
{
	if (std::filesystem::exists(".lit")) {
		std::cout << " repository has already been initialized; call clear if you want to remove it..." << std::endl;
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

void executeHelp(int args, char **argv)
{
	RepoCheck();
}

void createNewBranch(const std::string &branchName, const std::string &currentRevisionNumber)
{
	saveContentToFile(REFS + branchName, currentRevisionNumber, true);
	setHead(branchName);
}

void executeCheckout(int args, char **argv)
{
	RepoCheck();

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
		std::cout << "cannot find commit/branch: " << revisionNumber << std::endl;
		return;
	}

	restoreCurrentlyCommittedFiles(revisionNumber);
	CopyCommitToWorkingArea();
	std::cout << "successfully checked out branch: " << branchName << std::endl;
}

// pulled this function out of executeCommit so i can reuse this in the executeMerge function
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
	bool noDifference = ObtainDifferenceToWorkspace(currentCommit, addedFiles, removedFiles, differences);

	if (noDifference) {
		std::cout << "nothing to commit..." << std::endl;
		return;
	}

	auto revisionNrDirectory = COMMITS_PATH + revisionNumber;
	if (!std::filesystem::create_directory(revisionNrDirectory)) {
		std::cout << "Problem while adding a directory. Please check the access rights." << std::endl;
		return;
	}
	CopyAddedFiles(revisionNrDirectory + "/", addedFiles);

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

void executeCommit(int args, char **argv)
{
	RepoCheck();

	if (args < 3) {
		// todo: display usage
		std::cout << "wrong usage of commit..." << std::endl;
		return;
	}

	commit(argv[2], "");
}