#include "raylib.h"

bool inMainInterface = true;
bool CreditsInterface = false;
bool inChooseCharacterInterface = false;
bool PigGame = false;

void ShowPigGame();
void ShowCreditsInterface();
void ShowChooseCharacterInterface();

int main() {
    // Inicializa a janela
    InitWindow(768, 768, "Exemplo com Raylib");
    SetTargetFPS(60);  // Define a taxa de frames para 60 FPS

    // Carrega a textura do fundo
    Texture2D fundo = LoadTexture("fundo.png");

    // Define a área (retângulo) onde você quer detectar o clique
    Rectangle creditos = { 260, 432, 220, 83 };  // x, y, largura, altura
    Rectangle play = { 276, 324, 191, 83 };

    while (!WindowShouldClose()) {  // Loop principal
        if (inMainInterface) {
            // Obtém a posição do mouse
            Vector2 mousePosition = GetMousePosition();

            // Verifica se o botão esquerdo do mouse foi pressionado dentro da área
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(mousePosition, creditos)) {
                inMainInterface = false;
                CreditsInterface = true;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(mousePosition, play)) {
                inMainInterface = false;
                inChooseCharacterInterface = true;
            }

            // Desenho na tela
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Desenhar o fundo
            DrawTexture(fundo, 0, 0, RAYWHITE);

            // Verifica se o mouse está na área e muda a cor do retângulo
            if (CheckCollisionPointRec(mousePosition, creditos)) {
                DrawRectangleLinesEx(creditos, 3, RED);
            } else {
                DrawRectangleLinesEx(creditos, 3, BLACK);
            }
            if (CheckCollisionPointRec(mousePosition, play)) {
                DrawRectangleLinesEx(play, 3, RED);
            } else {
                DrawRectangleLinesEx(play, 3, BLACK);
            }

            EndDrawing();
        } else if (inChooseCharacterInterface) {
            ShowChooseCharacterInterface();
        } else if (CreditsInterface) {
            ShowCreditsInterface();
        }
    }

    // Libera a memória da textura do fundo
    UnloadTexture(fundo);

    // Fecha a janela e limpa os recursos
    CloseWindow();

    return 0;
}

