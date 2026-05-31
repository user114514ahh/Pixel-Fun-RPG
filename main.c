#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TILE_SIZE 16
#define SCALE 3
#define DRAW_SIZE (TILE_SIZE * SCALE)

#define NPC_COUNT 3

#define MAP_WIDTH 45
#define MAP_HEIGHT 30

#define FRAME_SIZE 16
#define CHARACTER_WIDTH (FRAME_SIZE * 3)

typedef enum {
    DOWN,
    LEFT,
    RIGHT,
    UP
} Direction;

int ground[MAP_HEIGHT][MAP_WIDTH];
int objectMap[MAP_HEIGHT][MAP_WIDTH];

typedef struct {
    int x;
    int y;
    float drawX;
    float drawY;
    Direction dir;
    int frame;
    float animTimer;
    int moving;
} Player;

typedef struct {
    int x;
    int y;
    char dialog[100];
} NPC;

Player player = {
    7, 7,
    2 * DRAW_SIZE,
    2 * DRAW_SIZE,
    DOWN,
    1,
    0,
    0
};

NPC npcs[NPC_COUNT] = {
    {8, 6, "help me to do task 1 ?"},
    {8, 8, "help me to do task 2 ?"},
    {12, 12, "help me to do task 3 ?"}
};

Texture2D groundTextures[10];
Texture2D objectTextures[5];
Texture2D playerTexture;
Texture2D npcTexture;

Camera2D camera = { 0 };

float moveTimer = 0;
float moveDelay = 0.15f;

int showDialog = 0;
int talkingNPC = -1;

// 漸層控制變數
float fadeAlpha = 0.0f;
int isFadingOut = 0;
int isFadingIn = 0;

void LoadLayer(const char* filename, int layer[MAP_HEIGHT][MAP_WIDTH]) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to load %s\n", filename);
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int c = fgetc(fp);
            while (c != EOF && (c < '0' || c > '9')) {
                c = fgetc(fp);
            }
            if (c != EOF) {
                layer[y][x] = c - '0';
            }
            else {
                layer[y][x] = 0;
            }
        }
    }
    fclose(fp);
}

