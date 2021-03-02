from inc_noesis import *
from inc_fuel import *
from ctypes import *

validVersionStrings = [
	'v1.381.67.09 - Asobo Studio - Internal Cross Technology',
	'v1.381.66.09 - Asobo Studio - Internal Cross Technology',
	'v1.381.65.09 - Asobo Studio - Internal Cross Technology',
	'v1.381.64.09 - Asobo Studio - Internal Cross Technology',
	'v1.379.60.09 - Asobo Studio - Internal Cross Technology',
	'v1.325.50.07 - Asobo Studio - Internal Cross Technology',
	'v1.220.50.07 - Asobo Studio - Internal Cross Technology'
]

classFileExtensions = {
	549480509: 'TOMNI',
	705810152: 'TRTC',
	838505646: 'TGENWORLD',
	848525546: 'TLIGHTDATA',
	849267944: 'TSOUND',
	849861735: 'TOTEMBITMAP',
	866453734: 'TROTSHAPE',
	954499543: 'TPARTICLESDATA',
	968261323: 'TWORLD',
	1114947943: 'TWARP',
	1135194223: 'TSPLINE',
	1175485833: 'TANIM',
	1387343541: 'TMESH',
	1391959958: 'TUSERDEFINE',
	1396791303: 'TSKIN',
	1471281566: 'TBITMAP',
	1536002910: 'TFONT',
	1625945536: 'TROTSHAPEDATA',
	1706265229: 'TSURFACE',
	1910554652: 'TSPLINEGRAPH',
	1943824915: 'TLOD',
	2204276779: 'TMATERIAL',
	2245010728: 'TNODE',
	2259852416: 'TBINARY',
	2398393906: 'TVOLUME',
	2906362741: 'TWORLDREF',
	3312018398: 'TPARTICLES',
	3412401859: 'TLODDATA',
	3611002348: 'TSKEL',
	3626109572: 'TMESHDATA',
	3747817665: 'TSURFACEDATAS',
	3834418854: 'TMATERIALANIM',
	3845834591: 'TGWROAD',
	4096629181: 'TGAMEOBJ',
	4240844041: 'TCAMERA',
}

def getFileExtension(classCRC32):
	if classCRC32 in classFileExtensions:
		return classFileExtensions[classCRC32]
	else:
		return str(classCRC32)

def getFileName(crc32, classCRC32):
	return '{}.{}'.format(crc32, getFileExtension(classCRC32))

ClassHeader = namedtuple('ClassHeader', 'dataSize classObjectSize decompressedSize compressedSize classCRC32 crc32')

class Resource:
	classHeader = ClassHeader(0, 0, 0, 0, 0, 0)
	classObjectData = bytearray()
	data = bytearray()

def registerNoesisTypes():
	handle = noesis.register('FUEL Archive','.DPC')
	noesis.setHandlerExtractArc(handle, ExtractArc)	
	return 1

def ExtractArc(fileName, fileLen, justChecking):
	with open(fileName, 'rb') as f:
		PrimaryHeader = namedtuple('PrimaryHeader', 'versionString isNotRTC unitCount unitWorkingBufferCapacity resourceWorkingBufferCapacity paddedSize versionPatch versionMinor relationalTablePaddedSize relationalTableOffset relationalTableUnused0 relationalTableUnused1 relationalTableUnused2 incrediBuilderUnused0 incrediBuilderUnused1 incrediBuilderUnused2 incrediBuilderString')
		s = struct.Struct('<256s7I1540x8I128s')
		primaryHeader = PrimaryHeader._make(s.unpack(f.read(s.size)))
		primaryHeader = primaryHeader._replace(versionString = create_string_buffer(primaryHeader.versionString).value.decode('utf8'))
		primaryHeader = primaryHeader._replace(incrediBuilderString = create_string_buffer(primaryHeader.incrediBuilderString).value.decode('utf8'))

		if justChecking:
			return primaryHeader.versionString in validVersionStrings
		
		unitDescriptions = []
		UnitDescription = namedtuple('UnitDescription', 'unitType resourceCount paddedSize dataSize workingBufferOffset crc32')

		f.seek(284)

		for i in range(primaryHeader.unitCount):
			s = struct.Struct('<6I')
			unitDescriptions.append(UnitDescription._make(s.unpack(f.read(s.size))))
		
		f.seek(2048)
		
		s = struct.Struct('<IIIIII')

		resources = {}

		for unitDescription in unitDescriptions:
			for i in range(unitDescription.resourceCount):
				classHeader = ClassHeader._make(s.unpack(f.read(s.size)))
				resource = Resource()
				resource.classHeader = classHeader
				resource.classObjectData = f.read(classHeader.classObjectSize)
				if classHeader.dataSize != classHeader.classObjectSize:
					resource.data = f.read(classHeader.dataSize - classHeader.classObjectSize)
				resources[classHeader.crc32] = resource
				print('Discovered {}'.format(getFileName(classHeader.crc32, classHeader.classCRC32)))
			f.seek(unitDescription.paddedSize - unitDescription.dataSize, 1)

		if primaryHeader.relationalTableOffset:
			f.seek((primaryHeader.relationalTableOffset << 0xB) + (primaryHeader.relationalTablePaddedSize << 0xB))
			
			while True:
				b = f.read(s.size)
				if not b:
					break;
				classHeader = ClassHeader._make(s.unpack(b))
				resource = resources[classHeader.crc32]
				if resource.classHeader.dataSize == resource.classHeader.classObjectSize:
					resource.data = f.read(classHeader.dataSize)
					resource.classHeader = resource.classHeader._replace(dataSize = resource.classHeader.dataSize + classHeader.dataSize, decompressedSize = classHeader.decompressedSize, compressedSize = classHeader.compressedSize)
					f.seek(fuel.calculatePaddingSize(s.size + classHeader.dataSize), 1)
					resources[classHeader.crc32] = resource
				else:
					f.seek(fuel.calculatePaddedSize(s.size + classHeader.dataSize) - s.size, 1)
				print('Discovered {}'.format(getFileName(classHeader.crc32, classHeader.classCRC32)))

		for crc32, resource in resources.items():
			print('Exporting {}'.format(getFileName(resource.classHeader.crc32, resource.classHeader.classCRC32)))
			with open(rapi.exportArchiveFileGetName(getFileName(resource.classHeader.crc32, resource.classHeader.classCRC32)), 'wb') as o:
				o.write(s.pack(*resource.classHeader))
				o.write(resource.classObjectData)
				o.write(resource.data)

	return 1
