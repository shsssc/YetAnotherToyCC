#include <iostream>
#include "Grammar.h"
#include "./Nondirectional-Parsing/UngerParser.h"
#include "./regular-expression/LongestReMatcher.h"
#include "./LL-Parsing/LL1Parser.h"

int main() {
    //http://pages.cs.wisc.edu/~fischer/cs536.f13/lectures/f12/Lecture22.4up.pdf
    auto s = readfile(PROJ_ROOT
                      R"(/test/configs/ll1-prog)");
    Grammar g(s);
    LL1Parser p(g);

    s = readfile(PROJ_ROOT
                 R"(/regular-expression/configs/ll1)");
    LongestReMatcher l(s);

    auto tokens = l.match("{ id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id;id = id + id; }");
    LongestReMatcher::removeTokenFromList(tokens, "SPACE");
    std::vector<Token> vtokens(tokens.begin(), tokens.end());
    if (p.validate(vtokens)) {
        std::cout << "Good!!" << std::endl;
    } else {
        std::cout << "bad!!" << std::endl;
    }
    return 0;
}


