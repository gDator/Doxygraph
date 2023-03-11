#ifndef TREE_HPP
#define TREE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <filesystem>
#include <deque>
#include <tuple>
#include <thread>
#include <mutex>

//#include <clang-c/Index.h>
#include "Arrow.hpp"
#include "program.hpp"

//include for doxyapp
#include "dir.h"
#include "doxygen.h"
#include "outputgen.h"
#include "parserintf.h"
#include "classdef.h"
#include "namespacedef.h"
#include "filedef.h"
#include "util.h"
#include "classlist.h"
#include "config.h"
#include "filename.h"
#include "version.h"
#include "tooling.hpp"

namespace gc
{
        //[SECTION] Class Pre-Declaration
    class Node;

    //[SECTION] class enums/enums
    enum class NodeType {NAMESPACE, CLASS, STRUCT, UNION, ENUM, FUNCTION, VARIABLE, TYPEDEF, POINTER, OBJECT, LIBRARY, TEMPLATE, EXTERN, UNKNOWN};
    enum class ReferenceType {CALL, DATATYPE, INHERITANCE};
    enum class ActionState {NONE, PANING, SELECTING, DRAGING};
    
    //[SECTION] Static functions
    static std::string nodeTypeAsString(NodeType t)
    {
        switch (t)
        {
            case NodeType::CLASS: return "CLASS";
            case NodeType::NAMESPACE: return "NAMESPACE";
            case NodeType::ENUM: return "ENUM";
            case NodeType::EXTERN: return "EXTERN";
            case NodeType::FUNCTION: return "FUNCTION";
            case NodeType::LIBRARY: return "LIBRARY";
            case NodeType::OBJECT: return "OBJECT";
            case NodeType::POINTER: return "POINTER";
            case NodeType::STRUCT: return "STRUCT";
            case NodeType::TEMPLATE: return "TEMPLATE";
            case NodeType::VARIABLE: return "VARIABLE";
            case NodeType::UNKNOWN:
            default: return "UNKNOWN";
        }
    }

    static std::string refTypeAsString(ReferenceType t)
    {
        switch(t)
        {
            default:
            case ReferenceType::CALL: return "CALL";
            case ReferenceType::DATATYPE: return "DATATYPE";
            case ReferenceType::INHERITANCE: return "INHERITANCE";
        }
    }

    //[SECTION] Structs
    struct DrawStyle
    {
        std::map<NodeType, sf::Color> colormap;
        std::map<ReferenceType, sf::Color> reference_colormap;
        std::map<NodeType, bool> draw_map;
        std::map<ReferenceType, bool> draw_references;
        sf::Font font;
        float border_size = 3.f;
        int fill_color_alpha = 0;
        float border_distance = 10.f;
        bool grid = true;

        bool admin_mode;
        bool full_name = true;
        bool highlight_selected;
        int highlight_alpha = 128;
        int highlight_depth = 1;
        bool highlight_referneces;
        sf::Color highlight_color = sf::Color::White;

        DrawStyle()
        {
            font.loadFromFile("res/UbuntuMono.ttf");
        }
    };

    class Reference
    {
        private:
            Node* m_from;
            Node* m_to;
            sf::Color m_color;
            sf::Arrow m_arrow;
            ReferenceType m_type;
            bool m_highlighted = false;
            bool m_visible = false;
        public:
            void deleteRef();
            void draw(sf::RenderWindow & window, DrawStyle* s);
            void update(DrawStyle* s); //update with drawstyle (including colors)
            void update();  //update Positions only

            void highlight(int depth, DrawStyle *s);
            void clip();
            void unvisible() {m_visible = false;}
            void visible() {m_visible = true;}
            Reference(Node *s, Node* e, ReferenceType t)
            {
                m_from = s; 
                m_to = e;
                m_type = t;
                m_color = sf::Color::Green;
                m_arrow.update(m_color);
                //if(start->getPosition().x + )
            }
    };


    //[SECTION] Classes
    class Node
    {
        private:
            unsigned int m_id;
            std::string m_name;
            std::string m_scope;
            std::vector<std::tuple<std::string, std::string>> m_information;
            std::vector<Node*> p_childs;
            std::vector<Reference*> p_references; 
            Node* p_parent;
            unsigned int m_child_count = 0;
            NodeType m_type;
            //xgl::graphics::List list;
            bool m_visible = false;
            bool m_highlighted = false;
            sf::RectangleShape m_border;
            sf::Text m_name_text;
            sf::VertexArray m_separator;
            
            
        protected:
            
            void addChild(std::string n, NodeType t, Node* p);
            void addChild(Node* n);
        public:
            unsigned int getID();
            std::string getScope();
            std::string getName();
            Node* getParent();

