BF(HRESULT __usercall Real_DSOUND_IDirectSoundBuffer_Play AT eax)(LPDIRECTSOUNDBUFFER that, DWORD dwReserved1, DWORD dwReserved2, DWORD dwFlags)
	(0x40120);

FF(HRESULT __usercall FMTK_DSOUND_IDirectSoundBuffer_Play AT eax)(LPDIRECTSOUNDBUFFER that, DWORD dwReserved1, DWORD dwReserved2, DWORD dwFlags)
(
	RETURN(Real_DSOUND_IDirectSoundBuffer_Play(that, dwReserved1, dwReserved2, dwFlags));
)

BF(HRESULT __usercall Real_DSOUND_IDirectSoundBuffer_SetVolume AT eax)(LPDIRECTSOUNDBUFFER that, LONG lVolume)
	(0x42770);

FF(HRESULT __usercall FMTK_DSOUND_IDirectSoundBuffer_SetVolume AT eax)(LPDIRECTSOUNDBUFFER that, LONG lVolume)
(
	RETURN(Real_DSOUND_IDirectSoundBuffer_SetVolume(that, lVolume));
)

BF(ULONG __usercall Real_DSOUND_IDirectSoundBuffer_Release AT eax)(LPDIRECTSOUNDBUFFER that)
	(0x403C0);

FF(ULONG __usercall FMTK_DSOUND_IDirectSoundBuffer_Release AT eax)(LPDIRECTSOUNDBUFFER that)
(
	RETURN(Real_DSOUND_IDirectSoundBuffer_Release(that));
)

using LPIUnknown = IUnknown FAR*;

BF(HRESULT __usercall Real_DSOUND_IDirectSound_CreateSoundBuffer AT eax)(LPDIRECTSOUNDBUFFER that, LPCDSBUFFERDESC lpcDSBufferDesc, LPLPDIRECTSOUNDBUFFER lplpDirectSoundBuffer, LPIUnknown pUnkOuter)
	(0x305C0);

FF(HRESULT __usercall FMTK_DSOUND_IDirectSound_CreateSoundBuffer AT eax)(LPDIRECTSOUNDBUFFER that, LPCDSBUFFERDESC lpcDSBufferDesc, LPLPDIRECTSOUNDBUFFER lplpDirectSoundBuffer, LPIUnknown pUnkOuter)
(
	HRESULT result = Real_DSOUND_IDirectSound_CreateSoundBuffer(that, lpcDSBufferDesc, lplpDirectSoundBuffer, pUnkOuter);
	(*lplpDirectSoundBuffer)->SetVolume(DSBVOLUME_MIN);
	RETURN(result);
)

// before user 0x00689244
