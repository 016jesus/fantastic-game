#include "protagonista.h"
#include "jefe.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

using namespace sf;
using namespace std;

string getNameFromUI(RenderWindow* window, Event &evento);

int main() {
    RenderWindow window(VideoMode(512, 256), "The legend of tilin");
    Protagonista player(100, 5, "suat", 0);
    Font f;
    f.loadFromFile("minecraft.otf");
    string name;

    while (window.isOpen()) {
        Event evento;
        while (window.pollEvent(evento)) {
            if (evento.type == Event::Closed) {
                window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                //sf::IntRect derecho(120, 0, 120.0, 80.0);
                //caballero.setTextureRect(derecho);
                //caballero.move(5,0);
            }
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                /* sf::IntRect derecho(120, 0, 120.0, 80.0);
                caballero.setTextureRect(derecho);
                caballero.move(-5, 0);*/
            }
        
        
        }

        window.clear(Color::White);
        
        window.draw(Text(name, f, 30u));
        window.display();
    }

    return 0;
}

string getNameFromUI(RenderWindow* window, Event &evento) {
    Text t;
    Font f;
    
    f.loadFromFile("Minecraft.ttf");
    t.setFont(f);
    t.setString("Hola amigo, por favor ingresa tu nombre: ");
    t.setScale(4, 4);
    window->draw(t);
    string name;

    if (evento.type == Event::TextEntered) { 
        if (evento.text.unicode < 128) {
            char entrada = static_cast<char>(evento.text.unicode);
            if (entrada == '\b' && !name.empty()) {
                name.pop_back();
            }
            else if (entrada != '\b') {
                name += entrada;
            }
        }
    }

    return name;
}