void LoadAssets() {
    playerTexture = LoadTexture("assets/player.png");
    npcTexture = LoadTexture("assets/npc.png");
    groundTextures[0] = LoadTexture("assets/grass.png");
    objectTextures[1] = LoadTexture("assets/tree1.png");
    objectTextures[2] = LoadTexture("assets/tree2.png");
    groundTextures[1] = LoadTexture("assets/water1.png");
    groundTextures[2] = LoadTexture("assets/water2.png");
    groundTextures[3] = LoadTexture("assets/water3.png");
    groundTextures[4] = LoadTexture("assets/water4.png");
    groundTextures[5] = LoadTexture("assets/water5.png");
    groundTextures[6] = LoadTexture("assets/water6.png");
    groundTextures[7] = LoadTexture("assets/water7.png");
    groundTextures[8] = LoadTexture("assets/water8.png");
    groundTextures[9] = LoadTexture("assets/water9.png");

    SetTextureFilter(playerTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(npcTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[0], TEXTURE_FILTER_POINT);
    SetTextureFilter(objectTextures[1], TEXTURE_FILTER_POINT);
    SetTextureFilter(objectTextures[2], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[1], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[2], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[3], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[4], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[5], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[6], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[7], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[8], TEXTURE_FILTER_POINT);
    SetTextureFilter(groundTextures[9], TEXTURE_FILTER_POINT);
}

void UpdatePlayer() {
    float dt = GetFrameTime();
    if (dt > 0.1f) dt = 0.016f;

    // 漸層動畫邏輯更新
    if (isFadingOut) {
        fadeAlpha += dt * 2.0f; // 約 0.5 秒淡出至全黑
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            isFadingOut = 0;

            // 畫面完全變黑後，呼叫 Python 小遊戲
            char command[256];
            sprintf(command, "python3 task_manager.py %d", talkingNPC);
            int taskResult = system(command);

            if (taskResult == 0) {
                printf("Task %d Success!\n", talkingNPC);
            }
            else {
                printf("Task %d Cancelled or Failed.\n", talkingNPC);
            }

            isFadingIn = 1; // 開始淡入回遊戲
        }
        return; // 漸層期間不處理移動與其他輸入
    }

    if (isFadingIn) {
        fadeAlpha -= dt * 2.0f; // 約 0.5 秒淡入還原
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            isFadingIn = 0;
            showDialog = 0;
            talkingNPC = -1;
        }
        return;
    }

    player.moving = 0;

    // 只有在沒在講話、沒在漸層時才可以移動
    if (!showDialog) {
        moveTimer += dt;
        int newX = player.x;
        int newY = player.y;

        if (moveTimer >= moveDelay) {
            if (IsKeyDown(KEY_W)) { newY--; player.dir = UP; moveTimer = 0; player.moving = 1; }
            else if (IsKeyDown(KEY_S)) { newY++; player.dir = DOWN; moveTimer = 0; player.moving = 1; }
            else if (IsKeyDown(KEY_A)) { newX--; player.dir = LEFT; moveTimer = 0; player.moving = 1; }
            else if (IsKeyDown(KEY_D)) { newX++; player.dir = RIGHT; moveTimer = 0; player.moving = 1; }

            int hitNPC = 0;
            for (int i = 0; i < NPC_COUNT; i++) {
                if (newX == npcs[i].x && newY == npcs[i].y) {
                    hitNPC = 1;
                }
            }

            if (newX >= 0 && newX < MAP_WIDTH &&
                newY >= 0 && newY < MAP_HEIGHT &&
                ground[newY][newX] == 0 && objectMap[newY][newX] != 1 && objectMap[newY][newX] != 2 && hitNPC == 0) {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    float targetX = player.x * DRAW_SIZE;
    float targetY = player.y * DRAW_SIZE;

    player.drawX += (targetX - player.drawX) * 0.2f;
    player.drawY += (targetY - player.drawY) * 0.2f;

    // 判斷畫面上的角色是不是還在移動
    if (abs((int)(targetX - player.drawX)) > 1 ||
        abs((int)(targetY - player.drawY)) > 1) {
        player.moving = 1;
    }
    else {
        player.moving = 0;
    }

    if (player.moving) {
        player.animTimer += dt;
        if (player.animTimer >= 0.12f) {
            player.frame++;
            if (player.frame > 2) player.frame = 0;
            player.animTimer = 0;
        }
    }
    else {
        player.frame = 1;
    }

    // 對話觸發與確認邏輯
    if (showDialog) {
        if (IsKeyPressed(KEY_ENTER)) {
            isFadingOut = 1; // 觸發淡出，不再直接執行 system()
        }
        else if (IsKeyPressed(KEY_E)) {
            showDialog = 0;
            talkingNPC = -1;
        }
    }
    else if (IsKeyPressed(KEY_E)) {
        int targetX = player.x;
        int targetY = player.y;

        switch (player.dir) {
        case UP:
            targetY--;
            break;
        case DOWN:
            targetY++;
            break;
        case LEFT:
            targetX--;
            break;
        case RIGHT:
            targetX++;
            break;
        }

        for (int i = 0; i < NPC_COUNT; i++) {
            if (npcs[i].x == targetX && npcs[i].y == targetY) {
                showDialog = 1;
                talkingNPC = i;
                break;
            }
        }
    }
}

void DrawMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int screenX = x * DRAW_SIZE;
            int screenY = y * DRAW_SIZE;
            int tile = ground[y][x];

            DrawTextureEx(
                groundTextures[tile],
                (Vector2) {
                screenX, screenY
            },
                0,
                SCALE,
                WHITE
            );

            int obj = objectMap[y][x];
            if (obj != 0) {
                DrawTextureEx(
                    objectTextures[obj],
                    (Vector2) {
                    screenX, screenY
                },
                    0,
                    SCALE,
                    WHITE
                );
            }
        }
    }

    for (int i = 0; i < NPC_COUNT; i++) {
        DrawTextureEx(
            npcTexture,
            (Vector2) {
            npcs[i].x* DRAW_SIZE,
                npcs[i].y* DRAW_SIZE
        },
            0,
            SCALE,
            WHITE
        );
    }

    int characterIndex = 1;

    Rectangle source = {
        characterIndex * CHARACTER_WIDTH + player.frame * FRAME_SIZE,
        player.dir * FRAME_SIZE,
        FRAME_SIZE,
        FRAME_SIZE
    };

    Rectangle dest = {
        player.drawX,
        player.drawY,
        DRAW_SIZE,
        DRAW_SIZE
    };

    DrawTexturePro(
        playerTexture,
        source,
        dest,
        (Vector2) {
        0, 0
    },
        0,
        WHITE
    );
}

