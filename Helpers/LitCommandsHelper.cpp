#include "LitCommandsHelper.h"
#include "../Commit/Commit.h"
#include "../Lit/LitStructs.h"
#include "Constants.h"
#include "FileSystemHelper.h"
#include "ShellCommands.h"
#include <fstream>
#include <iostream>
#include <string>

bool BranchExists(const std::string &branchName)
{
	std::ifstream branchFile(REFS + branchName);
	if (branchFile.is_open()) {
		return true;
	}

	return false;
}

std::string GetRevisionNumberOfBranch(const std::string &branchName)
{
	std::ifstream branchFile(REFS + branchName);
	if (branchFile.is_open()) {
		std::string line;
		std::getline(branchFile, line);
		return line;
	}

	return {};
}

std::string getCurrentRevisionNumber()
{
	std::ifstream inFile(CURRENT_REVISION_NUMBER_PATH);
	if (inFile.is_open()) {
		std::string revisionNumber;
		getline(inFile, revisionNumber);
		inFile.close();

		return "r" + revisionNumber;
	} else {
		return {};
	}
}

void increaseRevisionNumber()
{
	std::ifstream inFile(CURRENT_REVISION_NUMBER_PATH);
	if (inFile.is_open()) {
		std::string revisionNumber;
		std::getline(inFile, revisionNumber);
		inFile.close();

		// todo: improve this
		std::ofstream out_file(CURRENT_REVISION_NUMBER_PATH);
		if (out_file.is_open()) {
			int temp = stoi(revisionNumber);
			temp++;
			auto outputString = std::to_string(temp);
			out_file.write(outputString.c_str(), size(outputString));
		}
	}
}

std::string getCurrentCommit()
{
	// todo: improve this
	std::ifstream refsHeadFile(HEAD);
	if (refsHeadFile.is_open()) {
		std::string line;
		getline(refsHeadFile, line);
		refsHeadFile.close();
		std::ifstream revisionNumberFile(LIT_PATH + line);
		if (revisionNumberFile.is_open()) {
			getline(revisionNumberFile, line);
			return line;
		}
	}

	std::cout << "error while opening head or refs file..." << std::endl;
	return {};
}

bool compareToCurrentCommit(const std::string &filePath, std::list<LitDifference> &differences,
                            std::list<std::string> &addedFiles)
{
	bool noDifference = true;
	std::string alteredFilePath = CURRENT_COMMIT + filePath.substr(2);

	std::ifstream file(alteredFilePath);
	if (!file.good()) {
		addedFiles.emplace_back(filePath.substr(2));
		noDifference = false;
	} else {
		int status;
		auto result = execDiff(alteredFilePath, filePath, status);
		if (status != 0) {
			// TODO: rename
			LitDifference litDifference(alteredFilePath, filePath, result);
			differences.emplace_back(litDifference);
			noDifference = false;
		}
	}

	return noDifference;
}

std::string getParentCommit(const std::string &revisionNumber)
{
	Commit commit(COMMITS_PATH + revisionNumber + ".commit");
	return commit.parentCommit;
}

void restoreAddedFiles(const std::list<std::string> &addedFiles, const std::string &revisionNrDirectory)
{
	for (const std::string &addedFile : addedFiles) {
		copyFileAndDirectories(revisionNrDirectory + addedFile, CURRENT_COMMIT + addedFile);
	}
}

void removeRemovedFiles(const std::list<std::string> &removedFiles)
{
	for (const std::string &removedFile : removedFiles) {
		removeFileAndDirectories(CURRENT_COMMIT + removedFile);
	}
}

void applyChanges(const std::list<LitDifference> &differences)
{
	for (const LitDifference &difference : differences) {
		int status;
		execPatch(difference, status);
	}
}

void restoreCurrentlyCommittedFiles(std::string revisionNumber)
{
	// clearing current commit folder
	system("rm -rf .lit/currentCommit/*");

	std::list<std::string> commitNumberList;
	// by design r0 is our initial commit and if we iterate through every parent commit, we have to end up here.
	while (revisionNumber != "r0") {
		commitNumberList.push_front(revisionNumber);
		revisionNumber = getParentCommit(revisionNumber);
	}

	for (const std::string &commitNumber : commitNumberList) {
		Commit commit(COMMITS_PATH + commitNumber + ".commit");
		std::string revisionNrDirectory = COMMITS_PATH + commitNumber + "/";
		restoreAddedFiles(commit.addedFiles, revisionNrDirectory);
		removeRemovedFiles(commit.removedFiles);
		applyChanges(commit.differences);
	}
}

void CopyAddedFiles(const std::string &revisionNrDirectory, const std::list<std::string> &addedFiles)
{
	for (const std::string &addedFile : addedFiles) {
		copyFileAndDirectories("./" + addedFile, revisionNrDirectory + addedFile);
	}
}

bool isRevisionNumberOk(const std::string &revisionNumber)
{
	int intOfRevisionNumber;
	try {
		intOfRevisionNumber = stoi(revisionNumber.substr(1, revisionNumber.length() - 1));
	} catch (std::exception &e) {
		return false;
	}

	std::ifstream revisionNumberFile(CURRENT_REVISION_NUMBER_PATH);
	if (revisionNumberFile.is_open()) {
		std::string line;
		std::getline(revisionNumberFile, line);
		int intOfCurrentRevisionNumber = stoi(line);
		if (intOfCurrentRevisionNumber > intOfRevisionNumber) {
			return true;
		}
	}

	return false;
}

// I know the next two functions could be merged together and be made generic, but I did not want to invest the time
// into this
std::string strFromStringList(const std::list<std::string> &list)
{
	std::string str;
	for (const auto &piece : list)
		str += piece + "\n";
	return str;
}

std::string strFromDifferenceList(const std::list<LitDifference> &list)
{
	std::string str;
	for (const auto &piece : list)
		str += piece.diffOutput + "\n";
	return str;
}

bool ObtainDifferenceToWorkspace(const std::string &revisionNumber, std::list<std::string> &addedFiles, std::list<std::string> &removedFiles,
                                 std::list<LitDifference> &differences)
{
	restoreCurrentlyCommittedFiles(revisionNumber);

	std::list<std::string> repoFiles;
	std::list<std::string> committedFiles;
	bool noDifference = true;

	listFiles("./", repoFiles);
	listFiles(".lit/currentCommit/", committedFiles);

	for (const auto &file : repoFiles) {
		// todo: make this better
		noDifference = compareToCurrentCommit(file, differences, addedFiles) && noDifference;
	}

	for (const auto &file : committedFiles) {
		if (hasFileBeenRemoved(file, removedFiles)) {
			noDifference = false;
		}
	}

	return noDifference;
}