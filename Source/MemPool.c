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
#define MAX_NUM_MEMPOOLS 1
/************************************************************
  LOCAL VARIABLES
************************************************************/
MemPool memPools[MAX_NUM_MEMPOOLS] = {0};
uint8 memPoolsIdx = 0xFF;


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
static int8* MemPool_Alloc(void * self, MEMPOOL_SIZE_TYPE size, uint16 moduleID)
{
  int8* addr = NULL;
  uint8 numBlocksToReserve = 0;
  uint8 i;
  uint8 contiguousCount = 0;
  uint8 startOfContiguousBlocks = 0;

  /* 
    immediate failure cases:
    SELF is null, requested size is 0, size requested is bigger than the pool is
  */
  if( (NULL == SELF) || (0 == size || SELF->poolSize < size) )
  {
    addr = NULL;
  }
  else 
  {
    /*
      We need to determine how many blocks we must allocate for this request.
    */
    numBlocksToReserve = size/SELF->blockSize;
    if( size % SELF->blockSize != 0 )
    {
      /* integer division will trigger truncation, so we must check for remainder and add one if necessary */
      numBlocksToReserve++; 
    }

    /* If there are not enough blocks currently free, we must report NULL to caller */
    if( SELF->numFreeBlocks < numBlocksToReserve )
    {
      addr = NULL;
    }
    else
    {
      /* iterate through the entire array, searching for first contiguous group of that is large enough */
      for( i = 0; i < SELF->numTotalBlocks; i++ )
      {
        /* only free blocks count */
        if( 0x00F0 == SELF->blocks[i] )
        {
          contiguousCount++;
        }
        else
        {
          contiguousCount = 0;
        }
        if( contiguousCount == numBlocksToReserve )
        {
          startOfContiguousBlocks = i - contiguousCount + 1;
          /* address will be assumed to be the next available block <----- this must change later */ /* DEBT_01 */
          addr = (int8*)(SELF->poolStartAddr + (startOfContiguousBlocks*SELF->blockSize));
          SELF->blocks[startOfContiguousBlocks] = (((numBlocksToReserve-1) << MEMPOOL_BLOCKCOUNT_OFFSET) | moduleID);
          for( i = 1; i < numBlocksToReserve; i++ )
          {
            SELF->blocks[startOfContiguousBlocks + i] = (((0x00) << MEMPOOL_BLOCKCOUNT_OFFSET) | moduleID);
          }
          SELF->numFreeBlocks = SELF->numFreeBlocks - numBlocksToReserve;

          /* Intentional break instead of modifying for loop condition.  */
          break;
        }
      }
    }
  }
  return addr;
}

/**
 * MemPool->Free()
 * Given an address to an already allocated memory block, and a matching requestor ID, frees the block.
 * @param self: Mempool object that is being used
 * @param addr: address for the start of the region that must be freed
 *                NB: should match the addr that was given to the caller in alloc
 * @param moduleID: identifier for the module that had been allocated this memory space
 *                NB: If this does not match the original request, free will fail
 * 
 * @return pointer to the first byte of the me mory space requested, if allocation fails->NULL
 */
/* Reserve and allocate a contiguous memory space to the module */
static Std_ReturnType MemPool_Free(void * self, MEMPOOL_ADDR_TYPE addr, uint16 moduleID)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 blockIdx = 0;
  uint8 numBlocksToClear = 0;
  uint8 i;

  /* We need to identify if the address is reasonable, we can use the helper function for that */  
  if( E_OK == MemPool_CheckAddressAndRangeContained(self, addr, 1) )
  {
    /* Now we just need to confirm that this is the expected caller */
    blockIdx = (addr-(SELF->poolStartAddr))/SELF->blockSize;
    if( (SELF->blocks[blockIdx] & 0xFF) == moduleID )
    {
      numBlocksToClear = ((SELF->blocks[blockIdx] & 0xFF00) >> 8) + 1;
      for( i = 0; i < numBlocksToClear; i++ )
      {
        SELF->blocks[blockIdx+i] = ((0x00 << 8) | MOD_ID_MEMPOOL);
        SELF->numFreeBlocks++;
      }
      retVal = E_OK;
    }
  }

  (void) moduleID;
  
  return retVal;
}

/************************************************************
  GLOBAL FUNCTIONS
************************************************************/
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
Std_ReturnType MemPool_Create(MemPool * self, MEMPOOL_ADDR_TYPE addr, MEMPOOL_SIZE_TYPE size)
{
  Std_ReturnType retVal = E_OK;
  uint8 i; /*idx for iterating through block list */

  /* consider smarter testing of addr, i.e. legal address in RAM for this microcontroller/derivative. */
  /* 
    if self is NULL, we should assume that the derefernce may fail. 
    if addr is not aligned to MEMPOOL_BLOCK_SIZE, or the size is not a multiple of MEMPOOL_BLOCK_SIZE, we should fail.
    size of zero would not make sense for a mempool, so that should fail too.
  */
  if( (NULL == self) || (0 != (addr % MEMPOOL_BLOCK_SIZE)) || (0 != (size % MEMPOOL_BLOCK_SIZE)) || (0 == size) )
  {
    retVal = E_NOT_OK;
  }

  /* if there are no errors in the parameters, we should*/
  if( E_OK == retVal )
  {
    SELF->poolStartAddr = addr;
    SELF->poolSize = size;
    SELF->blockSize = MEMPOOL_BLOCK_SIZE;
    SELF->numTotalBlocks = size/MEMPOOL_BLOCK_SIZE;
    SELF->numFreeBlocks = SELF->numTotalBlocks;

    /* We should mark every block as "free" or assigned to the MEMPOOL to be overwritten. */
    for( i = 0; i < SELF->numTotalBlocks; i++ )
    {
      /* Writing of zero bits here is not needed, but is present for clarity and readability.*/
      SELF->blocks[i] = ((0x00 << MEMPOOL_BLOCKCOUNT_OFFSET) | MOD_ID_MEMPOOL);
    }

    /* consider adding a possibility to clear/wipe the mempool. */

    /* overwrite member function pointers (these may not ever change, could consider constant assignment)*/
    SELF->alloc = &MemPool_Alloc;
    SELF->free = &MemPool_Free;
  }
  return retVal;
}

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
  if( (mp->poolStartAddr > address) || ((mp->poolStartAddr + mp->poolSize) < (address+length)) ||
      mp->poolSize < length )
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

Std_ReturnType MemPool_GetGlobalMemPool( MemPool ** mp )
{
  Std_ReturnType retVal = E_NOT_OK;
  if(0xFF == memPoolsIdx)
  {
    retVal = MemPool_Create(&memPools[0], MEMPOOL_STARTADDR, MEMPOOL_SIZE);
    memPoolsIdx = 0;
  }

  *mp =  &memPools[memPoolsIdx];
  return retVal;
}

void MemPool_ClearInternalMemPool( void )
{
  memPools[0].poolStartAddr = 0;
  memPools[0].poolSize = 0;
  memPools[0].blockSize = 0;
  memPools[0].numTotalBlocks = 0;
  memPools[0].numFreeBlocks = 0;
  memPoolsIdx = 0xFF;
}


/* 
  DEBT_01 
  Assumes that the blocks can not be freed, so just moves to the next block. 
  When blocks can be freed, we should look for the first contiguous free spaces.

*/