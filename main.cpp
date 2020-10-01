#include <iostream>
#include "Grammar.h"
#include "./Nondirectional-Parsing/UngerParser.h"
#include "./regular-expression/LongestReMatcher.h"
#include "./LL-Parsing/LL1Parser.h"

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
  //http://pages.cs.wisc.edu/~fischer/cs536.f13/lectures/f12/Lecture22.4up.pdf
    auto s = readfile(PROJ_ROOT
                      R"(/test/configs/ll1-first-test)");
    Grammar g(s);
    LL1Parser p(g);

    s = readfile(PROJ_ROOT
                 R"(/regular-expression/configs/simple)");
    LongestReMatcher l(s);

    auto tokens = l.match(" ({(abc);});");
    LongestReMatcher::removeTokenFromList(tokens, "SPACE");
    std::vector<Token> vtokens(tokens.begin(), tokens.end());
    if (p.validate(vtokens)) {
        std::cout << "Good!!" << std::endl;
    }else{
        std::cout << "bad!!" << std::endl;
    }
    return 0;
}


