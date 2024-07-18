#include "raylib.h"
#include "raymath.h" // Required for: Vector2Clamp()

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

const int width = 800;
const int height = 600;

bool inMainInterface = true;
bool CreditsInterface = false;
bool inChooseCharacterInterface = false;
bool PigGame = false;
bool ChickenGame = false;
bool DuckGame = false;
bool CowGame = false;

bool audioPlaying = false;

typedef struct Animal {
    Rectangle texturaRect;
    Rectangle boxA;
    Rectangle boxB;
    Vector2 center;
    int radius;
    Texture2D texturaAnimal;
    float X;
    float Y;
    int cod;
    float velocidade;
    bool vivo;
} Animal;

void Game(Animal animal);
void ShowPigGame(Animal *animal);
void ShowChickenGame(Animal *animal);
void ShowDuckGame(Animal *animal);
void ShowCowGame(Animal *animal);
void ShowCreditsInterface();
void ShowChooseCharacterInterface();

int main() {
    // Inicializa o dispositivo de áudio
    InitAudioDevice();

    // Carrega um arquivo de áudio
    Sound trilhasonora = LoadSound("audio.fundo.mp3");

    // Define o volume do áudio
    SetSoundVolume(trilhasonora, 0.5f);
    int gameScreenWidth = 800;
    int gameScreenHeight = 600;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(width, height, "Animals Rocket");
    SetWindowMinSize(320, 240);

    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);

    // Carrega a textura do fundo
    Texture2D fundo = LoadTexture("fundo.png");

    // Define a área (retângulo) onde você quer detectar o clique
    Rectangle creditos = { 260, 432, 220, 83 };  // x, y, largura, altura
    Rectangle play = { 276, 324, 191, 83 };

    while (!WindowShouldClose()) {  // Loop principal
        // Compute required framebuffer scaling
        float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });

        if (!audioPlaying) {
            PlaySound(trilhasonora);
            audioPlaying = true;
        }

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

            BeginTextureMode(target);
            ClearBackground(RAYWHITE);  // Clear render texture background color

            DrawText("If executed inside a window,\nyou can resize the window,\nand see the screen scaling!", 10, 25, 20, WHITE);
            DrawText(TextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, GREEN);
            DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)virtualMouse.x, (int)virtualMouse.y), 350, 55, 20, YELLOW);
            EndTextureMode();

            // Desenho na tela
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Desenhar o fundo redimensionado
            DrawTextureEx(fundo, (Vector2){0, 0}, 0.0f, scale, WHITE);

            // Verifica se o mouse está na área e muda a cor do retângulo
            if (CheckCollisionPointRec(virtualMouse, creditos)) {
                DrawRectangleLinesEx((Rectangle){creditos.x * scale, creditos.y * scale, creditos.width * scale, creditos.height * scale}, 3, RED);
            } else {
                DrawRectangleLinesEx((Rectangle){creditos.x * scale, creditos.y * scale, creditos.width * scale, creditos.height * scale}, 3, BLACK);
            }
            if (CheckCollisionPointRec(virtualMouse, play)) {
                DrawRectangleLinesEx((Rectangle){play.x * scale, play.y * scale, play.width * scale, play.height * scale}, 3, RED);
            } else {
                DrawRectangleLinesEx((Rectangle){play.x * scale, play.y * scale, play.width * scale, play.height * scale}, 3, BLACK);
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
    UnloadSound(trilhasonora);

    CloseAudioDevice();

    // Fecha a janela e limpa os recursos
    CloseWindow();

    return 0;
}
void Game(Animal animal) {
    typedef struct Canos {
        Rectangle rec;
        Texture2D texturaCano;
        float scale;
        Color color;
        bool active;
    } Canos;

    float X = 0;
    float Y = 300;
    float velocidade = 0;
    bool vivo = true;

    int score;
    int hiScore = 0;
    int seg;
    int min;
    
    // Carrega os canos
    Texture2D canoTextura = LoadTexture("cano.png");
    static Canos canos[200] = { 0 };
    static Vector2 canosPos[100] = { 0 };
    static int canosVelocidadeX = 0;
 
    // Inicialização dos canos
    for (int i = 0; i < 100; i++) {
        canosPos[i].x = 600 + 380 * i;
        canosPos[i].y = -GetRandomValue(0, 150);
    }
    
    int screenUpperLimit = 40;
    bool pause = false;

    Rectangle boxCollision = { 0 };
    bool collisionA = false;
    bool collisionB = false;
    bool collisionC = false;
    
    float startTime = GetTime();
    float elapsedTime = 0.0f;

    // Inicialização dos canos
    while (!WindowShouldClose() && !inMainInterface) {
        if (IsKeyPressed(KEY_W)) {
            inChooseCharacterInterface = true;  // Define o estado de seleção de personagem como verdadeiro
            PigGame = false;  // Define o estado do jogo PigGame como falso
            return;  // Retorna imediatamente para a seleção de personagem
        }

        float scale = MIN((float)GetScreenWidth()/width, (float)GetScreenHeight()/height);
        
        if (IsKeyPressed('P')) pause = !pause;
        
        score = 0;
        
        if (animal.cod == 1){
            animal.boxA.x = X+170;
            animal.boxA.y = Y+125;
            animal.boxB.x = X+155;
            animal.boxB.y = Y+195;
            animal.center.x = X+270;
            animal.center.y = Y+160;
        }
        if (animal.cod == 2){
            animal.boxA.x = X+115;
            animal.boxA.y = Y+130;
            animal.boxB.x = X+164;
            animal.boxB.y = Y+195;
            animal.center.x = X+188;
            animal.center.y = Y+140;
        }
        if (animal.cod == 3){
            animal.boxA.x = X+58;
            animal.boxA.y = Y+120;
            animal.boxB.x = X+75;
            animal.boxB.y = Y+160;
            animal.center.x = X+132;
            animal.center.y = Y+95;
        }
        if (animal.cod == 4){
            animal.boxA.x = X+50;
            animal.boxA.y = Y+110;
            animal.boxB.x = X+130;
            animal.boxB.y = Y+140;
            animal.center.x = X+140;
            animal.center.y = Y+110;
        }
        
        if (!pause && vivo) {
            canosVelocidadeX = 2;
            for (int i = 0; i < 100; i++) canosPos[i].x -= canosVelocidadeX;

            for (int i = 0; i < 200; i += 2) {
                canos[i].rec.x = canosPos[i / 2].x;
                canos[i].rec.y = canosPos[i / 2].y;
                canos[i].rec.width = 82;
                canos[i].rec.height = 381;

                canos[i + 1].rec.x = canosPos[i / 2].x;
                canos[i + 1].rec.y = 600 + canosPos[i / 2].y;
                canos[i + 1].rec.width = 82;
                canos[i + 1].rec.height = 381;

                canos[i / 2].active = true;
            }
        }

        animal.texturaRect.x = animal.X;
        animal.texturaRect.y = animal.Y;

        // Colisões
        for (int i = 0; i < 200; i++) {
            collisionA = CheckCollisionRecs(animal.boxA, canos[i].rec);
            collisionB = CheckCollisionRecs(animal.boxB, canos[i].rec);
            collisionC = CheckCollisionCircleRec(animal.center,animal.radius, canos[i].rec);
            if (collisionA || collisionB || collisionC) {
                //boxCollision = GetCollisionRec(animal.texturaRect, canos[i].rec);
                vivo = false;

            }
            else if( X > canosPos[i].x && vivo &&  X > canos[i / 2].rec.x){
                score += 100;
                if (score > hiScore) hiScore = score;
            }
        }

         if (vivo && !pause) {
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
            elapsedTime = GetTime() - startTime;

        } else {
            if (IsKeyPressed(KEY_R)) {
                X = 40;
                Y = 30;
                velocidade = 0;
                vivo = true;
                
                score = 0;
                startTime = GetTime();
                elapsedTime = 0.0f;
                
                for (int i = 0; i < 100; i++) {
                    canosPos[i].x = 600 + 400 * i;
                    canosPos[i].y = -GetRandomValue(0, 150);
                }
            }
        }

        min = (int)elapsedTime / 60;
        seg = (int)elapsedTime % 60;
                
        BeginDrawing();

        ClearBackground(BLUE);
        
        for (int i = 0; i < 100; i++) {
            DrawTextureRec(
                canoTextura,
                (Rectangle){ 148, 0, (float)canoTextura.width, -(float)canoTextura.height },  // Inverter a arvore
                (Vector2){ canos[i * 2].rec.x, canos[i * 2].rec.y },
                WHITE
            );
            DrawTextureRec(
                canoTextura,
                (Rectangle){ 148, 0, (float)canoTextura.width, (float)canoTextura.height }, 
                (Vector2){ canos[i * 2 + 1].rec.x, canos[i * 2 + 1].rec.y },
                WHITE
            );
        }
     
        DrawTextureRec(
            animal.texturaAnimal,
            (Rectangle){ 0, 0, (float)animal.texturaAnimal.width, (float)animal.texturaAnimal.height },
            (Vector2){ X, Y },
            WHITE
        );
        
        if (pause) {
            Rectangle pausa = { 200, 225, 400, 104 };
            DrawRectangleRec(pausa, ORANGE);
            DrawRectangleLinesEx(pausa, 5, YELLOW);
            
            DrawText("JOGO PAUSADO", width/2 - MeasureText("JOGO PAUSADO", 40)/2, height/2 - 40, 40, WHITE);
        }
        if (!vivo) {
            Rectangle perdeu = { 200, 235, 400, 104 };
            DrawRectangleRec(perdeu, RED);
            DrawRectangleLinesEx(perdeu, 5, BLACK);
            DrawText("VOCÊ PERDEU!", width/2 - MeasureText("VOCÊ PERDEU!", 40)/2, height/2 - 40, 40, BLACK);
            DrawText("Aperte R para recomeçar", width/2 - MeasureText("Aperte R para recomeçar", 20)/2, height/2+42 - 40, 20, BLACK);
        }
       
        DrawText(TextFormat("%04i", score), 20, 20, 40, WHITE);
        DrawText(TextFormat("HI-SCORE: %04i", hiScore), 20, 70, 20, LIGHTGRAY);
        DrawText(TextFormat("%02i:%02i", min, seg), 700, 20, 30, WHITE);
        EndDrawing();
    }

    // Libera a memória das texturas carregadas
    UnloadTexture(animal.texturaAnimal);
    UnloadTexture(canoTextura);
}

