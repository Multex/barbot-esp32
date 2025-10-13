# Calibración

## Homing

1. Asegúrate de que el endstop en `ENDSTOP_PIN` se acciona con la base en origen.
2. Enciende y observa el log: debe detectar el origen y poner `currentDispenser = 1`.

## Pasos por dispensador

- `STEPS_PER_REVOLUTION` y `TOTAL_VUELTAS` definen los pasos totales de la base.
- `TOTAL_DISPENSADORES` define el número de posiciones válidas.
- El paso entre dispensadores se calcula como:

```
pasosPorDisp = (STEPS_PER_REVOLUTION * TOTAL_VUELTAS) / (TOTAL_DISPENSADORES - 1)
```

Ajusta `TOTAL_VUELTAS` para que el giro completo pase por todos los dispensadores y regrese a origen con precisión.

## Volumen (oz)

La rutina `serveOz(oz)` realiza ciclos de avance/retroceso del motor del dispensador con retardos fijos. Ajusta:

- Velocidad del `Stepper` del dispensador (`serveStepper.setSpeed(...)`).
- Número de pasos por onza y retardos entre ciclos para tu bomba/rosca.

Repite con medidas reales (vaso medidor) hasta que 1 oz sea consistente.

