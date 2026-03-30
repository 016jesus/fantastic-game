#include "ui/widgets/Dropdown.h"

Dropdown::Dropdown() {
    header.setFillColor(sf::Color(50, 50, 70));
    header.setOutlineThickness(1.f);
    header.setOutlineColor(sf::Color(100, 100, 140));

    arrowText.setString("v");
    arrowText.setFillColor(sf::Color(180, 180, 200));
}

void Dropdown::setFont(const sf::Font& f, unsigned size) {
    font     = &f;
    fontSize = size;

    headerText.setFont(f);
    headerText.setCharacterSize(size);
    headerText.setFillColor(sf::Color::White);

    arrowText.setFont(f);
    arrowText.setCharacterSize(size);

    buildOptionWidgets();
}

void Dropdown::setOptions(const std::vector<std::string>& opts) {
    options = opts;
    if (selectedIndex >= static_cast<int>(options.size()))
        selectedIndex = 0;
    buildOptionWidgets();
}

void Dropdown::setSelected(int index) {
    if (index >= 0 && index < static_cast<int>(options.size())) {
        selectedIndex = index;
        if (font) {
            headerText.setString(options[selectedIndex]);
        }
    }
}

int Dropdown::getSelectedIndex() const {
    return selectedIndex;
}

const std::string& Dropdown::getSelectedOption() const {
    static const std::string empty;
    if (options.empty()) return empty;
    return options[selectedIndex];
}

void Dropdown::setOnSelected(SelectCallback cb) {
    callback = std::move(cb);
}

void Dropdown::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
        hovered = header.getGlobalBounds().contains(mouse);
    }

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));

        if (header.getGlobalBounds().contains(mouse)) {
            expanded = !expanded;
            arrowText.setString(expanded ? "^" : "v");
            return;
        }

        if (expanded) {
            int hovOpt = getHoveredOption(mouse.y);
            if (hovOpt >= 0) {
                selectedIndex = hovOpt;
                if (!options.empty()) {
                    headerText.setString(options[selectedIndex]);
                }
                if (callback) callback(selectedIndex, options[selectedIndex]);
            }
            expanded = false;
            arrowText.setString("v");
        }
    }

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
        expanded = false;
        arrowText.setString("v");
    }
}

void Dropdown::update(float /*dt*/) {
    // Highlight de la opción bajo el cursor
    if (expanded && font) {
        // nada que actualizar en tiempo real por ahora
    }
}

void Dropdown::draw(sf::RenderWindow& window) {
    window.draw(header);
    if (font) {
        window.draw(headerText);
        window.draw(arrowText);
    }

    if (expanded) {
        for (std::size_t i = 0; i < optionBgs.size(); ++i) {
            window.draw(optionBgs[i]);
            window.draw(optionTexts[i]);
        }
    }
}

void Dropdown::setPosition(float px, float py) {
    x = px;
    y = py;
    rebuildLayout();
}

void Dropdown::setSize(float pw, float ph) {
    w = pw;
    h = ph;
    rebuildLayout();
}

sf::FloatRect Dropdown::getBounds() const {
    if (expanded) {
        float totalH = h + static_cast<float>(options.size()) * h;
        return {x, y, w, totalH};
    }
    return {x, y, w, h};
}

bool Dropdown::isHovered() const {
    return hovered;
}

void Dropdown::buildOptionWidgets() {
    if (!font) return;

    optionBgs.clear();
    optionTexts.clear();

    for (std::size_t i = 0; i < options.size(); ++i) {
        sf::RectangleShape bg;
        bg.setSize({w, h});
        bg.setFillColor(sf::Color(40, 40, 60));
        bg.setOutlineThickness(1.f);
        bg.setOutlineColor(sf::Color(80, 80, 110));

        sf::Text txt;
        txt.setFont(*font);
        txt.setCharacterSize(fontSize);
        txt.setFillColor(sf::Color(220, 220, 220));
        txt.setString(options[i]);

        optionBgs.push_back(std::move(bg));
        optionTexts.push_back(std::move(txt));
    }

    rebuildLayout();

    if (!options.empty()) {
        headerText.setString(options[selectedIndex]);
    }
}

void Dropdown::rebuildLayout() {
    header.setPosition(x, y);
    header.setSize({w, h});

    if (font) {
        sf::FloatRect hb = headerText.getLocalBounds();
        headerText.setPosition(x + 6.f,
                               y + h / 2.f - hb.height / 2.f);

        sf::FloatRect ab = arrowText.getLocalBounds();
        arrowText.setPosition(x + w - ab.width - 8.f,
                              y + h / 2.f - ab.height / 2.f);
    }

    for (std::size_t i = 0; i < optionBgs.size(); ++i) {
        float oy = y + h * (static_cast<float>(i) + 1.f);
        optionBgs[i].setPosition(x, oy);
        optionBgs[i].setSize({w, h});

        sf::FloatRect tb = optionTexts[i].getLocalBounds();
        optionTexts[i].setPosition(x + 6.f,
                                   oy + h / 2.f - tb.height / 2.f);
    }
}

int Dropdown::getHoveredOption(float mouseY) const {
    for (std::size_t i = 0; i < optionBgs.size(); ++i) {
        sf::FloatRect r = optionBgs[i].getGlobalBounds();
        if (mouseY >= r.top && mouseY < r.top + r.height)
            return static_cast<int>(i);
    }
    return -1;
}
