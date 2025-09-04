
// These must be included in exactly one source file
#define SDL_MAIN_USE_CALLBACKS


#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif


#include <memory>

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "simulator.hh"
#include "camera.hh"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct MousePosition {
    float x, y;
};

class AppState {
public:
    AppState();
    ~AppState() = default;
    std::unique_ptr<ParticleMeshSimulator> sim_ptr;
    SDL_Window *window_ptr;
    SDL_GLContext context_ptr;
    bool app_finished{false};
    bool show_demo_window{true};
    bool show_another_window{false};
    ImVec4 clear_color{ImVec4(0.0f, 0.0f, 0.0f, 1.00f)};

    bool sim_initialized{false};
    bool pause_state{true};
    bool mouse_dragging{false};
    std::unique_ptr<Camera> cam;
};

AppState::AppState()
{
    sim_ptr = std::make_unique<ParticleMeshSimulator>();
    cam = std::make_unique<Camera>(glm::vec3(0.0f,0.0f,-5.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
}



SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{   
    *appstate = new AppState;
    auto *app = static_cast<AppState*>(*appstate);

    SDL_SetAppMetadata("Example Renderer Points", "1.0", "com.example.renderer-points");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

    app->window_ptr  = SDL_CreateWindow("ImGui+SDL3+OpenGL3", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    app->context_ptr = SDL_GL_CreateContext(app->window_ptr);



    if (app->window_ptr == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    

    if (app->context_ptr == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_MakeCurrent(app->window_ptr, app->context_ptr);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetWindowPosition(app->window_ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(app->window_ptr);




    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(app->window_ptr, app->context_ptr);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details. If you like the default font but want it to scale better, consider using the 'ProggyVector' from the same author!
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    //app->sim_ptr->initialize_simulation();
    app->sim_ptr->sim_change_pause_state(true);
    app->sim_ptr->sim_set_write_output(false);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}








/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *ev)
{
    auto *app = static_cast<AppState*>(appstate);

    ImGui_ImplSDL3_ProcessEvent(ev);

    // Quit if escape key pressed, window is closed, or termination signal is received
    if ((ev->type == SDL_EVENT_KEY_DOWN && ev->key.key == SDLK_ESCAPE) ||
        (ev->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && ev->window.windowID == SDL_GetWindowID(app->window_ptr)) ||
        (ev->type == SDL_EVENT_QUIT)) {

        // Sim seg faults if forced to quit before initialization
        if (!app->sim_initialized) { app->sim_ptr->initialize_simulation(1.0f, 1, 1, 1, 1.0f); }

        // Stop and exit
        app->app_finished = true;
        return SDL_APP_SUCCESS;
    }

    // Handle mouse inputs
    switch(ev->type) {

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (ev->button.button == SDL_BUTTON_LEFT) {
                app->mouse_dragging = true;
                app->cam->start_drag(glm::vec2(ev->button.x, ev->button.y));
            }
            break;

        case SDL_EVENT_MOUSE_MOTION:
            if (app->mouse_dragging) {
                ImGuiIO& io = ImGui::GetIO();
                app->cam->update_drag(glm::vec2(ev->motion.x, ev->motion.y), glm::vec2(io.DisplaySize.x, io.DisplaySize.y));
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (ev->button.button == SDL_BUTTON_LEFT) {
                app->mouse_dragging = false;
                app->cam->end_drag();
            }
            break;
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto *app = static_cast<AppState*>(appstate);

    if (SDL_GetWindowFlags(app->window_ptr) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();

    // Get input from user
    static int NGRID = 2;
    static int NBODS = 2097152;
    static float GMAX = 64.0;
    static float RSHIFT = 50.0;
    static int NSTEPS = 1000;
    {
        ImGui::Begin("Options");

            ImGui::SeparatorText("Inputs");

            ImGui::Text("Number of grid cells in 1D -- N in NxNxN");
            ImGui::RadioButton("32", &NGRID, 0); ImGui::SameLine();
            ImGui::RadioButton("64", &NGRID, 1); ImGui::SameLine();
            ImGui::RadioButton("128", &NGRID, 2); ImGui::SameLine();
            ImGui::RadioButton("256", &NGRID, 3); ImGui::SameLine();
            ImGui::RadioButton("512", &NGRID, 4);

            ImGui::InputInt("Number of particles", &NBODS);

            ImGui::InputFloat("Grid length in 1D", &GMAX);

            ImGui::InputFloat("Redshift value", &RSHIFT);

            ImGui::InputInt("Number of timesteps", &NSTEPS);

        ImGui::End();


        ImGui::Begin("Controls");

            if (ImGui::Button("START")) {
                app->pause_state = false;
                app->sim_ptr->sim_change_pause_state(app->pause_state);
            }

            if (ImGui::Button("PAUSE")) {
                app->pause_state = true;
                app->sim_ptr->sim_change_pause_state(app->pause_state);
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(app->clear_color.x * app->clear_color.w, app->clear_color.y * app->clear_color.w, app->clear_color.z * app->clear_color.w, app->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!app->sim_ptr->sim_is_paused() && !app->sim_initialized) {
        app->sim_ptr->initialize_simulation(RSHIFT, NSTEPS, NBODS, NGRID, GMAX);
        app->sim_initialized = true;
    }

    if (!app->sim_ptr->sim_is_paused()) { app->sim_ptr->advance_single_timestep(); }


glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glm::mat4 projection = glm::perspective(glm::radians(60.0f), io.DisplaySize.x / io.DisplaySize.y, 0.1f, 200.0f);
glLoadMatrixf(glm::value_ptr(projection));

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glm::mat4 view_matrix = app->cam->get_view_matrix();
glMultMatrixf(glm::value_ptr(view_matrix));


glBegin(GL_QUADS);

    // Front
    glColor3f(1, 0, 0);
    glVertex3f(-1, -1,  1);
    glVertex3f( 1, -1,  1);
    glVertex3f( 1,  1,  1);
    glVertex3f(-1,  1,  1);

    // Back
    glColor3f(0, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1,  1, -1);
    glVertex3f( 1,  1, -1);
    glVertex3f( 1, -1, -1);

    // Top
    glColor3f(0, 0, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1,  1);
    glVertex3f( 1, 1,  1);
    glVertex3f( 1, 1, -1);

    // Bottom
    glColor3f(1, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f( 1, -1, -1);
    glVertex3f( 1, -1,  1);
    glVertex3f(-1, -1,  1);

    // Right
    glColor3f(1, 0, 1);
    glVertex3f(1, -1, -1);
    glVertex3f(1,  1, -1);
    glVertex3f(1,  1,  1);
    glVertex3f(1, -1,  1);

    // Left
    glColor3f(0, 1, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1,  1);
    glVertex3f(-1,  1,  1);
    glVertex3f(-1,  1, -1);

glEnd();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(app->window_ptr);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}






void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if (appstate != NULL) {
        auto *app = static_cast<AppState*>(appstate);
        SDL_GL_DestroyContext(app->context_ptr);
        SDL_DestroyWindow(app->window_ptr);
        delete app;
    }
}

