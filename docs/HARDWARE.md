# HARDWARE — Fase 1

## Materiales

| Cant. | Componente |
|---|---|
| 1 | ESP32 DevKit (30 pines) |
| 1 | Protoboard |
| 1 | Sensor ultrasónico HC-SR04 |
| 2 | Micro servo (SG90 o similar) |
| 1 | Resistencia 1kΩ + 1 resistencia 2kΩ (divisor para ECHO) |
| 1 | Capacitor 470–1000 µF (alimentación servos) |
| 1 | Fuente 5V (USB/power bank con módulo, o 4xAA) |
| — | Cables jumper |

## Pinout

| Función | Componente | GPIO ESP32 |
|---|---|---|
| Servo pan | señal (naranja) | 18 |
| Servo tilt | señal (naranja) | 19 |
| HC-SR04 TRIG | Trig | 5 |
| HC-SR04 ECHO | Echo (vía divisor) | 4 |

Los pines se definen en [`config.h`](../firmware/robot_proto/config.h).

## Alimentación

| Componente | Alimentación |
|---|---|
| ESP32 | USB (5V) |
| HC-SR04 | 5V (VIN del ESP32 si va por USB) |
| Servos | 5V externo (riel de la protoboard) |

**GND común** entre ESP32, servos, sensor y fuente externa.

## Divisor de tensión para ECHO

```
HC-SR04 Echo ──[1kΩ]──┬── GPIO4
                      │
                    [2kΩ]
                      │
                     GND
```
Salida: 5V × 2/3 ≈ 3.3V.

## Advertencias

- No alimentar servos desde el pin 3V3.
- Conectar el ECHO directo a 5V puede dañar el ESP32.
- Si el ESP32 se reinicia al mover servos: agregar capacitor o fuente separada.
- Evitar GPIO 0, 2, 12, 15 (strapping) para cosas críticas.
