#include "LitStructs.h"
#include <string>
#include <utility>
// TODO maybe not needed
LitDifference::LitDifference(std::string srcFile, std::string destFile, std::string diffOutput)
    : srcFile(std::move(srcFile)), destFile(std::move(destFile)), diffOutput(std::move(diffOutput))
{
}
