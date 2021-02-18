#include "pluginshare.h"

noePluginFn_t *g_nfn = NULL;
mathImpFn_t *g_mfn = NULL;

extern const char *g_pPluginName;
extern const char *g_pPluginDesc;
extern bool NPAPI_InitLocal(void);
extern void NPAPI_ShutdownLocal(void);

//=========================================
//Main Noesis interface
//=========================================

//called by Noesis to init the plugin
NPLUGIN_API bool NPAPI_Init(mathImpFn_t *mathfn, noePluginFn_t *noepfn)
{
	g_mfn = mathfn;
	g_nfn = noepfn;
	return NPAPI_InitLocal();
}

//called by Noesis before the plugin is freed
NPLUGIN_API void NPAPI_Shutdown(void)
{
	NPAPI_ShutdownLocal();
}

//returns current version
NPLUGIN_API int NPAPI_GetPluginVer(void)
{
	return NOESIS_PLUGIN_VERSION;
}

//copies off plugin info strings
NPLUGIN_API bool NPAPI_GetPluginInfo(noePluginInfo_t *infOut)
{
	strcpy_s(infOut->pluginName, 64, g_pPluginName);
	strcpy_s(infOut->pluginDesc, 512, g_pPluginDesc);
	return true;
}
