#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <functional>
#include <list>
#include <thread>

#define DEMO_USE_AUDIO false

//--------------

struct StatsSummary
{
    int min = 99999999;
    int max = 0;
    int last = -1;
};

void ComputeStatsSummary(const std::list<int>& statValues, StatsSummary& statsSummary)
{
    for (int value : statValues)
    {
        statsSummary.max = value >= 0 ? std::max(statsSummary.max, value) : statsSummary.max;
        statsSummary.min = value >= 0 ? std::min(statsSummary.min, value) : statsSummary.min;
        statsSummary.last = statsSummary.last == -1 && value >= 0 ? value : statsSummary.last;
    }

    statsSummary.last = std::max(0, statsSummary.last);
}

void DrawHistogram(const std::list<int>& statValues, const StatsSummary& statsSummary, sf::VertexArray& curve, sf::Vector2f position, sf::RenderWindow* renderWindow)
{
    float curveHeight = 50.f;
    float pointOffset = 0.5f;

    int curveTopValue = std::max(4, statsSummary.max);
    float curvePointScaleY = curveHeight / curveTopValue;

    int index = 0;
    for (int value : statValues)
    {
        value = std::max(0, value);
        curve[index * 2].position = sf::Vector2f(pointOffset + position.x + index * 2, position.y + curveHeight);
        curve[index * 2 + 1].position = sf::Vector2f(pointOffset + position.x + index * 2, position.y + curveHeight - value * curvePointScaleY);
        ++index;
    }

    renderWindow->draw(curve);
}

//--------------

void func()
{
    sf::sleep(sf::milliseconds(4));
}

//--------------

int main()
{
    //--------------
    // Note: Setup threads usage.
    bool useThreads = true;
    size_t waitFramesBeforeThreads = 60;

    //--------------
    size_t maxStatCount = 150;
    std::list<int> statsLoop;
    std::list<int> statsRender;

    sf::VertexArray curveLoop;
    curveLoop.setPrimitiveType(sf::PrimitiveType::Lines);
    curveLoop.resize(maxStatCount * 2);

    sf::VertexArray curveRender;
    curveRender.setPrimitiveType(sf::PrimitiveType::Lines);
    curveRender.resize(maxStatCount * 2);

    //--------------
    size_t entityCount = 400;
    std::vector<sf::VertexArray*> entities;
    for (size_t e = 0; e < entityCount; ++e)
    {
        size_t pointCount = 10000;
        size_t itemPerLine = 100;
        float offsetPoints = 4.f;
        float positionX = 10.f;
        float positionY = 10.f;

        sf::VertexArray* vertices = new sf::VertexArray;
        vertices->setPrimitiveType(sf::PrimitiveType::Points);
        vertices->resize(pointCount);
        for (size_t i = 0; i < pointCount; ++i)
        {
            (*vertices)[i].position = sf::Vector2f(positionX + offsetPoints * (i % itemPerLine), positionY + offsetPoints * (i / itemPerLine));
        }

        entities.push_back(vertices);
    }

    //--------------
    sf::Font font;
    bool fontOk = font.openFromFile("Assets/Fonts/roboto/Roboto-Regular.ttf");

    sf::Text textStats(font);

    //--------------
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 2;

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(800, 600), 32),
        "SFML Test",
        sf::Style::Resize | sf::Style::Close,
        sf::State::Windowed,
        settings);
    
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(false);

    //--------------

#if DEMO_USE_AUDIO
    // Load a music to play
    sf::Music music("nice_music.ogg");

    // Play the music
    music.play();
#endif

    //--------------
    size_t frameCount = 0;
    bool pendingThreads = true;

    sf::Clock clockLoop;
    while (window.isOpen())
    {
        sf::Time timeLoop = clockLoop.restart();

        statsLoop.push_front(timeLoop.asMilliseconds());
        if (statsLoop.size() > maxStatCount)
        {
            statsLoop.pop_back();
        }

        //---------------
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        //---------------
        // Note: Switch window state.
        //window.setActive(false);

        //--------------
        sf::Clock clockThread;
        if (useThreads)
        {
            ++frameCount;
            if (frameCount > waitFramesBeforeThreads)
            {
                pendingThreads = false;

                std::thread t1(func);
                std::thread t2(func);

                t1.join();
                t2.join();
            }
        }
        else
        {
            func();
        }

        sf::Time timeThread = clockThread.getElapsedTime();

        //---------------
        // Note: Switch window state.
        bool activeOk = window.setActive(true);

        //---------------
        window.clear(sf::Color::Black);

        sf::Clock clockRender;
        for (size_t e = 0; e < entities.size(); ++e)
        {
            window.draw(*entities[e]);
        }
        sf::Time timeRender = clockRender.getElapsedTime();

        statsRender.push_front(timeRender.asMilliseconds());
        if (statsRender.size() > maxStatCount)
        {
            statsRender.pop_back();
        }

        //---------------
        textStats.setPosition(sf::Vector2f(420.f, 0.f));
        textStats.setString("Loop (ms): " + std::to_string(timeLoop.asMilliseconds()));
        window.draw(textStats);

        StatsSummary summaryLoop;
        ComputeStatsSummary(statsLoop, summaryLoop);
        DrawHistogram(statsLoop, summaryLoop, curveLoop, sf::Vector2f(420.f, 60.f), &window);

        textStats.setPosition(sf::Vector2f(420.f, 150.f));
        textStats.setString("Render (ms): " + std::to_string(timeRender.asMilliseconds()));
        window.draw(textStats);

        StatsSummary summaryRender;
        ComputeStatsSummary(statsRender, summaryRender);
        DrawHistogram(statsRender, summaryRender, curveRender, sf::Vector2f(420.f, 210.f), &window);

        textStats.setPosition(sf::Vector2f(420.f, 300.f));
        if (useThreads)
            textStats.setString("Thread (ms): " + (pendingThreads ? "pending" : std::to_string(timeThread.asMilliseconds())));
        else
            textStats.setString("Fake Thread (ms): " + std::to_string(timeThread.asMilliseconds()));
        window.draw(textStats);

        window.display();

        //---------------
        // Note: Switch window state.
        bool inactiveOk = window.setActive(false);
    }

    return 0;
}
