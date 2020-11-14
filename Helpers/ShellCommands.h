#include "../Lit/LitStructs.h"
#include <functional>
#include <list>
#include <string>
#ifndef LIT_SHELLCOMMANDS_H
#define LIT_SHELLCOMMANDS_H

#endif // LIT_SHELLCOMMANDS_H

std::string execPatch(const Differences &difference, int &status);
std::string execDiff(const std::string &dst, const std::string &src, int &status);