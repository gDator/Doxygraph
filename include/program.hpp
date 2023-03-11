#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <stack>
#include <SFML/Graphics.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <iostream>
#include <string>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_sfml.h"
#include "Console.hpp"

#define DEB(msg, lvl)
static bool console_open;
extern Console console; 
void cmsg(std::string msg);
void cmsg(const char* msg);


namespace xgl
{
    namespace program
    {
        class ProgramState;
        class Program
        {
            private:
                
                sf::RenderWindow window;
                std::stack<ProgramState*> states;
            public:
                void init();
                void loop();
                void pushState(ProgramState* state);
                void popState();
                void changeState(ProgramState* state);
                sf::RenderWindow& getRenderWindow();
                ProgramState* peekState();

                Program();
                ~Program();
        };
    }
    
}

#endif /*PROGRAM*/