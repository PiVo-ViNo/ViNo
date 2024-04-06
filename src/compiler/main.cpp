/*
 * main.cpp
 *
 * Created on Mon Feb 26 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#include "MainCompiler.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    try {
        m_comp::compilation_main(argc, const_cast<const char **>(argv));
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    }
    return 0;
}