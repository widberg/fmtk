from inc_noesis import *
from inc_fuel import *

def registerNoesisTypes():
	handle = noesis.register('FUEL Skeleton','.TSKEL')
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

	bs.seek(20,1)	
	jointCount = bs.readUInt()
	bs.seek(24,1)
	
	jointList = []	
	for i in range(jointCount):
		start = bs.tell()
		bs.seek(140,1)
		mat = NoeMat44.fromBytes(bs.readBytes(64)).toMat43()		
		unk = bs.readInt()
		parent = bs.readInt()		
		joint = NoeBone(i, str(i), mat, None, parent)
		jointList.append(joint)
		bs.seek(start + 248)
	
	try:
		mdl = rapi.rpgConstructModel()
	except:
		mdl = NoeModel()
		
	mdl.setBones(jointList)
	mdlList.append(mdl)
	
	return 1
	
	