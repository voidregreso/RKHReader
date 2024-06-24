cmake_minimum_required(VERSION 3.10)
project(RKHReader)

set(CMAKE_CXX_STANDARD 17)

# Add OpenSSL library
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${OPENSSL_INCLUDE_DIR})

# Add source files
set(SOURCES
    main.cpp
    ByteOperations.cpp
    Converter.cpp
    QualcommPartition.cpp
)

# Add header files (for IDEs)
set(HEADERS
    ByteOperations.h
    Converter.h
    QualcommPartition.h
)

# Check if using MSVC compiler
if(MSVC)
    # Enable parallel build
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    # Set CMake flag to use static runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Alternatively, set compiler flags to explicitly specify /MT and /MTd
    # Set /MT, /MTd for all targets
    foreach(flag_var
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
endif()

# Create the executable
add_executable(RKHReader ${SOURCES} ${HEADERS})

# Link OpenSSL libraries
target_link_libraries(RKHReader OpenSSL::SSL OpenSSL::Crypto)

# Link system libraries on Windows
if (WIN32)
    target_link_libraries(RKHReader ws2_32.lib crypt32.lib)
endif()
