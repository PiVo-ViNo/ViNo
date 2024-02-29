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
#include <stdexcept>

namespace vino {

class tokenize_error : public std::exception { 
    std::string what_str;
public:
    tokenize_error(std::string errstr) noexcept : what_str(errstr) {}
    
    tokenize_error() noexcept : 
        tokenize_error("Error while tokenizing script file;") 
    {}

    const char* what() const noexcept 
    {
        return what_str.c_str();
    }
};

class null_ptr_exc : public std::exception {
public:

    const char* what() const noexcept
    {
        return "Error: dereferencing nullptr\n";
    }

};

}