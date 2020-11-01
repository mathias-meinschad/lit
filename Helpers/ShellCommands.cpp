#include "ShellCommands.h"
#include "../Lit/LitStructs.h"
#include "Constants.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

std::string execCommand(const std::string &cmd, int &out_exitStatus)
{
	out_exitStatus = 0;
	auto pPipe = ::popen(cmd.c_str(), "r");
	if (pPipe == nullptr) {
		throw std::runtime_error("Cannot open pipe");
	}

	std::array<char, 256> buffer{};

	std::string result;

	while (not std::feof(pPipe)) {
		auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
		result.append(buffer.data(), bytes);
	}

	auto rc = ::pclose(pPipe);

	if (WIFEXITED(rc)) {
		out_exitStatus = WEXITSTATUS(rc);
	}

	return result;
}

std::string execDiff(const std::string &dest, const std::string &src, int &status)
{
	std::string cmd = "diff -u " + dest + " " + src;
	return execCommand(cmd, status);
}

std::string ObtainSourceFile(const std::string &difference)
{
	auto diff = difference.substr(4, difference.length() - 4);
	int n = diff.find(' ');
	return diff.substr(0, n - 1);
}


int clearFile(const std::string &patchFile)
{
    std::string fullPathFileName = PATCH_PATH + patchFile;
    if (remove(fullPathFileName.c_str()) != 0)
        return 1;

    return 0;
}

std::string execPatch(const LitDifference &difference, int &status)
{
	std::string patchFileName = PATCH_PATH + "test.patch";
	saveContentToFile(patchFileName, difference.diffOutput);
	std::stringstream cmd;
	cmd << "patch -u " << difference.srcFile << " -i " << patchFileName;
	auto result = execCommand(cmd.str(), status);
    clearFile(patchFileName);
	return result;
}

void saveContentToFile(const std::string &filename, const std::string &content)
{
	std::ofstream outfile(filename);
	if (!outfile) {
		std::cout << "Problems while trying saving patch file to .lit folder..." << std::endl;
		return;
	}
	outfile << content;
	outfile.close();
}

// todo remove this
// std::string execDiffAndPatch(const std::string &dst, const std::string &src)
//{
//	int status;
//	std::string diff = execDiff(dst, src, status);
//	std::string patchFileName = PATCH_PATH + "patch_" + dst + "_" + src;
//	saveContentToFile(patchFileName, diff);
//	if (status == 1) {
//		int statusPatch;
//		execPatch(patchFileName, statusPatch);
//		if (statusPatch == 0) {
//			clearFile(patchFileName);
//		}
//	}
//
//	return diff;
//}