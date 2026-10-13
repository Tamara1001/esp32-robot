# STRUCTURE

```
esp32-robot/
├── README.md
├── STRUCTURE.md
├── DESIGN.md
├── CODESTYLE.md
├── ROADMAP.md
├── CONTRIBUTING.md
├── LICENSE
├── .gitignore
├── .editorconfig
├── docs/
│   ├── HARDWARE.md        # BOM, pinout, cableado
│   ├── PROTOCOL.md        # Mensajes WebSocket
│   └── images/            # Esquemas, fotos, capturas
├── firmware/
│   └── robot_proto/       # Sketch Fase 1 (nombre de carpeta = nombre del .ino)
│       ├── robot_proto.ino
│       ├── config.h       # Pines, WiFi, constantes
│       └── data/          # Web servida desde LittleFS
│           ├── index.html
│           ├── style.css
│           └── app.js
├── hardware/              # (futuro) modelos 3D (STL/STEP), esquemas
└── tools/                 # (futuro) scripts auxiliares
```

## Reglas

- Cada fase nueva de firmware va en su propia carpeta bajo `firmware/` (ej. `robot_drive`, `robot_cam`) cuando el cambio sea grande; si no, se evoluciona la existente.
- Todo lo que sea configurable (pines, SSID, límites) va en `config.h`, nunca hardcodeado en el `.ino`.
- La web vive solo en `data/`, sin build ni dependencias externas (debe funcionar sin internet, ya que el AP no tiene salida).
- Documentación en español, código en inglés.
