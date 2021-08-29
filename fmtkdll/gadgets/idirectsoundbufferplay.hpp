BF(HRESULT __usercall Real_IDirectSoundBuffer_Play AT eax)(LPDIRECTSOUNDBUFFER that, DWORD dwReserved1, DWORD dwReserved2, DWORD dwFlags)
	(0x53E30120);

FF(HRESULT __usercall FMTK_IDirectSoundBuffer_Play AT eax)(LPDIRECTSOUNDBUFFER that, DWORD dwReserved1, DWORD dwReserved2, DWORD dwFlags)
(
	long volume;
	if (that->GetVolume(&volume) == DS_OK && volume != DSBVOLUME_MIN)
	{
		LOG(trace, FMTK, "Volume: {}", volume);
	}

	RETURN(Real_IDirectSoundBuffer_Play(that, dwReserved1, dwReserved2, dwFlags));
)

// SetVolume 0x53E32770
// CreateSoundBuffer 0x53E205C0