void ShowCreditsInterface() {
    Texture2D creditos = LoadTexture("creditos.png");

    while (!WindowShouldClose() && !inMainInterface) {
        if (IsKeyPressed(KEY_W)) {
            inMainInterface = true;
            CreditsInterface = false;
        }

        float scale = MIN((float)GetScreenWidth()/creditos.width, (float)GetScreenHeight()/creditos.height);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(creditos, (Vector2){0, 0}, 0.0f, scale, RAYWHITE);
        EndDrawing();
    }

    // Libera a memória da textura dos créditos
    UnloadTexture(creditos);
}

void ShowPigGame(Animal *animal) {
    animal->texturaAnimal = LoadTexture("personagem1.png");
    animal->boxA = { 0, 0, 80, 70 };
    animal->boxB = { 0, 0, 20, 20 };
    animal->center = { 0, 500 };
    animal->radius = { 30 };  
    animal->X = 0;
    animal->Y = 300;
    animal->velocidade = 0;
    animal->vivo = true;
    animal->cod = 1;
    Game(*animal);
}

void ShowChickenGame(Animal *animal) {
    Image image = LoadImage("personagem2.gif");  // Carregar imagem original
    ImageResize(&image, 250, 250);          // Redimensionar imagem
    animal->texturaAnimal =  LoadTextureFromImage(image);
    animal->boxA = { 0, 0, 90, 70 };
    animal->boxB = { 0, 0, 20, 20 };
    animal->center = { 0, 500 };
    animal->radius = { 30 }; 
    animal->X = 0;
    animal->Y = 300;
    animal->velocidade = 0;
    animal->vivo = true;
    animal->cod = 2;
    Game(*animal);
}

