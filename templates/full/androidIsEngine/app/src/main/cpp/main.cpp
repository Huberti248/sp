#ifdef __ANDROID__
#include "isEngine/core/GameEngine.h"
#else
#include <SFML/Graphics.hpp>
#endif

#define BG_COLOR 100, 100, 100

int main(int argc, char* argv[])
{
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(), "AppName_");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(BG_COLOR));
        window.display();
    }
}