#include "LitCommands.h"
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <spawn.h>
#include <sys/stat.h>
#include <wait.h>
#include <zconf.h>
using namespace std;

#define MAX_ARGS 1024

void parsecmd(char **pString, char *pString1[1024]);
bool checkIfRepoIsInitialized()
{
	DIR *dir = opendir(".lit");
	if (dir) {
		return true;
	}

	return false;
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
	cout << "inside status" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}
}

void executeInit(int args, char **argv)
{
	if (checkIfRepoIsInitialized()) {
		cout << "repository has been initialized already" << endl;

		return;
	}

	cout << "Initializing repository..." << endl;

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
	if (mkdir(".lit/objects", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}
	if (mkdir(".lit/branches", 0777)) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}

	std::ofstream outfile(".lit/HEAD");
	if (!outfile) {
		cout << "problem while trying to initialize repository" << endl;
		system("rm -rf .lit");
	}

	outfile << "ref: refs/heads/master" << std::endl;
	outfile.close();
}

void executeHelp(int args, char **argv)
{
	cout << "inside help" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}
}

int runcmd(char *cmd)
{
	char *argv[MAX_ARGS];
	pid_t child_pid;
	int child_status;

	parsecmd(cmd, argv);
	child_pid = fork();
	if (child_pid == 0) {
		/* This is done by the child process. */

		execv(argv[0], argv);

		/* If execv returns, it must have failed. */

		printf("Unknown command\n");
		exit(0);
	} else {
		/* This is run by the parent.  Wait for the child
		   to terminate. */

		pid_t tpid;
		do {
			tpid = wait(&child_status);
		} while (tpid != child_pid);

		return child_status;
	}
}

void parsecmd(char *cmd, char *argv[1024]) {
    
}

void executeCommit(int args, char **argv)
{
	cout << "inside commit" << endl;

	if (!checkIfRepoIsInitialized()) {
		cout << "repository has to be initialized" << endl;

		return;
	}

	char *cmd = "diff -u foo.txt bar.txt >> patch";
	int status = runcmd(cmd);
}
