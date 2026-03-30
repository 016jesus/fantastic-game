#include "states/PauseMenuState.h"
#include "core/GameStateManager.h"
#include "states/MenuState.h"
#include "states/OptionsState.h"
#include "core/SaveSystem.h"
#include "ui/tabs/CreditsTab.h"

// ---------------------------------------------------------------------------
PauseMenuState::PauseMenuState(GameStateManager* gsmPtr) {
    this->gsm = gsmPtr;
}

// ---------------------------------------------------------------------------
// onEnter: carga la fuente e inicializa toda la UI
// ---------------------------------------------------------------------------
void PauseMenuState::onEnter() {
    bool loaded = font.loadFromFile("minecraft.otf");
    if (!loaded) {
        font.loadFromFile("Minecraft.ttf");
    }
    buildUI();
}

// ---------------------------------------------------------------------------
// buildUI: construye overlay, panel, título y botones
// ---------------------------------------------------------------------------
void PauseMenuState::buildUI() {
    // --- Overlay semitransparente sobre el PlayingState ---
    overlay.setSize(sf::Vector2f(512.f, 256.f));
    overlay.setPosition(0.f, 0.f);
    overlay.setFillColor(sf::Color(0, 0, 0, 160));

    // --- Panel central (180x160 centrado en la ventana 512x256) ---
    // Centro ventana: 256, 128. Panel: x=166, y=48
    const float panelW = 180.f;
    const float panelH = 160.f;
    const float panelX = (512.f - panelW) / 2.f;  // 166.f
    const float panelY = (256.f - panelH) / 2.f;  // 48.f

    panel.setSize(sf::Vector2f(panelW, panelH));
    panel.setPosition(panelX, panelY);
    panel.setFillColor(sf::Color(25, 25, 40));

    // Borde del panel (outline)
    panelBorder.setSize(sf::Vector2f(panelW, panelH));
    panelBorder.setPosition(panelX, panelY);
    panelBorder.setFillColor(sf::Color::Transparent);
    panelBorder.setOutlineColor(sf::Color::White);
    panelBorder.setOutlineThickness(1.f);

    // --- Título "PAUSA" ---
    titleText.setFont(font);
    titleText.setString("PAUSA");
    titleText.setCharacterSize(20u);
    titleText.setFillColor(sf::Color::White);
    {
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    }
    titleText.setPosition(256.f, 58.f);

    // --- Botones: w=140, h=24, centrados en x=186 ---
    //   Centrado en x: panelX + panelW/2 = 166 + 90 = 256
    //   Botón centrado: x = 256 - 140/2 = 186
    const float btnW     = 140.f;
    const float btnH     = 24.f;
    const float btnX     = 256.f - btnW / 2.f;  // 186.f
    const float btnYStart = 85.f;
    const float btnStep   = 34.f;

    buttons.clear();
    buttons.resize(4);

    struct BtnDef { const char* label; };
    const BtnDef defs[4] = {
        {"Reanudar"},
        {"Opciones"},
        {"Guardar"},
        {"Menu principal"}
    };

    for (int i = 0; i < 4; ++i) {
        Button& btn = buttons[i];
        btn.setFont(font, 12u);
        btn.setLabel(defs[i].label);
        btn.setSize(btnW, btnH);
        btn.setPosition(btnX, btnYStart + static_cast<float>(i) * btnStep);
        btn.setNormalColor(sf::Color(50, 50, 75),   sf::Color::White);
        btn.setHoverColor (sf::Color(80, 80, 120),  sf::Color::White);
        btn.setActiveColor(sf::Color(120, 100, 40), sf::Color::Yellow);
    }

    // Asignar callbacks
    buttons[0].setCallback([this]() { resume();       });
    buttons[1].setCallback([this]() { openOptions();  });
    buttons[2].setCallback([this]() { saveGame();     });
    buttons[3].setCallback([this]() { goToMainMenu(); });

    // --- Notificación de guardado ---
    saveNotification.setFont(font);
    saveNotification.setCharacterSize(10u);
    saveNotification.setFillColor(sf::Color(100, 230, 100));
    saveNotification.setString("Partida guardada");
    {
        sf::FloatRect nb = saveNotification.getLocalBounds();
        saveNotification.setOrigin(nb.left + nb.width / 2.f, nb.top);
    }
    saveNotification.setPosition(256.f, 220.f);
    saveNotificationTimer = 0.f;
}

// ---------------------------------------------------------------------------
// resume: saca el PauseMenuState del stack, volviendo al PlayingState
// ---------------------------------------------------------------------------
void PauseMenuState::resume() {
    gsm->pop();
}

// ---------------------------------------------------------------------------
// openOptions: empuja OptionsState con fromPause=true
// ---------------------------------------------------------------------------
void PauseMenuState::openOptions() {
    auto opts = std::make_unique<OptionsState>(gsm, /*fromPause=*/true);
    // Inyectamos la pestaña de créditos para que esté disponible también desde pausa
    opts->addTab(std::make_unique<CreditsTab>(font));
    gsm->push(std::move(opts));
}

// ---------------------------------------------------------------------------
// saveGame: guarda una partida stub (sin acceso directo al PlayingState)
// ---------------------------------------------------------------------------
void PauseMenuState::saveGame() {
    // Stub: guarda datos vacíos para confirmar el mecanismo.
    // Una integración completa requeriría un Bus de eventos o pasar SaveData
    // desde PlayingState mediante un callback/servicio.
    SaveData sd;
    sd.playerName = "jugador";
    sd.gender     = "male";
    sd.hp         = 100;
    sd.money      = 0;
    sd.posX       = 64.f;
    sd.posY       = 148.f;
    sd.section    = 0;
    SaveSystem::getInstance().save(sd);

    // Activa la notificación visual
    saveNotificationTimer = NOTIF_DURATION;
}

// ---------------------------------------------------------------------------
// goToMainMenu: reemplaza el stack completo por MenuState (pierde partida)
// ---------------------------------------------------------------------------
void PauseMenuState::goToMainMenu() {
    gsm->replace(std::make_unique<MenuState>(gsm));
}

// ---------------------------------------------------------------------------
// handleEvent: Escape reanuda; todos los botones reciben el evento
// ---------------------------------------------------------------------------
void PauseMenuState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            resume();
            return;
        }
    }

    for (auto& btn : buttons) {
        btn.handleEvent(event);
    }
}

// ---------------------------------------------------------------------------
// update: actualiza botones y temporizador de notificación
// ---------------------------------------------------------------------------
void PauseMenuState::update(float deltaTime) {
    for (auto& btn : buttons) {
        btn.update(deltaTime);
    }

    if (saveNotificationTimer > 0.f) {
        saveNotificationTimer -= deltaTime;
        if (saveNotificationTimer < 0.f) saveNotificationTimer = 0.f;
    }
}

// ---------------------------------------------------------------------------
// render: overlay → panel → título → botones → notificación opcional
// ---------------------------------------------------------------------------
void PauseMenuState::render(sf::RenderWindow& window) {
    // El PlayingState debajo se renderiza por el GSM antes de que este estado
    // sea llamado, ya que GSM::render() itera todo el stack (o solo el top).
    // Este overlay cubre el juego subyacente.
    window.draw(overlay);
    window.draw(panel);
    window.draw(panelBorder);
    window.draw(titleText);

    for (auto& btn : buttons) {
        btn.draw(window);
    }

    // Notificación de guardado: visible solo mientras el timer esté activo
    if (saveNotificationTimer > 0.f) {
        window.draw(saveNotification);
    }
}
