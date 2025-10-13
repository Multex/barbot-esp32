# Recetas (recipes.json)

El archivo `recipes.json` en la raíz de la tarjeta SD define las bebidas disponibles. Cada entrada es un arreglo de pasos con dos campos:

- `disp`: índice de dispensador (1..TOTAL_DISPENSADORES)
- `oz`: onzas a servir en ese paso

Ejemplo:

```json
{
  "VodkaCola": [
    { "disp": 2, "oz": 1 },
    { "disp": 1, "oz": 2 }
  ]
}
```

## Recomendaciones

- Verifica que `disp` está dentro de rango según `include/config.h` (`TOTAL_DISPENSADORES`).
- Ajusta `oz` según el caudal real de tu hardware (ver `docs/CALIBRATION.md`).
- Agrupa recetas por categorías (con alcohol / sin alcohol) para una UI más clara.
- Guarda el archivo como UTF-8 sin BOM.

## Validación rápida

- Al iniciar, el firmware intenta parsear `recipes.json`; si falla, se detiene y lo reporta por serie.
- Puedes iniciar recetas por consola serie enviando el nombre exacto cuando el estado esté en reposo (`IDLE`).

