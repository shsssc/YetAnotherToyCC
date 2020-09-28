//
// Created by Sichen Song on 7/26/2020.
//

#include "Rule.h"
#include <fstream>
#include <iostream>
#include <vector>

Rhs &Rules::addRule(Lhs const &l, const Rhs &r) {
    auto &result = _rules[l];
    result.insert(result.begin(), r);
    return *result.begin();
}

void Rules::setRule(RhsList::iterator rule, const Rhs &newRhs) {
    *rule = newRhs;
}

RhsList &Rules::getProductionFor(Lhs const &l) {
    if (!contains(l)) {
        throw "failed to get rule from l";
    }
    return _rules[l];
}

bool Rules::contains(Lhs const &l) {
    return _rules.find(l) != _rules.end();
}

void Rules::deleteRule(RhsList &rhslist, RhsList::iterator rule) {
    rhslist.erase(rule);
}

struct NextCharReader {
    std::string s;
    int inx;

    NextCharReader(const std::string &s) {
        this->s = s;
        inx = 0;
    }

    int nextChar() {
        if (inx < s.length())return (int) s[inx++];
        return -1;
    }

    bool eof() {
        return inx >= s.length();
    }
};

/*
int nextChar(std::ifstream &fin) {
    if (fin.eof())return -1;
    char c;
    fin.get(c);
    return (int) c;
}
*/

void Rules::print() {
    for (auto &p:this->_rules) {
        std::cout << p.first << std::endl;
        for (auto &rule : p.second) {
            std::cout << "    ";
            for (auto &token : rule) {
                std::cout << token << " ";
            }
            std::cout << "\n";
        }
    }
}

bool fail(std::string s) {
    std::cerr << s << std::endl;
    exit(1);
}

/**
 * Load grammar from string
 * hand-written one-pass DFA parser
 * @param strGrammar
 */
void Rules::load(std::string strGrammar) {
    NextCharReader nc(strGrammar);
    std::string lhs, rhsToken;
    bool tokenIsTerminal = true;
    std::list<Symbol *> rhs;
    enum state {
        BEFORE_LEFT, LEFT, SPACE_AFTER_LEFT, COLLON_EQUAL, BEFORE_RIGHT, RIGHT, AFTER_RIGHT_TERMINAL
    } s;
    s = BEFORE_LEFT;
    char c;
    while (!nc.eof()) {
        if (s == BEFORE_LEFT) {
            while ((c = (char) nc.nextChar()) != -1) {
                if (c != ' ' && c != '\t' && c != '\n')break;
            }
            //this is the only exit
            if (nc.eof())break;
            lhs = c;
            s = LEFT;
        } else if (s == LEFT) {
            while ((c = (char) nc.nextChar()) != -1) {
                if (c == ' ' || c == '\t' || c == '\n' || c == ':')break;
                lhs += c;
            }
            while (c != ':' && (c = (char) nc.nextChar()) != -1) {
                //if (c == ':')
                //    break;
            }

            if ((c = (char) nc.nextChar()) != '=') {
                fail("expect = after :");
            }

            s = BEFORE_RIGHT;
        } else if (s == BEFORE_RIGHT) {
            while ((c = (char) nc.nextChar()) != -1) {
                if (c != ' ' && c != '\t' && c != '\n')break;
            }
            if (c == -1) {
                fail("Expect RHS");
            }
            if (c == '|') {
                //push lhs,rhs,clear rhslist and symbol, goto before rhs
                this->addRule(Symbol::nonTerminal(lhs), rhs);
                rhsToken = "";
                rhs.clear();
                s = BEFORE_RIGHT;
            } else if (c == ';') {
                //push lhs,rhs, clear rhslist and rhssymbol and lhssymbol, goto before left.
                this->addRule(Symbol::nonTerminal(lhs), rhs);
                rhsToken = "";
                lhs = "";
                rhs.clear();
                s = BEFORE_LEFT;
            } else {
                tokenIsTerminal = c == '\'';
                if (!tokenIsTerminal) rhsToken = c;
                s = RIGHT;
            }
        } else if (s == RIGHT && tokenIsTerminal) {
            while ((c = (char) nc.nextChar()) != -1) {
                if (c == '\'') {
                    rhs.push_back(Symbol::terminal(rhsToken));
                    rhsToken = "";
                    s = BEFORE_RIGHT;
                    break;
                }
                if (c == '\\') {
                    c = (char) nc.nextChar();
                }
                if (c == -1) fail("unfinished rhs");
                rhsToken += c;
            }
        } else if (s == RIGHT && !tokenIsTerminal) {
            while ((c = (char) nc.nextChar()) != -1) {
                if (c == ' ' || c == '\t' || c == '\n') {
                    rhs.push_back(Symbol::nonTerminal(rhsToken));
                    rhsToken = "";
                    s = BEFORE_RIGHT;
                    break;
                }
                if (c == '|') {
                    //push lhs,rhs,clear rhslist and symbol, goto before rhs
                    if (!rhsToken.empty())rhs.push_back(Symbol::nonTerminal(rhsToken));
                    this->addRule(Symbol::nonTerminal(lhs), rhs);
                    rhsToken = "";
                    rhs.clear();
                    s = BEFORE_RIGHT;
                    break;
                } else if (c == ';') {
                    //push lhs,rhs, clear rhslist and rhssymbol and lhssymbol, goto before left.
                    if (!rhsToken.empty())rhs.push_back(Symbol::nonTerminal(rhsToken));
                    this->addRule(Symbol::nonTerminal(lhs), rhs);
                    rhsToken = "";
                    lhs = "";
                    rhs.clear();
                    s = BEFORE_LEFT;
                    break;
                }
                if (c == -1) fail("unfinished RHS");
                rhsToken += c;
            }
        }
    }
}

void Rules::clearProductionFor(Lhs const &l) {
    if (!contains(l))return;
    _rules.erase(_rules.find(l));
}


