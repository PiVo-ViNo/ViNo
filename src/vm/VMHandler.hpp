/*
 * VMHandler.hpp
 *
 * Created on Sun Apr 07 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once

#include <gui/Box.hpp>
#include <gui/Window.hpp>
namespace vm_main {

struct ViNoWindowComponents {
    vino::Window *window = nullptr;
    vino::FullscreenTexture *fs_texture = nullptr;
    vino::LowBox<char32_t> *low_box = nullptr;
};

class IHandler {
public:
    virtual ~IHandler() = default;
    virtual void handle_instruction() = 0;
};

class NopHandler : public IHandler {
public:
    void handle_instruction() override {}
};

class StartHandler : public IHandler {
public:
    void handle_instruction() override
    {
        // ... init values, etc.
    }
};

class ExitHandler : public IHandler {
public:
    void handle_instruction() override
    {

    }
};

class LoadBgHandler : public IHandler {
public:
    void handle_instruction() override
    {

    }

};

class ClearBgHandler : public IHandler {
public:
    void handle_instruction() override
    {

    }

};

class LoadFgHandler : public IHandler {
public:
    void handle_instruction() override
    {

    }

};

class LoadTxtLineHandler : public IHandler {
public:
    void handle_instruction() override
    {

    }

};

class JmpHandler : public IHandler {
public:
    void handle_instruction() override
    {

    }
};

} // namespace vm_main