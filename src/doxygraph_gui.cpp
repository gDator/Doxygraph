#include "doxygraph_gui.hpp"

namespace gc
{

void Gui::draw(sf::RenderWindow &w)
{
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("New Project")) {}
                if(ImGui::MenuItem("Save Project")) {}
                if(ImGui::MenuItem("Load Project")) {}
                ImGui::Separator();
                if(ImGui::BeginMenu("Import"))
                {
                    if(ImGui::MenuItem("Folder Structure")) 
                    {
                        show_file_explorer = true;
                        std::filesystem::path selected_path = std::filesystem::absolute(std::filesystem::current_path());
                        file_reader->reset();
                        file_explorer.PickFolder(std::filesystem::absolute(std::filesystem::current_path()), [&](const std::filesystem::path& selected_path) {file_reader->fillAllFileData(selected_path);});
                        
                    }
                    if(ImGui::MenuItem("Single File")) 
                    {
                        show_file_explorer = true;
                        file_reader->reset();
                        std::filesystem::path selected_path = std::filesystem::absolute(std::filesystem::current_path());
                        file_explorer.PickFile(std::filesystem::absolute(std::filesystem::current_path()), [&](const std::filesystem::path& selected_path) {file_reader->fillAllFileData(selected_path);});
                    }
                    ImGui::EndMenu();
                }
                
