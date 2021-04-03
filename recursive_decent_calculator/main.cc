#include<string>
#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>

#include<fmt/format.h>

auto error(std::string_view tokens, size_t& idx, std::string_view msg) -> void;
auto consume_whitespace(std::string_view tokens, size_t& idx) -> void;
auto is_whitespace(char c) -> bool;
auto is_digit(char c) -> bool;
auto digits(std::string_view tokens, size_t& idx) -> double;
auto factor(std::string_view tokens, size_t& idx) -> double;
auto exponent(std::string_view tokens, size_t& idx) -> double;
auto term(std::string_view tokens, size_t& idx) -> double;
auto expr(std::string_view tokens, size_t& idx) -> double;

namespace ansii {
    constexpr auto black    = "\u001b[30m";
    constexpr auto red      = "\u001b[31m";
    constexpr auto green    = "\u001b[32m";
    constexpr auto yellow   = "\u001b[33m";
    constexpr auto blue     = "\u001b[34m";
    constexpr auto magenta  = "\u001b[35m";
    constexpr auto cyan     = "\u001b[36m";
    constexpr auto white    = "\u001b[37m";
    constexpr auto reset    = "\u001b[0m";
}

using fmt::print;

// prints an error message and throws exception
auto error(std::string_view tokens, size_t& idx, std::string_view msg) -> void { //throws
    print("{}{}\n{}{}{}\n", ansii::red, msg, ansii::white, tokens, ansii::red);
    for(size_t i{0}; i < idx; i++) {
        print("-");
    }
    print("^ here{}\n", ansii::reset);
    throw std::exception();
}

constexpr
auto is_whitespace(char c) -> bool {
    return c == '\n' || c == '\t' || c == '\r' || c == ' ';
}

auto consume_whitespace(std::string_view tokens, size_t& idx) -> void {
    while(idx < tokens.length() && is_whitespace(tokens[idx])) {
        idx++;
    }
}

// determines if the character is a digit
auto is_digit(char c) -> bool {
    return c >= '0' && c <= '9';
}

// parses digits and returns result
auto digits(std::string_view tokens, size_t& idx) -> double {
    std::string value{};
    while(is_digit(tokens[idx]) && idx < tokens.length()) {
        value += tokens[idx];
        idx++;
    }
    return std::stod(value);
}

// parses factor and returns result
auto factor(std::string_view tokens, size_t& idx) -> double {
    if(idx >= tokens.length()){
        error(tokens, idx, "Unexpected end of line");
    }
    if(tokens[idx] == '(') {
        auto result{expr(tokens, ++idx)};
        if(idx >= tokens.length() || tokens[idx] != ')'){
            error(tokens, idx, "Missing closing parenthesis");
        }
        idx++;
        consume_whitespace(tokens, idx);
        return result;
    }
    else if(is_digit(tokens[idx])) {
        const auto result{digits(tokens, idx)};
        consume_whitespace(tokens, idx);
        return result;
    }
    error(tokens, idx, "Unexpected token");
    return 0;
}

// parses exponent and returns the result
auto exponent(std::string_view tokens, size_t& idx) -> double {
    consume_whitespace(tokens, idx);
    auto result{factor(tokens, idx)};

    if(idx >= tokens.length()){
        return result;
    }

    if(tokens[idx] == '^') {
        idx++;
        result = std::pow(result, exponent(tokens, idx));
    }
    consume_whitespace(tokens, idx);
    return result;
}

// parses term and returns the result
auto term(std::string_view tokens, size_t& idx) -> double {
    consume_whitespace(tokens, idx);
    auto result{exponent(tokens, idx)};

    if(idx >= tokens.length()){
        return result;
    }

    while(idx < tokens.length() && (tokens[idx] == '*' || tokens[idx] == '/')){
        auto const token{tokens[idx]};
        idx++;
        if(token == '*'){
            result *= exponent(tokens, idx);
        }
        else if(token == '/'){
            result /= exponent(tokens, idx);
        }
    }
    consume_whitespace(tokens, idx);
    return result;
}

// parses an expression and returns the result
auto expr(std::string_view tokens, size_t& idx) -> double {
    consume_whitespace(tokens, idx);
    auto result = term(tokens, idx);
    while(idx < tokens.length() && (tokens[idx] == '+' || tokens[idx] == '-')){
        auto const token{tokens[idx]};
        idx++;
        if(token == '+') {
            result += term(tokens, idx);
        }
        else if(token == '-') {
            result -= term(tokens, idx);
        }
    }
    consume_whitespace(tokens, idx);
    return result;
}

// evaluates the line
auto evaluate(std::string_view line) -> double {
    size_t idx{0};
    auto const result{expr(line, idx)};
    if(idx < line.length()) {
        error(line, idx, "Unexpected character");
    }
    return result;
}

// function for the command line interface
void cli() {
    print("Ready to accept input. Type {}done{} to exit\n", ansii::red, ansii::reset);
    std::string input;
    while(true) {
        print("{}>>> {}", ansii::green, ansii::reset);
        getline(std::cin, input);
        if(input == "done") {
            break;
        }
        try{
            auto const result {evaluate(input)};
            print("Calculated result of {}{}{} is {}{:.6}{}\n",
                    ansii::cyan, input, ansii::reset,
                    ansii::green, result, ansii::reset);
        } catch(std::exception const& e) {
            print("Try again?\n");
        }
    }
    print("Goodbye!\n");
}

void file_interface(std::string path) {
    print("Reading from {}\n", path);
    auto file = std::ifstream(path);
    std::string line;
    while(std::getline(file, line)) {
        // std::getline keeps the newline characters in the string
        if(auto tmp = std::find(std::begin(line), std::end(line), '\n'); tmp != std::end(line)){
            line.erase(tmp);
        }

        if(auto tmp = std::find(std::begin(line), std::end(line), '\r'); tmp != std::end(line)){
            line.erase(tmp);
        }

        if(line.length() == 0) {continue;}
        try{
            auto const result {evaluate(line)};
            print("Calculated result of {}{}{} is {}{:.6}{}\n",
                    ansii::cyan, line, ansii::reset,
                    ansii::green, result, ansii::reset);
        } catch(std::exception const& e) {
        }
    }

}

auto main(int argc, char** argv) -> int {
    if(argc == 2) {
        file_interface(argv[1]);
    } else if (argc == 1) {
        cli();
    } else {
        print("{} <optional file>", argv[0]);
    }
}
