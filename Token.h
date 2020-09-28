//
// Created by Sichen Song on 9/26/2020.
//

#ifndef PROJECT_TOKEN_H
#define PROJECT_TOKEN_H

#include <string>

struct Token {
    const std::string type;
    const std::string value;

    Token(std::string t, std::string v);
};

#endif //PROJECT_TOKEN_H
