#include "FileSystemHelper.h"
#include "Constants.h"
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <iostream>

std::filesystem::path stripRoot(const std::filesystem::path &p)
{
	const std::filesystem::path &parent_path = p.parent_path();
	if (parent_path.empty() || parent_path.string() == "/")
		return std::filesystem::path();
	else
		return stripRoot(parent_path) / p.filename();
}

bool hasFileBeenRemoved(const std::string &file, std::list<std::string> &removedFiles)
{
	std::filesystem::path pathToFile(file);
	// strip root twice to get ride of .lit/currentCommit
	pathToFile = stripRoot(pathToFile);
	pathToFile = stripRoot(pathToFile);
	std::stringstream filePathToWorkingDirectory;
	filePathToWorkingDirectory << "./" << pathToFile.string();

	std::ifstream in_file(filePathToWorkingDirectory.str());
	if (!in_file.good()) {
		removedFiles.emplace_back(filePathToWorkingDirectory.str());
		return true;
	}

	return false;
}

bool copyFileAndDirectories(const std::string &src, const std::string &dest)
{
	// todo: maybe use paths as arguments and cast paths in the function call
	std::filesystem::path pathToDest(dest);
	std::filesystem::create_directories(pathToDest.remove_filename());
	std::ifstream srcFile(src, std::ios::binary);
	std::ofstream destFile(dest, std::ios::binary);
	destFile << srcFile.rdbuf();
	return srcFile && destFile;
}

void removeFileAndDirectories(const std::string &src)
{
	std::filesystem::path pathToSrc(src);
	if (std::filesystem::remove(pathToSrc)) {
		auto pathWithoutFile = pathToSrc.parent_path();
		while (pathWithoutFile != "./") {
			if (std::filesystem::is_empty(pathWithoutFile)) {
				std::filesystem::remove(pathWithoutFile);
				pathWithoutFile = pathWithoutFile.parent_path();
			} else {
				return;
			}
		}
	}
}

void CopyCommitToWorkingArea()
{
	system("rm -rf *");
	std::filesystem::path pathToSrc(CURRENT_COMMIT);
	std::filesystem::path pathToDest("./");
	std::filesystem::copy(pathToSrc, pathToDest);
}

void listFiles(const std::string &path, std::list<std::string> &files)
{
	if (auto dir = opendir(path.c_str())) {
		while (auto f = readdir(dir)) {
			if (!f->d_name || f->d_name[0] == '.')
				continue;
			if (f->d_type == DT_DIR)
				listFiles(path + f->d_name + "/", files);

			if (f->d_type == DT_REG)
				files.emplace_back(path + f->d_name);
		}
		closedir(dir);
	}
}

void setHead(const std::string &branch)
{
	std::ofstream headFile(HEAD);
	if (headFile.is_open()) {
		headFile << "refs/heads/" << branch;
	}
}

void setRefs(const std::string &revisionNumber)
{
	// todo: improve this
	std::ifstream headFile(HEAD);
	if (headFile.is_open()) {
		std::string line;
		getline(headFile, line);
		headFile.close();
		std::ofstream ref_file(LIT_PATH + line);
		if (ref_file.is_open()) {
			ref_file << revisionNumber;
			return;
		}
	}

	std::cout << "error while opening head or refs file; cannot change ref..." << std::endl;
}

void saveContentToFile(const std::string &filename, const std::string &content, bool createMissingDirectories)
{
	if (createMissingDirectories) {
		std::filesystem::path pathToDest(filename);
		std::filesystem::create_directories(pathToDest.remove_filename());
	}
	std::ofstream outfile(filename);
	if (!outfile) {
		std::cout << "Problems while trying saving patch file to .lit folder..." << std::endl;
		return;
	}
	outfile << content;
	outfile.close();
}