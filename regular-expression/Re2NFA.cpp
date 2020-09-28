//
// Created by Sichen Song on 9/16/2020.
//

#include "Re2NFA.h"

const std::unordered_set<char> specialChar({'(', ')', '+', '*', '[', ']', '?', '|', '\0'});

bool Re2NFA::isSpecialChar(char c) {
    if (specialChar.find(c) != specialChar.end()) {
        return true;
    }
    return false;
}

Re2NFA::Re2NFA(const std::string &p) {
    pattern = p + '\0';
    auto result = parse(pattern, 0);
    if (result.first != pattern.length()) {
        std::cout << "Invalid regular expression";
    }
    _nfa = result.second;
}

NFA::NFA Re2NFA::getNFA() {
    return _nfa;
}

std::pair<int, NFA::NFA> Re2NFA::parse(const std::string &str, int start) {
    int r = start;
    std::string pendingChar;
    std::list<NFA::NFA> pendingNFA;
    while (true) {
        while (!isSpecialChar(str[r])) {
            if (str[r] == '\\') {
                if (isSpecialChar(str[r + 1]) || str[r + 1] == '\\') {
                    pendingChar += str[r + 1];
                    r += 2;
                } else {
                    throw std::runtime_error("unexpected escape char");
                }
            } else {
                pendingChar += str[r];
                r++;
            }
        }
        if (!pendingChar.empty()) {
            pendingNFA.push_back(NFA::nfaFromWords(pendingChar));
            pendingChar.clear();
        }
        char c = str[r];
        switch (c) {
            case ')':
            case '\0': {
                if (pendingNFA.empty())throw std::runtime_error("unexpected empty");
                auto &first = *pendingNFA.begin();
                auto p = pendingNFA.begin();
                p++;
                if (pendingNFA.size() >= 3 && p->getAcceptState() == nullptr) {
                    p++;
                    if (p == pendingNFA.end() || p->getAcceptState() == nullptr) {
                        throw std::runtime_error("unexpected |");
                    }
                    NFA::nfaOr(first, *p);
                    p++;
                }
                while (p != pendingNFA.end()) {
                    auto &currentNFA = *p;
                    p++;
                    if (p == pendingNFA.end()) {
                        NFA::nfaConcat(first, currentNFA);
                        break;
                    }
                    if (p->getAcceptState() != nullptr) {
                        NFA::nfaConcat(first, currentNFA);
                        continue;
                    }
                    p++;
                    if (p == pendingNFA.end() || p->getAcceptState() == nullptr) {
                        throw std::runtime_error("unexpected |");
                    }
                    NFA::nfaOr(currentNFA, *p);
                    NFA::nfaConcat(first, currentNFA);
                    p++;
                }
                return std::pair<int, NFA::NFA>{r + 1, first};
            }
            case ']':
                throw std::runtime_error("unexpected ]");
            case '(': {
                auto result = parse(str, r + 1);
                r = result.first;
                pendingNFA.push_back(result.second);
                break;
            }
            case '[': {
                r++;
                std::string charSet;
                while (r < str.size()) {
                    char ch = str[r];
                    if (ch == ']') {
                        r++;
                        break;
                    }

                    charSet += ch;
                    if (ch == '\\') {
                        charSet += str[r + 1];
                        r += 2;
                    } else {
                        r++;
                    }
                }
                pendingNFA.push_back(NFA::nfaFromCharSet(charSet));
            }
                break;
            case '?':
                if (pendingNFA.empty())throw std::runtime_error("unexpected ?");
                NFA::nfaQuestion(*pendingNFA.rbegin());
                r++;
                break;
            case '+':
                if (pendingNFA.empty())throw std::runtime_error("unexpected +");
                NFA::nfaPlus(*pendingNFA.rbegin());
                r++;
                break;
            case '*':
                if (pendingNFA.empty())throw std::runtime_error("unexpected *");
                NFA::nfaStar(*pendingNFA.rbegin());
                r++;
                break;
            case '|':
                pendingNFA.emplace_back();
                r++;
                break;
            default:
                break;
        }
    }
}
