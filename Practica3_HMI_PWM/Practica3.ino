#include <Controllino.h>
#include "Stone_HMI_Define.h"
#include "Procesar_HMI.h"

// Definición de pines
const int led1             = CONTROLLINO_D7;  // Salida digital D0 para LED 1
const int led2             = CONTROLLINO_D8;  // Salida digital D6 para LED 2
const int boton_led1       = CONTROLLINO_I16; // Botón físico para LED 1
const int boton_led2       = CONTROLLINO_I17; // Botón físico para LED 2

// Variables para LED 1
int   pwmValue1         = 0;              // valor PWM convertido (0-255)
float dutyCyclePercent1 = 0;              // valor en porcentaje (0-100)
bool  led1Enabled       = false;         // estado del LED 1 (habilitado/deshabilitado)

// Variables para LED 2
int   pwmValue2         = 0;              // valor PWM convertido (0-255)
float dutyCyclePercent2 = 0;              // valor en porcentaje (0-100)
bool  led2Enabled       = false;         // estado del LED 2 (habilitado/deshabilitado)

// Variables para control de botones con banderas
int bandera1 = 0;                         // bandera para LED 1 (0=apagado, 1=encendido)
int bandera2 = 0;                         // bandera para LED 2 (0=apagado, 1=encendido)
int estadoBoton1Anterior = LOW;           // estado anterior del botón 1
int estadoBoton2Anterior = LOW;           // estado anterior del botón 2

void setup() {
  Serial.begin(115200);   // Comunicación serial con el PC
  Serial2.begin(115200);  // Comunicación serial con el HMI
  
  // Configuración de pines
  pinMode(led1, OUTPUT);       // LED 1 como salida
  pinMode(led2, OUTPUT);       // LED 2 como salida
  pinMode(boton_led1, INPUT);  // Botón 1 como entrada
  pinMode(boton_led2, INPUT);  // Botón 2 como entrada
  
  // Inicialización del HMI
  HMI_init();             // Inicializa el sistema de colas para las respuestas el HMI
  
  // Inicializar valores en el HMI
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 0);  // SpinBox para LED 1
  Stone_HMI_Set_Value("spin_box", "spin_box2", NULL, 0);  // SpinBox para LED 2
  
  // Inicializar LEDs apagados
  analogWrite(led1, 0);
  analogWrite(led2, 0);
  
  Serial.println("Sistema iniciado - Control de dos LEDs");
  Serial.println("LED 1: SpinBox1 + Botón I17");
  Serial.println("LED 2: SpinBox2 + Botón I18");
}

void loop() {
  // Manejar botones físicos
  handleButton1();
  handleButton2();
  
  // Obtener valores de los SpinBoxes del HMI
  dutyCyclePercent1 = HMI_get_value("spin_box", "spin_box1");
  dutyCyclePercent2 = HMI_get_value("spin_box", "spin_box2");
  
  // Procesar LED 1
  processLED1();
  
  // Procesar LED 2
  processLED2();
  
  // Pequeña pausa para estabilidad
  delay(50);
}

void handleButton1() {
  // Lectura y detección de flanco ascendente para botón 1
  int estadoBoton1 = digitalRead(boton_led1);
  
  if (estadoBoton1 == HIGH && estadoBoton1Anterior == LOW) {
    bandera1 = !bandera1;  // Cambiar estado (toggle)
    
    Serial.print("Botón 1 presionado - LED 1 ");
    Serial.println(bandera1 ? "HABILITADO" : "DESHABILITADO");
    
    // Si se deshabilitó, apagar inmediatamente
    if (!bandera1) {
      analogWrite(led1, 0);
    }
  }
  estadoBoton1Anterior = estadoBoton1;
}

void handleButton2() {
  // Lectura y detección de flanco ascendente para botón 2
  int estadoBoton2 = digitalRead(boton_led2);
  
  if (estadoBoton2 == HIGH && estadoBoton2Anterior == LOW) {
    bandera2 = !bandera2;  // Cambiar estado (toggle)
    
    Serial.print("Botón 2 presionado - LED 2 ");
    Serial.println(bandera2 ? "HABILITADO" : "DESHABILITADO");
    
    // Si se deshabilitó, apagar inmediatamente
    if (!bandera2) {
      analogWrite(led2, 0);
    }
  }
  estadoBoton2Anterior = estadoBoton2;
}

void processLED1() {
  // Validar rango del SpinBox 1
  if (dutyCyclePercent1 >= 0 && dutyCyclePercent1 <= 100) {
    pwmValue1 = map(dutyCyclePercent1, 0, 100, 0, 255);  // Mapear a valores PWM
    
    // Solo aplicar PWM si el LED está habilitado por el botón
    if (bandera1) {
      analogWrite(led1, pwmValue1);
    } else {
      analogWrite(led1, 0);  // Mantener apagado si está deshabilitado
    }
    
    // Mostrar información en serial (solo cambios significativos)
    static float lastDutyCycle1 = -1;
    static bool lastEnabled1 = !bandera1;
    
    if (abs(dutyCyclePercent1 - lastDutyCycle1) > 0.5 || bandera1 != lastEnabled1) {
      Serial.print("LED 1 - Duty cycle (%): ");
      Serial.print(dutyCyclePercent1);
      Serial.print(" -> PWM: ");
      Serial.print(pwmValue1);
      Serial.print(" -> Estado: ");
      Serial.println(bandera1 ? "ON" : "OFF");
      
      lastDutyCycle1 = dutyCyclePercent1;
      lastEnabled1 = bandera1;
    }
  } else {
    Serial.println("LED 1: Ingresa un valor entre 0 y 100 en SpinBox1.");
  }
}

void processLED2() {
  // Validar rango del SpinBox 2
  if (dutyCyclePercent2 >= 0 && dutyCyclePercent2 <= 100) {
    pwmValue2 = map(dutyCyclePercent2, 0, 100, 0, 255);  // Mapear a valores PWM
    
    // Solo aplicar PWM si el LED está habilitado por el botón
    if (bandera2) {
      analogWrite(led2, pwmValue2);
    } else {
      analogWrite(led2, 0);  // Mantener apagado si está deshabilitado
    }
    
    // Mostrar información en serial (solo cambios significativos)
    static float lastDutyCycle2 = -1;
    static bool lastEnabled2 = !bandera2;
    
    if (abs(dutyCyclePercent2 - lastDutyCycle2) > 0.5 || bandera2 != lastEnabled2) {
      Serial.print("LED 2 - Duty cycle (%): ");
      Serial.print(dutyCyclePercent2);
      Serial.print(" -> PWM: ");
      Serial.print(pwmValue2);
      Serial.print(" -> Estado: ");
      Serial.println(bandera2 ? "ON" : "OFF");
      
      lastDutyCycle2 = dutyCyclePercent2;
      lastEnabled2 = bandera2;
    }
  } else {
    Serial.println("LED 2: Ingresa un valor entre 0 y 100 en SpinBox2.");
  }
}