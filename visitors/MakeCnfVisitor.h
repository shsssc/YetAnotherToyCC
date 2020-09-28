//
// Created by Sichen Song on 9/14/2020.
//

#ifndef PROJECT_MAKECNFVISITOR_H
#define PROJECT_MAKECNFVISITOR_H

#include "../Grammar.h"

class MakeCnfVisitor : public GrammarVisitor {
    int _counter = 0;

    std::string next() {
        return "_" + std::to_string(_counter++) + "";
    }

    void makeRuleCnf(Symbol *lhs, Rhs &rhs, Grammar &g) {
        if (rhs.size() <= 2)return;
        auto newLhs = Symbol::nonTerminal(lhs->getName() + next());
        auto from = rhs.begin();
        from++;
        Rhs tail;
        tail.insert(tail.begin(), from, rhs.end());
        rhs = Rhs({*rhs.begin(), newLhs});
        makeRuleCnf(newLhs, g.getRules().addRule(newLhs, tail), g);
        /**
         * s -> ABC become S -> A S1, S1 -> BC
         */
    }

public:
    void visit(Grammar &g) override {
        for (Symbol *lhs:g.getNonTerminal()) {
            if (!g.getRules().contains(lhs))continue;
            auto &rhslist = g.getRules().getProductionFor(lhs);
            for (auto &rhs :rhslist) {
                makeRuleCnf(lhs, rhs, g);
            }
        }
    }
};


#endif //PROJECT_MAKECNFVISITOR_H