                if(ImGui::BeginMenu("Export As"))
                {
                    if(ImGui::MenuItem("PDF")) {}
                    if(ImGui::MenuItem("PNG")) {}
                    if(ImGui::MenuItem("SVG")) {}
                    if(ImGui::MenuItem("XML")) {}
                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Edit"))
            {
                if(ImGui::MenuItem("Search")) {show_search = true;}
                if(ImGui::MenuItem("File Extensions")) {}
                if(ImGui::MenuItem("Admin Mode", NULL ,&map->getDrawStyle()->admin_mode)) {}
                if(ImGui::MenuItem("Compile")) {callback_interpreter(file_reader->getData());}
                if(ImGui::MenuItem("Simulate")) {}
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View"))
            {
                if(ImGui::MenuItem("Map", NULL, &show_map)) {show_map = true;}
                if(ImGui::MenuItem("Texteditor", NULL, show_texteditor)) {show_texteditor = true;}
                if(ImGui::MenuItem("Console", NULL, &show_console)) {show_console = true;}
                if(ImGui::MenuItem("Settings", NULL, &show_settings)) {show_settings = true;}
                if(ImGui::MenuItem("Info Box", NULL, &show_info_box)) {show_info_box = true;}
                if(ImGui::MenuItem("Childs Box", NULL, &show_info_box)) {show_info_box = true;}
                if(ImGui::MenuItem("References Box", NULL, &show_info_box)) {show_info_box = true;}
                if(ImGui::BeginMenu("Statistics")) 
                {
                    if(ImGui::MenuItem("File Statistics")) {}
                    if(ImGui::MenuItem("Language Statistics")) {}
                    ImGui::EndMenu();
                }

                if(ImGui::BeginMenu("Appearance")) 
                {
                    if(ImGui::MenuItem("Code Map")) {}
                    if(ImGui::MenuItem("Code Tree")) {}
                    if(ImGui::MenuItem("Code Structure")) {} //open extra window?
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Options"))
            {
                if(ImGui::MenuItem("Fullscreen", NULL, &fullscreen)) 
                {
                    if(fullscreen)
                    {
                        w.create(sf::VideoMode(1000,1000), "XG TaskManager", sf::Style::Fullscreen);
                    }                        
                    else
                    {
                        w.create(sf::VideoMode(1200,800), "XG TaskManager");
                    }
                    w.setFramerateLimit(144);
                }
                if(ImGui::MenuItem("Design")) {}
                if(ImGui::MenuItem("API Documentation")) {}
                ImGui::Separator();
                if(ImGui::MenuItem("About")) {}
                ImGui::EndMenu();
            }
            
           
            ImGui::EndMainMenuBar();
           
        } 
        if(ImGui::Begin("Progress", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_MenuBar |ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::ProgressBar(map->getProgress(), sf::Vector2f(100.0f, 20.0f));
            ImGui::End();
        }

        if(show_map)
            showMap(w);
        if(show_texteditor)
            showTexteditor(w);
        if(show_console)
            showConsole(w);
        if(show_settings)
            showSettings(w);
        if(show_info_box)
            showInfoBox(w);
        if(show_search)
            showSearch(w);
        if(!show_file_explorer)
        {
            file_explorer.Hide();
        }   

        file_explorer.Update();    
}

void Gui::showMap(sf::RenderWindow &w)
{
    map->draw(w);
}
void Gui::showTexteditor(sf::RenderWindow &w)
{
    static Node* last = nullptr;
    if(map->getSelectedNode() != nullptr)
    {
        if(last != map->getSelectedNode())
        {
            last = map->getSelectedNode();
            std::cout << "Node: " << last->getName()<< std::endl;
            
            auto path_string = std::get<1>(map->getSelectedNode()->getSingleInformation("File"));
            auto line = std::stoi(std::get<1>(map->getSelectedNode()->getSingleInformation("Line")));
            std::cout << "Path: " << path_string << std::endl;
            std::filesystem::path path(path_string);
            if(std::filesystem::exists(path))
            {
                try
                {
                    std::ifstream file(path);
                    // file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
                    std::vector<std::string> data;
                    std::string puffer;
                    if(file.is_open())
                    {
                        while(file.good())
                        {
                            std::getline(file, puffer);
                            data.push_back(puffer);
                        }
                        file.close();
                    }
                    text_editor.SetTextLines(data);
                    if(line < data.size())
                        text_editor.SetCursorPosition(TextEditor::Coordinates(line - 1, 0));
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }                
            }
        }
    }
    
      
    auto cpos = text_editor.GetCursorPosition();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_MenuBar;
    if(ImGui::Begin("Texteditor", &show_texteditor, window_flags))
    {
        if (ImGui::BeginMenuBar())
        {   
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    auto textToSave = text_editor.GetText();
                    /// save text....
                }
                if (ImGui::MenuItem("Quit", "Alt-F4"))
                    show_texteditor = false;
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Edit"))
            {
                bool ro = text_editor.IsReadOnly();
                if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                    text_editor.SetReadOnly(ro);
                ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && text_editor.CanUndo()))
                    text_editor.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && text_editor.CanRedo()))
                    text_editor.Redo();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, text_editor.HasSelection()))
                    text_editor.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && text_editor.HasSelection()))
                    text_editor.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && text_editor.HasSelection()))
                    text_editor.Delete();
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                    text_editor.Paste();

                ImGui::Separator();

                if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    text_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(text_editor.GetTotalLines(), 0));

                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Dark palette"))
                    text_editor.SetPalette(TextEditor::GetDarkPalette());
                if (ImGui::MenuItem("Light palette"))
                    text_editor.SetPalette(TextEditor::GetLightPalette());
                if (ImGui::MenuItem("Retro blue palette"))
                    text_editor.SetPalette(TextEditor::GetRetroBluePalette());
                ImGui::EndMenu();
            }
           ImGui::EndMenuBar();
        }
        
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, text_editor.GetTotalLines(),
        text_editor.IsOverwrite() ? "Ovr" : "Ins",
        text_editor.CanUndo() ? "*" : " ",
        text_editor.GetLanguageDefinition().mName.c_str(), "fileToEdit");
        text_editor.Render("TextEditor");
    }
    ImGui::End();
}
void Gui::showConsole(sf::RenderWindow &w)
{
    console->draw();
}
void Gui::showSettings(sf::RenderWindow &w)
{
    if(ImGui::Begin("Settings", &show_settings))
    {
        if(ImGui::Button("Update"))
            map->sortAndUpdate();
        ImGui::Checkbox("Highlight Selected", &(map->getDrawStyle()->highlight_selected));
        ImGui::SliderInt("Highlight Alpha", &(map->getDrawStyle()->highlight_alpha), 1.f, 255);
        ImGui::InputInt("Highlight Depth", &map->getDrawStyle()->highlight_depth);
        if(ImGui::Checkbox("Show Full Name", &map->getDrawStyle()->full_name)){map->sortAndUpdate();}
        ImGui::Checkbox("Namespace", &map->getDrawStyle()->draw_map[NodeType::NAMESPACE]);
        ImGui::Checkbox("Class", &map->getDrawStyle()->draw_map[NodeType::CLASS]);
        ImGui::Checkbox("Struct", &map->getDrawStyle()->draw_map[NodeType::STRUCT]);
        ImGui::Checkbox("Enum", &map->getDrawStyle()->draw_map[NodeType::ENUM]);
        ImGui::Checkbox("Function", &map->getDrawStyle()->draw_map[NodeType::FUNCTION]);
        ImGui::Checkbox("Variable", &map->getDrawStyle()->draw_map[NodeType::VARIABLE]);
        ImGui::Checkbox("Object", &map->getDrawStyle()->draw_map[NodeType::OBJECT]);
        ImGui::Checkbox("Pointer", &map->getDrawStyle()->draw_map[NodeType::POINTER]);
        ImGui::Checkbox("Unknown", &map->getDrawStyle()->draw_map[NodeType::UNKNOWN]);
        ImGui::Checkbox("Call", &map->getDrawStyle()->draw_references[ReferenceType::CALL]);
        ImGui::Checkbox("Inheritance", &map->getDrawStyle()->draw_references[ReferenceType::INHERITANCE]);
        ImGui::Checkbox("Datatype", &map->getDrawStyle()->draw_references[ReferenceType::DATATYPE]);
    }
    ImGui::End();
}
void Gui::showInfoBox(sf::RenderWindow &w)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    if(ImGui::Begin("Information", &show_info_box, window_flags))
    {
        static ImGuiTableFlags table_flags =
            ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
            | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
            //| ImGuiTableFlags_SizingFixedFit;
        std::vector<std::tuple<std::string, std::string>>* content;
        if(map->getDrawStyle()->admin_mode)
        {
            std::string id, value;
            ImGui::InputText("ID", &id);
            ImGui::SameLine();
            ImGui::InputText("Value", &value);
            ImGui::SameLine();
            if(ImGui::Button("Add"))
                content->push_back(std::make_tuple(id, value));
        }
        if(ImGui::BeginTable("information", 2, table_flags))
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_NoSort);
            ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
            ImGui::TableHeadersRow();

            if(map->getSelectedNode() != nullptr)
            {
                
                content = map->getSelectedNode()->getInformation();
                if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
                if (sorts_specs->SpecsDirty)
                {
                    //order of sorted list: forward or backward
                    if (content->size() > 1)
                        std::reverse(content->begin(), content->end());
                    sorts_specs->SpecsDirty = false;
                }
                
                ImGuiListClipper clipper;
                clipper.Begin(content->size());
                while (clipper.Step())
                    for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                    {
                        // Display a data item
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text(std::get<0>(content->at(row_n)).c_str());
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(std::get<1>(content->at(row_n)).c_str());
                    }
            }
            
            ImGui::EndTable();
        }
        ImGui::End();
    }
    
}

