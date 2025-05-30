#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>

using namespace sf;
using namespace std;

enum class Turno { JUGADOR, IA };

struct Juego {
    vector<bool> tambor;
    int balas_reales;
    int balas_falsas;
    int indice_actual = 0;

    Turno turno = Turno::JUGADOR;
    
    bool turno_extra = false;
    bool jugador_vivo = true;
    bool ia_viva = true;

    int vidas_jugador = 1;
    int vidas_ia = 1;

    Juego() {
        // Aleatorizar cantidad de balas reales (1 a 6)
        tambor.clear();
        srand(time(nullptr));
        balas_reales = rand() % 4 + 2; // Entre 1 y 5 balas reales
        balas_falsas = 7 - balas_reales; // Total de 7 balas

        // Cargar el tambor con balas reales y falsas
        tambor = vector<bool>(6, false); 
        for (int i = 0; i < balas_reales; ++i) {
            tambor[i] = true;
        }
        // Mezcla el tambor
        shuffle(tambor.begin(), tambor.end(), default_random_engine(time(nullptr)));

        
    }

    bool disparar(bool a_jugador) {
        if (indice_actual >= tambor.size()) return false;

        bool bala = tambor[indice_actual++];
        if (bala) {
            if (a_jugador)
                vidas_jugador--;
            if (vidas_jugador <= 0) jugador_vivo = false;
            else
                vidas_ia--;
            if (vidas_ia <= 0) ia_viva = false;
        }
        return bala;
    }
};

// Cargar texturas y sprites
Texture texJugadorIdle, texJugadorApunta;
Texture texIAIdle, texIAApunta;
Texture texMesa, texBanana;
Texture texUIBalas, texBalaReal, texBalaFalsa;
Texture texCorazonIA, texCorazonJugador;
Texture texturaFondo;

Sprite sprJugador, sprIA, sprMesa, sprBanana, sprFondo, sprUIBalas;
Sprite sprJugadorApunta, sprIAApunta;

vector<Sprite> balasSprite;
vector<Sprite> corazonesJugador(3), corazonesIA(3);


bool cargarSprites() {
    return
        texJugadorIdle.loadFromFile("assets/sprites/jugador_idle.png") &&
        texJugadorApunta.loadFromFile("assets/sprites/jugador_apunta.png") &&
        texIAIdle.loadFromFile("assets/sprites/ia_idle.png") &&
        texIAApunta.loadFromFile("assets/sprites/ia_apunta.png") &&
        texMesa.loadFromFile("assets/sprites/mesa.png") &&
        texBanana.loadFromFile("assets/sprites/banana.png") &&
        texBalaReal.loadFromFile("assets/sprites/bala_real.png") &&
        texBalaFalsa.loadFromFile("assets/sprites/bala_falsa.png") &&
        texUIBalas.loadFromFile("assets/sprites/ui_balas.png") &&
        texCorazonIA.loadFromFile("assets/sprites/ia_corazon.png") &&
        texCorazonJugador.loadFromFile("assets/sprites/jugador_corazon.png") &&
        texturaFondo.loadFromFile("assets/sprites/bg.png");
}

void animarApuntado(Sprite& sprite, Vector2f inicio, Vector2f final, RenderWindow& window, const Sprite& sprFondo, const Sprite& sprMesa, const Sprite& sprJugador, const Sprite& sprIA, Text& textoLog) {
    Clock reloj;
    Time duracion = milliseconds(300); // duracion total
    while (reloj.getElapsedTime() < duracion) {
        float progreso = reloj.getElapsedTime().asSeconds() / duracion.asSeconds();

        // Interpolacion de posicion
        Vector2f pos;
        pos.x = inicio.x + (final.x - inicio.x) * progreso;
        pos.y = inicio.y + (final.y - inicio.y) * progreso;
        sprite.setPosition(pos);

        // Fade in
        Color c = sprite.getColor();
        c.a = static_cast<sf::Uint8>(255 * progreso);
        sprite.setColor(c);

        // Redibujar frame por frame
        window.clear();
        window.draw(sprFondo);
        window.draw(sprIA);
        window.draw(sprMesa);
        window.draw(sprJugador);
        window.draw(sprite);
        window.draw(textoLog);
        window.display();
    }

    // Al final, asegurarse de que esté en la posicion final con opacidad total
    sprite.setPosition(final);
    sprite.setColor(Color(255, 255, 255, 255));
}


void agregarLog(std::vector<std::string>& historial, const std::string& mensaje, Text& textoLog, int maxLineas = 15) {
    historial.push_back(mensaje);

    if (historial.size() > maxLineas) {
        historial.erase(historial.begin());  // Elimina la línea más antigua
    }

    std::string textoCompleto;
    for (const auto& linea : historial) {
        textoCompleto += linea + "\n";
    }

    textoLog.setString(textoCompleto);
}


