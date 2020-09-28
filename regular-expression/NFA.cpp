//
// Created by Sichen Song on 9/16/2020.
//

#include "NFA.h"

void NFA::NFA::findEpsilonClosure(State s, std::unordered_set<State> &result) {
    for (State n: s->next(NFAState::EPSILON)) {
        if (result.find(n) != result.end())continue;
        result.insert(n);
        findEpsilonClosure(n, result);
    }
}

std::unordered_set<NFA::State> NFA::NFA::findEpsilonClosure(const std::unordered_set<State> &states) {
    std::unordered_set<State> result = states;
    for (State s: states) {
        std::unordered_set<State> epClosure;
        findEpsilonClosure(s, epClosure);
        result.insert(epClosure.begin(), epClosure.end());
    }
    return result;
}

NFA::NFA::NFA(State start, State accept) {
    _start = start;
    _accept = accept;
    refresh();
}

void NFA::NFA::refresh() {
    _current_location.clear();
    _current_location.insert(_start);
    _current_location = findEpsilonClosure(_current_location);
}

void NFA::NFA::setStartState(State s) {
    _start = s;
}

void NFA::NFA::take(char c) {
    std::unordered_set<State> current_states = _current_location, nextStates;
    for (State s : current_states) {
        std::unordered_set<State> reachable = s->next(c);
        nextStates.insert(reachable.begin(), reachable.end());
    }
    nextStates = findEpsilonClosure(nextStates);
    _current_location = nextStates;
}

void NFA::NFA::setAcceptState(State s) {
    _accept = s;
}

NFA::State NFA::NFA::getStartState() const {
    return _start;
}

NFA::State NFA::NFA::getAcceptState() const {
    return _accept;
}

std::unordered_set<NFA::State> NFA::NFA::getCurrentState() const {
    return _current_location;
}

bool NFA::NFA::accepted() const{
    return _current_location.end() != _current_location.find(_accept);
}

bool NFA::NFA::invalidPrefix() const{
    return _current_location.empty();
}

void NFA::NFA::take(const std::string &s) {
    for (const char i : s) {
        this->take(i);
    }
}

NFA::NFA::NFA() {
    _start = nullptr;
    _accept = nullptr;
}

NFA::NFA NFA::nfaFromWords(const std::string &w) {
    if (w.empty()) {
        throw "Invalid input";
    }
    State start = newState();
    State end = newState();
    NFA result(start, end);
    for (int i = 0; i < w.length() - 1; i++) {
        char c = w[i];
        State next = newState();
        start->addNext(c, next);
        start = next;
    }
    start->addNext(w[w.length() - 1], end);
    return result;
}

void NFA::nfaConcat(NFA &n1, const NFA &n2) {
    NFA &result = n1;
    result.getAcceptState()->addNext(NFAState::EPSILON, n2.getStartState());
    result.setAcceptState(n2.getAcceptState());
    result.refresh();
}

void NFA::nfaStar(NFA &n) {
    State newStart = newState();
    newStart->addNext(NFAState::EPSILON, n.getAcceptState());
    newStart->addNext(NFAState::EPSILON, n.getStartState());
    n.getAcceptState()->addNext(NFAState::EPSILON, n.getStartState());
    n.setStartState(newStart);
    n.refresh();
}

void NFA::nfaQuestion(NFA &n) {
    State newAccept = newState();
    n.getStartState()->addNext(NFAState::EPSILON, newAccept);
    n.getAcceptState()->addNext(NFAState::EPSILON, newAccept);
    n.setAcceptState(newAccept);
    n.refresh();
}

void NFA::nfaPlus(NFA &n) {
    n.getAcceptState()->addNext(NFAState::EPSILON, n.getStartState());
    n.refresh();
}

void NFA::nfaOr(NFA &n1, const NFA &n2) {
    State newStart = newState();
    State newAccept = newState();
    newStart->addNext(NFAState::EPSILON, n1.getStartState());
    newStart->addNext(NFAState::EPSILON, n2.getStartState());
    n1.getAcceptState()->addNext(NFAState::EPSILON, newAccept);
    n2.getAcceptState()->addNext(NFAState::EPSILON, newAccept);
    n1.setStartState(newStart);
    n1.setAcceptState(newAccept);
    n1.refresh();
}

NFA::NFA NFA::nfaFromCharSet(const std::string &charSet) {
    std::unordered_set<char> s;
    int i = 0;
    while (i < charSet.size()) {
        char c = charSet[i];
        switch (c) {
            default:
                s.insert(c);
                i++;
                break;
            case '\\':
                i++;
                if (i == charSet.size()) {
                    throw std::runtime_error("Invalid Syntax");
                }
                c = charSet[i];
                s.insert(c);
                i++;
                break;
            case '-':
                if (i == 0 || i + 1 == charSet.size()) {
                    throw std::runtime_error("unexpected -");
                }
                char l = charSet[i - 1];
                char r = charSet[i + 1];
                if (r < l) {
                    throw std::runtime_error("invalid char range");
                }
                for (char ch = l; ch <= r; ch++) {
                    s.insert(ch);
                }
                i += 2;
                break;
        }
    }
    State start = newState(), accept = newState();
    for (auto c:s) {
        start->addNext(c, accept);
    }
    return NFA(start, accept);
}
