#ifndef PROGRAM_STATE_TEST_HPP
#define PROGRAM_STATE_TEST_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"
#include "program_state.hpp"
#include "Console.hpp"
#include "ViewHandler.hpp"

#include "backend.hpp"
#include "tree.hpp"
#include "doxygraph_gui.hpp"
#include "FilePicker.hpp"
#include <functional>
#include <mutex>

namespace xgl
{
    namespace program
    {
        class ProgramStateTest : public ProgramState
        {
            private: 
                int error;
            public:
                Program* program;
                ImFontAtlas fonts;
                sf::Texture font_texture;
                unsigned char* pixels;
                int width, height;
                gc::CodeMap cm;
                gc::DrawStyle style;
                gc::FileReader file_reader;
                gc::CppInterpreter interpreter = gc::CppInterpreter(&cm);
                sf::ViewHandler view;
                gc::Gui gui = gc::Gui(&cm, &console, &file_reader, &view);
                
                         
                void draw(const float dt, sf::RenderWindow &window);
                void update(const float dt, sf::Time elapsed);
                void handleInput(sf::RenderWindow &window);
                void handleKeyboard(sf::Event event);
                void handleMouse(sf::Event event);

                ProgramStateTest(Program* p);

                void createTree();
        };
    }
}

#endif /*PROGRAMM_STATE_TEST_HPP*/