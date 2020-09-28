//
// Created by Sichen Song on 9/3/2020.
//

#include "Grammar.h"

Grammar::Grammar(std::string g_str) {
    _rules.load(g_str);
    int l = 0, r = 0;
    while (g_str[l] == ' ' || g_str[l] == '\t' || g_str[l] == '\n') {
        l++;
    }
    for (r = 0; g_str[r] != ' ' && g_str[r] != '\t' && g_str[r] != '\n' && g_str[r] != ':'; r++);
    _startSym = Symbol::nonTerminal(g_str.substr(l, r));
}

void Grammar::print() {
    std::cout << "start symbol:\n  " << _startSym << std::endl << " \n\nRules:\n";
    _rules.print();
}

Rules &Grammar::getRules() {
    return _rules;
}

Symbol *&Grammar::getStartSymbol() {
    return _startSym;
}

Grammar &Grammar::accept(GrammarVisitor &gt) {
    gt.visit(*this);
    return *this;
}

std::set<Symbol *> Grammar::getNonTerminal() {
    return Symbol::nonTerminals();
}

std::set<Symbol *> Grammar::getTerminal() {
    return Symbol::terminals();
}
