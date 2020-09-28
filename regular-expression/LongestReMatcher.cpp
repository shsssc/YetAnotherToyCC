//
// Created by Sichen Song on 9/19/2020.
//

#include "LongestReMatcher.h"

std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << "[" << token.value << "(" << token.type << ")]";
    return out;
}

void LongestReMatcher::removeDeadRe() {
    _inUseRe.remove_if(
            [](const std::pair<std::string, NFA::NFA> &s) {
                return s.second.invalidPrefix();
            }
    );
}

bool LongestReMatcher::impossiblePrefix() {
    return _inUseRe.empty();
}

void LongestReMatcher::resetRe() {
    _inUseRe.clear();
    _inUseRe.insert(_inUseRe.begin(), _allRe.begin(), _allRe.end());
    for (auto &p:_inUseRe) {
        p.second.refresh();
    }
}

void LongestReMatcher::reset() {
    _l = 0;
    _lastR = -1;
    _last_nonTerminal = "invalid";
    resetRe();
}

bool LongestReMatcher::accepted() {
    return std::any_of(_inUseRe.begin(), _inUseRe.end(),
                       [](const std::pair<std::string, NFA::NFA> &p) {
                           return p.second.accepted();
                       }
    );
}

void LongestReMatcher::take(char c) {
    for (auto &p:_inUseRe) {
        p.second.take(c);
    }
}

void LongestReMatcher::loadConfigList(const std::list<std::pair<std::string, std::string>> &m) {
    for (auto &p:m) {
        auto &nonTerminal = p.first;
        auto &re = p.second;
        auto nfa = Re2NFA(re).getNFA();
        nfa.refresh();
        _allRe.emplace_back(nonTerminal, nfa);
        reset();
    }
}

LongestReMatcher::LongestReMatcher(const std::list<std::pair<std::string, std::string>> &m) {
    loadConfigList(m);
}

LongestReMatcher::LongestReMatcher(const std::string &s) {
    int l = 0;
    std::list<std::pair<std::string, std::string>> result;
    while (l < s.size()) {
        std::string type, regex;
        while (l < s.size() && (s[l] == ' ' || s[l] == '\n'))l++;
        while (l < s.size() && s[l] != '=' && s[l] != ' ')type += s[l++];
        while (l < s.size() && s[l] == ' ')l++;
        if (l < s.size() && s[l] != '=')throw std::runtime_error("= expected");
        l++;
        while (l < s.size() && s[l] == ' ')l++;
        while (l < s.size() && s[l] != '\n')regex += s[l++];
        //std::cout << type << "\t" << regex << std::endl;
        if (!type.empty() && !regex.empty()) result.emplace_back(type, regex);
    }
    loadConfigList(result);
}

std::list<Token> LongestReMatcher::match(const std::string &s) {
    std::list<Token> result;
    reset();
    for (int i = 0; i < s.length();) {
        char c = s[i];
        take(c);
        removeDeadRe();
        if ((impossiblePrefix()) && _lastR >= 0) {
            //std::cout << s.substr(_l, _lastR - _l + 1) << "  <-> " << _last_nonTerminal << std::endl;
            result.emplace_back(_last_nonTerminal, s.substr(_l, _lastR - _l + 1));
            resetRe();
            _l = _lastR + 1;
            i = _lastR + 1;
            _lastR = -1;
            _last_nonTerminal = "invalid";
            continue;
        }
        if (impossiblePrefix() && _lastR < 0) {
            throw std::runtime_error("failed to tokenize");
        }
        if (accepted()) {
            for (auto &p:_inUseRe) {
                if (p.second.accepted()) {
                    _lastR = i;
                    _last_nonTerminal = p.first;
                    break;
                }
            }
        }
        i++;
    }
    if (_lastR != -1) {
        //std::cout << s.substr(_l, _lastR - _l + 1) << "  <-> " << _last_nonTerminal << std::endl;
        result.emplace_back(_last_nonTerminal, s.substr(_l, _lastR - _l + 1));
    }
    return result;
}

void LongestReMatcher::removeTokenFromList(std::list<Token> &tokens, const std::string & type) {
    for (auto i = tokens.begin(); i != tokens.end();) {
        if (i->type == type) {
            auto pos = i;
            i++;
            tokens.erase(pos);
        } else {
            i++;
        }
    }
}
