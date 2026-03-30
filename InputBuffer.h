#pragma once
#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H
#include <SFML/Window/Keyboard.hpp>
#include <deque>
#include <vector>
#include <string>
#include <functional>

// ---------------------------------------------------------------------------
// BufferedKey — registro de una tecla pulsada con su marca de tiempo.
// ---------------------------------------------------------------------------
struct BufferedKey {
    sf::Keyboard::Key key;
    float             timestamp;  // tiempo absoluto en segundos desde inicio
};

// ---------------------------------------------------------------------------
// InputBuffer — buffer circular de las últimas N pulsaciones de teclado.
//
// Permite detectar combos (secuencias de teclas dentro de una ventana de
// tiempo) sin modificar la lógica de cada estado del juego.
//
// Principio Open/Closed: nuevos combos se comprueban con checkCombo() sin
// tocar esta clase.
// Singleton de Meyer (thread-safe desde C++11).
// ---------------------------------------------------------------------------
class InputBuffer {
public:
    static InputBuffer& getInstance();

    // Llamar desde el event loop principal en cada sf::Event::KeyPressed.
    void recordKey(sf::Keyboard::Key key, float currentTime);

    // Avanza el tiempo interno y expira pulsaciones antiguas.
    void update(float deltaTime);

    // Comprueba si la secuencia de teclas indicada se ejecutó dentro de
    // maxTime segundos (en orden cronológico, de más antigua a más reciente).
    // Ejemplo: checkCombo({J, J, J}, 0.8f) → true si J se pulsó 3 veces
    // consecutivas en menos de 0.8 s.
    bool checkCombo(const std::vector<sf::Keyboard::Key>& sequence,
                    float maxTime = 0.8f) const;

    // Vacía el buffer manualmente.
    void clear();

    // Acceso de solo lectura al buffer (útil para debug).
    const std::deque<BufferedKey>& getBuffer() const;

private:
    InputBuffer() = default;

    // No copiable ni movible — es un singleton.
    InputBuffer(const InputBuffer&)            = delete;
    InputBuffer& operator=(const InputBuffer&) = delete;

    static constexpr int   BUFFER_SIZE = 16;
    static constexpr float MAX_AGE     = 3.0f;  // segundos antes de expirar

    std::deque<BufferedKey> buffer;
    float currentTime = 0.f;

    // Elimina entradas más antiguas que MAX_AGE respecto al tiempo actual.
    void pruneOld();
};
#endif
