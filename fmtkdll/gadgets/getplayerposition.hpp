BF(char __usercall Real_GetPlayerPosition AT al)(p_float_t playerPosVecOut AT edi)
	(0x00421F60);

float* GetPlayerPosition()
{
	static float vec[3];
	if (Real_GetPlayerPosition_trampoline(vec))
	{
		return vec;
	}

	return nullptr;
}
