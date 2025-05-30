# 🎯 mathRoulette

**mathRoulette** es un juego de ruleta rusa hecho en C++ usando la librería SFML. Inspirado por el juego *Bullshot Roulette*, este proyecto busca mezclar mecánicas de azar con lógica matemática.

> ⚠️ El juego está en desarrollo. Todavía no está completo, pero puedes probar las mecánicas actuales.

## 🎮 Mecánicas actuales

- Puedes elegir entre dispararte o disparar a la IA.
- La IA es básica: actúa con una probabilidad aleatoria del 50%.
- Las balas (reales y falsas) se aleatorizan al inicio de cada partida.

## 🌱 Próximas mejoras

- Ejercicios de matemáticas para mejorar tus probabilidades en acciones:
  - Por ejemplo, si resuelves un ejercicio de álgebra en menos de 5 segundos, tienes 100% de éxito en disparar.
- Añadir ítems únicos que cambien el flujo del juego, al estilo Bullshot Roulette.
- Mejorar la IA para que tome decisiones estratégicas.
- Añadir efectos visuales y más animaciones con sprites personalizados.

## 🧑‍🎨 Arte

Todos los sprites fueron hechos a mano por mí usando **LibreSprite**. ¡100% original!

## 🛠️ Compilación

Este proyecto usa la biblioteca SFML. Puedes compilarlo tú mismo o ejecutar el binario ya compilado para Windows.

**Opción 1: Ejecutar directamente (Windows)**

1. Descarga el proyecto.

2. Ve a la carpeta descargada.

3. Ejecuta el archivo ruleta.exe (doble clic).

 == Asegúrate de que los archivos .dll de SFML estén en la misma carpeta que el .exe. ==

**Opción 2: Compilar desde el código fuente**

1, Abre el archivo main.cpp con tu editor favorito (Visual Studio Code recomendado).

2. Compila el archivo usando un compilador compatible con SFML.

## 🤖 Requisitos

- C++17 o superior
- SFML 2.5 o superior

## 📦 Instalación rápida (Linux)

```bash
sudo apt install g++ libsfml-dev
g++ src/main.cpp -o mathRoulette -lsfml-graphics -lsfml-window -lsfml-system
./mathRoulette
