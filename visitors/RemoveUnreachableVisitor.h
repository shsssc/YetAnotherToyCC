//
// Created by Sichen Song on 9/7/2020.
//

#ifndef PROJECT_REMOVEUNREACHABLEVISITOR_H
#define PROJECT_REMOVEUNREACHABLEVISITOR_H

#include "GetReachableVisitor.h"

class RemoveUnreachableVisitor : public GrammarVisitor {
public:
    void visit(Grammar &g) override;
};


#endif //PROJECT_REMOVEUNREACHABLEVISITOR_H
