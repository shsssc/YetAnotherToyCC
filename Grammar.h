//
// Created by Sichen Song on 9/3/2020.
//
#ifndef PROJECT_GRAMMAR_H
#define PROJECT_GRAMMAR_H

#include <iostream>
#include "Rule.h"
#include "Symbol.h"
#include "utils.h"

class Grammar;

class GrammarVisitor;

class GrammarVisitor {
public:
    virtual void visit(Grammar &g) = 0;
};

class Grammar {
    Rules _rules;
    Symbol *_startSym;
public:
    explicit Grammar(std::string g_str);

    void print();

    Rules &getRules();

    Symbol *&getStartSymbol();

    Grammar &accept(GrammarVisitor &gt);

    std::set<Symbol *> getNonTerminal();

    std::set<Symbol *> getTerminal();
};


#endif //PROJECT_GRAMMAR_H
