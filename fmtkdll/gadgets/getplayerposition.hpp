//$ symbol fuel::GetPlayerPosition, "bool (__usercall *)@<al>(float *playerPosVecOut@<edi>)";
//$ address fuel::GetPlayerPosition, [retail], @0x00421F60;

bool (__usercall *Real_GetPlayerPosition)@<al>(float *playerPosVecOut@<edi>) =
	reinterpret_cast<bool (__usercall *)@<al>(float *playerPosVecOut@<edi>)>(0x00421F60);

float* GetPlayerPosition()
{
	static float vec[3];
	if (Real_GetPlayerPosition(vec))
	{
		return vec;
	}

	return nullptr;
}