void DrawDialogBox() {
    // 依據 1080x720 調整對話框尺寸與置底位置
    DrawRectangle(50, 520, 980, 140, WHITE);
    DrawRectangleLines(50, 520, 980, 140, BLACK);

    if (talkingNPC != -1) {
        // 校正：將文字也移至 Y=520 對話框範圍內
        DrawText(
            npcs[talkingNPC].dialog,
            80,
            545,
            26,
            BLACK
        );
        DrawText("Press ENTER to play task, E to leave", 80, 610, 18, DARKGRAY);
    }
}

int main() {
    InitWindow(1080, 720, "Pixel RPG");
    SetTargetFPS(60);

    camera.zoom = 1.0f;
    LoadAssets();
    LoadLayer("ground.txt", ground);
    LoadLayer("object.txt", objectMap);

    while (!WindowShouldClose()) {
        UpdatePlayer();

        float targetX = player.drawX + DRAW_SIZE / 2.0f;
        float targetY = player.drawY + DRAW_SIZE / 2.0f;

        float minX = 1080 / 2.0f;
        float maxX = (MAP_WIDTH * DRAW_SIZE) - (1080 / 2.0f);
        float minY = 720 / 2.0f;
        float maxY = (MAP_HEIGHT * DRAW_SIZE) - (720 / 2.0f);

        if (targetX < minX) targetX = minX;
        if (targetX > maxX) targetX = maxX;
        if (targetY < minY) targetY = minY;
        if (targetY > maxY) targetY = maxY;

        camera.target = (Vector2){ targetX, targetY };

        camera.offset = (Vector2){
            1080 / 2.0f,
            720 / 2.0f
        };

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        DrawMap();
        EndMode2D();

        if (showDialog) {
            DrawDialogBox();
        }

        DrawText(
            "WASD Move | E Talk",
            20,
            20,
            24,
            WHITE
        );

        // 在最上層繪製全螢幕黑色漸層遮罩
        if (fadeAlpha > 0.0f) {
            DrawRectangle(0, 0, 1080, 720, Fade(BLACK, fadeAlpha));
        }

        EndDrawing();
    }

    UnloadTexture(playerTexture);
    UnloadTexture(npcTexture);
    UnloadTexture(groundTextures[0]);
    UnloadTexture(objectTextures[1]);
    UnloadTexture(objectTextures[2]);
    UnloadTexture(groundTextures[1]);
    UnloadTexture(groundTextures[2]);
    UnloadTexture(groundTextures[3]);
    UnloadTexture(groundTextures[4]);
    UnloadTexture(groundTextures[5]);
    UnloadTexture(groundTextures[6]);
    UnloadTexture(groundTextures[7]);
    UnloadTexture(groundTextures[8]);
    UnloadTexture(groundTextures[9]);

    CloseWindow();
    return 0;
}