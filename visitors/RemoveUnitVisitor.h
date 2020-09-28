//
// Created by Sichen Song on 9/11/2020.
//

#ifndef PROJECT_REMOVEUNITVISITOR_H
#define PROJECT_REMOVEUNITVISITOR_H

#include <algorithm>
#include "../Grammar.h"

class RemoveUnitVisitor : public GrammarVisitor {

    static void dfs(Grammar &g, Symbol *sym, std::set<Symbol *> &seen) {
        //already have
        if (seen.find(sym) != seen.end())return;
        seen.insert(sym);
        if (!g.getRules().contains(sym))return;//has no production
        for (auto &rhs :g.getRules().getProductionFor(sym)) {
            if (rhs.size() == 1 && !(*rhs.begin())->isTerminal()) {
                dfs(g, *rhs.begin(), seen);
            }
        }
    }

    static void removeUnitRule(Grammar &g) {
        for (const auto lhs:Symbol::nonTerminals()) {
            if (!g.getRules().contains(lhs))continue;
            auto &rhslist = g.getRules().getProductionFor(lhs);
            for (auto rhs = rhslist.begin(); rhs != rhslist.end();) {
                if (rhs->size() == 1 && !(*rhs->begin())->isTerminal()) {
                    rhslist.erase(rhs++);
                } else {
                    rhs++;
                }
            }
        }
    }

    static bool containUnitRule(Grammar &g) {
        for (const auto lhs:Symbol::nonTerminals()) {
            if (!g.getRules().contains(lhs))continue;
            auto &rhslist = g.getRules().getProductionFor(lhs);
            for (auto &rhs : rhslist) {
                if (rhs.size() == 1 && !(*rhs.begin())->isTerminal()) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    void visit(Grammar &g) override {
        for (const auto lhs:Symbol::nonTerminals()) {
            if (!g.getRules().contains(lhs))continue;
            std::set<Symbol *> reachable;
            dfs(g, lhs, reachable);
            auto &rhslist = g.getRules().getProductionFor(lhs);
            for (auto sym:reachable) {
                if (sym == lhs || !g.getRules().contains(sym))continue;
                auto &rulesToAdd = g.getRules().getProductionFor(sym);
                for (auto &rhs:rulesToAdd) {
                    if (rhs.size() == 1 && !(*rhs.begin())->isTerminal())continue;
                    if (std::find(rhslist.begin(), rhslist.end(), rhs) != rhslist.end())continue;
                    rhslist.push_back(rhs);
                }
            }
        }
        removeUnitRule(g);
    }

};


#endif //PROJECT_REMOVEUNITVISITOR_H
