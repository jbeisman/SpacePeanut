
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
  
  ImGui::Begin("INPUTS AND CONTROLS");

    ImGui::SeparatorText("INPUTS");

      ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);

      if (ImGui::Combo("Grid resolution", &app->ngrid_selector, ngrid_items, IM_ARRAYSIZE(ngrid_items))) {
        app->ngrid = ngrid_values[app->ngrid_selector];
      }

      ImGui::InputInt("Number of particles", &app->nbods);
      if (app->nbods < 1) app->nbods = 2097152;

      ImGui::InputFloat("Grid length", &app->gmax);
      if (app->gmax <= 0.0f) app->gmax = 128.0f;

      ImGui::InputDouble("Redshift value", &app->rshift);
      if (app->rshift <= 0.0) app->rshift = 50.0;

      ImGui::InputInt("Number of timesteps", &app->nsteps);
      if (app->nsteps < 1) app->nsteps = 1000;

      ImGui::PopItemWidth();

    ImGui::SeparatorText("DISPLAY CONTROLS");

      ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);

      ImGui::ColorEdit3("Background color", (float*)&app->clear_color);

      if (ImGui::Combo("Color palette", &app->color_selector, color_items, IM_ARRAYSIZE(color_items))) {
        app->color = color_values[app->color_selector];
        app->change_color_palette = true;
      }

      ImGui::PopItemWidth();

      ImGui::Checkbox("Log color scale", &app->log_scale);
      ImGui::BeginDisabled(app->log_scale);
      ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
      ImGui::SliderFloat("Mass Clippping", &app->clip_factor, 0.000001f, 1.0f, "Value: %.6f", ImGuiSliderFlags_Logarithmic);
      ImGui::PopItemWidth();
      ImGui::EndDisabled();
      if (app->log_scale) app->clip_factor = 1.0f;

    ImGui::SeparatorText("SIMULATION CONTROLS");

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

      ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.33f);
      ImGui::SliderFloat("System evolution boost", &app->evolution_speed, 1.0f, 2.0f, "Value: %.2f");
      ImGui::PopItemWidth();

      ImGui::Checkbox("Cyclical time", &app->reverse_time);

    ImGui::SeparatorText("RUNTIME OUTPUT");

      ImGui::Text("Current redshift value %.3f", app->current_redshift);

      ImGui::Text("%.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);

  ImGui::End();
}

