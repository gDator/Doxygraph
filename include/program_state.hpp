#ifndef PROGRAMM_STATE_HPP
#define PROGRAMM_STATE_HPP

#include "SFML/Graphics.hpp"
#include "program.hpp"
namespace xgl
{
    namespace program
    {
        
        class ProgramState
        {
            public:
                Program* program;
                virtual void draw(const float dt, sf::RenderWindow &window) = 0;
                virtual void update(const float dt, sf::Time elapsed) = 0;
                virtual void handleInput(sf::RenderWindow &window) = 0;
                virtual void handleKeyboard(sf::Event event) = 0;
                virtual void handleMouse(sf::Event event) = 0;
        };
    }
}

#endif /*PROGRAMM_STATE*/