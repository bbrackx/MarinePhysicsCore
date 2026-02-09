#include "raylib.h"
#include "rlgl.h"
#include "SeaState.h"
#include <vector>
#include <cmath>
#include <algorithm>

int main() {
    // --- Sea state definition ---
    SeaState sea;

    float jonswapHs = 2.5;
    float jonswapTp = 8.0;
    float jonswapDir = 0.0;
    sea.addJONSWAP({jonswapHs, jonswapTp, jonswapDir});

    float swellH = 0.5;
    float swellT = 12.0;
    float swellDir = 45.0;
    sea.addSwell({swellH, swellT, swellDir});

    // --- Grid setup ---
    const int gridSize = 200;
    const float spacing = 1.0f;
    const float offsetX = (gridSize * spacing) / 2.0f;
    const float offsetZ = (gridSize * spacing) / 2.0f;

    // Pre-allocate elevation grid
    std::vector<float> elevations(gridSize * gridSize);

    // --- raylib window ---
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

        // --- Precompute all elevations once per frame ---
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

                // Coordinate axes
                DrawLine3D({0, 0, 0}, {20, 0, 0}, RED);
                DrawLine3D({0, 0, 0}, {0, 20, 0}, GREEN);
                DrawLine3D({0, 0, 0}, {0, 0, 20}, BLUE);

                // --- Wave surface using immediate mode ---
                rlBegin(RL_TRIANGLES);
                for (int iz = 0; iz < gridSize - 1; iz++) {
                    for (int ix = 0; ix < gridSize - 1; ix++) {
                        float x0 = ix * spacing - offsetX;
                        float z0 = iz * spacing - offsetZ;
                        float x1 = (ix + 1) * spacing - offsetX;
                        float z1 = (iz + 1) * spacing - offsetZ;

                        float y00 = elevations[iz * gridSize + ix];
                        float y10 = elevations[iz * gridSize + (ix + 1)];
                        float y01 = elevations[(iz + 1) * gridSize + ix];
                        float y11 = elevations[(iz + 1) * gridSize + (ix + 1)];

                        // Color based on elevation
                        unsigned char g00 = (unsigned char)std::clamp(100.0f + y00 * 40.0f + 80.0f, 0.0f, 255.0f);
                        unsigned char g10 = (unsigned char)std::clamp(100.0f + y10 * 40.0f + 80.0f, 0.0f, 255.0f);
                        unsigned char g01 = (unsigned char)std::clamp(100.0f + y01 * 40.0f + 80.0f, 0.0f, 255.0f);
                        unsigned char g11 = (unsigned char)std::clamp(100.0f + y11 * 40.0f + 80.0f, 0.0f, 255.0f);

                        // Triangle 1
                        rlColor4ub(0, g00, 200, 255); rlVertex3f(x0, y00, z0);
                        rlColor4ub(0, g01, 200, 255); rlVertex3f(x0, y01, z1);
                        rlColor4ub(0, g10, 200, 255); rlVertex3f(x1, y10, z0);

                        // Triangle 2
                        rlColor4ub(0, g10, 200, 255); rlVertex3f(x1, y10, z0);
                        rlColor4ub(0, g01, 200, 255); rlVertex3f(x0, y01, z1);
                        rlColor4ub(0, g11, 200, 255); rlVertex3f(x1, y11, z1);
                    }
                }
                rlEnd();

                // --- Grid lines on surface ---
                int gridLineSpacing = 5;
                rlBegin(RL_LINES);
                rlColor4ub(255, 255, 255, 60);

                for (int iz = 0; iz < gridSize; iz += gridLineSpacing) {
                    for (int ix = 0; ix < gridSize - 1; ix++) {
                        float xa = ix * spacing - offsetX;
                        float xb = (ix + 1) * spacing - offsetX;
                        float z = iz * spacing - offsetZ;
                        rlVertex3f(xa, elevations[iz * gridSize + ix], z);
                        rlVertex3f(xb, elevations[iz * gridSize + (ix + 1)], z);
                    }
                }

                for (int ix = 0; ix < gridSize; ix += gridLineSpacing) {
                    for (int iz = 0; iz < gridSize - 1; iz++) {
                        float x = ix * spacing - offsetX;
                        float za = iz * spacing - offsetZ;
                        float zb = (iz + 1) * spacing - offsetZ;
                        rlVertex3f(x, elevations[iz * gridSize + ix], za);
                        rlVertex3f(x, elevations[(iz + 1) * gridSize + ix], zb);
                    }
                }
                rlEnd();

            EndMode3D();

            // Axis labels
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
