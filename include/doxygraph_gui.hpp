#ifndef GC_FRONTEND
#define GC_FRONTEND

#include <thread>
#include <SFML/Graphics.hpp>
#include "backend.hpp"
#include "imgui_sfml.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <iostream>
#include "Console.hpp"
#include "FilePicker.hpp"
#include "TextEditor.hpp"
#include "ViewHandler.hpp"
#include "tree.hpp"


namespace gc
{
    class Gui
    {
        private:
            bool show_map = true;
            bool show_texteditor = false;
            bool show_console = false;
            bool show_file_explorer = false;
            bool show_settings = false;
            bool show_info_box = false;
            bool show_search = false;
            bool admin_mode = false; //allows moving the tree structure/editing params
            bool fullscreen = false;
         
            std::filesystem::path selected_path;
            CodeMap* map;
            Console* console;
            FileReader* file_reader;
            sf::ViewHandler* p_view_handler;
            TextEditor text_editor;
            const TextEditor::LanguageDefinition &editor_language = TextEditor::LanguageDefinition::CPlusPlus();

            void showMap(sf::RenderWindow &w);
            void showTexteditor(sf::RenderWindow &w);
            void showConsole(sf::RenderWindow &w);
            void showSettings(sf::RenderWindow &w);
            void showInfoBox(sf::RenderWindow &w);
            void showFileExplorer(sf::RenderWindow &w);
            void showSearch(sf::RenderWindow &w);

        public:
            //Callback Varaiables
            std::function<void(std::vector<std::filesystem::path>*)> callback_interpreter;
            std::function<void(std::filesystem::path p)> callback_file_reader;
            std::function<void(std::vector<std::string>)> callback_interpreter_flags;

            void draw(sf::RenderWindow &w);
            FilePicker file_explorer;
            
            Gui(CodeMap* cm, Console* c, FileReader* fr, sf::ViewHandler* view_handler)
            {
                map = cm;
                console = c;
                file_reader = fr;
                console->linkVisibleVaraibale(&show_console);
                sf::Texture file, folder;
                file.loadFromFile("./res/File.png");
                folder.loadFromFile("./res/Folder.png");
                file_explorer.SetDefaultFileIcon(file);
                file_explorer.SetFolderIcon(folder);
                text_editor.SetLanguageDefinition(editor_language);
                p_view_handler = view_handler;
            }

    };
}

#endif /*GC_FRONTEND*/