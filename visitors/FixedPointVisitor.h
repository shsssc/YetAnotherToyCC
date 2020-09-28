//
// Created by Sichen Song on 9/4/2020.
//

#ifndef PROJECT_FIXEDPOINTVISITOR_H
#define PROJECT_FIXEDPOINTVISITOR_H

#include "../Grammar.h"
#include <set>

class FixedPointVisitor : public GrammarVisitor {

protected:
    std::set<Symbol *> result;

    virtual std::set<Symbol *> genInitialSet(Grammar &g) = 0;

    std::set<Symbol *> getResult();

    virtual void genOneStep(Grammar &g,
                            std::set<Symbol *> &alreadyIncluded,
                            std::set<Symbol *> &queueThisStep,
                            std::set<Symbol *> &queueNextStep) = 0;

public:
    void visit(Grammar &g) override;


};


#endif //PROJECT_FIXEDPOINTVISITOR_H
