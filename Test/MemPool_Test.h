/**
 * 
* @file "MemPool_Test.h"
* @author Madrick3
* @brief Basic Memory Pool implementation
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

#ifndef MEMPOOL_TEST_H
#define MEMPOOL_TEST_H

/************************************************************
  INCLUDES
************************************************************/
#include "MemPool.h"

/************************************************************
  LOCAL VARIABLES
************************************************************/


/************************************************************
  EXTERNAL FUNCTIONS
************************************************************/
/**
 * Create Tests
 */
extern void test_MemPool_Create_ReturnsOK(void);
extern void test_MemPool_Create_ReturnsNotOk_UnalignedStartAddress(void);
extern void test_MemPool_Create_ReturnsNotOk_UnalignedSize(void);
extern void test_MemPool_Create_ReturnsNotOk_ZeroSize(void);
extern void test_MemPool_Create_Persists_AlignedSize(void);
extern void test_MemPool_Create_Persists_AlignedAddress(void);
extern void test_MemPool_Create_Persists_BlockCount(void);
extern void test_MemPool_Create_FreeBlockCountIsCorrect(void);
extern void test_MemPool_Create_BlockArrayIsAllFree(void);

/**
 * CheckAddressAndRangeContained
 */
extern void test_MemPool_CheckRangeContained_OK(void);
extern void test_MemPool_CheckRangeContained_OK_FullRange(void);
extern void test_MemPool_CheckRangeContained_StartInLengthOut_NOTOK(void);
extern void test_MemPool_CheckRangeContained_StartOutLengthIn_NOTOK(void);
extern void test_MemPool_CheckRangeContained_StartOutLengthOut_NOTOK(void);
extern void test_MemPool_CheckRangeContained_StartInLengthHuge_OVERFLOW_NOTOK(void);

/* create setup helper func */
extern MemPool testHelper_MemPool_Create(void);
extern void testHelper_PrintShortArray(uint16 * arr, uint8 numElements);
extern void testHelper_PrintPointerArray(uint8** arr, uint8 numElements);

/**
 * Allocate (Create Setup is needed)
 */
extern void test_MemPool_Alloc_ReturnsPointer(void);
extern void test_MemPool_Alloc_SizeZero_ReturnsNULL(void);
extern void test_MemPool_Alloc_SizeTooBig_ReturnsNULL(void);
extern void test_MemPool_Alloc_AllocOneBlock(void);
extern void test_MemPool_Alloc_AllocTwoBlocks_TwoDifferentIdentifiers_TwoRequest(void);
extern void test_MemPool_Alloc_AllocTwoBlocks_OneRequest(void);
extern void test_MemPool_Alloc_AllocThreeBlocks(void);
extern void test_MemPool_Alloc_AllocAllBlocks_ReturnsPointer(void);
extern void test_MemPool_Alloc_AllocTooManyBlocks_ReturnsNull(void);
extern void test_MemPool_Alloc_AllocAllBlocks_AllPointersInMemPool(void);
extern void test_MemPool_Alloc_AllocAllBlocks_AllPointersDifferent(void);

/**
 * Free (Create Setup is needed)
 */
extern void test_MemPool_Free_ReturnsOK(void);
extern void test_MemPool_Free_PointerNotInMemPool_ReurnsNOTOK(void);
extern void test_MemPool_Free_PointerInMempoolButNotCorrectModule_ReturnsNOTOK(void);
extern void test_MemPool_Free_FreesCorrectSpace_AllocOneBlockAndFreeOneBlock_IsFree(void);
extern void test_MemPool_Free_FreesCorrectSpace_AllocTwoBlocksAndFreeOneBlock_IsFreeAndAlloc(void);
extern void test_MemPool_Free_FreesCorrectSpace_AllocAllAsOneAndFreeAll_AllIsFree(void);
extern void test_MemPool_Free_NumBlocksEqualsNumFreeBlocks(void);
extern void test_MemPool_Free_1BlockFreeAfterAllUsedAnd1Freed(void);

/**
 * Auxiliary tests, that don't belong in these other spaces.
 */
extern void test_MemPool_Aux_CanaryTest(void);
extern void test_MemPool_Aux_AllocAllFreeHalfAllocOne(void);

#endif /* MEMPOOL_TEST_H */