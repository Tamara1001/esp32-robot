# HARDWARE — Fase 1

## Inventario disponible

| Componente | Cant. | Uso en Fase 1 |
|---|---|---|
| ESP32-S (DevKit 38 pines) | 1 | Controlador |
| Protoboard | 1 | Montaje |
| Servo 9g (SG90 o similar) | 2 | Pan y tilt (cabeza) |
| Motor paso a paso 28BYJ-48 + placa ULN2003 | 1 | Eje extra (control `T:`) |
| Potenciómetro 10K | 1 | Entrada analógica: joystick físico de prueba / ajuste |
| HC-SR04 | 1 | Distancia |
| Buzzer activo (y pasivos) | varios | Alerta de proximidad (usar el activo en GPIO 23) |
| LEDs | varios | Estado / indicadores |
| Resistencias 220Ω, 1kΩ, 10kΩ | varias | LEDs (220Ω), divisor de ECHO (1kΩ), pull-ups (10kΩ) |

### Faltantes
- **Resistencia de 2kΩ:** no hace falta, el divisor se arma con 3×1kΩ (ver abajo).
- **Para la batería (después):** boost 5V ≥2A, 18650 y portabaterías.

## Pinout (ESP32 DevKit 38 pines)

| Función | Componente | GPIO |
|---|---|---|
| Servo pan | señal | 18 |
| Servo tilt | señal (si hay 2do servo) | 19 |
| HC-SR04 TRIG | Trig | 5 |
| HC-SR04 ECHO | Echo (vía divisor) | 4 |
| Potenciómetro | pin central | 34 (ADC1, solo entrada) |
| Buzzer | + (vía GPIO) | 23 |
| LED de estado | ánodo vía 220Ω | 2 (LED integrado) / 25 externo |

Los pines se definen en [`config.h`](../firmware/robot_proto/config.h).

> [!NOTE]
> El potenciómetro va en un pin **ADC1** (GPIO 32–39). Los pines ADC2 no funcionan con WiFi activo. GPIO 34–39 son solo entrada y sin pull-up interno.

## Alimentación

| Componente | Alimentación |
|---|---|
| ESP32 | USB (5V) |
| HC-SR04 | 5V (pin VIN del ESP32 si va por USB) |
| Servo 9g | 5V (VIN del ESP32 sirve para 1 servo en pruebas; con 2 o con carga, fuente externa) |
| 28BYJ-48 + ULN2003 | 5V externo (consume ~240 mA, no usar 3V3) |
| Potenciómetro | 3V3 y GND (nunca 5V en un pin ADC) |

**GND común** entre todo. Para pruebas con batería: 1×18650 + boost a 5V.

## Divisor de tensión para ECHO (con lo que tenés)

```
HC-SR04 Echo ──[1kΩ]──┬── GPIO4
                      │
                    [1kΩ]
                      │
                    [1kΩ]
                      │
                     GND
```
Dos resistencias de 1kΩ en serie hacen 2kΩ. Salida: 5V × 2/3 ≈ 3.3V.

## Orden de armado sugerido

1. **LED + 220Ω** en GPIO 25: probar que el ESP32 y la placa funcionan.
2. **HC-SR04** con divisor: ver la distancia por el monitor serie y en la web.
3. **Servo** en GPIO 18: moverlo desde el joystick web.
4. **Buzzer** en GPIO 23: pitido cuando la distancia < 15 cm.
5. **Potenciómetro** en GPIO 34: leer valor y usarlo como segundo control de prueba.
6. **28BYJ-48** (si hay ULN2003): IN1–IN4 a GPIO 13, 14, 27, 26.

## Advertencias

- No alimentar servos ni el 28BYJ-48 desde el pin 3V3.
- Conectar el ECHO directo a 5V puede dañar el ESP32.
- Si el ESP32 se reinicia al mover el servo: agregar capacitor 470–1000 µF o fuente separada.
- Evitar GPIO 0, 2, 12, 15 para cosas críticas (strapping).
- Un buzzer **activo** suena con solo dar HIGH; uno **pasivo** necesita una señal PWM/tono. Verificar cuál tenés.
