#include "program_state_test.hpp"

namespace xgl
{
    namespace program
    {
        /*Konstruktor of Task state*/
        ProgramStateTest::ProgramStateTest(Program* pro)
        {
            //DEB(lg, trace) << "ProgramStateTest::ProgramStateTest()";
            this->program = pro;
            //createTree();
            ImGuiContext* im_context = ImGui::GetCurrentContext();
            if(im_context == nullptr)
                ImGui::CreateContext();
            im_context->IO.Fonts->AddFontFromFileTTF("res/UbuntuMono.ttf", 13.0);
            ImPlot::CreateContext();
            //im_context->IO.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
            
            // font_texture.create(width, height);
            //font_texture.update(pixels);
            //ImTextureID texID = xgl::graphics::convertGLTextureHandleToImTextureID(font_texture.getNativeHandle());
            //im_context->IO.Fonts->SetTexID(texID);
            view.setRenderWindow(this->program->getRenderWindow());
            style.colormap.emplace(gc::NodeType::NAMESPACE, sf::Color::White);
            style.colormap.emplace(gc::NodeType::CLASS, sf::Color(255,39,127));
            style.colormap.emplace(gc::NodeType::STRUCT, sf::Color(96,0,55));
            style.colormap.emplace(gc::NodeType::ENUM, sf::Color(229,166,0));
            style.colormap.emplace(gc::NodeType::FUNCTION, sf::Color(233,255,0));
            style.colormap.emplace(gc::NodeType::VARIABLE, sf::Color(0,221,204));
            style.colormap.emplace(gc::NodeType::POINTER, sf::Color(0,188,52));
            style.colormap.emplace(gc::NodeType::OBJECT, sf::Color(0,224,116));
            style.colormap.emplace(gc::NodeType::LIBRARY, sf::Color(78,75,200));
            style.colormap.emplace(gc::NodeType::TEMPLATE, sf::Color(255,0,255));
            style.colormap.emplace(gc::NodeType::EXTERN, sf::Color(185,224,116));
            style.colormap.emplace(gc::NodeType::UNKNOWN, sf::Color::Red);
            style.reference_colormap.emplace(gc::ReferenceType::DATATYPE, sf::Color(128,128, 255));
            style.reference_colormap.emplace(gc::ReferenceType::CALL, sf::Color(128, 128, 64));
            style.reference_colormap.emplace(gc::ReferenceType::INHERITANCE, sf::Color(255,255,255));
            cm.setDrawStyle(&style);
            style.grid = false;
            style.fill_color_alpha = 1;
            style.border_size = 3;
            console_open = true;
            cm.throwMessage = std::bind(&Console::printLine, &console, std::placeholders::_1);
            cm.throwError = std::bind(&Console::throwError, &console, std::placeholders::_1);
            cm.throwInfo = std::bind(&Console::throwInfo, &console, std::placeholders::_1);
            gui.callback_file_reader = std::bind(&gc::FileReader::fillAllFileData, &file_reader, std::placeholders::_1);
            gui.callback_interpreter = std::bind(&gc::CppInterpreter::transmit_Code, &interpreter, std::placeholders::_1);
            interpreter.setCompilerArguments(" -I ./include");
            cm.print();
        }

        

        /*
            @name: draw
            @func: MainLoop of the Game; manage all the incoming states
            @args: none
            @return: none
        */
        void ProgramStateTest::draw(const float dt, sf::RenderWindow &window)
        {
            //DEB(lg, trace) << "ProgramStateTask::draw()";
            //program->getRenderWindow().clear(xgl::graphics::getXglImguiColors(xgl::graphics::WindowBg));
            view.active();
            if(view.wasMoved())
                cm.clip(window.getView());
            gui.draw(window);
            // ImGui::ShowDemoWindow();
            // ImPlot::ShowDemoWindow();
        }

        /*
            @name: update
            @func: 
            @args: @dt is the time between two update processes
            @return: none
        */
        void ProgramStateTest::update(const float dt, sf::Time elapsed)
        {
            //DEB(lg, trace) << "ProgramStateTask::update()";
        }
        
