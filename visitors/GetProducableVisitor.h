//
// Created by Sichen Song on 9/13/2020.
//

#ifndef PROJECT_GETPRODUCABLEVISITOR_H
#define PROJECT_GETPRODUCABLEVISITOR_H

#include "../Grammar.h"

class GetProducableVisitor : public GrammarVisitor {
    std::set<Symbol *> result;
public:
    std::set<Symbol *> getResult();

    void visit(Grammar &g) override;
};


#endif //PROJECT_GETPRODUCABLEVISITOR_H
