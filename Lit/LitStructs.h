#include <string>
#ifndef LIT_LITSTRUCTS_H

struct LitDifference {
    LitDifference(std::string basicString, std::string  basicString1, std::string  basicString2);
    std::string srcFile;
    std::string destFile;
    std::string diffOutput;
};

#define LIT_LITSTRUCTS_H

#endif // LIT_LITSTRUCTS_H