        /*
            @name: handleInput
            @func: Handle Inputs in Main Menu; Event can be:
                    @Window closed
                    @Window resized
                    @key pressed

            @args: none
            @return: none
        */
        void ProgramStateTest::handleInput(sf::RenderWindow &window)
        {
            //DEB(lg, trace) << "ProgramStateTask::handleInput()";
            sf::Event event;

            //Process all Events
            while(window.pollEvent(event))
            {
                //Redirect to ImGUI
                if(ImGui::GetCurrentContext()->IO.WantCaptureMouse != 0)
                    ImGui::SFML::ProcessEvent(event);
                else
                {
                    //Redirect to CodeMap
                    view.handleInput(event);
                    cm.handleInput(event, window);
                }
                    

                //Process in SFML
                switch(event.type)
                {
                    //Close Window
                    case sf::Event::Closed:
                    {
                        window.close();
                        break;
                    }

                    //Resize Window: View gets same size as window
                    case sf::Event::Resized:
                    {
                        break;
                    }

                    //Keyboard active
                    case sf::Event::KeyPressed:
                    {
                        handleKeyboard(event);
                        break;
                    }
                    
                    default:
                        handleMouse(event);
                        break;
                }
            }
            return;
        }

        /*
            @name: handleKeyboard
            @func: Process all Keyboard Inputs
            @args: @event
            @return: none
        */
        void ProgramStateTest::handleKeyboard(sf::Event event)
        {
            //DEB(lg, trace) << "ProgramStateTask::handleKeyboard()";
            switch(event.type)
            {
                //Close Window
                case sf::Event::Closed:
                {}
            }
        }

        /*
            @name: handleMouse
            @func: Process all Mouse Inputs
            @args: @event
            @return: none
        */
        void ProgramStateTest::handleMouse(sf::Event event)
        {
            //DEB(lg, trace) << "ProgramStateTask::handleMouse()";
        }

        /*
            @name: createTree
            @func: builds the tree, which will be tested 
            @args: 
            @return: 
        */

        void ProgramStateTest::createTree()
        {
            cm.addNode("Player", gc::NodeType::CLASS, true);
            cm.addNode("PlayerColor", gc::NodeType::ENUM);
            cm.addNode("draw(sf::RenderWindow &window, const float dt)", gc::NodeType::FUNCTION);

            cm.addNode("update (float dt)", gc::NodeType::FUNCTION, true);
            cm.addNode("float dt", gc::NodeType::VARIABLE);
            cm.goUp();
            cm.addNode("handleInput()", gc::NodeType::FUNCTION);
            cm.addNode("civilization", gc::NodeType::OBJECT);
            cm.addNode("id", gc::NodeType::VARIABLE);
            cm.addNode("inBounds(sf::Vector2f position, sf::IntRect area, sf::Vector2f reference", gc::NodeType::FUNCTION);
            
            cm.goUp();
            cm.addNode("width_s", gc::NodeType::UNKNOWN);
            cm.addNode("height_s", gc::NodeType::POINTER);
            cm.addNode("worldToMapCoordinates(sf::Vector2f vector)", gc::NodeType::FUNCTION);
            cm.addNode("MapToWorldCoordinates(sf::Vector2i vector)", gc::NodeType::FUNCTION);
            cm.addNode("Map", gc::NodeType::STRUCT, true);
            cm.addNode("tiles", gc::NodeType::VARIABLE);
            cm.addNode("objects", gc::NodeType::OBJECT);
            cm.addNode("load(const std::string &filename, unsigned int width, unsigned height)", gc::NodeType::FUNCTION);
            cm.addNode("save()", gc::NodeType::FUNCTION);
            cm.addNode("generateSeed(const unsigned int width, const unsigned height)", gc::NodeType::FUNCTION);

            cm.addReference("Map", "tiles", gc::ReferenceType::CALL);
            cm.addReference("Map", "objects", gc::ReferenceType::CALL);
            cm.addReference("draw(sf::RenderWindow &window, const float dt)", "worldToMapCoordinates(sf::Vector2f vector)", gc::ReferenceType::CALL);

        }
    }
}