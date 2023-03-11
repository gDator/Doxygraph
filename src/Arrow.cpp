#include "Arrow.hpp"

namespace sf
{

Arrow::Arrow(sf::Vector2f s, sf::Vector2f e)
{
    // std::cout << "Arrow::Arrow()" << std::endl;
    m_start_pos = s;
    m_end_pos = e;
    m_color = sf::Color::White;
    m_connection = sf::VertexArray(sf::PrimitiveType::LineStrip, 2);
    m_type = sf::ArrowType::H_RECTLINEAR;
    m_start_type = sf::ArrowEndType::RECTANGLE;
    m_end_type = sf::ArrowEndType::RECTANGLE;
    m_offset = 20;
    m_type_size = 10;
    updateArrow();

}

void Arrow::update(sf::Color c)
{
    //set color of arrow
    m_color = c;
    m_end->setFillColor(m_color);
    m_start->setFillColor(m_color);
    for(int i = 0; i < m_connection.getVertexCount(); i++)
        m_connection[i].color = m_color;

}

void Arrow::update(sf::Vector2f start, sf::Vector2f end)
{
    sf::Vector2f origin = sf::Vector2f(m_type_size/2, m_type_size/2);
    m_start_pos = start;
    m_end_pos = end;
    m_end->setOrigin(origin);
    m_end->setPosition(m_end_pos);
    m_start->setOrigin(origin);
    m_start->setPosition(m_start_pos);

     //Construct Arrow line
    switch (m_type)
    {
        default:
        case ArrowType::DIRECT:
            m_connection[0].position = m_start_pos;
            m_connection[1].position = m_end_pos;
            break;
        case ArrowType::H_OFFSET_DIRECT:
            m_connection.resize(4);
            m_connection[0].position = m_start_pos;
            if(m_end_pos.x > m_start_pos.x)
            {
                m_connection[1].position = m_start_pos + sf::Vector2f(m_offset, 0);
                m_connection[2].position = m_start_pos - sf::Vector2f(m_offset, 0);
            }
            else
            {
                m_connection[1].position = m_start_pos - sf::Vector2f(m_offset, 0);
                m_connection[2].position = m_end_pos + sf::Vector2f(m_offset, 0);
            }
            m_connection[3].position = m_end_pos;
            break;
        case ArrowType::V_OFFSET_DIRECT:
            m_connection.resize(4);
            m_connection[0].position = m_start_pos;
            if(m_end_pos.y > m_start_pos.y)
            {
                m_connection[1].position = m_start_pos + sf::Vector2f(0, m_offset);
                m_connection[2].position = m_end_pos - sf::Vector2f(0, m_offset);
            }
            else
            {
                m_connection[1].position = m_start_pos - sf::Vector2f(0, m_offset);
                m_connection[2].position = m_end_pos + sf::Vector2f(0, m_offset);
            }
            m_connection[3].position = m_end_pos;
            break;
        case ArrowType::H_RECTLINEAR:
            m_connection.resize(4);
            m_connection[0].position = m_start_pos;
            if(m_end_pos.x > m_start_pos.x)
            {
                m_connection[1].position = m_start_pos  + sf::Vector2f(m_offset, 0);
                m_connection[2].position = sf::Vector2f(m_connection[1].position.x, m_end_pos.y);
            }
            else
            {
                m_connection[1].position = m_start_pos - sf::Vector2f(m_offset, 0);
                m_connection[2].position = sf::Vector2f(m_connection[1].position.x, m_end_pos.y);
            }
            m_connection[3].position = m_end_pos;
            break;
        case ArrowType::V_RECTLINEAR:
            m_connection.resize(4);
            m_connection[0].position = m_start_pos;
            if(m_end_pos.y > m_start_pos.y)
            {
                m_connection[1].position = m_start_pos  + sf::Vector2f(0,m_offset);
                m_connection[2].position = sf::Vector2f(m_end_pos.x,m_connection[1].position.y);
            }
            else
            {
                m_connection[1].position = m_start_pos - sf::Vector2f(0, m_offset);
                m_connection[2].position = sf::Vector2f(m_end_pos.x,m_connection[1].position.y);
            }
            m_connection[3].position = m_end_pos;
            break;
    }
}   

void Arrow::updateArrow()
{
    //Construct Arrow end
    
    switch (m_end_type)
    {
        case ArrowEndType::ARROW:
            //calculated below
        case ArrowEndType::ROUND:
            m_end = std::make_unique<sf::CircleShape>(m_type_size/2);
        break;
        case ArrowEndType::RECTANGLE:
            m_end = std::make_unique<sf::RectangleShape>(sf::Vector2f(m_type_size,m_type_size));
        break;
        case ArrowEndType::NONE:
        default:
            break;
    }

    //Construct Arrow start
    switch (m_start_type)
    {
        case ArrowEndType::ARROW:
            //calculated below
        case ArrowEndType::ROUND:
            m_start = std::make_unique<sf::CircleShape>(m_type_size/2);
            
        break;
        case ArrowEndType::RECTANGLE:
            m_start = std::make_unique<sf::RectangleShape>(sf::Vector2f(m_type_size,m_type_size));
        break;
        case ArrowEndType::NONE:
        default:
            break;
    }
    update(m_color);
    update(m_start_pos, m_end_pos);
    
}
void Arrow::draw(sf::RenderTarget& target) const
{
    target.draw(*m_end);
    target.draw(*m_start);
    target.draw(m_connection, sf::RenderStates::Default);
}
}//namespace sf