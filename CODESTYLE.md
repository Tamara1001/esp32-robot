# CODESTYLE

Documentación en **español**, código e identificadores en **inglés**.

## C++ / Arduino (firmware)

- Indentación: 2 espacios. Sin tabs.
- Funciones y variables: `camelCase` (`readDistanceCm`). Constantes y macros: `UPPER_SNAKE_CASE` (`PIN_TRIG`). Clases: `PascalCase`.
- Prefijos de pines: `PIN_<COMPONENTE>` ; unidades en el nombre (`_MS`, `_US`, `_CM`, `_DEG`).
- Nada de números mágicos: van a `config.h`.
- Evitar `delay()` en `loop()`; usar `millis()`. (Excepción: pulsos cortos de microsegundos del HC-SR04.)
- Evitar `String` en rutas calientes; preferir `char[]`.
- No hacer trabajo pesado dentro de callbacks del WebSocket: parsear, guardar estado y salir.
- Comentarios solo para explicar el *por qué*. Sin código comentado.
- Entradas externas (WebSocket) siempre validadas / `constrain()`.

## JavaScript (web)

- ES6+, `"use strict"`, sin frameworks ni CDN (la red es local, sin internet).
- 2 espacios, comillas dobles, punto y coma.
- `camelCase` funciones/variables, `UPPER_SNAKE_CASE` constantes.
- `const` por defecto; `let` solo si se reasigna; nada de `var`.
- Limitar la tasa de envío al ESP32 (throttle).

## HTML / CSS

- Clases en `kebab-case` o `camelCase` consistente con el existente; IDs en `camelCase`.
- Colores y espaciados vía variables CSS (`:root`).
- Mobile-first; debe usarse con el pulgar en un celular.

## Git

- Commits en formato [Conventional Commits](https://www.conventionalcommits.org/): `feat:`, `fix:`, `docs:`, `refactor:`, `chore:`.
- Mensajes en inglés, imperativo, <72 caracteres.
- Ramas: `main` estable; trabajo en `feat/<tema>`.
- Nunca commitear credenciales WiFi reales: usar los valores de ejemplo de `config.h`.
