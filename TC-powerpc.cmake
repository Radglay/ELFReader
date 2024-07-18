set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR powerpc)

set(CMAKE_CXX_COMPILER /usr/bin/powerpc-linux-gnu-g++)
set(CMAKE_C_COMPILER /usr/bin/powerpc-linux-gnu-gcc)

set(CMAKE_FIND_ROOT_PATH /usr/powerpc-linux-gnu)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_CROSSCOMPILING_EMULATOR "qemu-ppc-static;-L;/usr/powerpc-linux-gnu")