#include <fmtksdk/fmtksdk.hpp>

class FMTKLua : FMTKMod
{
public:
	virtual void Initialize()
	{
		fmtk->print("TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST");
	}
};

REGISTER_MOD(FMTKLua);