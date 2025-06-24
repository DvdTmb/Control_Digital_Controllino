clc; clear; close all;

% Se necesita la librería Control System Toolbox

% Cargar los datos del CSV en formato de matriz
datos = readmatrix('motorsebas');  % Asegúrate que el archivo se llama así

% Asignar las columnas a variables
tiempo_t = datos(:,1);
excitacion_sistema_u = datos(:,2);
respuesta_sistema_y = datos(:,3);

% Obtener valores de líneas guía:
Linea_inicio = min(respuesta_sistema_y); 
Linea_final = max(respuesta_sistema_y); 

% Variación en entrada y salida
Delta_u = max(excitacion_sistema_u) - min(excitacion_sistema_u);
delta_y = max(respuesta_sistema_y) - min(respuesta_sistema_y);

% ----- 1) Método de Ziegler & Nichols -----
k1 = delta_y / Delta_u;
theta1 = 0.35;
tau1 = 1.5 - theta1;

disp('-----Modelo de Ziegler & Nichols-----')
G1 = tf([k1], [tau1 1], 'InputDelay', theta1);

% Simulación con escalón de 4.976
[y1, t1] = step(G1);
y1 = y1 * 4.976; % entrada experimental

% ----- 2) Método de Miller -----
k2 = k1;
theta2 = 0.35;

% Tiempo al 63.21% de salida
porcentaje_63 = find(respuesta_sistema_y >= 0.6321*delta_y + Linea_inicio, 1);
tiempo_63 = tiempo_t(porcentaje_63);
tau2 = tiempo_63 - theta2;

disp('------Modelo de Miller------')
G2 = tf([k2], [tau2 1], 'InputDelay', theta2);
[y2, t2] = step(G2);
y2 = y2 * 4.976;

% ----- 3) Método Analítico -----
k3 = k1;
porcentaje_28 = find(respuesta_sistema_y >= 0.284*delta_y + Linea_inicio, 1);

syms theta3 tau3
eq = solve(theta3 + tau3 == tiempo_t(porcentaje_63), ...
           theta3 + tau3/3 == tiempo_t(porcentaje_28));
theta3 = double(eq.theta3);
tau3 = double(eq.tau3);

disp('------Modelo Analítico ------')
G3 = tf([k3], [tau3 1], 'InputDelay', theta3);
[y3, t3] = step(G3);
y3 = y3 * 4.976;

% ----- Gráfica -----
figure
plot(tiempo_t, respuesta_sistema_y, 'k', 'LineWidth', 1.5) % Real
hold on
plot(tiempo_t, excitacion_sistema_u, 'b--')               % Entrada real
plot(t1, y1, 'r', t2, y2, 'g', t3, y3, 'm', 'LineWidth', 1.2)

% Líneas guía
plot([0.35 1.5], [Linea_inicio Linea_final], 'k--', ...
     [0 tiempo_t(end)], [Linea_inicio Linea_inicio], 'b--', ...
     [0 tiempo_t(end)], [Linea_final Linea_final], 'c--')

legend('Respuesta del Proceso', 'Excitación del sistema', ...
       'Ziegler & Nichols', 'Miller', 'Analítico', ...
       'Línea tangente', 'Línea base', 'Línea 100%')

xlabel('Tiempo (s)');
ylabel('y(t)');
title('\bf\color{red}Modelos del Sistema')
axis([0 tiempo_t(end) -0.1 Linea_final + 1])
grid on