void ShowPigGame() {
    typedef struct Canos {
        Rectangle rec;
        Texture2D texturaCano;
        float scale;
        Color color;
        bool active;
    } Canos;

    typedef struct Animal {
        Rectangle texturaRect;
        Texture2D texturaAnimal;
    } Animal;

    const int width = 768;
    const int height = 768;

    float X = 0;
    float Y = 300;
    float velocidade = 0;
    bool vivo = true;

    // Carrega as texturas
    Texture2D canoTextura = LoadTexture("cano.png");

    Animal animal;
    animal.texturaAnimal = LoadTexture("personagem.png");

    animal.texturaRect.width = 80;
    animal.texturaRect.height = 180;
    animal.texturaRect.x = X;
    animal.texturaRect.y = Y;

    static Canos canos[200] = { 0 };
    static Vector2 canosPos[100] = { 0 };
    static int canosVelocidadeX = 0;

    int screenUpperLimit = 40;
    bool pause = false;

    bool collisionB = false;
    bool collisionC = false;

    // Inicialização dos canos
    for (int i = 0; i < 100; i++) {
        canosPos[i].x = 600 + 380 * i;
        canosPos[i].y = -GetRandomValue(0, 150);
    }

    while (!WindowShouldClose() && !inMainInterface) {
    if (IsKeyPressed(KEY_W)) {
        inChooseCharacterInterface = true;  // Define o estado de seleção de personagem como verdadeiro
        PigGame = false;  // Define o estado do jogo PigGame como falso
        return;  // Retorna imediatamente para a seleção de personagem
    }

        if (!pause && vivo) {
            canosVelocidadeX = 2;
            for (int i = 0; i < 100; i++) canosPos[i].x -= canosVelocidadeX;

            for (int i = 0; i < 200; i += 2) {
                canos[i].rec.x = canosPos[i / 2].x;
                canos[i].rec.y = canosPos[i / 2].y;
                canos[i].rec.width = 100;
                canos[i].rec.height = 300;

                canos[i + 1].rec.x = canosPos[i / 2].x;
                canos[i + 1].rec.y = 600 + canosPos[i / 2].y - 0;
                canos[i + 1].rec.width = 100;
                canos[i + 1].rec.height = 300;

                canos[i / 2].active = true;
            }
        }

        animal.texturaRect.x = X;
        animal.texturaRect.y = Y;

        // Colisões
        for (int i = 0; i < 200; i++) {
            collisionB = CheckCollisionRecs(animal.texturaRect, canos[i].rec);
            collisionC = CheckCollisionRecs(animal.texturaRect, canos[i].rec);
            if (collisionB || collisionC) {
                vivo = false;
            }
        }

        if (vivo) {
            Y += velocidade;
            velocidade += 0.4;

            if (IsKeyDown(KEY_SPACE)) {
                velocidade = -7.5;
            }
            if (IsKeyDown(KEY_C)) {
                velocidade = -4;
            }

            if (Y > 480 || Y < -110) {
                vivo = false;
            }

        } else {
            if (IsKeyPressed(KEY_R)) {
                X = 40;
                Y = 30;
                velocidade = 0;
                vivo = true;
                for (int i = 0; i < 100; i++) {
                    canosPos[i].x = 600 + 380 * i;
                    canosPos[i].y = -GetRandomValue(0, 150);
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLUE);

        for (int i = 0; i < 100; i++) {
            DrawTextureRec(
                canoTextura,
                (Rectangle){ 0, 0, (float)canoTextura.width, -(float)canoTextura.height },
                (Vector2){ canos[i * 2].rec.x, canos[i * 2].rec.y },
                WHITE
            );
            DrawTextureRec(
                canoTextura,
                (Rectangle){ 0, 0, (float)canoTextura.width, (float)canoTextura.height },  // Inverter a textura para o cano de baixo
                (Vector2){ canos[i * 2 + 1].rec.x, canos[i * 2 + 1].rec.y },
                WHITE
            );
        }

        DrawTextureRec(
            animal.texturaAnimal,
            (Rectangle){ 0, 0, (float)animal.texturaAnimal.width, (float)animal.texturaAnimal.height },
            (Vector2){ X, Y },
            RAYWHITE
        );

        if (!vivo) {
            DrawText("VOCÊ PERDEU! Aperte R para recomeçar", 100, 380, 20, RED);
        }

        EndDrawing();
    }

    // Libera a memória das texturas carregadas
    UnloadTexture(animal.texturaAnimal);
    UnloadTexture(canoTextura);
}

void ShowCreditsInterface() {
    Texture2D creditos = LoadTexture("creditos.png");

    while (!WindowShouldClose() && !inMainInterface) {
        if (IsKeyPressed(KEY_W)) 
        {
            inMainInterface = true;
            CreditsInterface = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(creditos, 0, 0, RAYWHITE);
        EndDrawing();
    }

    // Libera a memória da textura dos créditos
    UnloadTexture(creditos);
}

void ShowChooseCharacterInterface() {
    Texture2D choosecharacter = LoadTexture("selecao_de_personagens.png");
    Rectangle pigbutton = { 45, 312, 160, 160 };
    Rectangle chickenbutton = {230,312, 160, 160};
    Rectangle duckbutton = {410, 312, 160, 160};
    Rectangle cowbutton = {586, 312, 160, 160};

    while (!WindowShouldClose() && !inMainInterface) {
     if (!PigGame)
     {
        if (IsKeyPressed(KEY_W)) {
            inMainInterface = true;
            inChooseCharacterInterface = false;
        }

        Vector2 mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, pigbutton))
        {
            PigGame = true;
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(choosecharacter, 0, 0, RAYWHITE);

        if (CheckCollisionPointRec(mousePosition, pigbutton)) {
            DrawRectangleLinesEx(pigbutton, 5, RED);
        } 
        if (CheckCollisionPointRec(mousePosition, chickenbutton)) {
            DrawRectangleLinesEx(chickenbutton, 5, RED);
        } 
        if (CheckCollisionPointRec(mousePosition, duckbutton)) {
            DrawRectangleLinesEx(duckbutton, 5, RED);
        } 
        if (CheckCollisionPointRec(mousePosition, cowbutton)) {
            DrawRectangleLinesEx(cowbutton, 5, RED);
        } 
        EndDrawing();
     }else if(PigGame){
        ShowPigGame();
     }
    }

    // Libera a memória da textura da seleção de personagens
    UnloadTexture(choosecharacter);
}
