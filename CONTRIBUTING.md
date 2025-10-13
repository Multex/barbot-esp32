# Contribuir a Barbot ESP32

Gracias por tu interés en contribuir. Este proyecto busca ser una guía reutilizable para construir un barbot con ESP32.

## Flujo de trabajo

1. Haz fork y crea una rama descriptiva (`feat/...`, `fix/...`).
2. Ejecuta `platformio run` y `platformio test -e esp32doit-devkit-v1` localmente.
3. Abre un Pull Request contra `main` con descripción clara y evidencias (logs/fotos si aplica).

## Convenciones de commit

Sigue Conventional Commits, por ejemplo:

- `feat: agregar tiempo de homing configurable`
- `fix: corregir lectura de recipes.json`
- `docs: explicar calibración de oz`

## Estilo de código

- Indentación de 2 espacios, llaves en misma línea.
- Cadenas visibles para el usuario en español.
- Evita asignaciones dinámicas en rutas críticas de tiempo.
- Coloca pines y constantes en `include/config.h`.

## Pruebas

- Ubica suites en `test/<feature>_test/` con Unity/PlatformIO.
- Cubre transiciones de estado, parseo de recetas y enclavamientos.
- Ejecuta `platformio test -e esp32doit-devkit-v1` antes de enviar PR.

## Documentación

- Mantén actualizado `README.md` y `docs/` (hardware, cableado, recetas, calibración, troubleshooting).

