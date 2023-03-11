#include <stack>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "program_state.hpp"
#include "program.hpp"

#include <iostream>

Console console("Console", &console_open); //gloabl variable
void cmsg(std::string msg)
{   
    console.printLine(msg);
}

void cmsg(const char* msg)
{   
    console.printLine(msg);
}

namespace xgl
{
namespace program
{

Program::Program()
{
    //DEB(lg, trace) << "Program::Program()";
    this->window.create(sf::VideoMode(1200,800), "Doxygraph");//, sf::Style::Fullscreen);
    //this->window.setIcon(32, 32, )
    this->window.setFramerateLimit(144);
    ImGui::SFML::Init(this->window);
}

Program::~Program()
{
    while(!this->states.empty())
        popState();
}


/*
    @name: pushState
    @func: Add a new GameState
    @args: @state which should ne added 
    @return: none
*/
void Program::pushState(ProgramState* state)
{
    //DEB(lg, trace) << "Program::pushState()";
    this->states.push(state);
    return;
}


/*
    @name: popState
    @func: remove the topmost GameState
    @args: none
    @return: none
*/

void Program::popState()
{
    //DEB(lg, trace) << "Program::popState";
    //delete this->states.top();
    this->states.pop();

    return;
}

/*
    @name: changeState
    @func: Changing the State
    @args: @state is the state to which should be changed 
    @return: none
*/

void Program::changeState(ProgramState* state)
{
    //DEB(lg, trace) << "Program::changeState()";
    if(!this->states.empty())
        popState();
    pushState(state);
    return;
}

/*
    @name: peekState
    @func: Looking at the next state to work with
    @args: none
    @return: the first @GameState
*/

ProgramState* Program::peekState()
{
    //DEB(lg, trace) << "Program::peekState()";
    if(this->states.empty())
        return nullptr;
    return this->states.top();
}

/*
    @name: loop
    @func: MainLoop of the Game; manage all the incoming states
    @args: none
    @return: none
*/
void Program::loop()
{
    //DEB(lg, trace) << "Program::loop()";
    sf::Clock clock;
    sf::Time elapsed;
    
    while(this->window.isOpen())
    {
        elapsed = clock.restart();
        float dt =  elapsed.asSeconds();

        if(peekState() == nullptr)
        {
            std::cout<< "error: no state\n"; 
            break;
        }
        peekState() -> handleInput(this->window);
        peekState() -> update(dt, elapsed);
        ImGui::SFML::Update(this->window, elapsed);
        this->window.clear(sf::Color::Black);
        peekState() -> draw(dt, this->window);
        ImGui::SFML::Render(this->window);
        this->window.display();
        if(dt != 0)
            std::cout << "FPS: " << (int)1/dt << "\r";
    }
}

/*
    @name: init
    @func: initializerfunction
    @args: none
    @return: none
*/
void Program::init()
{
    //DEB(lg, trace) << "Program::init()";
}

sf::RenderWindow& Program::getRenderWindow()
{
    return window;
} 



}
}
