#ifndef ARROW_HPP
#define ARROW_HPP

#include "SFML/Graphics.hpp"

#include <iostream>
#include <memory>

#define DEBUG
namespace sf
{
enum ArrowEndType {NONE, ROUND, ARROW, RECTANGLE};
enum ArrowType {DIRECT, V_OFFSET_DIRECT, H_OFFSET_DIRECT , V_RECTLINEAR, H_RECTLINEAR};
class Arrow
{
    private:
        sf::VertexArray m_connection;
        std::unique_ptr<sf::Shape> m_end;
        std::unique_ptr<sf::Shape> m_start;
        sf::Vector2f m_start_pos;
        sf::Vector2f m_end_pos;
        sf::Color m_color;
        ArrowEndType m_start_type, m_end_type;
        float m_offset = 0;
        float m_type_size;
        ArrowType m_type;
        void updateArrow();
    public:
        
        void draw(sf::RenderTarget& target) const;
        void update(sf::Color c);
        void update(sf::Vector2f start = sf::Vector2f(0,0), sf::Vector2f end = sf::Vector2f(0,0));
        
        Arrow(sf::Vector2f s = sf::Vector2f(0,0), sf::Vector2f e = sf::Vector2f(0,0));


    ~Arrow(){}
};

class BezierCurve
{

};
}//namespace sf
#endif /*ARROW_HPP*/