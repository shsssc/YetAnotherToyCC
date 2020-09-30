//
// Created by Sichen Song on 7/26/2020.
//

#ifndef PROJECT_SYMBOL_H
#define PROJECT_SYMBOL_H

#include <string>
#include <ostream>
#include <map>
#include <utility>
#include <set>


class Symbol {
    static std::map<std::string, Symbol *> _terminals;
    static std::map<std::string, Symbol *> _nonTerminals;
    const std::string _type;
    const bool _isTerminal;

    Symbol(bool isTerminal, std::string name);

    Symbol(const Symbol &s) = default;

public:
    std::string getName() const;

    bool isTerminal() const;

    static Symbol *nonTerminal(const std::string &n);

    static Symbol *terminal(const std::string &n);

    static void print();

    static std::set<Symbol *> terminals();

    static std::set<Symbol *> nonTerminals();

    static void clear();
};

std::ostream &operator<<(std::ostream &out, const Symbol &s);

std::ostream &operator<<(std::ostream &out, const Symbol *s);

#endif //PROJECT_SYMBOL_H
