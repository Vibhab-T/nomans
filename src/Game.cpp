#include "Game.h"
#include "Mouse.h"
#include "Physics.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <cmath>

namespace {
constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;

constexpr float kDemoBodyMass =
    1.5e12f; // this is like a mass of an asteroid, we will need to manage the
             // scale somehow, now this needs a bit of "unrealisticization"

constexpr float kSunMass = kDemoBodyMass * 20.f; // 3e12 kg
constexpr float kSunRadius = 5.f;
} // namespace

Game::Game() { init(); }

void Game::init() {
  InitWindow(kScreenWidth, kScreenHeight, "Planets");
  Mouse::setCapture(true);

  m_cameraTarget = nullptr; // start in free-fly

  m_camera.position = (Vector3){10.f, 100.f, 10.f};
  m_camera.target = (Vector3){0.f, 0.f, 0.f};
  m_camera.up = (Vector3){0.f, 1.f, 0.f};
  m_camera.fovy = 60.0f;
  m_camera.projection = CAMERA_PERSPECTIVE;

  SetTargetFPS(60);

  /*
   *AI CODE BELOW TO SPAWN PLANET AND SUN IN A ORIBITNG POSITION
   NEEDS TO BE REPLACED WITH PROPER SPAWNING OF CELESTIAL BODIES FROM THE DEBUG
   PANEL BY JUST DRAGGING AND DROPPING
   * */

  // the sun, stationary at the origin
  spawnCelestialBody(kSunMass, Vector3(0, 0, 0), Vector3(0, 0, 0), kSunRadius,
                     YELLOW);

  // four planets, each placed at a different angle around the sun and given
  // the circular-orbit velocity for its distance -- v = sqrt(G * M / r),
  // tangent to the orbit -- so they settle into stable-looking orbits
  // instead of falling straight in or flying off.
  struct Spec {
    float mass, radius, orbitRadius, angleDeg;
    Color color;
  };
  constexpr Spec planets[] = {
      {50000.f, 1.5f, 50.f, 0.f, RED},
      {60000.f, 1.2f, 75.f, 90.f, SKYBLUE},
      {70000.f, 1.8f, 150.f, 180.f, PURPLE},
      {80000.f, 1.0f, 190.f, 270.f, ORANGE},
  };

  for (const Spec &p : planets) {
    const float angleRad = p.angleDeg * DEG2RAD;

    const Vector3 position = {p.orbitRadius * cosf(angleRad), 0.f,
                              p.orbitRadius * sinf(angleRad)};

    // v_circular = sqrt(G * M / r)
    const float speed = sqrtf(Physics::G * kSunMass / p.orbitRadius);

    // tangent direction for a counterclockwise orbit (viewed from +Y):
    // d/dtheta of (cos theta, 0, sin theta)
    const Vector3 velocity =
        Vector3Scale((Vector3){-sinf(angleRad), 0.f, cosf(angleRad)}, speed);

    spawnCelestialBody(p.mass, velocity, position, p.radius, p.color);
  }

  /*
   *AI CODE ENDS
   *
   * */
}
void Game::run() {

  // the main game loop.
  while (!WindowShouldClose() && m_running) {
    const float dt = GetFrameTime();

    // first user input
    sUserInput();

    if (!m_paused) {
      m_entities.update();

      updateCamera(dt);

      updateCelestialBodies(dt);
      updateTrails();

      m_currFrame++;
    }

    // lastly render
    sRender();
  }

  CloseWindow();
}

void Game::setPaused() { m_paused = !m_paused; }

void Game::sUserInput() {
  Mouse::update();

  if (IsKeyPressed(KEY_Z)) {
    changeCameraTarget(nullptr); // to free fly
  }

  if (IsKeyPressed(KEY_C)) {
    cycleCameraTarget();
  }
}

void Game::sRender() {
  BeginDrawing();
  {
    ClearBackground(BLACK);

    BeginMode3D(m_camera);
    {

      auto allEntities = m_entities.getEntities();
      for (auto &e : allEntities) {
        if (!e->cModel || !e->cTransform) {
          continue;
        }

        DrawModel(e->cModel->model, e->cTransform->position,
                  e->cTransform->scale, e->cModel->color);
      }

      if (m_trailsEnabled) {
        for (auto &e : m_entities.getEntities("celestial")) {
          if (!e->cTrail)
            continue;
          const auto &pts = e->cTrail->points;
          const Color trailColor = e->cModel ? e->cModel->color : GRAY;

          for (std::size_t i = 1; i < pts.size(); i++) {
            DrawLine3D(pts[i - 1], pts[i], trailColor);
          }
        }
      }

      // DrawGrid(200, 1.f);
    }

    EndMode3D();
    sUi();
  }
  EndDrawing();
}

void Game::resetGame() {
  for (auto &e : m_entities.getEntities()) {
    e->destroy();
  }
  m_entities.update();

  m_currFrame = 0;
  m_paused = false;
}
