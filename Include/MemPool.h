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
/* offset into the blocks metadata structure for the number of consecutive blocks */
#define MEMPOOL_BLOCKCOUNT_OFFSET 8

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
extern int8 * mempool_start;
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
  uint16 blocks[MEMPOOL_MAX_NUM_BLOCKS];

  /* Allocate some data from the Mempool, returns the address of that region */
  int8* (*alloc) (void * self, MEMPOOL_SIZE_TYPE size, uint16 moduleID);
  Std_ReturnType (*free) (void * self, MEMPOOL_ADDR_TYPE addr, uint16 moduleID);
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
/**
 * MemPool_CheckAddressAndRangeContained
 * 
 * Utility function that takes a MemPool and verifies that the address and range are within that mempool.
 * @param mp: pointer to a memoryPool structure
 * @param address: address that we want to verify is in the mempool 'mp'.
 * @param length: length of the request that we want to verify is also all in the mempool.
 * 
 * @return Std_ReturnType: E_OK if the request is reasonable, i.e. the address and length is contained within the mempool
 *    E_NOT_OK if the request is not reasonable.
 */
extern Std_ReturnType MemPool_CheckAddressAndRangeContained(MemPool * mp, MEMPOOL_ADDR_TYPE address, MEMPOOL_ADDR_TYPE length);

/**
 * MemPool_Create()
 * 
 * Populates the mempool structure pointed to by 'self'. Given an address and size for the mempool by the caller.
 * @param self: pointer to mempool structure that must be populated.
 * @param addr: address where the mempool data will be stored. blocks come from this area.
 * @param size: overall size of the memory pool.
 * 
 * @return Std_ReturnType: could be Std_ErrorType instead, E_OK for successful creation or E_NOT_OK for failure.
 */
extern Std_ReturnType MemPool_Create(MemPool * self, MEMPOOL_ADDR_TYPE addr, MEMPOOL_SIZE_TYPE size);

extern Std_ReturnType MemPool_GetGlobalMemPool( MemPool ** mp );
extern void MemPool_ClearInternalMemPool( void );

#endif /* MEMPOOL_H */

