#include "ui/widgets/KeyBindWidget.h"

KeyBindWidget::KeyBindWidget() {
    actionBg.setFillColor(sf::Color(40, 40, 55));
    keyBg.setFillColor(sf::Color(55, 55, 75));
    keyBg.setOutlineThickness(1.f);
    keyBg.setOutlineColor(sf::Color(100, 100, 140));
}

void KeyBindWidget::setFont(const sf::Font& f, unsigned size) {
    font = &f;
    actionText.setFont(f);
    actionText.setCharacterSize(size);
    actionText.setFillColor(sf::Color(180, 180, 200));

    keyText.setFont(f);
    keyText.setCharacterSize(size);
    keyText.setFillColor(sf::Color::White);

    rebuildLayout();
}

void KeyBindWidget::setAction(const std::string& name) {
    actionName = name;
    actionText.setString(name);
    rebuildLayout();
}

void KeyBindWidget::setCurrentKey(sf::Keyboard::Key key) {
    boundKey = key;
    keyText.setString(keyToString(key));
    rebuildLayout();
}

sf::Keyboard::Key KeyBindWidget::getCurrentKey() const {
    return boundKey;
}

bool KeyBindWidget::isListening() const {
    return listening;
}

void KeyBindWidget::setOnKeyBound(BindCallback cb) {
    callback = std::move(cb);
}

void KeyBindWidget::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
        hovered = getBounds().contains(mouse);
    }

    if (!listening) {
        if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                               static_cast<float>(event.mouseButton.y));
            if (keyBg.getGlobalBounds().contains(mouse)) {
                enterListeningMode();
            }
        }
    } else {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                // Cancela sin cambiar la tecla
                listening = false;
                keyText.setString(keyToString(boundKey));
                keyBg.setFillColor(sf::Color(55, 55, 75));
            } else {
                exitListeningMode(event.key.code);
            }
        }
    }
}

void KeyBindWidget::update(float dt) {
    if (listening) {
        blinkTimer += dt;
        // Parpadeo del fondo: alterna cada 0.5 s
        bool blink = (static_cast<int>(blinkTimer / 0.5f) % 2) == 0;
        keyBg.setFillColor(blink ? sf::Color(100, 60, 20) : sf::Color(60, 40, 10));
    }
}

void KeyBindWidget::draw(sf::RenderWindow& window) {
    window.draw(actionBg);
    window.draw(keyBg);
    if (font) {
        window.draw(actionText);
        window.draw(keyText);
    }
}

void KeyBindWidget::setPosition(float px, float py) {
    x = px;
    y = py;
    rebuildLayout();
}

void KeyBindWidget::setSize(float pw, float ph) {
    w = pw;
    h = ph;
    rebuildLayout();
}

sf::FloatRect KeyBindWidget::getBounds() const {
    return {x, y, w, h};
}

bool KeyBindWidget::isHovered() const {
    return hovered;
}

void KeyBindWidget::enterListeningMode() {
    listening  = true;
    blinkTimer = 0.f;
    keyText.setString("Pulsa una tecla...");
    rebuildLayout();
}

void KeyBindWidget::exitListeningMode(sf::Keyboard::Key newKey) {
    listening = false;
    boundKey  = newKey;
    keyText.setString(keyToString(newKey));
    keyBg.setFillColor(sf::Color(55, 55, 75));
    rebuildLayout();
    if (callback) callback(boundKey);
}

void KeyBindWidget::rebuildLayout() {
    float actionW = w * 0.6f;
    float keyW    = w - actionW;

    actionBg.setPosition(x, y);
    actionBg.setSize({actionW, h});

    keyBg.setPosition(x + actionW, y);
    keyBg.setSize({keyW, h});

    if (font) {
        sf::FloatRect ab = actionText.getLocalBounds();
        actionText.setPosition(x + 6.f,
                               y + h / 2.f - (ab.top + ab.height) / 2.f);

        sf::FloatRect kb = keyText.getLocalBounds();
        keyText.setOrigin(kb.left + kb.width / 2.f,
                          kb.top  + kb.height / 2.f);
        keyText.setPosition(x + actionW + keyW / 2.f,
                            y + h / 2.f);
    }
}

