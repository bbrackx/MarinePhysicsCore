#include "raylib.h"
#include "rlgl.h"
#include "SeaState.h"
#include "RigidBody.h"
#include "Hydrostatics.h"
#include "Integrator.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdio>

int main() {
    SeaState sea;

    float jonswapHs = 0.0;
    float jonswapTp = 8.0;
    float jonswapDir = 0.0;
    sea.addJONSWAP({jonswapHs, jonswapTp, jonswapDir});

    float swellH = 0.0;
    float swellT = 12.0;
    float swellDir = 45.0;
    sea.addSwell({swellH, swellT, swellDir});

    const auto& components = sea.getComponents();
    int nWaves = components.size();
    std::vector<float> amp(nWaves), kx(nWaves), kz(nWaves), omega(nWaves), phase(nWaves);

    for (int w = 0; w < nWaves; w++) {
        amp[w] = components[w].getAmplitude();
        omega[w] = components[w].getOmega();
        phase[w] = components[w].getPhase();

        float k = components[w].getWaveNumber();
        float dir = components[w].getDirectionDeg() * 3.14159265358979323846f / 180.0f;
        kx[w] = k * std::cos(dir);
        kz[w] = k * std::sin(dir);
    }

    const int gridSize = 200;
    const float spacing = 5.0f;
    const float offsetX = (gridSize * spacing) / 2.0f;
    const float offsetZ = (gridSize * spacing) / 2.0f;
    std::vector<float> elevations(gridSize * gridSize);

    float vesselLength = 250.0f;
    float vesselBeam = 50.0f;
    float vesselDepth = 20.0f;
    float vesselDraft = 10.0f;

    HullConfig hull = {vesselLength, vesselBeam, vesselDepth, vesselDraft};
    Hydrostatics hydrostatics(hull);

    float rhoWater = 1025.0f;
    float mass = rhoWater * hydrostatics.getWaterplaneArea() * vesselDraft;

    float initialDrop = 5.0f;
    RigidBody vessel(mass, initialDrop);
    float damping = mass * 0.1f;

    float physicsDt = 1.0f / 120.0f;
    Integrator integrator(physicsDt);

    auto applyAllForces = [&](RigidBody& b) {
        hydrostatics.applyForces(b);
        b.addForce(-damping * b.getVelY());
    };

    InitWindow(1200, 800, "Marine Physics Core");
    SetTargetFPS(60);

    Camera3D camera = {};
    camera.position = {300.0f, 150.0f, 300.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float simTime = 0.0f;
    float renderDt = 1.0f / 60.0f;
    float physicsAccumulator = 0.0f;

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);
        simTime += renderDt;

        physicsAccumulator += renderDt;
        while (physicsAccumulator >= physicsDt) {
            integrator.step(vessel, applyAllForces);
            physicsAccumulator -= physicsDt;
        }

        for (int iz = 0; iz < gridSize; iz++) {
            float z = iz * spacing - offsetZ;
            for (int ix = 0; ix < gridSize; ix++) {
                float x = ix * spacing - offsetX;
                float eta = 0.0f;
                for (int w = 0; w < nWaves; w++) {
                    eta += amp[w] * std::cos(kx[w] * x + kz[w] * z - omega[w] * simTime + phase[w]);
                }
                elevations[iz * gridSize + ix] = eta;
            }
        }

        float vesselY = vessel.getPosY();
        float heaveForce = vessel.getForceY();
        float halfL = vesselLength / 2.0f;
        float halfB = vesselBeam / 2.0f;

        float keelY = vesselY - vesselDraft;
        float deckY = keelY + vesselDepth;

        BeginDrawing();
            ClearBackground(DARKBLUE);
            BeginMode3D(camera);

                DrawLine3D({0, 0, 0}, {50, 0, 0}, RED);
                DrawLine3D({0, 0, 0}, {0, 50, 0}, GREEN);
                DrawLine3D({0, 0, 0}, {0, 0, 50}, BLUE);

                rlBegin(RL_LINES);
                for (int iz = 0; iz < gridSize; iz++) {
                    for (int ix = 0; ix < gridSize - 1; ix++) {
                        float xa = ix * spacing - offsetX;
                        float xb = (ix + 1) * spacing - offsetX;
                        float zv = iz * spacing - offsetZ;
                        float ya = elevations[iz * gridSize + ix];
                        float yb = elevations[iz * gridSize + (ix + 1)];
                        unsigned char ga = (unsigned char)std::clamp(100.0f + ya * 40.0f + 80.0f, 0.0f, 255.0f);
                        unsigned char gb = (unsigned char)std::clamp(100.0f + yb * 40.0f + 80.0f, 0.0f, 255.0f);
                        rlColor4ub(0, ga, 200, 255); rlVertex3f(xa, ya, zv);
                        rlColor4ub(0, gb, 200, 255); rlVertex3f(xb, yb, zv);
                    }
                }
                for (int ix = 0; ix < gridSize; ix++) {
                    for (int iz = 0; iz < gridSize - 1; iz++) {
                        float xv = ix * spacing - offsetX;
                        float za = iz * spacing - offsetZ;
                        float zb = (iz + 1) * spacing - offsetZ;
                        float ya = elevations[iz * gridSize + ix];
                        float yb = elevations[(iz + 1) * gridSize + ix];
                        unsigned char ga = (unsigned char)std::clamp(100.0f + ya * 40.0f + 80.0f, 0.0f, 255.0f);
                        unsigned char gb = (unsigned char)std::clamp(100.0f + yb * 40.0f + 80.0f, 0.0f, 255.0f);
                        rlColor4ub(0, ga, 200, 255); rlVertex3f(xv, ya, za);
                        rlColor4ub(0, gb, 200, 255); rlVertex3f(xv, yb, zb);
                    }
                }
                rlEnd();

                DrawLine3D({-halfL, keelY, -halfB}, { halfL, keelY, -halfB}, YELLOW);
                DrawLine3D({ halfL, keelY, -halfB}, { halfL, keelY,  halfB}, YELLOW);
                DrawLine3D({ halfL, keelY,  halfB}, {-halfL, keelY,  halfB}, YELLOW);
                DrawLine3D({-halfL, keelY,  halfB}, {-halfL, keelY, -halfB}, YELLOW);

                DrawLine3D({-halfL, deckY, -halfB}, { halfL, deckY, -halfB}, YELLOW);
                DrawLine3D({ halfL, deckY, -halfB}, { halfL, deckY,  halfB}, YELLOW);
                DrawLine3D({ halfL, deckY,  halfB}, {-halfL, deckY,  halfB}, YELLOW);
                DrawLine3D({-halfL, deckY,  halfB}, {-halfL, deckY, -halfB}, YELLOW);

                DrawLine3D({-halfL, keelY, -halfB}, {-halfL, deckY, -halfB}, YELLOW);
                DrawLine3D({ halfL, keelY, -halfB}, { halfL, deckY, -halfB}, YELLOW);
                DrawLine3D({ halfL, keelY,  halfB}, { halfL, deckY,  halfB}, YELLOW);
                DrawLine3D({-halfL, keelY,  halfB}, {-halfL, deckY,  halfB}, YELLOW);

                DrawLine3D({-halfL, 0, -halfB}, { halfL, 0, -halfB}, WHITE);
                DrawLine3D({ halfL, 0, -halfB}, { halfL, 0,  halfB}, WHITE);
                DrawLine3D({ halfL, 0,  halfB}, {-halfL, 0,  halfB}, WHITE);
                DrawLine3D({-halfL, 0,  halfB}, {-halfL, 0, -halfB}, WHITE);

            EndMode3D();

            Vector2 xLabel = GetWorldToScreen({55, 0, 0}, camera);
            Vector2 yLabel = GetWorldToScreen({0, 55, 0}, camera);
            Vector2 zLabel = GetWorldToScreen({0, 0, 55}, camera);
            DrawText("X", (int)xLabel.x, (int)xLabel.y, 20, RED);
            DrawText("Y", (int)yLabel.x, (int)yLabel.y, 20, GREEN);
            DrawText("Z", (int)zLabel.x, (int)zLabel.y, 20, BLUE);

            // HUD
            DrawText("Marine Physics Core", 10, 10, 20, WHITE);
            DrawFPS(10, 40);
            DrawText(TextFormat("JONSWAP: Hs=%.1fm  Tp=%.1fs  Dir=%.0f deg", jonswapHs, jonswapTp, jonswapDir), 10, 70, 18, PURPLE);
            DrawText(TextFormat("Swell:   H=%.1fm   T=%.1fs   Dir=%.0f deg", swellH, swellT, swellDir), 10, 95, 18, PURPLE);

            DrawText(TextFormat("Vessel heave: %.3f m", vesselY), 10, 130, 18, YELLOW);
            DrawText(TextFormat("Vessel velocity: %.3f m/s", vessel.getVelY()), 10, 155, 18, YELLOW);
            DrawText(TextFormat("Sim time: %.1f s", simTime), 10, 180, 18, YELLOW);
            DrawText(TextFormat("Tn heave: %.0f Tn", heaveForce), 10, 205, 18, YELLOW);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
