#include "FileSystemHelper.h"
#include <filesystem>

std::filesystem::path stripRoot(const std::filesystem::path& p) {
    const std::filesystem::path& parent_path = p.parent_path();
    if (parent_path.empty() || parent_path.string() == "/")
        return std::filesystem::path();
    else
        return stripRoot(parent_path) / p.filename();
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