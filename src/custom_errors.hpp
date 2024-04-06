/*
 * custom_errors.hpp
 *
 * required :
 *
 * Created on Mon Feb 26 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#pragma once

#include <exception>
#include <string>
#include <utility>

namespace vino {

class TokenizeError : public std::exception {
    std::string what_str;

public:
    explicit TokenizeError(std::string errstr) noexcept :
        what_str(std::move(errstr))
    {
    }

    TokenizeError() noexcept :
        TokenizeError("Error while tokenizing script file;\n")
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        return what_str.c_str();
    }
};

class ParsingError : public std::exception {
    std::string what_str;

public:
    explicit ParsingError(std::string errstr) noexcept :
        what_str(std::move(errstr))
    {
    }

    ParsingError() noexcept : ParsingError("Error while parsing tokens;") {}

    [[nodiscard]] const char *what() const noexcept override
    {
        return what_str.c_str();
    }
};

class NullPtrExc : public std::exception {
public:
    [[nodiscard]] const char *what() const noexcept override
    {
        return "Error: dereferencing nullptr;\n";
    }
};

class SemanticError : public std::exception {
    std::string what_str;

public:
    explicit SemanticError(std::string errstr) noexcept :
        what_str(std::move(errstr))
    {
    }

    SemanticError() noexcept : SemanticError("Error in semantics;\n") {}

    [[nodiscard]] const char *what() const noexcept override
    {
        return what_str.c_str();
    }
};

class ArgsError : public std::exception {
    std::string what_str;

public:
    explicit ArgsError(std::string errstr) noexcept :
        what_str(std::move(errstr))
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        return what_str.c_str();
    }
};

class WindowError : public std::exception {
    std::string what_str;

public:
    explicit WindowError(std::string errstr) noexcept :
        what_str(std::move(errstr))
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        return what_str.c_str();
    }
};

class VmError : public std::exception {
    std::string what_str;

public:
    explicit VmError(std::string errstr) noexcept : 
        what_str(std::move(errstr))
    {
    }
};

}  // namespace vino
