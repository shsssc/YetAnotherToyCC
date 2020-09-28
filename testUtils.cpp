//
// Created by Sichen Song on 9/3/2020.
//

#include "utils.h"

std::string readfile(std::string fname) {
    std::ifstream t(fname);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    t.close();
    return str;
}
