//
// Created by Sichen Song on 9/4/2020.
//

#include "GetReachableVisitor.h"

std::set<Symbol *> GetReachableVisitor::genInitialSet(Grammar &g) {
    std::set<Symbol *> result;
    result.insert(g.getStartSymbol());
    return result;
}

void GetReachableVisitor::genOneStep(Grammar &g, std::set<Symbol *> &alreadyIncluded, std::set<Symbol *> &queueThisStep,
                                     std::set<Symbol *> &queueNextStep) {
    for (Symbol *nonTerminal:queueThisStep) {
        if (!g.getRules().contains(nonTerminal)) continue;
        if (alreadyIncluded.find(nonTerminal) != alreadyIncluded.end()) continue;
        auto &rules = g.getRules().getProductionFor(nonTerminal);
        for (auto &r : rules) {
            for (auto &s:r) {
                if (!s->isTerminal() && queueThisStep.find(s) == queueThisStep.end() &&
                    alreadyIncluded.find(s) == alreadyIncluded.end())
                    queueNextStep.insert(s);
            }
        }
    }
}

std::set<Symbol *> GetReachableVisitor::getReachable() {
    return this->getResult();
}
