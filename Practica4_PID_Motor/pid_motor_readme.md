# Control PID de Motor DC con Controllino y HMI Stone

## Descripción del Proyecto

Este proyecto implementa un sistema de control PID (Proporcional-Integral-Derivativo) para regular automáticamente la velocidad de un motor DC. El sistema utiliza un Controllino como controlador principal y una interfaz HMI Stone para la interacción del usuario y visualización de datos en tiempo real.

## Características Principales

- **Control PID Digital**: Implementación de algoritmo PID mediante ecuación de recurrencias
- **Interfaz Gráfica**: HMI Stone con controles interactivos y gráficas en tiempo real
- **Medición de RPM**: Sensor de encoder con 36 pulsos por revolución
- **Control Manual**: Botones físicos para inicio/parada del sistema
- **Visualización en Tiempo Real**: Gráficas de referencia, RPM actual y señal de control
- **Ajuste de Parámetros**: Configuración de constantes Kp, Ki y Td desde la interfaz

## Componentes del Sistema

### Hardware
- **Controllino MAXI/MEGA**: Controlador principal basado en Arduino
- **HMI Stone**: Pantalla táctil para interfaz de usuario
- **Motor DC**: Motor con encoder incorporado (36 PPR)
- **Driver de Motor**: Para control PWM del motor
- **Botones Físicos**: Inicio y parada del sistema
- **LEDs Indicadores**: Estado del sistema

### Software
- **Arduino IDE**: Entorno de desarrollo
- **Librería Controllino**: Control de hardware específico
- **Stone HMI Library**: Comunicación con la pantalla HMI
- **Librerías Personalizadas**: Procesamiento de respuestas HMI

## Esquema del Sistema

```
RPM_ref ──○── e ──┌─────────────┐── u ──┌───────┐── y
    │    -       │ Controlador │       │ Planta│
    │            │     PID     │       │       │
    └────────────┤             │       └───┬───┘
                 └─────────────┘           │
                       ↑                   │
                 ┌─────────┐               │
                 │ Sensor  │←──────────────┘
                 └─────────┘
```

## Instalación y Configuración

### Requisitos Previos
1. Arduino IDE instalado
2. Librería Controllino instalada
3. Librerías Stone HMI configuradas
4. Hardware conectado según el esquema

### Conexiones Hardware

#### Entradas Digitales
- `CONTROLLINO_I16`: Botón de inicio
- `CONTROLLINO_I17`: Botón de apagado
- `CONTROLLINO_IN1`: Entrada de pulsos del encoder

#### Salidas Digitales
- `CONTROLLINO_D0`: Señal PWM al motor
- `CONTROLLINO_D6`: LED indicador de inicio
- `CONTROLLINO_D7`: LED indicador de apagado

#### Comunicación Serial
- `Serial`: Monitor serie (115200 baud)
- `Serial2`: Comunicación con HMI Stone (115200 baud)

### Configuración del Código

#### Parámetros del Sistema
```cpp
const float T = 0.05;           // Tiempo de muestreo (50ms)
const uint16_t PULSOS_POR_REV = 36;  // Pulsos por revolución del encoder
const float fs = 1/T;           // Frecuencia de muestreo
```

#### Parámetros PID Iniciales
```cpp
float Kp = 0.391;  // Ganancia proporcional
float Ki = 1.88;   // Ganancia integral
float Td = 0;      // Tiempo derivativo
```

## Funcionamiento del Sistema

### Algoritmo de Control

El controlador PID implementa la siguiente ecuación de recurrencias:

```
ΔU(k) = Kp * [e(k) - e(k-1) + (T/Ti) * e(k) + (Td/T) * (e(k) - 2*e(k-1) + e(k-2))]
U(k) = U(k-1) + ΔU(k)
```

Donde:
- `e(k)`: Error actual (referencia - RPM medida)
- `U(k)`: Señal de control PWM (0-255)
- `T`: Período de muestreo
- `Ti = Kp/Ki`: Tiempo integral

