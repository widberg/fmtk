//$ symbol fuel::GetPlayerPosition, "bool (__usercall *)@<al>(float *playerPosVecOut@<edi>)";
//$ address fuel::GetPlayerPosition, [retail], @0x00421F60;

float* GetPlayerPosition()
{
	static float vec[3];
	if (real_fuel_GetPlayerPosition(vec))
	{
		return vec;
	}

	return nullptr;
}
