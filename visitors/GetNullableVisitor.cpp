//
// Created by Sichen Song on 9/6/2020.
//

#include "GetNullableVisitor.h"

void GetNullableVisitor::visit(Grammar &g) {
    std::set<Symbol *> nullables;
    const auto &emptyString = Symbol::terminal("");
    while (true) {
        int oldSize = nullables.size();
        for (auto &s : g.getNonTerminal()) {
            if (!g.getRules().contains(s)) continue;
            bool isNullable = false;
            for (auto &r:g.getRules().getProductionFor(s)) {
                bool isNullRule = true;
                for (auto &rhsSym:r) {
                    if (rhsSym != emptyString
                        && nullables.find(rhsSym) == nullables.end())
                        isNullRule = false;
                }
                if (isNullRule)isNullable = true;
            }
            if (isNullable) nullables.insert(s);
        }

        if (oldSize == nullables.size()) {
            result = nullables;
            return;
        }
    }
}

std::set<Symbol *> GetNullableVisitor::getResult() {
    return result;
}
