#include "Commit.h"

#include <fstream>
#include <iostream>

void Commit::toFile(const std::string &fileName)
{
	std::ofstream out_file(fileName + ".commit");
	if (out_file.is_open()) {
		out_file << revisionNumber << std::endl;
		out_file << parentCommit << std::endl;
		out_file << commitMsg << std::endl;
		char buffer[40];
		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &date);
		std::string dateTimeString(buffer);
		out_file << dateTimeString << std::endl;
		out_file << "added files:" << std::endl;
		for (const std::string &addedFile : addedFiles) {
			out_file << addedFile << std::endl;
		}
		out_file << "removed files:" << std::endl;
		for (const std::string &removedFile : removedFiles) {
			out_file << removedFile << std::endl;
		}
		out_file << "differences:" << std::endl;
		for (const LitStructs &difference : differences) {
			out_file << difference.srcFile << std::endl;
			out_file << difference.destFile << std::endl;
			out_file << difference.diffOutput << std::endl;
		}
		out_file.close();
	} else {
		std::cout << "Problem while creating a commit file. Please check the access rights." << std::endl;
	}
}

Commit::Commit(const std::string &fileName)
{
	std::ifstream in_file(fileName);
	if (in_file.is_open()) {
		std::string line;
		getline(in_file, line);
		revisionNumber = line;
		getline(in_file, line);
		parentCommit = line;
		getline(in_file, line);
		commitMsg = line;
		getline(in_file, line);
		tm tm{};
		if (strptime(line.c_str(), "%d-%m-%Y %H:%M:%S", &tm))
			date = tm;
		// this line is addedFiles:
		getline(in_file, line);
		getline(in_file, line);
		// because the program writes the commit file and it should not be changed, I am using magic strings here.
		while (line != "removed files:") {
			addedFiles.emplace_back(line);
			getline(in_file, line);
		}
		getline(in_file, line);
		while (line != "differences:") {
			removedFiles.emplace_back(line);
			getline(in_file, line);
		}
		while (getline(in_file, line)) {
			std::string diffOutput;
			std::string srcFile;
			std::string destFile;
			if (!line.empty()) {
				srcFile = line;
				getline(in_file, line);
				destFile = line;
				getline(in_file, line);
			}
			while (!line.empty()) {
				diffOutput.append(line + "\n");
				getline(in_file, line);
			}
			if (!diffOutput.empty()) {
				LitStructs litDifference(srcFile, destFile, diffOutput);
				differences.emplace_back(litDifference);
			}
		}
	} else {
		std::cout << "commit file not found..." << std::endl;
	}
}

Commit::Commit(std::string revisionNumber, std::string parentCommit, std::string commitMsg, tm date,
               std::list<LitStructs> differences, std::list<std::string> addedFiles,
               std::list<std::string> removedFiles)
    : revisionNumber(revisionNumber), parentCommit(parentCommit), commitMsg(commitMsg), date(date),
      differences(differences), addedFiles(addedFiles), removedFiles(removedFiles)
{
}