#include "ShellCommands.h"
#include "../Lit/LitStructs.h"
#include "Constants.h"
#include "FileSystemHelper.h"
#include <array>
#include <iostream>
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

int clearFile(const std::string &patchFile)
{
	std::string fullPathFileName = PATCH_PATH + patchFile;
	if (remove(fullPathFileName.c_str()) != 0)
		return 1;

	return 0;
}

std::string execPatch(const Differences &difference, int &status)
{
	std::string patchFileName = PATCH_PATH + "patch";
	saveContentToFile(patchFileName, difference.diffOutput);
	std::stringstream cmd;
	cmd << "patch -u " << difference.srcFile << " -i " << patchFileName;
	auto result = execCommand(cmd.str(), status);
	clearFile(patchFileName);
	return result;
}