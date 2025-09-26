#include <optional>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <ctime>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include <discord.h>

#include "D3DTnL_Renderer_Z.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EXTERN_C HWND _0x00A7D36C;

EXTERN_C D3D_Renderer_Z *_0x00A7C084;

EXTERN_C bool __stdcall _0x0069A590(void *a1, const char *a2, int a3) asm("__0x0069A590");
EXTERN_C void *_0x00A7C080;

discord::Core *core = nullptr;
discord::Activity activity{};

struct ActivityInfo {
    const char *image;
    const char *text;
};

static const ActivityInfo hub_infos[] = {
    { "out_of_area", "Out of Area" },
    { "offshore_shack", "Offshore Shack" },
    { "tsunami_reef", "Tsunami Reef" },
    { "dry_rigs", "Dry Rigs" },
    { "big_cauldrons_edge", "Big Cauldron's Edge" },
    { "drownington_cove", "Drownington Cove" },
    { "the_ashtray", "The Ashtray" },
    { "pinwheels_ridge", "Pinwheels Ridge" },
    { "stargazers_heights", "Stargazers' Heights" },
    { "redrock_bluffs", "Redrock Bluffs" },
    { "dead_ladys_lake", "Dead Lady's Lake" },
    { "firemoor_fields", "Firemoor Fields" },
    { "blacktrail_pass", "Blacktrail Pass" },
    { "the_hangar", "The Hangar" },
    { "dustbowl_city", "Dustbowl City" },
    { "roughwood_hill", "Roughwood Hill" },
    { "smokesters_crater", "Smokesters' Crater" },
    { "the_white_flats", "The White Flats" },
    { "big_cleave", "Big Cleave" },
    { "rainier_peak", "Rainier Peak" },
};

static const ActivityInfo status_infos[] = {
    { "", "Idle" },
    { "freeride", "Offline free ride" },
    { "career", "Career Race" },
    { "career", "Ranked match" },
    { "career", "In Lobby" },
    { "career", "Player Match" },
    { "freeride", "Online free ride" },
    { "challenge", "Challenge Mode" },
    { "freeride", "Using Race Editor" },
};

static std::optional<int> current_hub = {};
static int current_status = 0;

void update_discord_activity(void)
{
    if (current_status != 0 && !current_hub.has_value())
        return;

    if (current_status < 0 || current_status >= sizeof(status_infos) / sizeof(status_infos[0]))
        current_status = 0;
    const ActivityInfo &status_info = status_infos[current_status];
    activity.GetAssets().SetSmallImage(status_info.image);
    activity.SetState(status_info.text);

    if (current_status == 0) {
        activity.GetAssets().SetLargeImage("fmtk");
        activity.SetDetails("");
    } else {
        int current_hub_plus_one = current_hub.value() + 1;
        if (current_hub_plus_one < 0 || current_hub_plus_one >= sizeof(hub_infos) / sizeof(hub_infos[0]))
            current_hub_plus_one = 0;
        const ActivityInfo &hub_info = hub_infos[current_hub_plus_one];
        activity.GetAssets().SetLargeImage(hub_info.image);
        activity.SetDetails(hub_info.text);
    }

    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
}

void fmtk_extension_point_after_engine_init(void)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_0x00A7D36C);
    ImGui_ImplDX9_Init(_0x00A7C084->pD3DDevice);

    discord::Core::Create(1053341192026333264, DiscordCreateFlags_Default, &core);
    activity.SetType(discord::ActivityType::Playing);
    activity.GetTimestamps().SetStart(std::time(0));
    update_discord_activity();
}

void fmtk_extension_point_before_core_main_loop(void)
{
    core->RunCallbacks();
}

void fmtk_extension_point_map_hud_info(int hub)
{
    if (current_hub != hub)
    {
        current_hub = hub;
        update_discord_activity();
    }
}

void fmtk_extension_point_presence_updated(int context_value)
{
    if (current_status != context_value)
    {
        current_status = context_value;
        update_discord_activity();
    }
}

void fmtk_extension_point_before_engine_shutdown(void)
{
    delete core;

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void fmtk_extension_point_imgui_frame_inner(void)
{
    // Our state
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static char command[2048] = {};

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        if (ImGui::InputText("cmd", command, sizeof(command), ImGuiInputTextFlags_EnterReturnsTrue)) {
            _0x0069A590(_0x00A7C080, command, 0);
            command[0] = 0;
        }

        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}

static bool show_imgui_demo = false;

void fmtk_extension_point_imgui_frame(void)
{
    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (show_imgui_demo)
        fmtk_extension_point_imgui_frame_inner();

    // Rendering
    ImGui::EndFrame();
}

void fmtk_extension_point_before_d3d_end_scene(void)
{
    fmtk_extension_point_imgui_frame(); // do this somewhere else

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

bool fmtk_extension_point_before_winproc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
        return true;

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantTextInput)
    {
        switch (Msg)
        {
        case WM_KEYDOWN:
            if (wParam == VK_OEM_3 && (lParam & 0x40000000) == 0)
            {
                show_imgui_demo = !show_imgui_demo;
                return true;
            }
            break;
        default:
            break;
        }
    }

    return false;
}
