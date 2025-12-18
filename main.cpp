#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "SFML 3 FPS Camera");
    window.setVerticalSyncEnabled(true);

    sf::Shader shader;
    if (!shader.loadFromFile("shader.frag", sf::Shader::Type::Fragment))
        return -1;

    sf::RectangleShape screen(sf::Vector2f(window.getSize()));

    // ===== Camera =====
    sf::Vector3f camPos = { -5.0f, 0.0f, 0.0f };
    float yaw   = 0.0f;
    float pitch = 0.0f;
    float mouseSensitivity = 0.0025f;
    float moveSpeed = 5.0f;

    bool mouseCaptured = true;
    bool ignoreNextMouse = true;

    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(
        { (int)window.getSize().x / 2, (int)window.getSize().y / 2 },
        window
    );

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            else if (event->is<sf::Event::MouseMoved>() && mouseCaptured)
            {
                const auto* e = event->getIf<sf::Event::MouseMoved>();
                if (!e) continue;

                if (ignoreNextMouse)
                {
                    ignoreNextMouse = false;
                    sf::Mouse::setPosition(
                        { (int)window.getSize().x / 2, (int)window.getSize().y / 2 },
                        window
                    );
                    continue;
                }

                int cx = window.getSize().x / 2;
                int cy = window.getSize().y / 2;

                int dx = e->position.x - cx;
                int dy = e->position.y - cy;

                yaw   += dx * mouseSensitivity;
                pitch += dy * mouseSensitivity;

                pitch = std::clamp(pitch, -1.55f, 1.55f);

                sf::Mouse::setPosition({ cx, cy }, window);
            }

            else if (event->is<sf::Event::MouseButtonPressed>())
            {
                mouseCaptured = true;
                ignoreNextMouse = true;
                window.setMouseCursorVisible(false);

                sf::Mouse::setPosition(
                    { (int)window.getSize().x / 2, (int)window.getSize().y / 2 },
                    window
                );
            }

            else if (event->is<sf::Event::KeyPressed>())
            {
                const auto* e = event->getIf<sf::Event::KeyPressed>();
                if (!e) continue;

                if (e->code == sf::Keyboard::Key::Escape)
                {
                    mouseCaptured = false;
                    window.setMouseCursorVisible(true);
                }
            }
        }

        sf::Vector3f forward = {
            cosf(pitch) * cosf(yaw),
            cosf(pitch) * sinf(yaw),
            sinf(pitch)
        };

        // Мир вверх
        sf::Vector3f worldUp = {0.0f, 0.0f, 1.0f};

        sf::Vector3f right = {
            forward.y * worldUp.z - forward.z * worldUp.y,
            forward.z * worldUp.x - forward.x * worldUp.z,
            forward.x * worldUp.y - forward.y * worldUp.x
        };

        float rlen = sqrtf(right.x*right.x + right.y*right.y + right.z*right.z);
        right.x /= rlen; right.y /= rlen; right.z /= rlen;

        sf::Vector3f up = {
            right.y * forward.z - right.z * forward.y,
            right.z * forward.x - right.x * forward.z,
            right.x * forward.y - right.y * forward.x
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            camPos += forward * moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            camPos -= forward * moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            camPos += right * moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            camPos -= right * moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            camPos += up * moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
            camPos -= up * moveSpeed * dt;

        shader.setUniform("u_time", clock.getElapsedTime().asSeconds());
        shader.setUniform("u_resolution",
            sf::Glsl::Vec2(window.getSize()));
        shader.setUniform("u_mouse",
            sf::Glsl::Vec2(-yaw, -pitch));
        shader.setUniform("u_camera",
            sf::Glsl::Vec3(camPos.x, camPos.y, camPos.z));

        window.clear();
        window.draw(screen, &shader);
        window.display();
    }

    return 0;
}
