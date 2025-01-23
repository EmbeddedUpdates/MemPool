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
extern MemPool testSetup_MemPool_Create(void);

/**
 * Allocate (Create Setup is needed)
 */

#endif /* MEMPOOL_TEST_H */