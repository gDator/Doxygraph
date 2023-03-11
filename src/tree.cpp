#include "tree.hpp"

namespace gc
{

/*
    @name: getName()
    @func: returns the full name of a Node
    @args: none
    @return: name as std::string 
*/

unsigned int Node::getID()
{
    return m_id;
}

std::string Node::getName()
{
    return m_name;
}

/*
    @name: getType()
    @func: returns the type of the Node
    @args: none
    @return: @type as gc::NodeType 
*/
NodeType Node::getType()
{
    return m_type;
}

/*
    @name: addInformation()
    @func: adding an information entry
    @args: @key is the Keyname, @info is the actual content
    @return: none 
*/
void Node::addInformation(std::string key, std::string info)
{   
    m_information.push_back(std::make_tuple(key, info));
}
/*
    @name: getInformation()
    @func: gives a pointer to the information vector
    @args: none
    @return: @&information as std::vector<std::tuple<std::string, std::string>>*
*/
std::vector<std::tuple<std::string, std::string>>* Node::getInformation()
{
    return &m_information;
}

/*
    @name: getSingleInformation()
    @func: gives a pointer to a single information tuple
    @args: @keys is the key of the information
    @return: @&information as std::tuple<std::string, std::string>*
*/
std::tuple<std::string, std::string> Node::getSingleInformation(std::string key)
{
    for(auto i : m_information)
    {
        if(std::get<0>(i).compare(key) == 0)
        {
            std::cout << "Information found" << std::endl;
            return i;
        }
    }
    std::cout << "No Information found" << std::endl;
    return std::make_tuple("", "0");
}

/*
    @name: find()
    @func: finds substrings in a node and add it to a result vector. a node could be added twice
    @args: @keyword is the substring you search for, @result is the resulting vector
    @return: @&information as std::vector<std::tuple<std::string, std::string>>*
*/
void  Node::find(std::string keyword, std::vector<std::tuple<Node*, std::string>> &result)
{   for(auto c : p_childs)
    {
        c->find(keyword, result);
    }

    for(auto i : m_information)
    {
        if(std::get<0>(i).find(keyword) < keyword.size() - 1 || std::get<1>(i).find(keyword) < keyword.size() - 1)
        {
            result.push_back(std::make_tuple(this, std::get<1>(i)));
            return; //make node only be added once
        }
            
    }   
}

/*
    @name: exists()
    @func: finds Node by id and let it point to selected Node
    @args: @kid is search id, @selected is the pointer to selection node
    @return: @&information as std::vector<std::tuple<std::string, std::string>>*
*/
void Node::exists(unsigned int id, Node*& selected) 
{
    //reference to pointer:  You would want to pass a pointer by reference if you have 
    //a need to modify the pointer rather than the object that the pointer is pointing to.
    if(id == m_id)
    {
        selected = this;
        return;
    }
    for(auto& c: p_childs)
    {
        c->exists(id, selected);
        // if(selected != nullptr)
        //     return;
    }
        
}

/*
    \name: exists()
    \brief: finds Node by id and let it point to selected Node
    \args: @kid is search id, @selected is the pointer to selection node
    \return: none
*/
void Node::exists(std::vector<std::string>& id, int depth, Node*& reference)
{
     std::cout << "Search: ";
    for(auto& i : id)
        std::cout << i << "::";
    std::cout << ", but this is " << m_scope << "::" << m_name << std::endl;
    if(depth > id.size())
        return;
    if(id.at(depth).compare(m_name) == 0)
    {
        
        if(depth == id.size() - 1) //this node
        {
            reference = this;
            std::cout << "found: " << id.at(depth) << std::endl;
            return;
        }
        else //one of its childs
        {
            std::cout << "go deeper" << std::endl;
            for(auto& c : p_childs)
            {
                c->exists(id, ++depth, reference);
                // if (reference != nullptr)
                //     return;
            }
        }        
    }

        
}

/*
    @name: getScope
    @func: returning the Name of the Node without its parents names
    @args: none
    @return: @print_name as std::string 
*/
std::string Node::getScope()
{
    return m_scope;
}

/*
    @name: print()
    @func: printing the Code Structure in Console. recursive function
    @args: @depth is the current depth of the node
    @return: none
*/
void Node::print(int depth)
{
    //print own entry
    for(int i = 0; i < depth; i++) //indent
    {
        std::cout << "\t";
    }
    if(depth != 0)
        std::cout << "|-" << std::endl; //tree structure
    std::cout << getScope() << std::endl;

    if(!p_childs.empty()) //print childs
    {
        for(auto c : p_childs)
        {
            print(depth+1);
        }
    }
}

/*
    @name: printSubtree()
    @func: recursive function to print the subtree of a node
    @args: @last is a bool to define if the current node is 
            the last of its depth. @prefix stores information to
            draw higher tree above its subtrees
    @return: none
*/
void Node::printSubtree(bool last, std::vector<bool> *prefix)
{
    //print prefix
    if(!prefix->empty())
    {
        for(int i = 0; i <= prefix->size()-1; i++)
        {
            if(prefix->at(i))
                std::cout << "|\t";
            else
                std::cout << "\t";
        }
    }
	
	if (last)
		std::cout << "L " + m_name;
	else
		std::cout << "|- " << m_name;	//Mittelbaum

	std::cout << std::endl; //Neue Zeile

	if (p_childs.empty())
		return;

	prefix->push_back(!last);
	for (int i = 0; i < p_childs.size(); i++)
	{
        if(p_childs[i] != nullptr)
        {
            if(i == p_childs.size() - 1)
            {
                
                    p_childs[i]->printSubtree(true, prefix);
                    prefix->pop_back();
            }
            else
                p_childs[i]->printSubtree(false, prefix);
        }
	}
}

/*
    @name: addNodeToTree()
    @func: adding this Node to a vector
    @args: @t is the name of the tree-vector
    @return: none
*/
void Node::addNodeToTree(std::vector<Node*> &t)
{
    t.push_back(this);
    for(auto c: p_childs)
    {
        c->addNodeToTree(t);
    }
}

/*
    @name: addChild()
    @func: adding a child node to this node
    @args: @n is name of the , @t type of child's node, @p pointer to parent (this)
    @return: none
*/
void Node::addChild(std::string n, NodeType t, Node* p)
{
    addChild(new Node(n, t, p));
}

/*
    @name: addChild()
    @func: adding a child node to this node
    @args: @n is a pointer to the child node
    @return: none
*/
void Node::addChild(Node* n)
{
    p_childs.push_back(n);
    // references.push_back(new Reference(this, n, ReferenceType::CHILD));
    addInformation("Child " + std::to_string(p_childs.size()), n->getName());
}

/*
    @name: addReference()
    @func: adding a reference from this node to another node
    @args: @end is reference to this node, @t is refernece type
    @return: none
*/
void Node::addReference(Node* end, ReferenceType t)
{
    p_references.push_back(new Reference(this, end, t));
    addInformation("Reference " + std::to_string(p_references.size()) + " to", end->getName());
    end->addInformation(refTypeAsString(t) + " from", m_scope + "::" + m_name); 
}

/*
    @name: getParent()
    @func: getting pointer to parent node
    @args: none
    @return: @parent is the pointer
*/
Node* Node::getParent ()
{
    return p_parent;
}

/*
    @name: getChilds()
    @func: getting the vector of childs 
    @args: none
    @return: childs
*/
std::vector<Node*> Node::getChilds()
{
    return p_childs;
}


/*
    @name: hasChilds()
    @func: check if this node has child nodes
    @args: none
    @return: @bool
*/
bool Node::hasChilds()
{
    return !p_childs.empty();
}


/*
    @name: getPosition()
    @func: get the position of the DrawObject
    @args: none
    @return: @sf::Vector2f is the float vector position
*/
sf::Vector2f Node::getPosition()
{
    return m_border.getPosition();
}


/*
    @name: getSize()
    @func: getSize of the node's draw object
    @args: none
    @return: @sf::Vector2f is the size vector
*/
sf::Vector2f Node::getSize()
{
    return m_border.getSize();
}


/*
    @name: update()
    @func: everytime somthing changed in the node (e.g. name, childs) the 
            draw objects have to be rearranged (recursive)
    @args: @s is the Drawstyle, how the nodes appear
    @return: none
*/
void Node::update(gc::DrawStyle* s)
{
    //sort information
    std::sort(m_information.begin(), m_information.end(), compareInformation());
    //DEB(lg, trace) << "Node::update()";
    if(s->full_name)
        m_name_text.setString(m_scope + "::" + m_name);
    else
        m_name_text.setString(m_name); 
    //RESET
    m_border.setPosition(0,0);
    m_name_text.setPosition(0,0);
    //update rectangle size! and sort childs in
    sf::Color c = s->colormap[this->m_type];
    m_name_text.setFont(s->font);
    m_name_text.setFillColor(c);
    m_border.setOutlineColor(c);
    m_border.setOutlineThickness(s->border_size);
    m_border.setFillColor(sf::Color(c.r, c.g, c.b, s->fill_color_alpha));
    //first resize the smallest elements
    if(p_childs.empty())
    {   
        //make frame a little bigger then 
        m_border.setSize(sf::Vector2f(m_name_text.getLocalBounds().width + 2*s->border_distance, m_name_text.getLocalBounds().height + 2*s->border_distance));
        //border.setOrigin(border.getSize()/2.f);
        //_name.setOrigin(sf::Vector2f(_name.getLocalBounds().width/2, _name.getLocalBounds().height/2));
    }
    else
    {
        for(auto c : p_childs)
        {   
            c->update(s); 
            // std::cout << name << ":: " << c->getName() <<" | x: " << c->getPosition().x << ", y: " << c->getPosition().y << std::endl;           
        }
        std::sort(p_childs.begin(), p_childs.end(), compareSize());
        
        //positioning all the small elements
        int grid_width;
        if(m_type == NodeType::NAMESPACE)//if(s->grid) //Gitteranordnung
            grid_width = (int) sqrt(p_childs.size());
        else 
            grid_width = 1;
        //if(childs.size() > 1)
        {
            //std::cout << "Moving smallest childs\n";
            int j = 0;
            float delta_y = 0, delta_x = 0, max_delta_y = 0;
            for(int i = 0; i < p_childs.size(); i++)
            {
                if(i%grid_width == 0)
                {
                    delta_y += max_delta_y + 2*s->border_distance; //y Coordinate grows with each row
                    max_delta_y = 0; //maximum distance to move resets every row
                    delta_x = 0; //dont move first element of a row
                }
                if(max_delta_y < p_childs[i]->getSize().y)
                    max_delta_y = p_childs[i]->getSize().y;

                p_childs[i]->move(sf::Vector2f(delta_x, delta_y));
                // std::cout << name << ":: " << i << " : "<< childs[i]->getName() <<" | x: " << delta_x << ", y: " << delta_y << std::endl;
                delta_x += p_childs[i]->getSize().x+ 2*s->border_distance; //setting delta x for next entry
            }
        }
        m_name_text.move(sf::Vector2f(s->border_distance, s->border_distance));
        for(auto c: p_childs)
        {
            if(c != nullptr)
            {
                c->move(sf::Vector2f(s->border_distance, m_name_text.getLocalBounds().height + 3*s->border_distance));
            }
        }
        //calculating new size
        m_border.setSize(sf::Vector2f(getDimensions().width + 2*s->border_distance,
                                        getDimensions().height + m_name_text.getLocalBounds().height + 6*s->border_distance));
        //border.setPosition(_name.getPosition() - sf::Vector2f(s->border_distance, s->border_distance));

        //move the childs to get border size of parent element
        
    }

    //calculate Separator
    if(!p_childs.empty())
    {
        m_separator[0].color = m_border.getOutlineColor();
        m_separator[1].color = m_border.getOutlineColor();
        float border_distance = m_name_text.getGlobalBounds().top - m_border.getPosition().y; 
        m_separator[0].position = sf::Vector2f(m_border.getPosition().x, m_border.getPosition().y + 2* border_distance + m_name_text.getLocalBounds().height);
        m_separator[1].position = sf::Vector2f(m_border.getPosition().x + m_border.getSize().x, m_border.getPosition().y + 2* border_distance + m_name_text.getLocalBounds().height);
    }
}

void Node::updateReferences(DrawStyle *s)
{
    for(auto r : p_references)
    {
        r->update(s);
    }
}

/*
    @name: getTextboxSize()
    @func: returning the size of the text content
    @args: none
    @return: @sf::Vector2f is the size vector
*/
sf::Vector2f Node::getTextboxSize()
{
    return sf::Vector2f(m_name_text.getLocalBounds().width, m_name_text.getLocalBounds().height);
}

/*
    @name: draw()
    @func: draw this node its childs and its references
    @args: @&window is Reference to render target, @style is the appearance of the noces and referenecs
    @return: none
*/
void Node::draw(sf::RenderWindow &window, DrawStyle* style)
{
 
    if(m_visible)
    {
        if(!style->draw_map[m_type]) //not visible
            return;
        //std::cout << name << ": " << border.getPosition().x << ", " << border.getPosition().y << std::endl;
        window.draw(m_name_text);
        window.draw(m_border);
        if(!p_childs.empty())
            window.draw(m_separator);
        for(auto& c : p_childs)
        {
                c->draw(window, style);
        }
        
    }
    for(auto& r: p_references)
    {
        r->draw(window, style);
    }
    
}

void Node::clipReferences()
{
    for(auto& r : p_references)
    {
        if(m_visible == true)
            r->visible();
        else
            r->clip();
    }
    for(auto& c: p_childs)
        c->clipReferences();
}

void Node::clipNodes(const sf::View &s)
{

    sf::Vector2f top_left = m_border.getPosition(), right_bottom = m_border.getPosition() + m_border.getSize();
    auto view_top_left = s.getCenter() - s.getSize()/2.f;
    auto view_bottom_right = s.getCenter() + s.getSize()/2.f;
    visible();
    if(right_bottom.x < view_top_left.x || top_left.x > view_bottom_right.x || top_left.y > view_bottom_right.y || right_bottom.y < view_top_left.y)
    {
        // std::cout << "Clipping " << m_name <<":(" << right_bottom.x << ", " << right_bottom.y << ")|(" << top_left.x <<","<< top_left.y<<") to (" <<
        // view_bottom_right.x << ", " << view_bottom_right.y << ")|(" << view_top_left.x <<","<< view_top_left.y<<")" << std::endl;
        unvisible();
    }    
    
    
    for(auto& c: p_childs)
    {
        if(m_visible == true)
            c->clipNodes(s);
        else
            c->unvisible();
    }

    
}

/*
    @name: setPosition()
    @func: renewing the position of the node
    @args: @p is the new position
    @return: none
*/
void Node::setPosition(sf::Vector2f p)
{
    m_border.setPosition(p);
    m_name_text.setPosition(p );//+ sf::Vector2f(20, 0));
}

/*
    @name: move()
    @func: setting the position relative to the old position
    @args: @d is the difference vector as float vector
    @return: none
*/
void Node::move(sf::Vector2f d)
{
    for(auto c: p_childs)
    {
        c->move(d);
    }
    m_name_text.move(d);
    m_border.move(d);
    for(auto r : p_references)
    {
        r->update();
    }
}

/*
    @name: move()
    @func: setting the position relative to the old position
    @args: @d is difference vector as integer vector
    @return: none
*/
void Node::move(sf::Vector2i d)
{
    move(sf::Vector2f(d.x, d.y));
}

/*
    @name: setSize()
    @func: set Size of the border of the draw object
    @args: @s is the new size
    @return: none
*/
void Node::setSize(sf::Vector2f s)
{
    m_border.setSize(s);
   
}

/*
    @name: getMostLeftChild()
    @func: if the childs are ordered get the most left child of the node
    @args: none
    @return: @Node* is the pointer to the child
*/
Node* Node::getMostLeftChild()
{
    Node* obj = nullptr;
    if(!p_childs.empty())
        obj = p_childs[0];
    for(auto n : p_childs)
    {
        if (obj != nullptr)
        {
            if(n->getPosition().x < obj->getPosition().x)
                obj = n;
        }
    }
    return obj;
    
}

/*
    @name: getMostRightChild()
    @func: if the childs are ordered get the most right child of the node
    @args: none
    @return: @Node* is the pointer to the child
*/
Node* Node::getMostRightChild()
{
    Node* obj = nullptr;
    if(!p_childs.empty())
        obj = p_childs[0];
    for(auto n : p_childs)
    {
        if (obj != nullptr)
        {
            if((n->getPosition().x + n->getSize().x) > (obj->getPosition().x + obj->getSize().x))
                obj = n;
        }
    }
    return obj;
}

/*
    @name: getMostTopChild()
    @func: if the childs are ordered get the most top child of the node
    @args: none
    @return: @Node* is the pointer to the child
*/
Node* Node::getMostTopChild()
{
    Node* obj = nullptr;
    if(!p_childs.empty())
        obj = p_childs[0];
    for(auto n : p_childs)
    {
        if (obj != nullptr)
        {
            if(n->getPosition().y < obj->getPosition().y)
                obj = n;
        }
    }
    return obj;
}

/*
    @name: getMostButtomChild()
    @func: if the childs are ordered get the most buttom child of the node
    @args: none
    @return: @Node* is the pointer to the child
*/
Node* Node::getMostButtomChild()
{
    Node* obj = nullptr;
    if(!p_childs.empty())
        obj = p_childs[0];    
    for(auto n : p_childs)
    {
        if (obj != nullptr)
        {
            if((n->getPosition().y + n->getSize().y) > (obj->getPosition().y + obj->getSize().y))
                obj = n;
        }
    }
    return obj;
}

/*
    @name: getDimensions()
    @func: how big is the Node when all childs are arranged
    @args: none
    @return: @FloatRect is the Size and Position of the node draw object
*/
sf::FloatRect Node::getDimensions()
{
    sf::FloatRect r;
    if(p_childs.empty())
    {
        return m_border.getGlobalBounds();
    }
    else
    {
        r.top = getMostTopChild()->getPosition().y;
        r.left = getMostLeftChild()->getPosition().x;
        r.width = getMostRightChild()->getPosition().x + getMostRightChild()->getSize().x - getMostLeftChild()->getPosition().x; 
        r.height = getMostButtomChild()->getPosition().y + getMostButtomChild()->getSize().y - getMostTopChild()->getPosition().y;
        r.width = r.width >= m_name_text.getGlobalBounds().width ? r.width : m_name_text.getGlobalBounds().width; //check if Text is wider
        return r;
    }
}

/*
    @name: inBounds()
    @func: is a point in bounds of the node's draw object
    @args: @reference is the point to compare to
    @return: @bool if its in range
*/
bool Node::inBounds(sf::Vector2f refernce)
{
    auto position = m_border.getPosition();
    auto area = m_border.getLocalBounds();
    if(refernce.x > position.x && refernce.x < position.x + area.width &&
        refernce.y > position.y && refernce.y < position.y + area.height)
    {
        return true;
    }
    else
    {
        return false;
    }    
}

/*
    @name: highlight()
    @func: highlight this Node
    @args: @reference is the point to check if this node is meant, @depth means how deep to highlight childs
    @return: @bool returns if the child has been highlighted
    @note: does not work
*/
Node* Node::highlight(sf::Vector2f reference, int depth, DrawStyle *s)
{   
    for(auto c : p_childs)
    {
        Node* temp = c->highlight(reference, depth, s);
        if(temp != nullptr)
        {
            return temp;
        }            
    }
    if(inBounds(reference))
    {
        highlight(depth, s);
        return this;        
    } 
    else
    {
        return nullptr;
    }   
}

/*
    @name: highlight()
    @func: highlight this Node
    @args: @depth means how deep to highlight refernces
    @return: @bool returns if the child has been highlighted
*/
bool Node::highlight(int depth, DrawStyle *s)
{
    if(depth > 0)
    {
        m_highlighted = true;
        auto color = s->highlight_color;
        m_border.setOutlineColor(s->highlight_color);
        m_name_text.setFillColor(s->highlight_color);
    
    }
        
    if(depth < 0)
    {
        m_highlighted = false;
        for(auto c: p_childs)
        {
            c->highlight(-1, s);

        }

        auto color = s->colormap[m_type];
        color.a = s->highlight_alpha;
        m_border.setOutlineColor(color);
        m_name_text.setFillColor(color);
    }
    if(depth > 1)
    {
        for(auto r: p_references)
        {
            r->highlight(depth, s);
        }
    }
    return 0;
}

/*
    @name: isHighlight()
    @func: is this node highlighted
    @args: none
    @return: @bool returns if is highlighted
*/
bool Node::isHighlighted()
{
    return m_highlighted;
}

Node* CodeMap::getSelectedNode()
{
    return p_selected_node;
}

void CodeMap::selectNode(unsigned int id)
{
    m_root->exists(id, p_selected_node);
}

Node* CodeMap::getLastAddedNode()
{
    return last_added;
}

Node* CodeMap::getCurrentNode()
{
    return current_node;
}

void CodeMap::print()
{
    throwMessage("Print Code Tree\n");
    std::vector<bool> prefix = {};
    m_root->printSubtree(true, &prefix);
}
Node* CodeMap::exists(std::string search)
{
    Node* result = nullptr;
    auto search_split = stringToVector(search, "::");
    m_root->exists(search_split, 0, result);
    return result;
}

/*
    @name: addNode()
    @func: addNode to code map. 
    @args: @reference is the point to check if this node is meant, @depth means how deep to highlight childs
    @return: @bool returns if the child has been highlighted
*/
void CodeMap::addNode(Node* n, bool update_current_node)
{

} 


bool CodeMap::addNode(std::string name, NodeType t, bool update_current_node)
{
    cmsg("Add: " + name);
    Node* temp = nullptr;
    // auto name_split = stringToVector(name, "::");
    // m_root->exists(name_split, 0, temp);
    if(temp != nullptr)
    {
    //     current_node = temp;
    //     cmsg("[warning] Already exists!");
        return false;
    }
    else
    {
        Node* temp = new Node(name, t, current_node);
        //all_nodes.push_back(temp);
        sorted = false;    
        if(update_current_node)
            current_node = temp;
        last_added = temp;
    }
    return true;

}

Node* CodeMap::getNodeRef(std::string n)
{
    // m_root->exits(n, )
    
}

void CodeMap::addReference(std::string s, std::string e, ReferenceType t)
{
    getNodeRef(s)->addReference(getNodeRef(e), t);
}

void CodeMap::setNode(Node* n)
{
    current_node = n;
}

void CodeMap::setNode(std::string n)
{
    current_node = getNodeRef(n);
}

void CodeMap::goLastNode()
{
    current_node = last_added;
}

void CodeMap::goUp()
{
    if(current_node->getParent() != nullptr)
        current_node = current_node->getParent();
    else
        current_node = m_root;
}

void CodeMap::draw(sf::RenderWindow &window)
{
    
    if(draw_style == nullptr)
    {   std::cout << "ERROR::NO_DRAW_STYLE\n";
        return;
    }
    m_root->draw(window, draw_style);
    if(sorted == false)
    {
        clip(window.getView());
        sorted = true;
    }
}

void CodeMap::sortAndUpdate() //Call After everything is added.
{
    m_root->update(draw_style);
    m_root->updateReferences(draw_style);
    sorted = false;
} 

void CodeMap::update(const float dt, sf::Time elapsed)
{

}

void CodeMap::handleInput(sf::Event e, sf::RenderWindow &window)
{
    switch (e.type)
    {
    case sf::Event::KeyPressed:
        handleKeyboard(e, window);
        break;
    
    default:
        handleMouse(e, window);
        break;
    }  
}

void CodeMap::handleKeyboard(sf::Event e, sf::RenderWindow &window)
{
}

void CodeMap::handleMouse(sf::Event e, sf::RenderWindow &window)
{
    
    switch (e.type)
    {
        case sf::Event::MouseMoved:
        mouse_end = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if(draw_style->admin_mode && action_state == ActionState::SELECTING)
        {
            p_selected_node->move(mouse_end - mouse_start);
            sorted = false;
        }
        mouse_start = mouse_end;
        
        break;
        case sf::Event::MouseButtonPressed:
            if(e.mouseButton.button == sf::Mouse::Left)
            {
                m_root->highlight(-1, draw_style);
                p_selected_node = nullptr;
                mouse_start = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                action_state = ActionState::SELECTING;
                
                p_selected_node = m_root->highlight(window.mapPixelToCoords(sf::Mouse::getPosition(window)), draw_style->highlight_depth, draw_style);
            }
            else if(e.mouseButton.button == sf::Mouse::Right)
            {
                action_state = ActionState::NONE;
                m_root->highlight(-1, draw_style);
                p_selected_node = nullptr;
            }
            break;
        case sf::Event::MouseButtonReleased:
            action_state = ActionState::NONE;
            mouse_start = sf::Vector2f(0,0);
            mouse_end = sf::Vector2f(0,0);
            break;
        
        default:
            break;
    }
}   

DrawStyle* CodeMap::getDrawStyle()
{
    return draw_style;
}
void CodeMap::setDrawStyle(gc::DrawStyle* s)
{
    this->draw_style = s;
}

void  CodeMap::find(std::string keyword, std::vector<std::tuple<Node*, std::string>> &result)
{
    m_root->find(keyword, result);
}

void Reference::deleteRef()
{
    delete this;
}

void Reference::draw(sf::RenderWindow &window, DrawStyle* s)
{
    // << start->getName() << "->" << end->getName() << std::endl;
    if(m_visible && s->draw_references[m_type])  
        m_arrow.draw(window);
}

void Reference::update()
{
     m_arrow.update(sf::Vector2f(m_from->getPosition().x + m_from->getSize().x, 
                                    m_from->getPosition().y + m_from->getSize().y/2),
                 sf::Vector2f(m_to->getPosition().x, 
                                    m_to->getPosition().y + m_to->getSize().y/2));

}
void Reference::update(DrawStyle* s)
{
    if(m_highlighted)
        m_arrow.update(sf::Color(s->highlight_color.r, s->highlight_color.g, s->highlight_color.b, s->highlight_alpha));
    else
        m_arrow.update(s->reference_colormap[m_type]);
    update();
}

void Reference::highlight(int depth, DrawStyle *s)
{
    if(depth > 1) //highlight this
    {
        m_to->highlight(--depth, s);
        m_highlighted = true;
    }
    else //remove highlight
    {
        m_highlighted = false;
    }
    update(s);
}

void Reference::clip()
{
    m_visible = m_from->isVisible() || m_to->isVisible();
}

}
