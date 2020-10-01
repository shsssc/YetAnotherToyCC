# 'Google_test' is the subproject name
project(Google_tests)
# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests

add_executable(Google_Tests_run ../test/SymbolTest.cpp ../Rule.cpp ../Symbol.cpp ../test/RuleTest.cpp ../test/testUtils.h ../testUtils.cpp ../utils.h ../Grammar.cpp ../Grammar.h ../visitors/GetReachableVisitor.cpp ../visitors/GetReachableVisitor.h ../visitors/FixedPointVisitor.cpp ../visitors/FixedPointVisitor.h ../test/ReachableTest.cpp ../visitors/GetNullableVisitor.cpp ../visitors/GetNullableVisitor.h ../test/NullableTest.cpp ../visitors/RemoveUnreachableVisitor.cpp ../visitors/RemoveUnreachableVisitor.h ../visitors/RemoveNullableVisitor.cpp ../visitors/RemoveNullableVisitor.h ../test/RemoveNullableTest.cpp ../visitors/RemoveUnitVisitor.cpp ../visitors/RemoveUnitVisitor.h ../test/RemoveUnitTest.cpp ../visitors/GetProducableVisitor.cpp ../visitors/GetProducableVisitor.h ../test/ProducableTest.cpp ../visitors/RemoveUnproducableVisitor.cpp ../visitors/RemoveUnproducableVisitor.h ../visitors/MakeCnfVisitor.cpp ../visitors/MakeCnfVisitor.h ../test/MakeCnfTest.cpp ../regular-expression/Re2NFA.cpp ../regular-expression/Re2NFA.h ../regular-expression/NFA.cpp ../regular-expression/NFA.h ../regular-expression/NFAState.cpp ../regular-expression/NFAState.h ../regular-expression/test/NFATest.cpp ../regular-expression/test/ReTest.cpp ../regular-expression/LongestReMatcher.cpp ../regular-expression/LongestReMatcher.h ../Token.h ../Token.cpp ../regular-expression/test/LongestReMatchTest.cpp ../Nondirectional-Parsing/UngerParser.cpp ../Nondirectional-Parsing/UngerParser.h ../Nondirectional-Parsing/test/UngerTest.cpp ../ParseTree.cpp ../ParseTree.h ../LL-Parsing/LL1Parser.cpp ../LL-Parsing/LL1Parser.h ../LL-Parsing/test/LL1ParserTest.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)
set_target_properties(Google_Tests_run PROPERTIES COMPILE_DEFINITIONS "PROJ_ROOT=\"${CMAKE_CURRENT_SOURCE_DIR}/../\"")