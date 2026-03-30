#pragma once
#ifndef LANGUAGE_SYSTEM_H
#define LANGUAGE_SYSTEM_H
#include <unordered_map>
#include <string>

// ---------------------------------------------------------------------------
// LanguageSystem — sistema de localización basado en ficheros .lang.
//
// Principio Open/Closed: añadir un idioma nuevo sólo requiere crear un
// fichero <código>.lang (ej. "fr.lang") y llamar a setLanguage("fr").
// Esta clase NUNCA se modifica para soportar nuevos idiomas.
//
// Formato de los ficheros .lang:
//   # líneas de comentario (ignoradas)
//   clave=valor traducido
//   líneas vacías también son ignoradas
//
// Singleton de Meyer (thread-safe desde C++11).
// ---------------------------------------------------------------------------
class LanguageSystem {
public:
    static LanguageSystem& getInstance();

    // Carga un fichero .lang (formato clave=valor, # para comentarios).
    // Retorna false si no puede abrir el fichero.
    bool load(const std::string& path);

    // Obtiene una cadena traducida por clave.
    // Si la clave no existe retorna "[clave]" como fallback.
    const std::string& get(const std::string& key) const;

    // Cambia de idioma cargando el fichero <langCode>.lang.
    // Retorna false si el fichero no puede abrirse.
    bool setLanguage(const std::string& langCode);  // "es", "en", ...

    // Devuelve el código del idioma actualmente cargado.
    std::string currentLanguage() const;

private:
    LanguageSystem() { setLanguage("es"); }

    // No copiable ni movible — es un singleton.
    LanguageSystem(const LanguageSystem&)            = delete;
    LanguageSystem& operator=(const LanguageSystem&) = delete;

    std::unordered_map<std::string, std::string> strings;
    std::string  currentLang = "es";
    mutable std::string fallback;  // para retornar referencia en get()
};
#endif
