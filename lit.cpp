#include "Lit/LitCommands.h"
#include <cstring>
#include <iostream>

int main(int args, char **argv)
{
	if (args < 2) {
        std::cout << "wrong usage of lit..." << std::endl;
        std::cout << "please use 'lit help' for more information about usage of lit..." << std::endl;
		return 0;
	}

	if (strcmp(argv[1], "help") == 0) {
		litHelp(args, argv);
	} else if (strcmp(argv[1], "clear") == 0) {
		litClear();
	} else if (strcmp(argv[1], "init") == 0) {
		litInit();
	} else if (strcmp(argv[1], "status") == 0) {
		litStatus(args, argv);
	} else if (strcmp(argv[1], "commit") == 0) {
		litCommit(args, argv);
	} else if (strcmp(argv[1], "show") == 0) {
		litShow(args, argv);
	} else if (strcmp(argv[1], "checkout") == 0) {
		litCheckout(args, argv);
	} else if (strcmp(argv[1], "merge") == 0) {
		litMerge(args, argv);
	} else if (strcmp(argv[1], "log") == 0) {
		litLog(args, argv);
	} else {
		std::cout << "'" << argv[1] << "'" << " is not a lit command. Use 'lit help' for usage information..." << std::endl;
	}

	return 0;
}