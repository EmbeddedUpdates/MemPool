#Component specific makefile for MemPool
$(info Executing: Components/MemPool/make/MemPool.mak)
#C files that should be compiled in this component
C_SOURCE_FILES += Components/MemPool/Source/MemPool.c

#include path for header files in this component
INCLUDE_PATH += $(ROOT_DIR)/Components/MemPool/Include
