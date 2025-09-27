#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <ctime>
#include <optional>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/wincolor_sink.h>

#include <lua.h>
#include <luaconf.h>
#include <lualib.h>
#include <luacode.h>
#include <LuaBridge/LuaBridge.h>
#include <Luau/Common.h>

#include <discord.h>

#include "D3DTnL_Renderer_Z.h"

#include "backward.hpp"

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

inline int luaL_loadstring(lua_State *L, const char *s)
{
    std::size_t bytecodeSize = 0;

    auto bytecode = std::shared_ptr<char>(
        luau_compile(s, std::strlen(s), nullptr, &bytecodeSize),
        [](char* x) { std::free(x); }
    );

    return luau_load(L, "...", bytecode.get(), bytecodeSize, 0);
}

inline int traceback(lua_State* L)
{
    // look up Lua's 'debug.traceback' function
    lua_getglobal(L, "debug");
    if (!lua_istable(L, -1))
    {
        lua_pop(L, 1);
        return 1;
    }

    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 2);
        return 1;
    }

    lua_pushvalue(L, 1);
    lua_pushinteger(L, 2);
    lua_call(L, 2, 1);

    const char *tracebackString = lua_tostring(L, -1);
    SPDLOG_ERROR(tracebackString);

    lua_pop(L, 2);

    return 1;
}

static lua_State *L = nullptr;

// Can't make this a global because of static initialization fiasco
backward::SignalHandling &theSignalHandling()
{
    static backward::SignalHandling sh;
    return sh;
}

void fmtk_extension_point_before_win_main(void)
{
    theSignalHandling();

    auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
    auto rotating_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/fmtk.txt", 1024ull * 1024ull * 1024ull, 5, true);
    auto ringbuffer_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(256);

    auto logger = std::make_shared<spdlog::logger>(spdlog::logger("fmtk_logger", {console_sink, rotating_file_sink, ringbuffer_sink}));
    spdlog::set_default_logger(logger);
    SPDLOG_INFO("Logging initialized");

    L = luaL_newstate();

    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginNamespace("fmtk")
            .addFunction("info", [](std::string msg) { SPDLOG_INFO(msg); })
            .addFunction("crash", []() {
                volatile int *null = nullptr;
                (void)*null;
            })
        .endNamespace();
}

void fmtk_extension_point_after_win_main(void)
{
    lua_close(L);
}

void fmtk_extension_point_after_engine_init(void)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_0x00A7D36C);
    ImGui_ImplDX9_Init(_0x00A7C084->pD3DDevice);

    discord::Core::Create(1053341192026333264, DiscordCreateFlags_Default, &core);
    core->SetLogHook(discord::LogLevel::Debug, [](discord::LogLevel discord_log_level, const char *msg) {
        switch (discord_log_level)
        {
        case discord::LogLevel::Error:
            SPDLOG_ERROR(msg);
            break;
        case discord::LogLevel::Warn:
            SPDLOG_WARN(msg);
            break;
        case discord::LogLevel::Info:
            SPDLOG_INFO(msg);
            break;
        case discord::LogLevel::Debug:
        default:
            SPDLOG_DEBUG(msg);
            break;
        }
    });

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

struct Console
{
    char                  TSCInputBuf[2048];
    char                  LuauInputBuf[2048];
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

    Console()
    {
        ClearLog();
        memset(TSCInputBuf, 0, sizeof(TSCInputBuf));
        memset(LuauInputBuf, 0, sizeof(LuauInputBuf));
        HistoryPos = -1;

        AutoScroll = true;
        ScrollToBottom = false;
        SPDLOG_INFO("Welcome to Dear ImGui!");
    }
    ~Console()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            ImGui::MemFree(History[i]);
    }

    static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void    ClearLog()
    {
    }

    void    Draw()
    {
        ImGui::SetNextWindowSize(ImVec2(540, 620), ImGuiCond_FirstUseEver);
        ImGui::Begin("Console");

        if (ImGui::SmallButton("Clear"))           { ClearLog(); }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O, ImGuiInputFlags_Tooltip);
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = 2 * (ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing());
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) ClearLog();
                ImGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
            // to only process visible items. The clipper will automatically measure the height of your first item and then
            // "seek" to display only items in the visible area.
            // To use the clipper we can replace your standard loop:
            //      for (int i = 0; i < Items.Size; i++)
            //   With:
            //      ImGuiListClipper clipper;
            //      clipper.Begin(Items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
                ImGui::LogToClipboard();
            for (auto item : dynamic_cast<spdlog::sinks::ringbuffer_sink_mt*>(spdlog::get("fmtk_logger")->sinks().at(2).get())->last_raw())
            {
                if (!Filter.PassFilter(item.payload.begin(), item.payload.end()))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                switch (item.level)
                {
                case spdlog::level::trace:
                case spdlog::level::debug:
                case spdlog::level::info:
                    color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                    break;
                case spdlog::level::warn:
                    color = ImVec4(1.0f, 0.85f, 1.0f, 1.0f);
                    break;
                case spdlog::level::err:
                case spdlog::level::critical:
                    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                    break;
                case spdlog::level::off:
                default:
                    continue;
                }
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item.payload.begin(), item.payload.end());
                ImGui::PopStyleColor();
            }
            if (copy_to_clipboard)
                ImGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("TSC", TSCInputBuf, IM_ARRAYSIZE(TSCInputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
        {
            char* s = TSCInputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            TSCInputBuf[0] = '\0';
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        if (ImGui::InputText("Luau", LuauInputBuf, IM_ARRAYSIZE(LuauInputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            luabridge::lua_pushcfunction_x(L, &traceback, "traceback");
            luaL_loadstring(L, LuauInputBuf);
            lua_pcall(L, 0, 0, -2);
            LuauInputBuf[0] = '\0';
        }

        ImGui::End();
    }

    void    ExecCommand(const char* command_line)
    {
        SPDLOG_INFO("# {}\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (_stricmp(History[i], command_line) == 0)
            {
                ImGui::MemFree(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(_strdup(command_line));

        _0x0069A590(_0x00A7C080, command_line, 0);

        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
    {
        Console* console = (Console*)data->UserData;
        return console->TextEditCallback(data);
    }

    int     TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
            {
                // Example of TEXT COMPLETION

                // Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf)
                {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Build a list of candidates
                ImVector<const char*> candidates;

                if (candidates.Size == 0)
                {
                    // No match
                    SPDLOG_INFO("No match for \"{}\"!\n", std::string_view(word_start, word_end - word_start));
                }
                else if (candidates.Size == 1)
                {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                }
                else
                {
                    // Multiple matches. Complete as much as we can..
                    // So inputting "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                    int match_len = (int)(word_end - word_start);
                    for (;;)
                    {
                        int c = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0)
                                c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches)
                            break;
                        match_len++;
                    }

                    if (match_len > 0)
                    {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    SPDLOG_INFO("Possible matches:\n");
                    for (int i = 0; i < candidates.Size; i++)
                        SPDLOG_INFO("- {}\n", candidates[i]);
                }

                break;
            }
        case ImGuiInputTextFlags_CallbackHistory:
            {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos)
                {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
};

void fmtk_extension_point_imgui_frame_inner(void)
{
    // Our state
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static char command[2048] = {};
    static Console console;

    console.Draw();

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

    ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

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
