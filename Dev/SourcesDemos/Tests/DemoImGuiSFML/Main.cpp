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

    bool windowHovered = false;
    bool wantSystemCursor = false;

    sf::RenderWindow window(sf::VideoMode(1080, 720), "ImGui + SFML Demo", sf::Style::Default);
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    sf::CircleShape greenCircleShape(100.f);
    greenCircleShape.setFillColor(sf::Color::Green);

    sf::CircleShape cursorShape(4.f);
    cursorShape.setFillColor(sf::Color::White);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseLeft:
                    windowHovered = false;
                    break;
                case sf::Event::MouseEntered:
                    windowHovered = true;
                    break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        //---------------
        // Tests with mouse cursor visibility.

        if (!ImGui::GetIO().WantCaptureMouse)
        {
            ImGui::SetMouseCursor(wantSystemCursor ? ImGuiMouseCursor_Arrow : ImGuiMouseCursor_None);
        }

        cursorShape.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));

        //---------------

        window.clear();
        window.draw(greenCircleShape);
        window.draw(cursorShape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    //----------------------------------------------

    return 0;
}
