//
// Created by Sichen Song on 9/4/2020.
//

#ifndef PROJECT_GETREACHABLEVISITOR_H
#define PROJECT_GETREACHABLEVISITOR_H

#include <iostream>
#include "../Rule.h"
#include "../utils.h"
#include "../Grammar.h"
#include "FixedPointVisitor.h"
#include <set>

class GetReachableVisitor : public FixedPointVisitor {

    std::set<Symbol *> genInitialSet(Grammar &g) override;

    void genOneStep(Grammar &g,
                    std::set<Symbol *> &alreadyIncluded,
                    std::set<Symbol *> &queueThisStep,
                    std::set<Symbol *> &queueNextStep) override;

public:
    std::set<Symbol *> getReachable();
};

#endif //PROJECT_GETREACHABLEVISITOR_H
