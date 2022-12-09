#include <vector>

#include "tester.h"

int main() {
    auto res = tester::ScannerTester("../tests/scanner/").run_tests();
    res += tester::SimpleParserTester("../tests/simple_parser/").run_tests();

    std::vector<std::string> parser_tests_dirs = {
        "../tests/parser/declarations/", "../tests/parser/statements/",
        "../tests/parser/expressions/", "../tests/parser/types/"};

    for (auto &dir : parser_tests_dirs) {
        res += tester::ParserTester(dir).run_tests();
    }
    std::cout << res;
    return EXIT_SUCCESS;
}
