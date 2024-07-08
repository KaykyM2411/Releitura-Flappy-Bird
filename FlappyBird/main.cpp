#include <raylib.h>
#include <raymath.h>

int main() {

    const int width = 800;
    const int height = 800;

    float X = 40;
    float Y = 30;
    float velocidade = 0;
    bool vivo = true;

    InitWindow(width, height, "Flap Bird");
    SetTargetFPS(60);
    Texture2D cenario = LoadTexture("cenario.gif");
    Texture2D player = LoadTexture("personagem.png");

    while (!WindowShouldClose()) {
        if (vivo) {
            // Atualiza a posição do jogador
            Y += velocidade;
            velocidade += 0.4;

            // Verifica se as teclas foram pressionadas
            if (IsKeyDown(KEY_SPACE)) {
                velocidade = -7.5;
            }
            if (IsKeyDown(KEY_C)) {
                velocidade = -4;
            }

            // Verifica se o jogador colidiu com o chão ou o teto
            if (Y > 480 || Y < -110) {
                vivo = false;
            }
        } else {
            // Reinicia o jogo ao pressionar a tecla R
            if (IsKeyPressed(KEY_R)) {
                X = 40;
                Y = 30;
                velocidade = 0;
                vivo = true;
            }
        }

        BeginDrawing();

        ClearBackground(WHITE);
        DrawTexture(cenario, 0, 0, RAYWHITE);
        DrawTextureRec(
            player,
            Rectangle{ 0, 0, (float)player.width, (float)player.height },
            Vector2{ X, Y },
            RAYWHITE
        );

        // Mostra a mensagem de Game Over
        if (!vivo) {
            DrawText("GAME OVER! Press R to restart", 100, 380, 20, RED);
        }

        EndDrawing();
    }

    UnloadTexture(player);  // Libera a memória da textura carregada
    CloseWindow();  // Fecha a janela

    return 0;
}