std::string KeyBindWidget::keyToString(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::A: return "A";
        case sf::Keyboard::B: return "B";
        case sf::Keyboard::C: return "C";
        case sf::Keyboard::D: return "D";
        case sf::Keyboard::E: return "E";
        case sf::Keyboard::F: return "F";
        case sf::Keyboard::G: return "G";
        case sf::Keyboard::H: return "H";
        case sf::Keyboard::I: return "I";
        case sf::Keyboard::J: return "J";
        case sf::Keyboard::K: return "K";
        case sf::Keyboard::L: return "L";
        case sf::Keyboard::M: return "M";
        case sf::Keyboard::N: return "N";
        case sf::Keyboard::O: return "O";
        case sf::Keyboard::P: return "P";
        case sf::Keyboard::Q: return "Q";
        case sf::Keyboard::R: return "R";
        case sf::Keyboard::S: return "S";
        case sf::Keyboard::T: return "T";
        case sf::Keyboard::U: return "U";
        case sf::Keyboard::V: return "V";
        case sf::Keyboard::W: return "W";
        case sf::Keyboard::X: return "X";
        case sf::Keyboard::Y: return "Y";
        case sf::Keyboard::Z: return "Z";

        case sf::Keyboard::Num0: return "0";
        case sf::Keyboard::Num1: return "1";
        case sf::Keyboard::Num2: return "2";
        case sf::Keyboard::Num3: return "3";
        case sf::Keyboard::Num4: return "4";
        case sf::Keyboard::Num5: return "5";
        case sf::Keyboard::Num6: return "6";
        case sf::Keyboard::Num7: return "7";
        case sf::Keyboard::Num8: return "8";
        case sf::Keyboard::Num9: return "9";

        case sf::Keyboard::Numpad0: return "Num0";
        case sf::Keyboard::Numpad1: return "Num1";
        case sf::Keyboard::Numpad2: return "Num2";
        case sf::Keyboard::Numpad3: return "Num3";
        case sf::Keyboard::Numpad4: return "Num4";
        case sf::Keyboard::Numpad5: return "Num5";
        case sf::Keyboard::Numpad6: return "Num6";
        case sf::Keyboard::Numpad7: return "Num7";
        case sf::Keyboard::Numpad8: return "Num8";
        case sf::Keyboard::Numpad9: return "Num9";

        case sf::Keyboard::Space:     return "Space";
        case sf::Keyboard::Return:    return "Enter";
        case sf::Keyboard::Escape:    return "Escape";
        case sf::Keyboard::BackSpace: return "Backspace";
        case sf::Keyboard::Tab:       return "Tab";
        case sf::Keyboard::LShift:    return "L.Shift";
        case sf::Keyboard::RShift:    return "R.Shift";
        case sf::Keyboard::LControl:  return "L.Ctrl";
        case sf::Keyboard::RControl:  return "R.Ctrl";
        case sf::Keyboard::LAlt:      return "L.Alt";
        case sf::Keyboard::RAlt:      return "R.Alt";

        case sf::Keyboard::Left:  return "Izq";
        case sf::Keyboard::Right: return "Der";
        case sf::Keyboard::Up:    return "Arriba";
        case sf::Keyboard::Down:  return "Abajo";

        case sf::Keyboard::F1:  return "F1";
        case sf::Keyboard::F2:  return "F2";
        case sf::Keyboard::F3:  return "F3";
        case sf::Keyboard::F4:  return "F4";
        case sf::Keyboard::F5:  return "F5";
        case sf::Keyboard::F6:  return "F6";
        case sf::Keyboard::F7:  return "F7";
        case sf::Keyboard::F8:  return "F8";
        case sf::Keyboard::F9:  return "F9";
        case sf::Keyboard::F10: return "F10";
        case sf::Keyboard::F11: return "F11";
        case sf::Keyboard::F12: return "F12";

        case sf::Keyboard::Delete:   return "Del";
        case sf::Keyboard::Insert:   return "Ins";
        case sf::Keyboard::Home:     return "Home";
        case sf::Keyboard::End:      return "End";
        case sf::Keyboard::PageUp:   return "PgUp";
        case sf::Keyboard::PageDown: return "PgDn";

        default: return "?";
    }
}
