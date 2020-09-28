//
// Created by Sichen Song on 9/13/2020.
//

#include "GetProducableVisitor.h"

std::set<Symbol *> GetProducableVisitor::getResult() {
    return result;
}

void GetProducableVisitor::visit(Grammar &g) {
    std::set<Symbol *> producables;
    const auto &emptyString = Symbol::terminal("");
    while (true) {
        int oldSize = producables.size();
        for (auto &s : g.getNonTerminal()) {
            if (!g.getRules().contains(s)) continue;
            bool isProducable = false;
            for (auto &r:g.getRules().getProductionFor(s)) {
                bool isProducableRule = true;
                for (auto &rhsSym:r) {
                    if (!rhsSym->isTerminal()
                        && producables.find(rhsSym) == producables.end())
                        isProducableRule = false;
                }
                if (isProducableRule)isProducable = true;
            }
            if (isProducable) producables.insert(s);
        }

        if (oldSize == producables.size()) {
            result = producables;
            return;
        }
    }
}
