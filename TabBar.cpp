#include "TabBar.h"

static const sf::Color COLOR_ACTIVE  {80,  130, 200};
static const sf::Color COLOR_INACTIVE{55,  55,  75 };
static const sf::Color COLOR_HOVER   {70,  100, 150};
static const sf::Color COLOR_TEXT_ACTIVE  {sf::Color::White};
static const sf::Color COLOR_TEXT_INACTIVE{180, 180, 200};

TabBar::TabBar() {}

void TabBar::setFont(const sf::Font& f, unsigned size) {
    font     = &f;
    fontSize = size;
    for (auto& tab : tabs) {
        tab.text.setFont(f);
        tab.text.setCharacterSize(size);
    }
    layoutTabs();
}

void TabBar::addTab(const std::string& title) {
    Tab t;
    t.title   = title;
    t.hovered = false;
    t.bg.setFillColor(COLOR_INACTIVE);

    if (font) {
        t.text.setFont(*font);
        t.text.setCharacterSize(fontSize);
        t.text.setString(title);
        t.text.setFillColor(COLOR_TEXT_INACTIVE);
    }

    tabs.push_back(std::move(t));
    layoutTabs();
}

void TabBar::setActiveTab(int index) {
    if (index >= 0 && index < static_cast<int>(tabs.size())) {
        activeTab = index;
        layoutTabs();
    }
}

int TabBar::getActiveTab() const {
    return activeTab;
}

void TabBar::setOnTabChanged(TabCallback cb) {
    callback = std::move(cb);
}

void TabBar::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
        for (auto& tab : tabs) {
            tab.hovered = tab.bg.getGlobalBounds().contains(mouse);
        }
    }

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y));
        for (int i = 0; i < static_cast<int>(tabs.size()); ++i) {
            if (tabs[i].bg.getGlobalBounds().contains(mouse)) {
                if (i != activeTab) {
                    activeTab = i;
                    layoutTabs();
                    if (callback) callback(activeTab);
                }
                break;
            }
        }
    }
}

void TabBar::update(float /*dt*/) {
    for (int i = 0; i < static_cast<int>(tabs.size()); ++i) {
        auto& tab = tabs[i];
        if (i == activeTab) {
            tab.bg.setFillColor(COLOR_ACTIVE);
            tab.text.setFillColor(COLOR_TEXT_ACTIVE);
        } else if (tab.hovered) {
            tab.bg.setFillColor(COLOR_HOVER);
            tab.text.setFillColor(COLOR_TEXT_ACTIVE);
        } else {
            tab.bg.setFillColor(COLOR_INACTIVE);
            tab.text.setFillColor(COLOR_TEXT_INACTIVE);
        }
    }
}

void TabBar::draw(sf::RenderWindow& window) {
    for (auto& tab : tabs) {
        window.draw(tab.bg);
        if (font) window.draw(tab.text);
    }
}

void TabBar::setPosition(float px, float py) {
    x = px;
    y = py;
    layoutTabs();
}

void TabBar::setSize(float pw, float ph) {
    w = pw;
    h = ph;
    layoutTabs();
}

sf::FloatRect TabBar::getBounds() const {
    return {x, y, w, h};
}

bool TabBar::isHovered() const {
    for (const auto& tab : tabs) {
        if (tab.hovered) return true;
    }
    return false;
}

void TabBar::layoutTabs() {
    if (tabs.empty()) return;
    float tabW = w / static_cast<float>(tabs.size());

    for (std::size_t i = 0; i < tabs.size(); ++i) {
        auto& tab = tabs[i];
        float tx  = x + static_cast<float>(i) * tabW;

        tab.bg.setPosition(tx, y);
        tab.bg.setSize({tabW - 1.f, h}); // 1px separador

        if (font) {
            tab.text.setString(tab.title);
            sf::FloatRect tb = tab.text.getLocalBounds();
            tab.text.setOrigin(tb.left + tb.width / 2.f,
                               tb.top  + tb.height / 2.f);
            tab.text.setPosition(tx + tabW / 2.f,
                                 y  + h / 2.f);
        }
    }
}
