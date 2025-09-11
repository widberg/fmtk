#include <ctime>
#include <fmtksdk/fmtksdk.hpp>
#include <string>

#include "discord.h"

discord::Core *core{};

class FMTKDiscord : FMTKMod {
public:
  virtual void Initialize() override {
    auto result = discord::Core::Create(1053341192026333264,
                                        DiscordCreateFlags_Default, &core);
    fmtk->Log(LogLevel::INFO, "FMTKDiscord",
              (std::string("discord::Core::Create = ") +
               std::to_string((unsigned int)result))
                  .c_str());
    discord::Activity activity{};
    activity.SetType(discord::ActivityType::Playing);
    activity.GetAssets().SetLargeImage("drownington_cove");
    activity.GetAssets().SetSmallImage("freeride");
    activity.SetDetails("Drownington Cove");
    activity.SetState("In offline free ride");
    activity.GetTimestamps().SetStart(std::time(0));
    core->ActivityManager().UpdateActivity(
        activity, [](discord::Result result) {
          fmtk->Log(LogLevel::INFO, "FMTKDiscord",
                    (std::string("core->ActivityManager().UpdateActivity = ") +
                     std::to_string((unsigned int)result))
                        .c_str());
        });
  }

  virtual void Tick() override { ::core->RunCallbacks(); }

  virtual void Shutdown() override {}
};

REGISTER_MOD(FMTKDiscord);
