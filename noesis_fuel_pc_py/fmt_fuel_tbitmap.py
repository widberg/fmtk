from inc_noesis import *
from inc_fuel import *

def registerNoesisTypes():
	handle = noesis.register('FUEL Bitmap','.TBITMAP')
	noesis.setHandlerTypeCheck(handle, CheckType)
	noesis.setHandlerLoadRGBA(handle, LoadRGBA)	
	return 1
	
def CheckType(data):
	bs = NoeBitStream(data)

	header = fuel.readClassHeader(bs)

	return header.classCRC32 == 1471281566 and header.classObjectSize == 32

def LoadRGBA(data, texList):
	bs = NoeBitStream(data)

	header = fuel.readClassHeader(bs)

	s = struct.Struct('<IHIIBBBBBBBBBBBBBBBBBB')
	crc32, u0, width, height, u1, u2, u3, u4, u5, bitmapType, u6, u7, u8, u9, u10, u11, dxtType, mipMapCount, u13, u14, u15, u16 = s.unpack(bs.readBytes(s.size))
	
	if header.compressedSize:
		bs = NoeBitStream(fuel.decompress(bs.readBytes(header.compressedSize)))

	texType = noesis.NOESISTEX_UNKNOWN
	if dxtType == 14:
		texType = noesis.NOESISTEX_DXT1
	elif dxtType == 16:
		texType = noesis.NOESISTEX_DXT5

	texList.append(NoeTexture('__fuel_tbitmap_{}'.format(crc32), width, height, bs.getBuffer(bs.tell(), bs.tell() + header.decompressedSize), texType))

	return 1