### Flujo de Operación

1. **Inicialización**: Configuración de pines, interrupciones y valores iniciales
2. **Lectura de Referencia**: Obtención del setpoint desde el slider del HMI
3. **Medición de RPM**: Conteo de pulsos del encoder cada segundo
4. **Cálculo PID**: Aplicación del algoritmo de control
5. **Actualización PWM**: Envío de señal de control al motor
6. **Visualización**: Actualización de gráficas y valores en el HMI

### Características de Seguridad

- **Anti-windup**: Limitación de la señal de control (0-255)
- **Protección contra División por Cero**: Validación de Ki > 0
- **Control de Estado**: Sistema de banderas para operación segura

## Interfaz de Usuario (HMI)

### Controles Disponibles
- **Slider de Referencia**: Ajuste del setpoint (0-100%)
- **SpinBox Kp**: Ajuste de ganancia proporcional
- **SpinBox Ki**: Ajuste de ganancia integral  
- **SpinBox Td**: Ajuste de tiempo derivativo

### Visualización
- **Label RPM**: Velocidad actual del motor
- **Label Duty Cycle**: Porcentaje de la señal de control
- **Gráficas en Tiempo Real**:
  - Serie 1: Valor de referencia
  - Serie 2: RPM medidas
  - Serie 3: Señal de control

## Especificaciones Técnicas

### Rendimiento del Sistema
- **Tiempo de Muestreo**: 50ms (20 Hz)
- **Resolución PWM**: 8 bits (0-255)
- **Precisión del Encoder**: 36 pulsos/revolución
- **Rango de Control**: 0-100% duty cycle

### Criterios de Diseño Cumplidos
- ✅ Velocidad alcanza referencia sin sobreimpulso excesivo
- ✅ Estabilidad sin oscilaciones (error < 5%)
- ✅ Visualización en tiempo real de todas las variables
- ✅ Implementación mediante ecuación de recurrencias

## Estructura del Código

```
├── Includes y Definiciones
├── Variables Globales
│   ├── Control de Sistema
│   ├── Parámetros PID
│   ├── Variables PWM
│   └── Variables de Medición
├── setup()
│   ├── Inicialización Serial
│   ├── Configuración de Pines
│   ├── Configuración de Interrupciones
│   └── Inicialización HMI
├── loop()
│   ├── Control de Botones
│   ├── Lectura de Parámetros HMI
│   └── Actualización de Pantalla
├── ISR(TIMER1_COMPA_vect)
│   ├── Cálculo de RPM
│   ├── Algoritmo PID
│   └── Actualización PWM
└── contarPulso()
    └── Incremento de Contador
```

## Uso del Sistema

### Puesta en Marcha
1. **Conectar Hardware**: Verificar todas las conexiones
2. **Cargar Código**: Subir el programa al Controllino
3. **Configurar HMI**: Verificar comunicación con la pantalla
4. **Iniciar Sistema**: Presionar botón de inicio
5. **Ajustar Parámetros**: Configurar PID según necesidades

### Operación Normal
1. Establecer referencia de velocidad con el slider
2. Ajustar parámetros PID si es necesario
3. Monitorear respuesta del sistema en las gráficas
4. Verificar estabilidad y precisión del control

### Detención del Sistema
- Presionar botón de apagado
- El motor se detiene inmediatamente (PWM = 0)
- Los LEDs indican el estado actual

## Solución de Problemas

### Problemas Comunes
- **Motor no responde**: Verificar conexiones PWM y alimentación
- **Lecturas RPM incorrectas**: Revisar conexión del encoder
- **HMI no actualiza**: Verificar comunicación Serial2
- **Sistema inestable**: Ajustar parámetros PID

### Depuración
El sistema incluye salida por monitor serie con información detallada:
- RPM medidas
- Pulsos contados
- Valor de referencia
- Señal de control
- Parámetros PID actuales