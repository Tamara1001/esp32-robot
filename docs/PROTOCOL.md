# PROTOCOL

Endpoint: `ws://192.168.4.1/ws`. Mensajes de texto plano.

## Cliente → ESP32

| Mensaje | Descripción | Rango |
|---|---|---|
| `S:<pan>,<tilt>` | Posición de los servos en grados | 0–180 (se limita en el firmware) |

Ejemplo: `S:90,45`

## ESP32 → Cliente

| Mensaje | Descripción |
|---|---|
| `D:<cm>` | Distancia en cm cada 200 ms; `-1.0` = sin eco / fuera de rango |

## Reservados (futuro)

| Prefijo | Uso |
|---|---|
| `M:` | Motores (dirección / velocidad) |
| `B:` | Buzzer |
| `A:` | Modo autónomo |

Reglas: mensajes < 32 bytes, un mensaje por frame, los desconocidos se ignoran.
