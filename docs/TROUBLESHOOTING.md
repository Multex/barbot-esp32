# Troubleshooting

## SD mount failed!

- Revisa cableado SPI y `SD_CS_PIN`.
- Formatea la SD en FAT32 y coloca `recipes.json` en la raíz.

## No aparece `barbot.local`

- mDNS puede tardar unos segundos. Prueba por IP mostrada en serie.
- Asegura que tu red permite mDNS/Multicast.

## Portal WiFi no aparece

- Mantén pulsado el botón (IO0) durante el arranque para resetear configuración.
- `wm.resetSettings()` se ejecuta si `RESET_BUTTON_PIN` está en bajo al iniciar.

## Motor vibra o salta pasos

- Verifica orden de fases en L298N.
- Reduce velocidad del `Stepper`.
- Revisa alimentación y masa común.

## Receta no encontrada

- El nombre debe coincidir exactamente con la clave en `recipes.json`.
- Valida índices de `disp` (1..TOTAL_DISPENSADORES).

