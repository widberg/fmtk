#include "instrument.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// #include <dsound.h>

#include <algorithm>
#include <cwctype>
#include <d3dx9shader.h>
#include <filesystem>
#include <fstream>
#include <sinker/sinker.hpp>

using namespace sinker;

#include "fmtkdll.hpp"
#include "backward.hpp"
#include "logging.hpp"

//$ symbol fuel::pGlobalCommandState, "const void**";
//$ address fuel::pGlobalCommandState, [retail], @0x00a7c080;

struct KeyState {
  bool is_down_test;
  bool some_condition1_or;
  bool is_digital;
  bool field_3;
  int zero1;
  bool some_condition1;
  bool field_9;
  bool some_down;
  bool field_B;
  float some_duration_minus_delta;
  float some_duration_minus_delta_replacement_value;
  float some_duration_minus_delta_replacement_value_other;
  int is_less_than;
  float activation_threshold_floor;
  float activation_threshold;
  float activation_threshold_ceil;
  bool some_condition;
  bool field_29;
  bool field_2A;
  bool field_2B;
  int should_not_square_result;
  float is_down_float_analog;
  bool is_something;
  bool is_key_pressed;
  bool is_down_final;
  bool field_37;
  float time_down_while_down;
  float time_since_last_down_event;
  float time_since_last_up_event;
  int zero;
  int key_code;
};

#define SINKER_SYMBOL(module_name, symbol_name, symbol_type)                   \
  auto real_##module_name##_##symbol_name = static_cast<symbol_type>(nullptr);
#include "fmtk.def"

#include "gadgets/coremainloop.hpp"
#include "gadgets/createdialogparama.hpp"
#include "gadgets/createfilew.hpp"
#include "gadgets/createwindowexw.hpp"
#include "gadgets/d3dxcompileshaderfromfilea.hpp"
#include "gadgets/getplayerposition.hpp"
#include "gadgets/input.hpp"
#include "gadgets/outputdebugstringa.hpp"
#include "gadgets/outputdebugstringw.hpp"
#include "gadgets/readfile.hpp"
#include "gadgets/registercommand.hpp"
#include "gadgets/regqueryvalueexw.hpp"
#include "gadgets/runcommand.hpp"
#include "gadgets/scriptmanagerinit.hpp"
#include "gadgets/securom.hpp"
#include "gadgets/validatememory.hpp"
#include "gadgets/winmain.hpp"

// #include "gadgets/idirectsoundbufferplay.hpp"

// Instrument
LONG AttachDetours() {
  LOG(trace, FMTK, "Concretizing modules");

  sinker::Context ctx;
  std::ifstream in("mods/fmtk/fmtk.skr", std::ios::binary);
  if (!in.good()) {
    LOG(critical, FMTK, "Can't open sinker file!");
    return 0;
  }

  if (!ctx.interpret(in, sinker::Language::SINKER, "fmtk.skr")) {
    LOG(critical, FMTK, "Interpreting sinker file failed!");
    return 0;
  }

#define SINKER_MODULE(module_name)                                             \
  if (!ctx.get_module(#module_name)->concretize()) {                           \
    LOG(critical, FMTK, "Concretizing module \"" #module_name "\" failed!");   \
    return 0;                                                                  \
  }
#include "fmtk.def"

  LOG(trace, FMTK, "Calculating addresses");

#define SINKER_SYMBOL(module_name, symbol_name, symbol_type)                   \
  auto module_name##_##symbol_name##_##calculated_address =                    \
      ctx.get_module(#module_name)                                             \
          ->get_symbol(#symbol_name)                                           \
          ->calculate_address<decltype(real_##module_name##_##symbol_name)>(); \
  if (!module_name##_##symbol_name##_##calculated_address &&                   \
      ctx.get_module(#module_name)                                             \
          ->get_symbol(#symbol_name)                                           \
          ->get_attribute<bool>("required")                                    \
          .value_or(true)) {                                                   \
    LOG(critical, FMTK,                                                        \
        "Calculate symbol \"" #module_name "_" #symbol_name                    \
        "_calculated_address"                                                  \
        "\" failed!");                                                         \
    return 0;                                                                  \
  }                                                                            \
  if (module_name##_##symbol_name##_##calculated_address) {                    \
    LOG(trace, FMTK,                                                           \
        "Calculate symbol \"" #module_name "_" #symbol_name                    \
        "_calculated_address"                                                  \
        "\" = {}",                                                             \
        (void *)(module_name##_##symbol_name##_##calculated_address.value())); \
    real_##module_name##_##symbol_name =                                       \
        module_name##_##symbol_name##_##calculated_address.value();            \
  }
#include "fmtk.def"

  LOG(trace, FMTK, "Attaching detours");

  Transaction transaction;

#define SINKER_TAG_hook_SYMBOL(module_name, symbol_name, symbol_type)          \
  auto detour##module_name##_##symbol_name = Detour(                           \
      real_##module_name##_##symbol_name, wrap_##module_name##_##symbol_name); \
  auto action##module_name##_##symbol_name =                                   \
      ActionInstall(&detour##module_name##_##symbol_name);                     \
  transaction.add(&action##module_name##_##symbol_name);
#include "fmtk.def"

  LOG(trace, FMTK, "Ready to commit");

  return transaction.commit();
}

LONG DetachDetours() {
  LOG(trace, FMTK, "Detaching detours");

  Transaction transaction;

#define SINKER_TAG_hook_SYMBOL(module_name, symbol_name, symbol_type)          \
  auto detour##module_name##_##symbol_name = Detour(                           \
      real_##module_name##_##symbol_name, wrap_##module_name##_##symbol_name); \
  auto action##module_name##_##symbol_name =                                   \
      ActionUninstall(&detour##module_name##_##symbol_name);                   \
  transaction.add(&action##module_name##_##symbol_name);
#include "fmtk.def"

  LOG(trace, FMTK, "Ready to commit");

  return transaction.commit();
}
