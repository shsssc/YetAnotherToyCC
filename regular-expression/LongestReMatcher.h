//
// Created by Sichen Song on 9/19/2020.
//

#ifndef PROJECT_LONGESTREMATCHER_H
#define PROJECT_LONGESTREMATCHER_H

#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <utility>
#include "NFA.h"
#include "Re2NFA.h"
#include "../Token.h"

std::ostream &operator<<(std::ostream &out, const Token &token);

class LongestReMatcher {
    std::list<std::pair<std::string, NFA::NFA>> _inUseRe;
    std::list<std::pair<std::string, NFA::NFA>> _allRe;

    int _l = -1;
    int _lastR = -1;
    std::string _last_nonTerminal;

    void removeDeadRe();

    bool impossiblePrefix();

    /**
     * reset pending regular expression. Keep the matching cursor
     */
    void resetRe();

    /**
     * reset NFA state and matching cursor. Only need when new match begins
     */
    void reset();

    bool accepted();

    void take(char c);

    void loadConfigList(const std::list<std::pair<std::string, std::string>> &m);

public:
    explicit LongestReMatcher(const std::list<std::pair<std::string, std::string>> &m);

    explicit LongestReMatcher(const std::string &s);

    std::list<Token> match(const std::string &s);

    static void removeTokenFromList(std::list<Token> &tokens, const std::string &);

};


#endif //PROJECT_LONGESTREMATCHER_H
