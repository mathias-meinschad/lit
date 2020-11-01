#include "Lit/LitCommands.h"
#include <cstring>
#include <iostream>

using namespace std;

void showUsageHelp();

bool checkIfRepoIsInitialized();

int main(int args, char **argv)
{
	if (args < 2) {
		showUsageHelp();

		return 0;
	}

	if (strcmp(argv[1], "help") == 0) {
		executeHelp(args, argv);
	} else if (strcmp(argv[1], "clear") == 0) {
		executeClear();
	} else if (strcmp(argv[1], "init") == 0) {
		executeInit(args, argv);
	} else if (strcmp(argv[1], "status") == 0) {
		executeStatus(args, argv);
	} else if (strcmp(argv[1], "commit") == 0) {
		executeCommit(args, argv);
	} else if (strcmp(argv[1], "show") == 0) {
		executeShow(args, argv);
	} else if (strcmp(argv[1], "checkout") == 0) {
		executeCheckout(args, argv);
	} else if (strcmp(argv[1], "merge") == 0) {
		executeMerge(args, argv);
	} else if (strcmp(argv[1], "log") == 0) {
		executeLog(args, argv);
	} else {
		cout << "'" << argv[1] << "'"
		     << " is not a lit command. See 'lit help'" << endl;
	}

	return 0;
}

void showUsageHelp()
{
	// TODO: add usage help here:
	cout << "later you can see some usage help here" << endl;
}