            void print(int depth);
            void printSubtree(bool last, std::vector<bool> *prefix);
            void draw(sf::RenderWindow &w, DrawStyle* s);
            void clipNodes(const sf::View &s);
            void clipReferences();
            void addInformation(std::string key, std::string info);
            std::vector<std::tuple<std::string, std::string>>* getInformation();
            std::tuple<std::string, std::string> getSingleInformation(std::string key);
            void find(std::string keyword, std::vector<std::tuple<Node*, std::string>> &result);
            void exists(std::vector<std::string>& id, int depth, Node*& reference);
            void exists(unsigned int id, Node*& selected);
            void unvisible() {for(auto& c:p_childs) c->unvisible(); m_visible = false;}
            void visible() {m_visible = true;}
            bool isVisible() const {return m_visible;} 

            void addNodeToTree(std::vector<Node*> &t);
            
            std::vector<Node*> getChilds();
            NodeType getType();
            bool hasChilds();
            void addReference(Node* end, ReferenceType t);
            sf::Vector2f getPosition();
            sf::Vector2f getSize();
            sf::Vector2f getTextboxSize();
            void setPosition(sf::Vector2f p);
            void move(sf::Vector2f d);
            void move(sf::Vector2i d);
            void setSize(sf::Vector2f s);
            bool inBounds(sf::Vector2f refernce);
            Node* highlight(sf::Vector2f reference, int depth, DrawStyle *s);
            bool highlight(int depth, DrawStyle *s);
            bool isHighlighted();
            
            Node* getMostLeftChild();
            Node* getMostRightChild();
            Node* getMostTopChild();
            Node* getMostButtomChild(); 

            sf::FloatRect getDimensions();
            void update(gc::DrawStyle* s);
            void updateReferences(gc::DrawStyle* s);

            struct compareSize
            {
                bool operator() (Node *lhs, Node * rhs) {return lhs->getSize().x <
                                rhs->getSize().x;} 
            };

            struct compareInformation
            {
                bool operator() (std::tuple<std::string, std::string> lhs, std::tuple<std::string, std::string> rhs) 
                {return std::get<0>(lhs).compare(std::get<0>(rhs))>0;}
            };

            Node(std::string n, NodeType t, Node* p) //parent muss angegeben werden 
            {
                ////DEB(lg, trace) << "gc::Node::Node()";
                static unsigned int id = 0;
                m_id = id;
                id++;
                p_parent = p;
                m_type = t;
                m_name = n;
                if(p != nullptr)
                {
                    m_scope = p->getScope() + "::" + p->getName();
                    p->addChild(this);
                }
                else
                {
                    m_scope = "";
                }
                m_name_text.setString(m_name);
                m_name_text.setPosition(0,0);
                m_border.setPosition(0,0); 
                addInformation("Name", m_name);
                addInformation("Parents", p!=nullptr?p->getName():"");
                addInformation("Scope", m_scope);
                m_separator = sf::VertexArray(sf::PrimitiveType::Lines, 2);
            }

            ~Node()
            {
                for(auto& c: p_childs)
                    c->~Node();
                for(auto& r : p_references)
                    r->~Reference();
                delete this;
            }
    };

    class CodeMap
    {
        private:
            Node* m_root; //only one startnode!
            Node* current_node;
            Node* last_added;
            bool sorted = false;
            unsigned int num_nodes = 0;
            std::vector<Node*> all_nodes;
            Node* p_selected_node = nullptr;
            gc::DrawStyle* draw_style = nullptr;
            double progress = 0;
            ActionState action_state = ActionState::NONE;
            sf::Vector2f mouse_start, mouse_end;

            void handleKeyboard(sf::Event e, sf::RenderWindow &window);
            void handleMouse(sf::Event e, sf::RenderWindow &window);

        public:

            std::function<void(std::string)> throwMessage;
            std::function<void(std::string)> throwError; //pointer to function
            std::function<void(std::string)> throwInfo;
            Node* getNodeRef(std::string n); 
            Node* getSelectedNode();
            void selectNode(unsigned int id);
            Node* getLastAddedNode();
            Node* getCurrentNode();
            Node* exists(std::string search);
            
            void addNode(Node* n, bool update_current_node = false);
            bool addNode(std::string name, NodeType t, bool update_current_node = false);
            void setNode(Node* n);
            void setNode(std::string n);
            void goLastNode();
            void goUp();            
            void addReference(std::string s, std::string e, ReferenceType t);
            
            void print();
            void draw(sf::RenderWindow &window);
            void clip(const sf::View &view) {m_root->clipNodes(view); m_root->clipReferences();}
            void update(const float dt, sf::Time elapsed); //?
            void sortAndUpdate(); //Call After everything is added.
            void handleInput(sf::Event e, sf::RenderWindow &window);

            void setDrawStyle(gc::DrawStyle* s);
            DrawStyle* getDrawStyle();

            inline void setProgress(double p) {progress=p;}
            inline double getProgress() {return progress;}

            void find(std::string keyword, std::vector<std::tuple<Node*, std::string>> &result);
            CodeMap()
            {
                //DEB(lg, trace) << "CodeMap::CodeMap()";
                m_root = new Node("", NodeType::NAMESPACE, nullptr);
                current_node = m_root;
                last_added = m_root;
                mouse_start = sf::Vector2f(0,0);
                mouse_end = sf::Vector2f(0,0);
            }

    };
}
#endif /*TREE_HPP*/