///////////////////////////////////////////////////////////////////////////////
// vinogui.hpp                  //    /^--^\     /^--^\     /^--^\           //
//                              //    \____/     \____/     \____/           //
// Created on Sat Apr 02 2024   //   /      \   /      \   /      \          //
//                              //  |        | |        | |        |         //
//                              //   \__  __/   \__  __/   \__  __/          //
//                              //|^|^|^\ \^|^|^|^/ /^|^|^|^|^\ \^|^|^|^|^|^|//
//                              //| | | |\ \| | |/ /| | | | | |\ \ | | | | | //
// (c) Andrjusha (aka SibJusha) //| | | |/ / | | \ \| | | | | // /| | | | | |//
//                              //| | | |\/| | | |\/| | | | | |\/ | | | | | |//
///////////////////////////////////////////////////////////////////////////////
///
/// @file Main ViNoGUI library header
///
/// @details Should be included instead of all other headers, because not only
///     it simplier, but also library should be initialized before actual usage
///     of any components, because init_vinogui() sets important callbacks in
///     GLFW. You could do it by your hands, but I recommend you just skip it
///     and use initialization from here.
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Box.hpp"
#include "Fonts.hpp"
#include "Window.hpp"
#include "ImgData.hpp"
#include "Shader.hpp"
#include "custom_errors.hpp"

namespace vino {

static void error_callback(int error, const char* description);

/**
 * @brief Main function for initialization of library
 *
 * @details Should be used before anything
 * @throw vino::WindowError() in case of any error, see what_str()
 */
void init_vinogui();

}  // namespace vino
