#include "raylib.h"
#include "rlgl.h"
#include "SeaState.h"
#include <vector>
#include <cmath>
#include <algorithm>

int main() {
    SeaState sea;

    float jonswapHs = 2.5;
    float jonswapTp = 8.0;
    float jonswapDir = 0.0;
    sea.addJONSWAP({jonswapHs, jonswapTp, jonswapDir});

    float swellH = 0.5;
    float swellT = 12.0;
    float swellDir = 45.0;
    sea.addSwell({swellH, swellT, swellDir});

    const int gridSize = 200;
    const float spacing = 1;
    const float offsetX = (gridSize * spacing) / 2.0f;
    const float offsetZ = (gridSize * spacing) / 2.0f;
    std::vector<float> elevations(gridSize * gridSize);

    InitWindow(1200, 800, "Marine Physics Core");
    SetTargetFPS(60);

    Camera3D camera = {};
    camera.position = {60.0f, 30.0f, 60.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float simTime = 0.0;
    float dt = 1.0 / 60.0;

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);
        simTime += dt;

        for (int iz = 0; iz < gridSize; iz++) {
            float z = iz * spacing - offsetZ;
            for (int ix = 0; ix < gridSize; ix++) {
                float x = ix * spacing - offsetX;
                elevations[iz * gridSize + ix] = (float)sea.getElevation(x, z, simTime);
            }
        }

        BeginDrawing();
            ClearBackground(DARKBLUE);
            BeginMode3D(camera);

                DrawLine3D({0, 0, 0}, {20, 0, 0}, RED);
                DrawLine3D({0, 0, 0}, {0, 20, 0}, GREEN);
                DrawLine3D({0, 0, 0}, {0, 0, 20}, BLUE);

                rlBegin(RL_LINES);

                for (int iz = 0; iz < gridSize; iz++) {
                    for (int ix = 0; ix < gridSize - 1; ix++) {
                        float xa = ix * spacing - offsetX;
                        float xb = (ix + 1) * spacing - offsetX;
                        float z = iz * spacing - offsetZ;
                        float ya = elevations[iz * gridSize + ix];
                        float yb = elevations[iz * gridSize + (ix + 1)];

                        unsigned char ga = (unsigned char)std::clamp(100.0f + ya * 40.0f + 80.0f, 0.0f, 255.0f);
                        unsigned char gb = (unsigned char)std::clamp(100.0f + yb * 40.0f + 80.0f, 0.0f, 255.0f);

                        rlColor4ub(0, ga, 200, 255); rlVertex3f(xa, ya, z);
                        rlColor4ub(0, gb, 200, 255); rlVertex3f(xb, yb, z);
                    }
                }

                // Lines along Z
                for (int ix = 0; ix < gridSize; ix++) {
                    for (int iz = 0; iz < gridSize - 1; iz++) {
                        float x = ix * spacing - offsetX;
                        float za = iz * spacing - offsetZ;
                        float zb = (iz + 1) * spacing - offsetZ;
                        float ya = elevations[iz * gridSize + ix];
                        float yb = elevations[(iz + 1) * gridSize + ix];

                        unsigned char ga = (unsigned char)std::clamp(100.0f + ya * 40.0f + 80.0f, 0.0f, 255.0f);
                        unsigned char gb = (unsigned char)std::clamp(100.0f + yb * 40.0f + 80.0f, 0.0f, 255.0f);

                        rlColor4ub(0, ga, 200, 255); rlVertex3f(x, ya, za);
                        rlColor4ub(0, gb, 200, 255); rlVertex3f(x, yb, zb);
                    }
                }

                rlEnd();

            EndMode3D();

            Vector2 xLabel = GetWorldToScreen({22, 0, 0}, camera);
            Vector2 yLabel = GetWorldToScreen({0, 22, 0}, camera);
            Vector2 zLabel = GetWorldToScreen({0, 0, 22}, camera);
            DrawText("X", (int)xLabel.x, (int)xLabel.y, 20, RED);
            DrawText("Y", (int)yLabel.x, (int)yLabel.y, 20, GREEN);
            DrawText("Z", (int)zLabel.x, (int)zLabel.y, 20, BLUE);

            DrawText("Marine Physics Core", 10, 10, 20, WHITE);
            DrawText(TextFormat("JONSWAP: Hs=%.1fm  Tp=%.1fs  Dir=%.0f°", jonswapHs, jonswapTp, jonswapDir), 10, 70, 18, PURPLE);
            DrawText(TextFormat("Swell:   H=%.1fm   T=%.1fs   Dir=%.0f°", swellH, swellT, swellDir), 10, 95, 18, PURPLE);
            DrawFPS(10, 40);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
