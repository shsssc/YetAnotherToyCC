//
// Created by Sichen Song on 9/27/2020.
//

#ifndef PROJECT_PARSETREE_H
#define PROJECT_PARSETREE_H

#include <vector>
#include <memory>
#include "Rule.h"

struct Node {
    bool _terminal;
    std::vector<std::shared_ptr<Node> > _children;
    Rhs _rule;
    /**
     * value of terminal.
     */
    std::string _value;
    /**
     * symbol in the tree.
     */
    Symbol *_symbol;

};

class ParseTree {
    std::vector<Node> _nodes;
    Node *_startSymbol;
};


#endif //PROJECT_PARSETREE_H
