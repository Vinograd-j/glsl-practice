#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "SFML works!");

    auto clock = sf::Clock();

    auto shape = sf::RectangleShape(sf::Vector2f { window.getSize() });

    sf::Shader shader;

    if (!shader.loadFromFile("shader.frag", sf::Shader::Type::Fragment))
        return -1;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        float time = clock.getElapsedTime().asSeconds();
        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        shader.setUniform("u_time", time);
        shader.setUniform("u_resolution", sf::Glsl::Vec2(window.getSize()));
        shader.setUniform("u_mouse", sf::Glsl::Vec2(mouse));

        window.clear();
        window.draw(shape, &shader);
        window.display();
    }
}
