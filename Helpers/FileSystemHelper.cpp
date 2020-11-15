#include "FileSystemHelper.h"
#include "Constants.h"
#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// basic check if lit folder is there
bool repoCheck()
{
	return fs::exists(LIT_PATH);
}

fs::path stripRoot(const fs::path &p)
{
	const fs::path &parent_path = p.parent_path();
	if (parent_path.empty() || parent_path.string() == "/")
		return fs::path();
	else
		return stripRoot(parent_path) / p.filename();
}

bool hasFileBeenRemoved(const std::string &file, std::list<std::string> &removedFiles)
{
	fs::path pathToFile(file);
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
	fs::path pathToDest(dest);
	fs::create_directories(pathToDest.remove_filename());
	std::ifstream srcFile(src, std::ios::binary);
	std::ofstream destFile(dest, std::ios::binary);
	destFile << srcFile.rdbuf();
	return srcFile && destFile;
}

void removeFileAndDirectories(const std::string &src)
{
	fs::path pathToSrc(src);
	if (fs::remove(pathToSrc)) {
		auto pathWithoutFile = pathToSrc.parent_path();
		while (pathWithoutFile != "./") {
			if (fs::is_empty(pathWithoutFile)) {
				fs::remove(pathWithoutFile);
				pathWithoutFile = pathWithoutFile.parent_path();
			} else {
				return;
			}
		}
	}
}

void copyCommitToWorkingArea()
{
	system("rm -rf *");
	fs::path pathToSrc(CURRENT_COMMIT);
	fs::path pathToDest("./");
	fs::copy(pathToSrc, pathToDest, fs::copy_options::recursive);
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
		fs::path pathToDest(filename);
		fs::create_directories(pathToDest.remove_filename());
	}
	std::ofstream outfile(filename);
	if (!outfile) {
		std::cout << "problems while trying to create file file " << filename << " to .lit folder..." << std::endl;
		return;
	}
	outfile << content;
	outfile.close();
}