int main(){
    std::vector<std::string> historialLog;

    RenderWindow window(VideoMode(1366, 768), "Ruleta Banana");    
    window.setFramerateLimit(60);
    Font font;
    if (!font.loadFromFile("assets/fonts/NFPixels-Regular.ttf")) {
        cerr << "Error al cargar la fuente.\n";
        return -1;
    }
    if (!cargarSprites()) {
        cerr << "Error al cargar sprites.\n";
        return -1;
    }
    
    sprFondo.setTexture(texturaFondo);
    sprFondo.setPosition(0, 0); // Fondo ocupa toda la ventana

    sprJugador.setTexture(texJugadorIdle);
    sprJugador.setPosition(500, 400); // Ajusta según diseño
    
    sprIA.setTexture(texIAIdle);
    sprIA.setPosition(560, 20);
    
    sprJugadorApunta.setTexture(texJugadorApunta);
    sprJugadorApunta.setPosition(760, 430); // Mismo lugar que el jugador

    sprIAApunta.setTexture(texIAApunta);
    sprIAApunta.setPosition(390, 100); // Mismo lugar que la IA

    sprMesa.setTexture(texMesa);
    sprMesa.setPosition(400, 250);

    sprBanana.setTexture(texBanana);
    sprBanana.setPosition(600, 260); // Encima de la mesa

    sprUIBalas.setTexture(texUIBalas);
    sprUIBalas.setPosition(230, 230);
    
    // Posicion de vidas
    for (int i = 0; i < 3; ++i) {
        corazonesJugador[i].setTexture(texCorazonJugador);
        corazonesJugador[i].setPosition(40+ i * 85, 600);
        corazonesIA[i].setTexture(texCorazonIA);
        corazonesIA[i].setPosition(1230 - i * 85, 30);
    }
    
    Text textoLog, textoSeleccionA, textoSeleccionB;
    textoLog.setFont(font);
    textoLog.setCharacterSize(20);
    textoLog.setFillColor(Color::White);
    textoLog.setPosition(960, 150);

    textoSeleccionA.setFont(font);
    textoSeleccionA.setCharacterSize(32);
    textoSeleccionA.setFillColor(Color::White);
    textoSeleccionA.setString("A: Dispararte");
    textoSeleccionA.setPosition(450, 300);

    textoSeleccionB.setFont(font);
    textoSeleccionB.setCharacterSize(32);
    textoSeleccionB.setFillColor(Color::White);
    textoSeleccionB.setString("B: Disparar a la IA");
    textoSeleccionB.setPosition(450, 350);

    Juego juego;
    agregarLog(historialLog, "Tu turno. Presiona A o B.", textoLog);

    bool jugadorApunta = false;
    bool iaApunta = false;
    
    // Mostrar cantidad de balas como iconos encima de UI
    auto actualizarBalasGraficas = [&](Juego& juego) {
        balasSprite.clear();
        for (int i = 0; i < juego.tambor.size(); ++i) {
            Sprite b;
            b.setTexture(juego.tambor[i] ? texBalaReal : texBalaFalsa);
            b.setPosition(295, 242 + i * 43); // Encima de UI de balas
            balasSprite.push_back(b);
        }
    };

    actualizarBalasGraficas(juego);
    
    auto pantallaBlanca = [&](RenderWindow& win) {
        RectangleShape flash(Vector2f(1366, 768));
        flash.setFillColor(Color::White);
        win.draw(flash);
        win.display();
        sleep(milliseconds(200));
    };

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && juego.turno == Turno::JUGADOR && juego.jugador_vivo && juego.ia_viva) {
                if (event.key.code == Keyboard::A) {
                    Vector2f inicioJ = {sprJugadorApunta.getPosition().x + 100, sprJugadorApunta.getPosition().y + 10};
                    Vector2f finalJ  = {sprJugadorApunta.getPosition().x + 30, sprJugadorApunta.getPosition().y + 10};
                    sprJugadorApunta.setColor(Color(255, 255, 255, 0)); // transparente
                    animarApuntado(sprJugadorApunta, inicioJ, finalJ, window, sprFondo, sprMesa, sprJugador, sprIA, textoLog);
                    sleep(seconds(1));
                    bool muerte = juego.disparar(true);
                    agregarLog(historialLog, "El jugador se disparo...", textoLog);
                    if (muerte) {
                        pantallaBlanca(window);
                        agregarLog(historialLog, "Perdiste.", textoLog);
                        juego.turno = Turno::IA;
                    } else {
                        agregarLog(historialLog, "Sobreviviste. Puedes disparar.", textoLog);
                        juego.turno_extra = true;
                    }
                    if (!juego.turno_extra) juego.turno = Turno::IA;
                    Juego();
                    actualizarBalasGraficas(juego);
                    for (int i = 0; i < 3; ++i) {
                        corazonesJugador[i].setColor(i < juego.vidas_jugador ? Color::White : Color(255, 255, 255, 50));
                        corazonesIA[i].setColor(i < juego.vidas_ia ? Color::White : Color(255, 255, 255, 50));
                    }
                    
                } else if (event.key.code == Keyboard::B) {
                    
                    Vector2f inicioJ = {sprIAApunta.getPosition().x - 100, sprIAApunta.getPosition().y + 10};
                    Vector2f finalJ  = {sprIAApunta.getPosition().x - 30, sprIAApunta.getPosition().y + 10};
                    sprIAApunta.setColor(Color(255, 255, 255, 0)); // transparente
                    animarApuntado(sprIAApunta, inicioJ, finalJ, window, sprFondo, sprMesa, sprJugador, sprIA, textoLog);

                    bool muerte = juego.disparar(false);
                    agregarLog(historialLog, "Disparaste a la IA.", textoLog);

                    if (muerte) {
                        pantallaBlanca(window);
                        agregarLog(historialLog, "La IA murio. Ganaste.", textoLog);
                    } else {
                        agregarLog(historialLog, "La IA sobrevivio. Ahora es su turno.", textoLog);

                    }
                    juego.turno = Turno::IA;
                    Juego();
                    actualizarBalasGraficas(juego);
                    for (int i = 0; i < 3; ++i) {
                        corazonesJugador[i].setColor(i < juego.vidas_jugador ? Color::White : Color(255, 255, 255, 50));
                        corazonesIA[i].setColor(i < juego.vidas_ia ? Color::White : Color(255, 255, 255, 50));
                    }
                    
                }
            }
        }
        
        // Turno de la IA
        if (juego.turno == Turno::IA && juego.jugador_vivo && juego.ia_viva) {
            bool decision = rand() % 2 == 0;
            bool muerte = juego.disparar(!decision);
            if (decision) {
                Vector2f inicioIA = {sprIAApunta.getPosition().x - 100, sprIAApunta.getPosition().y + 10};
                Vector2f finalIA  = {sprIAApunta.getPosition().x - 30, sprIAApunta.getPosition().y + 10};
                sprIAApunta.setColor(Color(255, 255, 255, 0)); // transparente
                animarApuntado(sprIAApunta, inicioIA, finalIA, window, sprFondo, sprMesa, sprJugador, sprIA, textoLog);
                sleep(seconds(1));
                agregarLog(historialLog, "La IA se disparo.", textoLog);
                if (muerte) {
                    pantallaBlanca(window);
                    agregarLog(historialLog, "La IA murio. Ganaste.", textoLog);
                } else {
                    agregarLog(historialLog, "La IA sobrevivio y vuelve a intentarlo.", textoLog);
                    juego.turno_extra = true;
                }
            } else {
                Vector2f inicioIA = {sprJugadorApunta.getPosition().x + 100, sprJugadorApunta.getPosition().y + 10};
                Vector2f finalIA  = {sprJugadorApunta.getPosition().x + 30, sprJugadorApunta.getPosition().y + 10};
                sprJugadorApunta.setColor(Color(255, 255, 255, 0)); // transparente
                animarApuntado(sprJugadorApunta, inicioIA, finalIA, window, sprFondo, sprMesa, sprJugador, sprIA, textoLog);
                sleep(seconds(1));
                agregarLog(historialLog, "La IA te disparo.", textoLog);
                if (muerte) {
                    pantallaBlanca(window);
                    agregarLog(historialLog, "Moriste. Perdiste.", textoLog);
                } else agregarLog(historialLog, "Sobreviviste. Es tu turno.", textoLog);
                ;
            }

            if (!(juego.turno_extra && juego.ia_viva))
                juego.turno = Turno::JUGADOR;

            juego.turno_extra = false;
            Juego();
            actualizarBalasGraficas(juego);
            for (int i = 0; i < 3; ++i) {
                corazonesJugador[i].setColor(i < juego.vidas_jugador ? Color::White : Color(255, 255, 255, 50));
                corazonesIA[i].setColor(i < juego.vidas_ia ? Color::White : Color(255, 255, 255, 50));
            }
        }
    
        window.clear();
        window.draw(sprFondo);
        window.draw(sprIA);
        window.draw(sprMesa);
        window.draw(sprJugador);
        window.draw(sprUIBalas);

        if (jugadorApunta)
            window.draw(sprJugadorApunta);
        
        if (iaApunta)
            window.draw(sprIAApunta);

        
        for (auto& b : balasSprite)
            window.draw(b);
        window.draw(textoLog);
        if (juego.turno == Turno::JUGADOR && juego.jugador_vivo && juego.ia_viva) {
            window.draw(textoSeleccionA);
            window.draw(textoSeleccionB);
        }

        for (int i = 0; i < 3; ++i) {
            window.draw(corazonesJugador[i]);
            window.draw(corazonesIA[i]);
        }

        window.display();
        
    }
        
    return 0;
}