void ShowDuckGame(Animal *animal) {
    Image image = LoadImage("personagem3.png");  // Carregar imagem original
    ImageResize(&image, 200, 200);          // Redimensionar imagem
    animal->texturaAnimal =  LoadTextureFromImage(image);
    animal->boxA = { 0, 0, 100, 50 };
    animal->boxB = { 0, 0, 80, 20 };
    animal->center = { 0, 500 };
    animal->radius = { 30 };  
    animal->X = 0;
    animal->Y = 300;
    animal->velocidade = 0;
    animal->vivo = true;
    animal->cod = 3;
    Game(*animal);
}

void ShowCowGame(Animal *animal) {
    Image image = LoadImage("personagem4.png");  // Carregar imagem original
    ImageResize(&image, 250, 250);          // Redimensionar imagem
    animal->texturaAnimal =  LoadTextureFromImage(image);
    animal->boxA = { 0, 0, 90, 70 };
    animal->boxB = { 0, 0, 20, 20 };
    animal->center = { 0, 500 };
    animal->radius = { 35 };  
    animal->X = 0;
    animal->Y = 300;
    animal->velocidade = 0;
    animal->vivo = true;
    animal->cod = 4;
    Game(*animal);
}

void ShowChooseCharacterInterface() {
    Texture2D choosecharacter = LoadTexture("selecao_de_personagens.png");
    Rectangle pigbutton = { 45, 312, 160, 160 };
    Rectangle chickenbutton = { 230, 312, 160, 160 };
    Rectangle duckbutton = { 410, 312, 160, 160 };
    Rectangle cowbutton = { 586, 312, 160, 160 };

    Animal animal = {0};

    while (!WindowShouldClose() && !inMainInterface) {
        if (IsKeyPressed(KEY_W)) {
            inMainInterface = true;
            inChooseCharacterInterface = false;
        }

        Vector2 mousePosition = GetMousePosition();
        float scale = MIN((float)GetScreenWidth()/choosecharacter.width, (float)GetScreenHeight()/choosecharacter.height);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, pigbutton)) {
            PigGame = true;
            inChooseCharacterInterface = false;
            ShowPigGame(&animal);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, chickenbutton)) {
            ChickenGame = true;
            inChooseCharacterInterface = false;
            ShowChickenGame(&animal);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, duckbutton)) {
            DuckGame = true;
            inChooseCharacterInterface = false;
            ShowDuckGame(&animal);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, cowbutton)) {
            CowGame = true;
            inChooseCharacterInterface = false;
            ShowCowGame(&animal);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(choosecharacter, (Vector2){0, 0}, 0.0f, scale, RAYWHITE);

        if (CheckCollisionPointRec(mousePosition, pigbutton)) {
            DrawRectangleLinesEx((Rectangle){pigbutton.x * scale, pigbutton.y * scale, pigbutton.width * scale, pigbutton.height * scale}, 5, RED);
        } 
        if (CheckCollisionPointRec(mousePosition, chickenbutton)) {
            DrawRectangleLinesEx((Rectangle){chickenbutton.x * scale, chickenbutton.y * scale, chickenbutton.width * scale, chickenbutton.height * scale}, 5, RED);
        } 
        if (CheckCollisionPointRec(mousePosition, duckbutton)) {
            DrawRectangleLinesEx((Rectangle){duckbutton.x * scale, duckbutton.y * scale, duckbutton.width * scale, duckbutton.height * scale}, 5, RED);
        } 
        if (CheckCollisionPointRec(mousePosition, cowbutton)) {
            DrawRectangleLinesEx((Rectangle){cowbutton.x * scale, cowbutton.y * scale, cowbutton.width * scale, cowbutton.height * scale}, 5, RED);
        } 
        EndDrawing();
    }

    // Libera a memória da textura da seleção de personagens
    UnloadTexture(choosecharacter);
}
