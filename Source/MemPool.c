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
  Std_ReturnType retVal = E_OK;

  /* 
    if the start is before the pool - it should fail. 
    Also, if the end is after the pool, it should fail. 
    Edge case, address+length has integer overflow and the happens to fall in the pool again, this should fail 
        Check that the start is before the end?
        Check that the length is less than the poolSize? */
  if((mp->poolStartAddr > address) || ((mp->poolStartAddr + mp->poolSize) < (address+length)) ||
      mp->poolSize < length)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}