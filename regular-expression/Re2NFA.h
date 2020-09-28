//
// Created by Sichen Song on 9/16/2020.
//

#ifndef PROJECT_RE2NFA_H
#define PROJECT_RE2NFA_H

#include <string>
#include <unordered_set>
#include <iostream>
#include <list>
#include "NFA.h"

extern const std::unordered_set<char> specialChar;

class Re2NFA {
    static bool isSpecialChar(char c);

    NFA::NFA _nfa;

    std::string pattern;
public:
    Re2NFA(const std::string &p);

    NFA::NFA getNFA();

    std::pair<int, NFA::NFA> parse(const std::string &str, int start);
};


#endif //PROJECT_RE2NFA_H
