#pragma once
#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H

// ---------------------------------------------------------------------------
// EnemyState — máquina de estados para todos los enemigos del juego.
// ---------------------------------------------------------------------------
enum class EnemyState {
    Idle,    // Quieto, esperando
    Patrol,  // Patrullando su rango de origen
    Chase,   // Persiguiendo al jugador
    Attack,  // Ejecutando animación de ataque
    Hit,     // Recibió daño (stun breve)
    Dead     // Muerto — listo para ser eliminado del vector
};

#endif
