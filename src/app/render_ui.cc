
#include "app.hh"

void renderUI(AppState* app) {

  static const char* ngrid_items[] = {"32", "64", "128", "256", "512"};
  static const int ngrid_values[] = {32, 64, 128, 256, 512};
  static const char* color_items[] = {"Magma", "BlueOrange", "Viridis", "Plasma", "Rainbow"};
  static const Color::ColorType color_values[] = {
    Color::ColorType::Magma,
    Color::ColorType::BlueOrange,
    Color::ColorType::Viridis,
    Color::ColorType::Plasma,
    Color::ColorType::Rainbow };

  ImGuiIO &io = ImGui::GetIO();
  
  ImGui::Begin("INPUTS");

    if (ImGui::Combo("Grid resolution", &app->ngrid_selector, ngrid_items, IM_ARRAYSIZE(ngrid_items))) {
      app->ngrid = ngrid_values[app->ngrid_selector];
    }

    ImGui::InputInt("Number of particles", &app->nbods);
    ImGui::InputFloat("Grid length", &app->gmax);
    ImGui::InputDouble("Redshift value", &app->rshift);
    ImGui::InputInt("Number of timesteps", &app->nsteps);

    ImGui::SeparatorText("CONTROLS");
    if (ImGui::Button("INITIALIZE")) {
      app->execute_sim_init = true;
    }
    ImGui::SameLine();

    if (ImGui::Button("RUN")) {
      app->pause_state = false;
      app->execute_sim_init = true;
    }
    ImGui::SameLine();

    if (ImGui::Button("PAUSE")) {
      app->pause_state = true;
    }
    ImGui::SameLine();

    if (ImGui::Button("RESET")) {
      if (app->sim_initialized) {
        app->resetSimulation();
      }
    }
    ImGui::SameLine();

    if (ImGui::Button("QUIT")) {
      SDL_Event sdl_quit;
      sdl_quit.type = SDL_EVENT_QUIT;
      SDL_PushEvent(&sdl_quit);
    }
    ImGui::SameLine();

    ImGui::Checkbox("Log scale", &app->log_scale);

    ImGui::ColorEdit3("Clear color", (float*)&app->clear_color);

    if (ImGui::Combo("Color palette", &app->color_selector, color_items, IM_ARRAYSIZE(color_items))) {
      app->color = color_values[app->color_selector];
      app->change_color_palette = true;
    }

    ImGui::SliderFloat("Clip factor", &app->clip_factor, 0.000001f, 1.0f, "Value: %.6f", ImGuiSliderFlags_Logarithmic);

    ImGui::Text("Current redshift value %.6lf", app->current_redshift);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);

  ImGui::End();
}

