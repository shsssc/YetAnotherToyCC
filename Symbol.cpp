//
// Created by Sichen Song on 7/26/2020.
//

#include "Symbol.h"
#include <iostream>
#include <utility>

std::map<std::string, Symbol *> Symbol::_terminals;
std::map<std::string, Symbol *> Symbol::_nonTerminals;

std::string Symbol::getName() const {
    return _type;
}

bool Symbol::isTerminal() const {
    return _isTerminal;
}

Symbol *Symbol::nonTerminal(const std::string &n) {
    if (_nonTerminals.find(n) != _nonTerminals.end()) {
        return _nonTerminals[n];
    }
    auto result = new Symbol(false, n);
    _nonTerminals[n] = result;
    return result;
}

Symbol *Symbol::terminal(const std::string &n) {
    if (_terminals.find(n) != _terminals.end()) {
        return _terminals[n];
    }
    auto result = new Symbol(true, n);
    _terminals[n] = result;
    return result;
}

Symbol::Symbol(bool isTerminal, std::string name) :
        _isTerminal(isTerminal),
        _type(std::move(name)) {
}

void Symbol::print() {
    std::cout << "terminals: " << std::endl;
    for (auto &s : _terminals) {
        std::cout << "   " << s.first;
    }
    std::cout << std::endl;
    std::cout << "non-terminals: " << std::endl;
    for (auto &s : _nonTerminals) {
        std::cout << "   " << s.first;
    }
    std::cout << std::endl;
}

std::set<Symbol *> Symbol::terminals() {
    std::set<Symbol *> result;
    for (const auto &p : _terminals) {
        result.insert(p.second);
    }
    return result;
}

std::set<Symbol *> Symbol::nonTerminals() {
    std::set<Symbol *> result;
    for (const auto &p : _nonTerminals) {
        result.insert(p.second);
    }
    return result;
}

void Symbol::clear() {
    _terminals.clear();
    _nonTerminals.clear();
}

std::ostream &operator<<(std::ostream &out, const Symbol &s) {
    out << (s.getName().empty() ? "<empty>" : s.getName())
        << "("
        << (s.isTerminal() ? "terminal" : "non-terminal")
        << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Symbol *s) {
    out << (s->getName().empty() ? "<empty>" : s->getName())
        << "("
        << (s->isTerminal() ? "terminal" : "non-terminal")
        << ")";
    return out;
}