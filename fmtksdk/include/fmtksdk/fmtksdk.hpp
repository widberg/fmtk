#ifndef FMTKSDK_HPP
#define FMTKSDK_HPP

#include <configure.hpp>

struct FMTKVersion
{
	unsigned int major = FMTK_VERSION_MAJOR;
	unsigned int minor = FMTK_VERSION_MINOR;
	unsigned int patch = FMTK_VERSION_PATCH;
	unsigned int tweak = FMTK_VERSION_TWEAK;
};

struct FMTKApi
{
	void (*print)(const char* msg);
};

class FMTKMod
{
public:
	virtual void Initialize() {}
	virtual void Tick() {}

#ifndef FMTK_INTERNAL
// Placeholder methods for forward compatibility
#include "placeholder.hpp"
#endif // FMTK_INTERNAL
};

#ifndef FMTK_INTERNAL

#define FMTKAPI extern "C" __declspec(dllexport) __declspec(noinline)
FMTKAPI const FMTKVersion* GetFMTKVersion();
extern const FMTKVersion fmtkVersion;
extern const FMTKApi* fmtk;

#define REGISTER_MOD(class_name) \
const FMTKVersion fmtkVersion; \
const FMTKApi* fmtk; \
FMTKAPI const FMTKVersion* GetFMTKVersion() { return &fmtkVersion; } \
class_name mod; \
FMTKAPI class_name* GetMod(const FMTKApi* fmtkApi) { fmtk = fmtkApi; return &mod; }

#endif // !FMTK_INTERNAL

#endif // FMTKSDK_HPP