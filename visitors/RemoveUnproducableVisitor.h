//
// Created by Sichen Song on 9/13/2020.
//

#ifndef PROJECT_REMOVEUNPRODUCABLEVISITOR_H
#define PROJECT_REMOVEUNPRODUCABLEVISITOR_H
#include "../Grammar.h"
#include "GetProducableVisitor.h"
class RemoveUnproducableVisitor : public GrammarVisitor{
public:
    void visit(Grammar &g) override;
};


#endif //PROJECT_REMOVEUNPRODUCABLEVISITOR_H
