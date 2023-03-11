#ifndef VIEW_HANDLER_HPP
#define VIEW_HANDLER_HPP

#include "SFML/Graphics.hpp"
#include <iostream>

namespace sf
{
class ViewHandler
{
    enum class ActionState{NONE, PANNING, SELECTING};
    private: 
        sf::View view;
        bool m_moved = false;
        float ZOOM_MAX = 10;
        float ZOOM_MIN = -10;
        float ZOOM_STEP = 1.25;
        sf::RenderWindow* window;
        float zoom_level = 1;
        sf::Vector2i panning_anchor;
        sf::Vector2f selection_start, selection_end;
        ActionState action_state;
        sf::RectangleShape selection_box;
        void handleKeyboard(sf::Event e);
        void handleMouse(sf::Event e);
        

    public:
        void handleInput(sf::Event e);
        void active();
        bool wasMoved() {if(m_moved) {m_moved = false; return true;} return false;}
        void setRenderWindow(sf::RenderWindow &w);
        void setCenter(sf::Vector2f cenetr);
        
    ViewHandler()
    {

    }
        

};
} //namespace sf

#endif /*VIEW_HANDLER_HPP*/