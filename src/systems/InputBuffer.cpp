#include "systems/InputBuffer.h"

// ---------------------------------------------------------------------------
// getInstance — singleton de Meyer.
// ---------------------------------------------------------------------------
InputBuffer& InputBuffer::getInstance() {
    static InputBuffer instance;
    return instance;
}

// ---------------------------------------------------------------------------
// recordKey — registra una pulsación al frente del deque.
// Si el buffer está lleno, elimina el registro más antiguo (el último).
// ---------------------------------------------------------------------------
void InputBuffer::recordKey(sf::Keyboard::Key key, float time) {
    BufferedKey bk;
    bk.key       = key;
    bk.timestamp = time;

    buffer.push_front(bk);

    if (static_cast<int>(buffer.size()) > BUFFER_SIZE) {
        buffer.pop_back();
    }

    pruneOld();
}

// ---------------------------------------------------------------------------
// update — avanza el tiempo interno y expira pulsaciones antiguas.
// ---------------------------------------------------------------------------
void InputBuffer::update(float deltaTime) {
    currentTime += deltaTime;
    pruneOld();
}

// ---------------------------------------------------------------------------
// pruneOld — elimina entradas con edad mayor que MAX_AGE.
// ---------------------------------------------------------------------------
void InputBuffer::pruneOld() {
    while (!buffer.empty() &&
           (currentTime - buffer.back().timestamp) > MAX_AGE)
    {
        buffer.pop_back();
    }
}

// ---------------------------------------------------------------------------
// checkCombo — busca la secuencia en el buffer en orden cronológico.
//
// Algoritmo:
//   1. El buffer está ordenado de más reciente (frente) a más antiguo (fondo).
//   2. Se busca hacia atrás la última tecla de la secuencia y, desde ahí,
//      se retrocede buscando cada tecla anterior de la secuencia.
//   3. Se verifica que el tiempo total entre la primera y la última tecla
//      encontrada sea menor que maxTime.
// ---------------------------------------------------------------------------
bool InputBuffer::checkCombo(const std::vector<sf::Keyboard::Key>& sequence,
                              float maxTime) const
{
    if (sequence.empty()) return false;
    if (buffer.size() < sequence.size()) return false;

    // Índice de la tecla que buscamos a continuación (partimos por el final
    // de la secuencia y vamos hacia el inicio).
    int seqIdx = static_cast<int>(sequence.size()) - 1;

    // Índice en el buffer de la última tecla de la secuencia encontrada.
    int lastFound = -1;

    // Índice en el buffer de la primera tecla de la secuencia encontrada.
    int firstFound = -1;

    // Recorrer el buffer de fondo (más antiguo) hacia el frente (más reciente)
    // para encontrar la secuencia en orden cronológico.
    // El buffer[0] es el más reciente; buffer[size-1] es el más antiguo.
    // Invertimos el recorrido: de buffer.size()-1 hacia 0.
    for (int i = static_cast<int>(buffer.size()) - 1; i >= 0 && seqIdx >= 0; --i) {
        if (buffer[static_cast<std::size_t>(i)].key == sequence[static_cast<std::size_t>(seqIdx)]) {
            if (seqIdx == static_cast<int>(sequence.size()) - 1) {
                // Última tecla de la secuencia: marca el inicio temporal.
                firstFound = i;
            }
            lastFound = i;
            --seqIdx;
        }
    }

    // Si no se encontró la secuencia completa, falla.
    if (seqIdx >= 0) return false;

    // Verificar la ventana de tiempo entre primera y última tecla encontradas.
    // firstFound apunta a la tecla más antigua (mayor índice = más antiguo).
    // lastFound  apunta a la tecla más reciente (menor índice = más reciente).
    const float tFirst = buffer[static_cast<std::size_t>(firstFound)].timestamp;
    const float tLast  = buffer[static_cast<std::size_t>(lastFound)].timestamp;

    return (tLast - tFirst) < maxTime;
}

// ---------------------------------------------------------------------------
// clear — vacía el deque manualmente.
// ---------------------------------------------------------------------------
void InputBuffer::clear() {
    buffer.clear();
}

// ---------------------------------------------------------------------------
// getBuffer — acceso de solo lectura (p.ej. para DebugOverlay).
// ---------------------------------------------------------------------------
const std::deque<BufferedKey>& InputBuffer::getBuffer() const {
    return buffer;
}
