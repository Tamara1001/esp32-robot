# ESP32 Robot

Prototipo de robot con **ESP32**, **Arduino IDE** y **joystick web por WiFi**.

## Estado actual: Fase 1 (prototipo en protoboard)

- ESP32 DevKit normal (sin cámara todavía)
- Sensor ultrasónico HC-SR04 (distancia)
- 2 micro servos (pan / tilt) controlados desde un joystick web
- El ESP32 crea su propia red WiFi (modo AP) y sirve la página web (LittleFS) + WebSocket

## Inicio rápido

1. Armar el circuito según [docs/HARDWARE.md](docs/HARDWARE.md).
2. Arduino IDE: instalar el soporte de placas **esp32** (Espressif) y las librerías:
   - `ESP32Servo`
   - `ESP Async WebServer` + `AsyncTCP` (organización ESP32Async)
3. Subir los archivos web: plugin de LittleFS para Arduino IDE (carpeta `firmware/robot_proto/data`).
4. Subir el sketch `firmware/robot_proto/robot_proto.ino`.
5. Conectarse a la red WiFi `ESP32-Robot` (clave en `config.h`) y abrir `http://192.168.4.1`.

## Documentación

| Archivo | Contenido |
|---|---|
| [STRUCTURE.md](STRUCTURE.md) | Estructura del repositorio |
| [DESIGN.md](DESIGN.md) | Arquitectura y decisiones de diseño |
| [CODESTYLE.md](CODESTYLE.md) | Convenciones de código |
| [ROADMAP.md](ROADMAP.md) | Fases y alcance (Mínimo / Ideal) |
| [docs/HARDWARE.md](docs/HARDWARE.md) | Materiales, pines y cableado |
| [docs/PROTOCOL.md](docs/PROTOCOL.md) | Protocolo WebSocket |
| [CONTRIBUTING.md](CONTRIBUTING.md) | Flujo de trabajo |

## Licencia

MIT
