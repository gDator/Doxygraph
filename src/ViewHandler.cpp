#include "ViewHandler.hpp"

namespace sf
{

void ViewHandler::setRenderWindow(sf::RenderWindow &w)
{
    window = &w;
    view.setViewport(sf::FloatRect(0,0,1,1));
    sf::Vector2f pos = sf::Vector2f(window->getSize());
    this->view.setSize(pos);
    pos *= 0.5f;
    this->view.setCenter(pos);
}

void ViewHandler::handleInput(sf::Event e)
{
    if(window == nullptr)
    {
        std::cout << "ERROR::VIEW::NO::REFERNCE_TO_WINDOW";
        return;
    }

    switch(e.type)
    {
        //Resize Window: View gets same size as window
        case sf::Event::Resized:
        {
            this->view.setSize(e.size.width,e.size.height);
            this->view.zoom(zoom_level);
            break;
        }

            //Keyboard active
        case sf::Event::KeyPressed:
        {
            handleKeyboard(e);
            break;
        }
        
        default:
            handleMouse(e);
            break;
    }
    return;
}


void ViewHandler::active()
{
    window->setView(this->view);
} 

void ViewHandler::handleMouse(sf::Event e)
{
    switch(e.type)
    {
        case sf::Event::MouseMoved:
        {
            /*Pan Camera*/
            if(this->action_state == ActionState::PANNING)
            {
                sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*window) - this->panning_anchor);
                this->view.move(-1.0f*pos*this->zoom_level);
                panning_anchor = sf::Mouse::getPosition(*window);
                m_moved = true;
            }
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            /*start panning*/
            if(e.mouseButton.button == sf::Mouse::Middle)
            {
                if(this->action_state != ActionState::PANNING)
                {
                    this->action_state = ActionState::PANNING;
                    this->panning_anchor = sf::Mouse::getPosition(*window);
                }
            }
            else if(e.mouseButton.button ==sf::Mouse::Left)
            {
                /*Select map tile*/
                if(this->action_state != ActionState::SELECTING)
                {
                }
            }
            else if(e.mouseButton.button == sf::Mouse::Right)
            {
                    this->action_state = ActionState::NONE;
            }
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            /*stop panning*/
            if(e.mouseButton.button == sf::Mouse::Middle)
            {
                this->action_state = ActionState::NONE;
            }
            else if(e.mouseButton.button ==sf::Mouse::Left)
            {
                if(this->action_state == ActionState::SELECTING)
                {
                    //Clear selection box 
                    this->action_state = ActionState::NONE;
                    this->selection_start = sf::Vector2f(0,0);
                    this->selection_end = sf::Vector2f(0,0);
                    this->selection_box.setSize(sf::Vector2f(selection_end-selection_start));
                }
            }
            break;
        }

        case sf::Event::MouseWheelScrolled:
        {
            
            if(e.mouseWheel.x < 0) //delta funktioniert nicht
            {
                if(zoom_level < pow(ZOOM_STEP, ZOOM_MAX)) //Begrenzung
                {                    
                    //this->view.setCenter(sf::Vector2f(sf::Mouse::getPosition(this->game->window).x, sf::Mouse::getPosition(this->game->window).y));
                    this->view.zoom(ZOOM_STEP);
                    zoom_level *= ZOOM_STEP;
                    m_moved = true;                    
                }
            }
            else
            {
                if(zoom_level > pow(ZOOM_STEP, ZOOM_MIN))
                {
                    //this->view.setCenter(0.5f*zoom_level*sf::Vector2f(sf::Mouse::getPosition(this->game->window).x, sf::Mouse::getPosition(this->game->window).y));
                    this->view.zoom(1/ZOOM_STEP);
                    zoom_level *= 1/ZOOM_STEP;
                    m_moved = true;                  
                }
            }
            break;
        }
        default:
            break;
    }

}

void ViewHandler::handleKeyboard(sf::Event event)
{
    switch(event.key.code)
    {
        case sf::Keyboard::Escape:
        {
            
        }
        case sf::Keyboard::Space:
        {
            view.setCenter(sf::Vector2f(0,0));
            zoom_level = 1;
            view.zoom(zoom_level);
        }
        default:
            break;
    }

}

void ViewHandler::setCenter(sf::Vector2f center)
{
    view.setCenter(center);
}

} //namespace sf