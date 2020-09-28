//
// Created by Sichen Song on 9/16/2020.
//

#ifndef PROJECT_NFASTATE_H
#define PROJECT_NFASTATE_H

#include <unordered_map>
#include <unordered_set>

class NFAState;

typedef std::unordered_set<NFAState *> NFAStateSet;


class NFAState {

    std::unordered_map<char, std::unordered_set<NFAState *>> _next_state;
public:
    static const char EPSILON = '\1';

    void addNext(char c, NFAState *nextState) {
        _next_state[c].insert(nextState);
    }

    NFAStateSet next(char c) {
        return _next_state[c];
    }
};



NFAState *newState();

#endif //PROJECT_NFASTATE_H
