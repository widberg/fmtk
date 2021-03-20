add_executable(crc32gpu
	crc32gpu.cu
	hashes.cu
	exts.cu
	dictionary.cu
)

target_compile_options(crc32gpu PRIVATE -rdc=true --default-stream per-thread -G -g)
target_link_libraries(crc32gpu PRIVATE cudadevrt cudart)
set_target_properties(crc32gpu PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(crc32gpu PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
set_target_properties(crc32gpu PROPERTIES CUDA_ARCHITECTURES 75)

set_target_properties(crc32gpu PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
	
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
