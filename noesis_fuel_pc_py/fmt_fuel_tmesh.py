from inc_noesis import *
from inc_fuel import *

def registerNoesisTypes():
	handle = noesis.register('FUEL Mesh','.TMESH')
	noesis.setHandlerTypeCheck(handle, CheckType)
	noesis.setHandlerLoadModel(handle, LoadModel)	
	return 1
	
def CheckType(data):
	bs = NoeBitStream(data)
	return 1

def LoadModel(data, mdlList):
	ctx = rapi.rpgCreateContext()
	bs = NoeBitStream(data)
	
	header = fuel.readClassHeader(bs)

	bs.seek(header.classObjectSize, 1)

	if header.compressedSize:
		bs = NoeBitStream(fuel.decompress(bs.readBytes(header.compressedSize)))

	bs.seek(24, 1)
	materialCRC32Count = bs.readUInt()
	bs.seek(materialCRC32Count * 4, 1)

	unknown5Count = bs.readUInt()
	bs.seek(unknown5Count * 32, 1)

	unknown6Count = bs.readUInt()
	bs.seek(unknown6Count * 8, 1)

	unknown7Count = bs.readUInt()

	vertices = []
	vertexStrides = []

	subMeshCount = bs.readUInt()
	for i in range(subMeshCount):
		vertexCount = bs.readUInt()
		vertexStride = bs.readUInt()
		bs.readUInt()

		vertices.append(bs.getBuffer(bs.tell(), bs.tell() + vertexCount * vertexStride))
		bs.seek(vertexCount * vertexStride, 1)
		vertexStrides.append(vertexStride)

	indicesCount = bs.readUInt()
	for i in range(indicesCount):
		indexCount = bs.readUInt()
		bs.readUInt()
		indices = bs.getBuffer(bs.tell(), bs.tell() + indexCount * 2)
		bs.seek(indexCount * 2, 1)
		
		rapi.rpgSetName('__fuel_tmesh_{}_{}'.format(header.crc32, i))
		rapi.rpgBindPositionBuffer(vertices[i], noesis.RPGEODATA_FLOAT, vertexStrides[i])
		rapi.rpgCommitTriangles(indices, noesis.RPGEODATA_USHORT, indexCount, noesis.RPGEO_TRIANGLE)

	rapi.rpgSetOption(noesis.RPGOPT_TRIWINDBACKWARD, True)
	mdl = rapi.rpgConstructModel()
	mdlList.append(mdl)

	return 1
