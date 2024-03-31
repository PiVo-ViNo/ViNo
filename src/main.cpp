/*
 * main.cpp
 *
 * Created on Mon Feb 26 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#include "MainCompiler.hpp"

int main(int argc, char **argv)
{
    m_comp::compilation_main(argc, const_cast<const char **>(argv));
    return 0;
}