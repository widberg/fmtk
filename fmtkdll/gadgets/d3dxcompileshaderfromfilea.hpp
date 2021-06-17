FUNCTION(D3DXCompileShaderFromFileA, 0x008b11e6, HRESULT, WINAPI,
	LPCSTR                          pSrcFile,
	CONST D3DXMACRO* pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER* ppShader,
	LPD3DXBUFFER* ppErrorMsgs,
	LPD3DXCONSTANTTABLE* ppConstantTable)
{
	LOG(trace, FMTK, "Compiling shader: {}", pSrcFile);

	*ppErrorMsgs = nullptr;
	LPD3DXBUFFER errorBuffer;

	HRESULT result = Real_D3DXCompileShaderFromFileA(pSrcFile, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, &errorBuffer, ppConstantTable);

	if (errorBuffer != nullptr)
	{
		LOG(error, FMTK, "{}", (char*)errorBuffer->GetBufferPointer());
	}

	return result;
}