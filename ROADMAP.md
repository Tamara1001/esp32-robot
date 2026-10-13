# ROADMAP

## Fase 1 — Prototipo en protoboard (actual)
- [x] Estructura del repo y documentación
- [x] Lectura HC-SR04 en ESP32 DevKit
- [x] AP WiFi + servidor web (LittleFS) + WebSocket
- [x] Joystick web → 2 servos
- [ ] Probar en hardware real y calibrar rango de servos
- [ ] Esquema de cableado (imagen en `docs/images/`)

## Fase 2 — Mínimo (MVP)
- [ ] Chasis de cartón, 4 ruedas, 4 motores DC + driver L298N
- [ ] Control de movimiento desde el joystick web (`M:`)
- [ ] Cámara ESP32-CAM con video en vivo en la web
- [ ] Cabeza giratoria (pan/tilt) con la cámara
- [ ] Sensor de distancia: frenado automático
- [ ] Buzzer
- [ ] Batería 18650 + TP4056 (carga USB-C)
- [ ] "Manitos" simples con servos

## Fase 3 — Ideal
- [ ] Impresión 3D (PETG)
- [ ] Joystick físico
- [ ] Pantalla OLED (cara / ojitos)
- [ ] Seguidor de línea (3x TCRT5000) / circuito recorrible
- [ ] Autonomía y detección de objetos
- [ ] Garra / manos complejas
- [ ] Audio: amplificador I2S + parlante, grabación, modo conversacional
- [ ] Estación de carga
- [ ] Evaluar 6 ruedas

## Preguntas abiertas
- ¿4 o 6 ruedas? (decidir antes de diseñar el chasis; más ruedas = más motores, más corriente)
- ¿Un solo ESP32-CAM o ESP32 de control + ESP32-CAM de video?
- ¿Alimentación de motores y servos: una batería o dos separadas?
