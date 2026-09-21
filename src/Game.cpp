#include "Game.h"
#include "Mouse.h"
#include "raylib/raylib.h"

namespace {
constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;
} // namespace

Game::Game() { init(); }

void Game::init() {
  InitWindow(kScreenWidth, kScreenHeight, "Planets");
  Mouse::setCapture(true);

  m_camera.position = (Vector3){10.f, 100.f, 10.f};
  m_camera.target = (Vector3){0.f, 0.f, 0.f};
  m_camera.up = (Vector3){0.f, 1.f, 0.f};
  m_camera.fovy = 60.0f;
  m_camera.projection = CAMERA_PERSPECTIVE;

  SetTargetFPS(60);

  spawnSystem();

}

void Game::run() {

  // the main game loop.
  while (!WindowShouldClose() && m_running) {
    const float dt = GetFrameTime();

    // first user input
    sUserInput();

    if (!m_paused) {
      m_entities.update();

      if (Mouse::isCaptured()) {
        UpdateCamera(&m_camera, CAMERA_FREE);
      }

      // update planet positions around the orbit
      updateSystem(dt);

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
    m_camera.target = {0.f, 0.f, 0.f};
  }
}

void Game::sRender() {
  BeginDrawing();
  {
    ClearBackground(BLACK);

    BeginMode3D(m_camera);
    {

      auto allEntities = m_entities.getEntities();
      for (auto &e : allEntities) { // since this returns all entities, both the
                                    // sun and planets will be herem no need for
                                    // tag specific drawing right mow
        if (!e->cModel || !e->cTransform) {
          continue;
        }

        DrawModel(e->cModel->model, e->cTransform->position,
                  e->cTransform->scale, e->cModel->color);
      }

      //     DrawGrid(200, 1.f);
    }

    EndMode3D();

    DrawFPS(10, 10);
    if (m_paused) {
      DrawText("PAUSED", 10, 40, 20, RED);
    }
  }
  EndDrawing();
}

void Game::sUi() {}

void Game::resetGame() {
  for (auto &e : m_entities.getEntities()) {
    e->destroy();
  }
  m_entities.update();

  m_currFrame = 0;
  m_paused = false;

  spawnSystem();
}
