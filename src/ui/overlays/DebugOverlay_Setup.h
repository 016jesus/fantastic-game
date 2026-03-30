#pragma once
// ---------------------------------------------------------------------------
// DebugOverlay_Setup.h
//
// Documentación de integración: cómo registrar las líneas de debug desde
// PlayingState. NO incluir este archivo en la compilación.
//
// Llamar desde PlayingState::onEnter() después de inicializar los datos del
// juego, para que las lambdas capturen referencias válidas:
//
//   #include "ui/overlays/DebugOverlay.h"
//
//   void PlayingState::onEnter() {
//       // ... inicialización existente ...
//
//       auto& dbg = DebugOverlay::getInstance();
//
//       // Posición del jugador en píxeles (coordenadas de mundo).
//       dbg.registerLine("Player X", [&]{
//           Skins* s = player.getSkin();
//           if (!s || !s->getSprite()) return std::string("?");
//           return std::to_string(static_cast<int>(s->getSprite()->getPosition().x));
//       });
//       dbg.registerLine("Player Y", [&]{
//           Skins* s = player.getSkin();
//           if (!s || !s->getSprite()) return std::string("?");
//           return std::to_string(static_cast<int>(s->getSprite()->getPosition().y));
//       });
//
//       // Vida del jugador.
//       dbg.registerLine("Player HP", [&]{
//           return std::to_string(player.getVida());
//       });
//
//       // Número de Skeletons vivos en la sección actual.
//       dbg.registerLine("Enemies", [&]{
//           return std::to_string(enemies.size());
//       });
//
//       // Nombre de la sección activa.
//       dbg.registerLine("Section", [&]{
//           return seccionManager.current()
//                  ? seccionManager.current()->getNombre()
//                  : std::string("?");
//       });
//
//       // Monedas del jugador.
//       dbg.registerLine("Money", [&]{
//           return std::to_string(player.getDinero());
//       });
//   }
//
// Además, en PlayingState::handleEvent() añadir:
//   DebugOverlay::getInstance().handleEvent(event);
//
// En PlayingState::update() añadir:
//   DebugOverlay::getInstance().update(deltaTime);
//
// En PlayingState::render() añadir (al final, encima del overlay de pausa):
//   DebugOverlay::getInstance().draw(window);
//
// NOTA: setFont() debe llamarse una sola vez con la fuente del juego.
// El lugar natural es en el constructor de PlayingState, después de cargar
// la fuente:
//   DebugOverlay::getInstance().setFont(font);
//
// ---------------------------------------------------------------------------
