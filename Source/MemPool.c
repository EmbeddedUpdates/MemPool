/**
 * 
* @file "MemPool.c"
* @author Madrick3
* @brief Basic implementation of a MemoryPool
* 
* @COMPONENT: MemPool
* @VERSION: 01.00.00
*/
/************************************************************
  Version History
  -----------------------------------------------------------
  Revision |  Author   |  Change ID      |  Description
  01.00.00 |  pflaherty|  --             |  Initial Creation
************************************************************/

/************************************************************
  DEFINES
************************************************************/

/************************************************************
  INCLUDES
************************************************************/
#include "MemPool.h"

/***************************************x*********************
  LOCAL DEFINES
************************************************************/
/* This cast is used often in the static functions, use this locally in this TU only.*/
#define SELF ((MemPool *)(self))

/************************************************************
  ENUMS AND TYPEDEFS
************************************************************/

/************************************************************
  LOCAL VARIABLES
************************************************************/

/************************************************************
  LOCAL FUNCTIONS
************************************************************/
/**
 * MemPool->Alloc()
 * Reserve and allocate a contiguous memory space to the calling module.
 * @param self: Mempool object that is being used
 * @param size: amount of data that must be reserved at minimum, will reserve at least this much
 * @param moduleID: identifier for the module that is being allocated this memory space
 * 
 * @return pointer to the first byte of the me mory space requested, if allocation fails->NULL
 */
/* Reserve and allocate a contiguous memory space to the module */
static uint8* MemPool_Alloc(void * self, MEMPOOL_SIZE_TYPE size, uint8 moduleID)
{
  uint8* addr;
  uint8 numBlocksToReserve = 0;
  uint8 i;

  /* 
    immediate failure cases:
    SELF is null, requested size is 0, size requested is bigger than the pool is
  */
  if((NULL == SELF) || (size == 0 || size > SELF->poolSize))
  {
    addr = NULL;
  }
  else 
  {
    /*
      We need to determine how many blocks we must allocate for this request.
    */
    numBlocksToReserve = size/SELF->blockSize;
    if(size % SELF->blockSize != 0)
    {
      /* integer division will trigger truncation, so we must check for remainder and add one if necessary */
      numBlocksToReserve++; 
    }

    /* If there are not enough blocks currently free, we must report NULL to caller */
    if(SELF->numFreeBlocks < numBlocksToReserve)
    {
      addr = NULL;
    }
    else
    {
      /* address will be assumed to be the next available block <----- this must change later */ /* DEBT_01 */
      addr = (uint8*)(SELF->poolStartAddr + (SELF->numTotalBlocks-SELF->numFreeBlocks));
      for(i = 0; i < numBlocksToReserve; i++)
      {
        SELF->blocks[SELF->numTotalBlocks - SELF->numFreeBlocks + i] = moduleID;
      }
      SELF->numFreeBlocks = SELF->numFreeBlocks - numBlocksToReserve;
    }
  }
  return addr;
}


/************************************************************
  GLOBAL FUNCTIONS
************************************************************/
Std_ReturnType MemPool_Create(MemPool * self, MEMPOOL_ADDR_TYPE addr, MEMPOOL_SIZE_TYPE size)
{
  Std_ReturnType retVal = E_OK;
  uint8 i; /*idx for iterating through block list */

  if( (addr % MEMPOOL_BLOCK_SIZE != 0) || (size % MEMPOOL_BLOCK_SIZE != 0) || (size == 0) )
  {
    retVal = E_NOT_OK;
  }

  if(E_OK == retVal)
  {
    SELF->poolStartAddr = addr;
    SELF->poolSize = size;
    SELF->blockSize = MEMPOOL_BLOCK_SIZE;
    SELF->numTotalBlocks = size/MEMPOOL_BLOCK_SIZE;
    SELF->numFreeBlocks = SELF->numTotalBlocks;

    for(i = 0; i < SELF->numTotalBlocks; i++)
    {
      SELF->blocks[i] = MOD_ID_MEMPOOL;
    }
    SELF->alloc = &MemPool_Alloc;
  }
  return retVal;
}


/**
 * MemPool_CheckAddressAndRangeContained
 * 
 * Utility function that takes a MemPool and verifies that the address and range are within that mempool.
 */
Std_ReturnType MemPool_CheckAddressAndRangeContained(MemPool * mp, MEMPOOL_ADDR_TYPE address, MEMPOOL_ADDR_TYPE length)
{
  /* assume that the address+range is in the mp. */
  Std_ReturnType retVal = E_OK;

  /* 
    if the start is before the pool - it should fail. 
    Also, if the end is after the pool, it should fail. 
    Edge case, address+length has integer overflow and the happens to fall in the pool again, this should fail 
        Check that the start is before the end? - Wouldn't always work.
        Check that the length is less than the poolSize? - Should always work. 
  */
  if((mp->poolStartAddr > address) || ((mp->poolStartAddr + mp->poolSize) < (address+length)) ||
      mp->poolSize < length)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}


/* 
  DEBT_01 
  Assumes that the blocks can not be freed, so just moves to the next block. 
  When blocks can be freed, we should look for the first contiguous free spaces.
  
*/