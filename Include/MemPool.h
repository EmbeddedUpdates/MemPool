/**
 * 
* @file "MemPool.h"
* @author Madrick3
* @brief A protocol agnostic data buffer for various project .
* 
* @COMPONENT: MemPool
* @VERSION: 01.00.00
*/
/************************************************************
  Version History
  -----------------------------------------------------------
  Revision |  Author   |  Change ID  |  Description
  01.00.00 |  Madrick3 |  Skeleton   |  Initial Creation
************************************************************/

#ifndef MEMPOOL_H
#define MEMPOOL_H

/************************************************************
  INCLUDES
************************************************************/
/* Only dependency - to track std_responsecodes in c programming projects */
#include "Platform_Types.h"

/************************************************************
  DEFINES
************************************************************/
/* The below reserves 16 KB for the maximum MEMPOOL, requestor must request multiples of 256 bytes.*/
#define MEMPOOL_MAX_NUM_BLOCKS 0x20
#define MEMPOOL_BLOCK_SIZE 0x100
#define MEMPOOL_SIZE ((uint64) (MEMPOOL_MAX_NUM_BLOCKS * MEMPOOL_BLOCK_SIZE))

#define MOD_ID_MEMPOOL 0xF0

/* A mempool is reserved for the RINGBUFFER, and is allocated through the linkerscript at compile time */
#if defined( RP2040 )
#define MEMPOOL_SIZE_TYPE         uint32
#define MEMPOOL_ADDR_TYPE         MEMPOOL_SIZE_TYPE
#endif
/* A mempool is allocated at runtime - for virtual test target */
#if defined ( VIRTUAL_TARGET )
#define MEMPOOL_SIZE_TYPE         uint64
#define MEMPOOL_ADDR_TYPE         MEMPOOL_SIZE_TYPE
#define MEMPOOL_STARTADDR ((uint64) mempool_start)
extern uint8 * mempool_start;
#endif

/************************************************************
  ENUMS AND TYPEDEFS
************************************************************/
typedef struct MemPool_tag
{
  MEMPOOL_ADDR_TYPE poolStartAddr;
  MEMPOOL_SIZE_TYPE poolSize;
  MEMPOOL_SIZE_TYPE blockSize;
  uint8 numTotalBlocks;
  uint8 numFreeBlocks;
  uint8 blocks[MEMPOOL_MAX_NUM_BLOCKS];

  /* Allocate some data from the Mempool, returns the address of that region */
  uint8* (*alloc) (void * self, MEMPOOL_SIZE_TYPE size, uint8 moduleID);
} MemPool;



/************************************************************
  EXTERN FUNCTIONS
************************************************************/

/************************************************************
   DEFAULT CLASS FUNCTIONS
************************************************************/

/************************************************************
  GLOBAL FUNCTIONS
************************************************************/
  /* helper function to check if the address given is */
extern Std_ReturnType MemPool_CheckAddressAndRangeContained(MemPool * mp, MEMPOOL_ADDR_TYPE address, MEMPOOL_ADDR_TYPE length);
extern Std_ReturnType MemPool_Create(MemPool * self, MEMPOOL_ADDR_TYPE addr, MEMPOOL_SIZE_TYPE size);
#endif /* MEMPOOL_H */

