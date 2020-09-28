//
// Created by Sichen Song on 9/13/2020.
//

#include "RemoveUnproducableVisitor.h"

void RemoveUnproducableVisitor::visit(Grammar &g) {
    GetProducableVisitor grv;
    g.accept(grv);
    auto producable = grv.getResult();
    for (const auto &symbol:Symbol::nonTerminals()) {
        if (producable.find(symbol) == producable.end()) {
            g.getRules().clearProductionFor(symbol);
        }
    }
}
