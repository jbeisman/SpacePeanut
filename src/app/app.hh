
#pragma once

#include "renderer.hh"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

class AppState {
public:
  AppState();
  ~AppState() = default;

  // Simulation and rendering control functions
  void initializeSimulation();
  void updateSimulation();
  void displaySimulation(float aspect_ratio);
  void resetSimulation();
  void changeColors();

  // Input callback functions
  void dragStart(const glm::vec2 &mouse_pos);
  void dragUpdate(const glm::vec2 &mouse_pos, const glm::vec2 &view_size);
  void dragEnd();
  void zoomUpdate(float scroll_offset);

  // Simulation input parameters
  int ngrid{64};
  int nbods{2097152};
  float gmax{128.0f};
  double rshift{50.0};
  int nsteps{1000};

  SDL_Window *window_ptr;
  SDL_GLContext context_ptr;
  ImVec4 clear_color{ImVec4(0.45f, 0.55f, 0.60f, 1.00f)};

  // Program control booleans
  bool sim_initialized{false};
  bool execute_sim_init{false};
  bool pause_state{true};
  bool mouse_dragging{false};
  bool change_color_palette{true};

  // Display controls
  Color::ColorType color{Color::ColorType::Magma};
  float clip_factor{1.0f};
  bool log_scale{true};

  // Imgui combo box selectors
  int ngrid_selector{1};
  int color_selector{0};

  // Simulation redshift for output display
  double current_redshift{0.0};

private:
	// Renderer - includes simulator, camera, and all simulation state
  std::unique_ptr<Renderer> renderer;
};

void renderUI(AppState* app);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv);

SDL_AppResult SDL_AppIterate(void *appstate);

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);

void SDL_AppQuit(void *appstate, SDL_AppResult result);
