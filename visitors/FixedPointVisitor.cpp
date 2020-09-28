//
// Created by Sichen Song on 9/4/2020.
//

#include "FixedPointVisitor.h"

void FixedPointVisitor::visit(Grammar &g) {
    std::set<Symbol *> alreadyIncluded;
    std::set<Symbol *> queueThisStep;
    std::set<Symbol *> queueNextStep;
    queueNextStep = genInitialSet(g);
    while (!queueNextStep.empty()) {
        alreadyIncluded.insert(queueThisStep.begin(), queueThisStep.end());
        queueThisStep = queueNextStep;
        queueNextStep.clear();
        genOneStep(g, alreadyIncluded, queueThisStep, queueNextStep);
    }
    alreadyIncluded.insert(queueThisStep.begin(), queueThisStep.end());
    result = alreadyIncluded;
}

std::set<Symbol *> FixedPointVisitor::getResult() {
    return result;
}