void Gui::showSearch(sf::RenderWindow &w)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    if(ImGui::Begin("Search", &show_search, window_flags))
    {
        static ImGuiTableFlags table_flags =
            ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
            | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
            //| ImGuiTableFlags_SizingFixedFit;
        static std::vector<std::tuple<gc::Node*, std::string>> content;
        static std::string search_value;
        static ImVector<unsigned int> selection;
        static std::string status = ""; 
        ImGui::InputText("Search_text", &search_value);
        ImGui::SameLine();
        if(ImGui::Button("Search"))
        {
            content.resize(0);
            selection.resize(0);
            status = "Searching...";
            map->find(search_value, content); //make extra thread
            status = "Ready...";
        }
        ImGui::SameLine();
        if(ImGui::Button("Go to"))
        {
            if(selection.size()>0) //somthing is selected
            {
                map->selectNode(selection[0]);
                if(map->getSelectedNode() != nullptr)
                {
                    p_view_handler->setCenter(map->getSelectedNode()->getPosition());
                }
                    
            }
        }
        ImGui::SameLine();
        ImGui::Text(status.c_str());
        
        if(ImGui::BeginTable("search_results", 3, table_flags))
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_NoSort);
            ImGui::TableSetupColumn("Node", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Search", ImGuiTableColumnFlags_NoSort);
            
            ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
            ImGui::TableHeadersRow();

            if(!content.empty())
            {
                // if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
                // if (sorts_specs->SpecsDirty)
                // {
                //     //order of sorted list: forward or backward
                //     if (content->size() > 1)
                //         std::reverse(content->begin(), content->end());
                //     sorts_specs->SpecsDirty = false;
                // }
                
                ImGuiListClipper clipper;
                clipper.Begin(content.size());
                while (clipper.Step())
                    for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                    {   
                        unsigned int id = std::get<0>(content.at(row_n))->getID();
                        bool selected = selection.contains(id);
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        //ImGui::Text(std::get<0>(content.at(row_n))->getName().c_str());                     
                        if (ImGui::Selectable(std::to_string(id).c_str(), selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, 0)))
                        {
                            if (ImGui::GetIO().KeyCtrl)
                            {
                                if (selected)
                                    selection.find_erase_unsorted(id);
                                else
                                    selection.push_back(id);
                            }
                            else
                            {
                                selection.clear();
                                selection.push_back(id);
                            }
                        }
                        
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(std::get<0>(content.at(row_n))->getName().c_str());
                        ImGui::TableSetColumnIndex(2);
                        ImGui::TextUnformatted(std::get<1>(content.at(row_n)).c_str());
                        
                    }
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }
    
}

}
