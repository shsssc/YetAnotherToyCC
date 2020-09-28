//
// Created by Sichen Song on 9/7/2020.
//

#include "RemoveUnreachableVisitor.h"

void RemoveUnreachableVisitor::visit(Grammar &g) {
    GetReachableVisitor grv;
    g.accept(grv);
    auto reachable = grv.getReachable();
    for (const auto &symbol:Symbol::nonTerminals()) {
        if (reachable.find(symbol) == reachable.end()) {
            g.getRules().clearProductionFor(symbol);
        }
    }
}
