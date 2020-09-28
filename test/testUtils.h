//
// Created by Sichen Song on 8/31/2020.
//

#ifndef PROJECT_TESTUTILS_H
#define PROJECT_TESTUTILS_H
#define EXPECT_RULE_EQUAL(rule,RHS)\
for (auto i = (rule).begin(), j = (RHS).begin(); i != (rule).end() && j != (RHS).end(); i++, j++) {\
EXPECT_EQ(*i, *j);\
}\
EXPECT_EQ((rule).size(), (RHS).size());

#endif //PROJECT_TESTUTILS_H
