#include <iostream>
#include "Grammar.h"
#include "./Nondirectional-Parsing/UngerParser.h"
#include "./regular-expression/LongestReMatcher.h"

int evaluate(std::shared_ptr<Node> tree) {
    if (tree->_terminal) {
        if (tree->_symbol->getName() == "Number") {
            return atoi(tree->_value.c_str());
        } else {
            throw std::runtime_error("unexpected non-number terminal");
        }
    }
    if (tree->_children[0]->_symbol->getName() == "LP") {
        return evaluate(tree->_children[1]);
    }
    if (tree->_children.size() == 1) {
        return evaluate(tree->_children[0]);
    }
    return evaluate(tree->_children[0]) + evaluate(tree->_children[2]);
}

int main() {

    auto s = readfile(PROJ_ROOT
                      R"(/test/configs/plus-exp-simple)");
    Grammar g(s);
    UngerParser parser(g);

    s = readfile(PROJ_ROOT
                 R"(/regular-expression/configs/simple)");
    LongestReMatcher l(s);

    auto tokens = l.match("(5+ 1 + (2 + 3) +1+1+1000)");
    LongestReMatcher::removeTokenFromList(tokens, "SPACE");
    std::vector<Token> vtokens(tokens.begin(), tokens.end());
    if (parser.validate(vtokens)) {
        std::cout << "Good!" << std::endl;
    } else {
        std::cout << "Bad!" << std::endl;
    }
    auto parseTree = parser.parse(vtokens);
    std::cout << evaluate(parseTree);
    return 0;
}


