#include "tester.h"

#include <filesystem>
#include <fstream>

#include "../src/generator/generator.h"
#include "../src/parser/parser.h"
#include "../src/visitor/generator_visitor.h"
#include "../src/visitor/printer_visitor.h"
#include "../src/visitor/semantic_visitor.h"

namespace tester {

TestReport Tester::run_tests() {
    TestReport report;
    auto tests_file_paths = get_path_to_files();

    for (const auto& test_file_path : tests_file_paths) {
        auto input_file_path = test_file_path + ".in";
        auto output_file_path = test_file_path + ".out";

        std::cout << input_file_path << std::endl;

        if (!file_exist(output_file_path)) {
            std::ofstream output_stream(output_file_path);
            try {
                auto answer = get_answer(input_file_path);
                output_stream << answer;
            } catch (TinyPascalException& ex) {
                output_stream << ex.what();
            }
            continue;
        }

        auto answer_in_out = read_file(output_file_path);

        try {
            auto answer = get_answer(input_file_path);
            if (answer == answer_in_out) {
                report.inc_success();
            } else {
                report.inc_failed();
                std::cout << "Expected:" << std::endl;
                std::cout << answer_in_out << std::endl;
                std::cout << "Taken: " << std::endl;
                std::cout << answer << std::endl;
            }
        } catch (TinyPascalException& ex) {
            if (answer_in_out == ex.what()) {
                report.inc_success();
            } else {
                report.inc_failed();
                std::cout << "Expected:" << std::endl;
                std::cout << answer_in_out << std::endl;
                std::cout << "Taken: " << std::endl;
                std::cout << ex.what() << std::endl;
            }
        }
    }
    return report;
}

void _get_path_to_files(const std::string& path_to_files,
                        std::set<std::string>& res) {
    for (const auto& entry :
         std::filesystem::directory_iterator(path_to_files)) {
        if (entry.is_directory()) {
            _get_path_to_files(entry.path().string(), res);
        } else {
            auto filename = entry.path().string();
            size_t index_of_separator = filename.find_last_of('.');
            std::string file_name_without_ext =
                filename.substr(0, index_of_separator);
            res.insert(file_name_without_ext);
        }
    }
}

std::set<std::string> Tester::get_path_to_files() {
    std::set<std::string> res;
    _get_path_to_files(path_to_files, res);
    return res;
}

std::string Tester::read_file(const std::string& path) {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);

    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(&buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

bool Tester::file_exist(std::string& path) {
    std::ifstream f(path);
    return f.good();
}

std::string ScannerTester::get_answer(const std::string& path_in) {
    auto stream = std::ifstream(path_in);
    scanner::Scanner scanner(stream);

    std::stringstream answer;

    for (;;) {
        auto token = scanner.next_token();

        answer << token;

        if (token == scanner::TokenType::eof)
            break;
        else
            answer << std::endl;
    }
    return answer.str();
}

std::string ParserTester::get_answer(const std::string& path_in) {
    auto stream = std::ifstream(path_in);
    scanner::Scanner scanner(stream);
    parser::Parser parser(scanner);
    std::stringstream answer;
    auto printer_visitor = std::make_shared<visitor::PrinterVisitor>(answer);
    printer_visitor->visit(parser.program().get());
    return answer.str();
}

std::string SemanticTester::get_answer(const std::string& path_in) {
    auto stream = std::ifstream(path_in);
    std::stringstream answer;
    scanner::Scanner scanner(stream);
    parser::Parser p(scanner);
    auto head = p.program();
    auto semantic_visitor = std::make_shared<visitor::SemanticVisitor>();
    semantic_visitor->visit(head.get());
    auto printer_visitor = std::make_shared<visitor::PrinterVisitor>(answer);
    printer_visitor->visit(head.get());
    answer << "\n";
    semantic_visitor->get_sym_table_stack()->draw(answer);
    return answer.str();
}

std::string GeneratorTester::get_answer(const std::string& path_in) {
    auto file = std::ifstream(path_in);
    std::stringstream answer;
    scanner::Scanner scanner(file);
    parser::Parser p(scanner);
    auto head = p.program();
    auto semantic_visitor = std::make_shared<visitor::SemanticVisitor>();
    semantic_visitor->visit(head.get());
    Generator g;
    auto generation_visitor = std::make_shared<visitor::GeneratorVisitor>(g);
    generation_visitor->visit(head.get());
    auto generator_out = std::ofstream("../tests/generator_out.asm");
    g.write(generator_out);
    generator_out.flush();
    generator_out.close();
    FILE* pipe = popen("powershell ../tests/compile.ps1", "r");
    if (!pipe) {
        answer << "Error: Failed to open pipe";
        return answer.str();
    }
    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            answer << buffer;
        }
    }
    return answer.str();
}
}  // namespace tester
