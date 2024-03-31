/*
 * custom_errors.h
 *
 * required :
 *
 * Created on Mon Feb 26 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#pragma once

#include <stdexcept>
#include <string>

namespace vino {

class TokenizeError : public std::exception {
    std::string what_str;

public:
    explicit TokenizeError(std::string errstr) noexcept : what_str(errstr) {}

    TokenizeError() noexcept :
        TokenizeError("Error while tokenizing script file;\n")
    {
    }

    const char *what() const noexcept { return what_str.c_str(); }
};

class ParsingError : public std::exception {
    std::string what_str;

public:
    explicit ParsingError(const std::string &errstr) noexcept : what_str(errstr)
    {
    }

    ParsingError() noexcept : ParsingError("Error while parsing tokens;") {}

    const char *what() const noexcept { return what_str.c_str(); }
};

class NullPtrExc : public std::exception {
public:
    const char *what() const noexcept
    {
        return "Error: dereferencing nullptr;\n";
    }
};

class SemanticError : public std::exception {
    std::string what_str;

public:
    explicit SemanticError(const std::string &errstr) noexcept :
        what_str(errstr)
    {
    }

    SemanticError() noexcept : SemanticError("Error in semantics;\n") {}

    const char *what() const noexcept { return what_str.c_str(); }
};

class ArgsError : public std::exception {
    std::string what_str;
public:
    explicit ArgsError(std::string errstr) noexcept :
        what_str(std::move(errstr))
    {
    }

    const char *what() const noexcept { return what_str.c_str(); }
};

}  // namespace vino