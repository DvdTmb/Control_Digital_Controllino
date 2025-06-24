# Práctica 2: Control FSM con Controllino Mega

Implementación de un control mediante máquina de estados finita (FSM):
- 2 botones controlan encendido/apagado de un LED.

## Objetivo:
Comprender FSM, uso de `if`, `switch`, manejo de entradas digitales.

## Archivos:
- Practica2.ino: Código de FSM.

## Instrucciones:
Parte A
Utilizar los tres botones del tablero de pruebas para controlar el patrón de
encendido de los LED ubicados en forma de matriz 3x3. Tanto los botones
como los LED ya se encuentran conectados directamente al Controllino
Mega.
– Botón 1: Encendido en espiral normal.
– Botón 2: Encendido en espiral inverso.
– Botón 3: Reinicia y apaga todos los LEDs.
Parte B
Diseñar un sistema que controle dos semáforos (Semáforo A y Semáforo B)
ubicados en una intersección perpendicular (Figura 18), usando el enfoque de
máquina de estados finita (FSM). El sistema debe simular el comportamiento
simple de los semáforos, de manera que nunca haya luz verde simultánea
para ambos sentidos, y se respeten los tiempos estándar de duración de
cada luz.
