# PROTOCOL

Endpoint: `ws://192.168.4.1/ws`. Mensajes de texto plano.

## Cliente → ESP32

| Mensaje | Descripción | Rango |
|---|---|---|
| `S:<pan>,<tilt>` | Posición de los servos en grados | 0–180 (se limita en el firmware) |
| `T:<deg>` | Ángulo objetivo del stepper 28BYJ-48 | 0–359 |

Ejemplo: `S:90,45`, `T:180`

## ESP32 → Cliente

| Mensaje | Descripción |
|---|---|
| `D:<cm>` | Distancia en cm cada 200 ms; `-1.0` = sin eco / fuera de rango |
| `P:<percent>` | Potenciómetro 0–100, solo cuando cambia ≥2% |

El buzzer y el LED se activan localmente en el firmware cuando la distancia es menor a `ALERT_DISTANCE_CM`.

## Reservados (futuro)

| Prefijo | Uso |
|---|---|
| `M:` | Motores (dirección / velocidad) |
| `B:` | Buzzer |
| `A:` | Modo autónomo |

Reglas: mensajes < 32 bytes, un mensaje por frame, los desconocidos se ignoran.
