from inc_noesis import *
from struct import *
from collections import namedtuple

class fuel:
	def readClassHeader(bs):
		s = struct.Struct('<IIIIII')
		ClassHeader = namedtuple('ClassHeader', 'dataSize classObjectSize decompressedSize compressedSize classCRC32 crc32')
		classHeader = ClassHeader._make(s.unpack(bs.readBytes(s.size)))
		return classHeader

	def calculatePaddedSize(unpaddedSize):
		return (unpaddedSize + 0x7ff) & 0xfffff800

	def calculatePaddingSize(unpaddedSize):
		return fuel.calculatePaddedSize(unpaddedSize) - unpaddedSize

	def decompress(data):
		WINDOW_LOG = 14
		WINDOW_SIZE = (1 << WINDOW_LOG)
		WINDOW_MASK = (1 << WINDOW_LOG) - 1

		bs = NoeBitStream(data)
		
		decompressedSize = bs.readUInt()
		compressedSize = bs.readUInt()

		windowBuffer = bytearray(WINDOW_SIZE)

		decompressed = NoeBitStream()
		
		flagbit = 0
		pos = 0
		while decompressed.getSize() < decompressedSize:
			if flagbit <= 1:
				flagmask = bs.readUByte() << 24
				flagmask |= bs.readUByte() << 16
				flagmask |= bs.readUByte() << 8
				flagmask |= bs.readUByte()
				flagbit = 32 - 1
				lenbits = WINDOW_LOG - (flagmask & 3)

			flag = (flagmask >> flagbit) & 1
			flagbit -= 1

			currentByte = bs.readUByte()

			if flag == 0:
				windowBuffer[pos & WINDOW_MASK] = currentByte
				pos += 1
				decompressed.writeByte(currentByte)
			else:
				d = bs.readUByte()
				j = (currentByte << 8) + d

				length = (j >> lenbits) + 3
				d = (j & ((1 << lenbits) - 1)) + 1

				for j in range(length):
					currentByte = windowBuffer[(pos - d) & WINDOW_MASK]
					windowBuffer[pos & WINDOW_MASK] = currentByte
					pos += 1
					decompressed.writeByte(currentByte)

		return decompressed.getBuffer()
