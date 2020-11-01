#include "LitCommands.h"
#include "../Commit/Commit.h"
#include "../Helpers/ShellCommands.h"
#include "LitStructs.h"
#include <Constants.h>
#include <FileSystemHelper.h>
#include <ctime>
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
using namespace std;

#define MAX_ARGS 1024

bool checkIfRepoIsInitialized()
{
	DIR *dir = opendir(".lit");
	if (dir) {
		return true;
	}

	return false;
}

void executeClear() {
    cout << "Are you sure you want to remove the repository. (Y/N) " << endl;
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y') {
        system("rm -rf .lit");
        cout << "repository has been deleted" << endl;
		return;
	}
}

void executeLog(int args, char **argv)
{
	cout << "inside log" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}
}

void executeMerge(int args, char **argv)
{
	cout << "inside merge" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}
}

void executeCheckout(int args, char **argv)
{
	cout << "inside checkout" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}
}

void executeShow(int args, char **argv)
{
	cout << "inside show" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}
}

void executeStatus(int args, char **argv)
{
//	cout << "inside status" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}

//	listSubdirectories();
}

void executeInit(int args, char **argv)
{
	if (checkIfRepoIsInitialized()) {
		cout << "repository has been initialized already" << endl;

		return;
	}

	cout << "Initializing repository..." << endl;

	// todo: improve this.

	if (mkdir(".lit", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
	}
	if (mkdir(".lit/refs", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/refs/heads", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/commits", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/branches", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/patches", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/currentCommit", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}

	saveContentToFile(".lit/commits/revisionNumber", "1");
	saveContentToFile(".lit/HEAD", "refs/heads/master");
	;
	saveContentToFile(".lit/refs/heads/master", "r0");
    time_t t = time(nullptr); // get time now
    tm *now = localtime(&t);
    list<LitDifference> emptyDifference;
	list<string> emptyList;
	Commit commit("r0", "r0", "lit init", *now, emptyDifference, emptyList, emptyList);
	commit.toFile(".lit/commits/r0");

	cout << "initializing repository was successful..." << endl;
}

void executeHelp(int args, char **argv)
{
	cout << "inside help" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}
}

string getCurrentRevisionNumber()
{
	ifstream inFile(CURRENT_REVISION_NUMBER_PATH);
	if (inFile.is_open()) {
		string revisionNumber;
		getline(inFile, revisionNumber);
		inFile.close();

		return "r" + revisionNumber;
	} else {
		return {};
	}
}

void increaseRevisionNumber()
{
    ifstream inFile(CURRENT_REVISION_NUMBER_PATH);
    if (inFile.is_open()) {
        string revisionNumber;
        getline(inFile, revisionNumber);
        inFile.close();

        // todo: improve this
        ofstream out_file(CURRENT_REVISION_NUMBER_PATH);
        if (out_file.is_open()) {
            int temp = stoi(revisionNumber);
            temp++;
            auto outputString = to_string(temp);
            out_file.write(outputString.c_str(), size(outputString));
        }
    }
}

string getCurrentCommit()
{
	// todo: improve this
	ifstream in_file(REFS_HEAD);
	if (in_file.is_open()) {
		string line;
		getline(in_file, line);
		in_file.close();
		ifstream ref_file(LIT_PATH + line);
		if (ref_file.is_open()) {
			getline(ref_file, line);
			return line;
		}
	}

	cout << "error while opening head or refs file..." << endl;
	return {};
}

void setRefs(string revisionNumber)
{
    // todo: improve this
    ifstream in_file(REFS_HEAD);
    if (in_file.is_open()) {
        string line;
        getline(in_file, line);
        in_file.close();
        ofstream ref_file(LIT_PATH + line);
        if (ref_file.is_open()) {
            ref_file << revisionNumber;
			return;
        }
    }

    cout << "error while opening head or refs file; cannot change ref..." << endl;
}

// TODO: remove unneeded argument
bool compareToCurrentCommit(const string &filePath, list<LitDifference> &differences, list<string> &addedFiles)
{
	bool noDifference = true;
    string alteredFilePath = CURRENT_COMMIT + filePath.substr(2);

    ifstream file(alteredFilePath);
    if(!file.good()) {
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

string getParentCommit(const string& revisionNumber)
{
	Commit commit(COMMITS_PATH + revisionNumber + ".commit");
	return commit.parentCommit;
}

void restoreAddedFiles(const list<string> &addedFiles, const string &revisionNrDirectory)
{
    for (const string &addedFile : addedFiles) {
		copyFileAndDirectories(revisionNrDirectory + addedFile, CURRENT_COMMIT + addedFile);
	}
}

void removeRemovedFiles(const list<string> &removedFiles)
{
    for (const string &removedFile : removedFiles) {
        removeFileAndDirectories(CURRENT_COMMIT + removedFile);
    }
}

void applyChanges(const list<LitDifference> &differences) {
    for(const LitDifference &difference : differences) {
		int status;
		execPatch(difference, status);
	}
}

void restoreCurrentlyCommittedFiles(string revisionNumber)
{
	// clearing current commit folder
	system("rm -rf .lit/currentCommit/*");

    list<string> commitNumberList;
	// by design r0 is our initial commit and if we iterate through every parent commit, we have to end up here.
    while (revisionNumber != "r0"){
        commitNumberList.push_front(revisionNumber);
        revisionNumber = getParentCommit(revisionNumber);
	}

	for(const string& commitNumber : commitNumberList) {
		Commit commit(COMMITS_PATH + commitNumber + ".commit");
		string revisionNrDirectory = COMMITS_PATH + commitNumber + "/";
		restoreAddedFiles(commit.addedFiles, revisionNrDirectory);
		removeRemovedFiles(commit.removedFiles);
		applyChanges(commit.differences);
	}
}

void CopyAddedFiles(const string &revisionNrDirectory, const list<string>& addedFiles)
{
    for (const string &addedFile : addedFiles) {
		copyFileAndDirectories("./" + addedFile, revisionNrDirectory + addedFile);
    }
}

bool hasFileBeenRemoved(const string &file, list<string>& removedFiles)
{
	std::filesystem::path pathToFile(file);
	// strip root twice to get ride of .lit/currentCommit
	pathToFile = stripRoot(pathToFile);
	pathToFile = stripRoot(pathToFile);
	stringstream filePathToWorkingDirectory;
	filePathToWorkingDirectory << "./" << pathToFile.string();

    ifstream in_file(filePathToWorkingDirectory.str());
    if(!in_file.good()) {
        removedFiles.emplace_back(filePathToWorkingDirectory.str());
        return true;
    }

    return false;
}
void executeCommit(int args, char **argv)
{
	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;
		return;
	}

	if(args < 3) {
		// todo: display usage
		cout << "Wrong usage of commit..." << endl;
		return;
	}

	string revisionNumber = getCurrentRevisionNumber();
	if (revisionNumber.empty()) {
		cout << "Repo has not been initialized properly please reinitialize the repo..." << endl;
		return;
	}

    string parentCommit;
    parentCommit = getCurrentCommit();
	restoreCurrentlyCommittedFiles(parentCommit);

    list<string> repoFiles;
	list<string> committedFiles;
    list<LitDifference> differences;
	list<string> addedFiles;
    list<string> removedFiles;
    bool noDifference = true;

    listFiles("./", repoFiles);
    listFiles(".lit/currentCommit/", committedFiles);

    for(const auto& file : repoFiles) {
		// todo: make this better
		noDifference = compareToCurrentCommit(file, differences, addedFiles) && noDifference;
    }

    for(const auto& file : committedFiles) {
		if (hasFileBeenRemoved(file, removedFiles)) {
			noDifference = false;
		}
    }

	if (noDifference) {
		cout << "nothing to commit..." << endl;
		return;
	}

    // todo improve this
    auto revisionNrDirectory = COMMITS_PATH + revisionNumber;
    if (mkdir(revisionNrDirectory.c_str(), 0777)) {
        cout << "Problem while adding a directory. Please check the access rights." << endl;
        return;
    }
    CopyAddedFiles(revisionNrDirectory + "/", addedFiles);

	time_t t = time(nullptr); // get time now
	tm *now = localtime(&t);
	string commitMsg = string(argv[2]);
	Commit commit(revisionNumber, parentCommit, commitMsg, *now, differences, addedFiles, removedFiles);
	setRefs(revisionNumber);

	commit.toFile(COMMITS_PATH + revisionNumber);
	increaseRevisionNumber();

	// TODO: maybe a better output here?
	cout << "commit created successfully..." << endl;
}