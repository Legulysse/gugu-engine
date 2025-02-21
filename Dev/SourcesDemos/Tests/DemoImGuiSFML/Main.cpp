////////////////////////////////////////////////////////////////
// Includes

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
    // Notes:
    // - The code used here is based on the example code for imgui-sfml.

    //----------------------------------------------

    sf::RenderWindow window(sf::VideoMode(1080, 720), "ImGui + SFML Demo");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    //----------------------------------------------

    return 0;
}
