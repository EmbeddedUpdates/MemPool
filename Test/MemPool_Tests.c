/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-24 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */


/* #include "Timer_RP2040.h" */
#include "MemPool_Test.h"
#include "unity.h"

/* Global variables for testing */
uint8 * mempool_start;

#pragma ab

void setUp(void)
{
}

/* 
  These are run after each test, so we want to make sure the global variable is 'UNINIT' 
  so that each test can behave as if its its own test. 
*/
void tearDown(void)
{

}

/* HELPER FUNCTIONS */
MemPool testHelper_MemPool_Create(void)
{
  MemPool mempool;
  (void)(MemPool_Create(&mempool, MEMPOOL_STARTADDR, MEMPOOL_SIZE));
  return mempool;
}

void testHelper_PrintCharArray(uint8 * arr, uint8 numElements)
{
  printf("array: \n");
  uint8 i;
  for(i = 0; i < numElements; i++)
  {
    printf(" %u |", arr[i]);
  }
  printf("\n");
}

void testHelper_PrintPointerArray(uint8** arr, uint8 numElements)
{
  printf("array: \n");
  uint8 i;
  for(i = 0; i < numElements; i++)
  {
    printf(" %p |", arr[i]);
  }
  printf("\n");
}

/* TESTS */
/* Mempool.Create() */
/* Calls ...Create() and returns Ok */
void test_MemPool_Create_ReturnsOK(void)
{
  MemPool mempool;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x2000);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_MemPool_Create_ReturnsNotOk_UnalignedStartAddress(void)
{
  MemPool mempool;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = MemPool_Create(&mempool, 1, 0x2000);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_Create_ReturnsNotOk_UnalignedSize(void)
{
  Std_ErrorCode retVal = E_NOT_OK;
  MemPool mempool;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x1FFF);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_Create_ReturnsNotOk_ZeroSize(void)
{
  Std_ErrorCode retVal = E_NOT_OK;
  MemPool mempool;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x0);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_Create_Persists_AlignedSize(void)
{
  Std_ErrorCode retVal = E_NOT_OK;
  MemPool mempool;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x2000);
  (void)retVal;
  TEST_ASSERT_EQUAL(0x2000, mempool.poolSize);
}

void test_MemPool_Create_Persists_AlignedAddress(void)
{
  Std_ErrorCode retVal = E_NOT_OK;
  MemPool mempool;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x2000);
  (void)retVal;
  TEST_ASSERT_EQUAL(MEMPOOL_STARTADDR, mempool.poolStartAddr);
}

void test_MemPool_Create_Persists_BlockCount(void)
{
  Std_ErrorCode retVal = E_NOT_OK;
  MemPool mempool;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x2000);
  (void)retVal;
  TEST_ASSERT_EQUAL((0x2000/0x100), mempool.numTotalBlocks);
}

void test_MemPool_Create_FreeBlockCountIsCorrect(void)
{
  Std_ErrorCode retVal = E_NOT_OK;
  MemPool mempool;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x2000);
  (void)retVal;
  TEST_ASSERT_EQUAL(32, mempool.numFreeBlocks);
}

void test_MemPool_Create_BlockArrayIsAllFree(void)
{
  Std_ErrorCode retVal = E_NOT_OK;
  MemPool mempool;
  retVal = MemPool_Create(&mempool, MEMPOOL_STARTADDR, 0x2000);
  /* GNU Extension syntax for initializing an array quickly */
  uint8 freeArray[32] = {[0 ... 31] = MOD_ID_MEMPOOL};
  (void)retVal;
  TEST_ASSERT_EQUAL_CHAR_ARRAY(&freeArray[0], &(mempool.blocks[0]), 32);
}

/* Utility Function Tests */
void test_MemPool_CheckRangeContained_OK(void)
{
  MemPool mp = testHelper_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR, 32);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_MemPool_CheckRangeContained_OK_FullRange(void)
{
  MemPool mp = testHelper_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR, MEMPOOL_SIZE);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_MemPool_CheckRangeContained_StartInLengthOut_NOTOK(void)
{
  MemPool mp = testHelper_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR+4, MEMPOOL_SIZE);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_CheckRangeContained_StartOutLengthIn_NOTOK(void)
{
  MemPool mp = testHelper_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR-10, MEMPOOL_SIZE-2);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_CheckRangeContained_StartOutLengthOut_NOTOK(void)
{
  MemPool mp = testHelper_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR-10, MEMPOOL_SIZE+10);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

/* 
  This test covers an edge case where the start is in the range, and the length is huge. 
  In this case an overflow could happen where the end is
*/
void test_MemPool_CheckRangeContained_StartInLengthHuge_OVERFLOW_NOTOK(void)
{
  MemPool mp = testHelper_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR, UINT64_MAX);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}


/* Alloc Tests */
void test_MemPool_Alloc_ReturnsPointer(void)
{
  Std_ReturnType retVal = E_NOT_OK;
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, 32, 0x0A);
  retVal = MemPool_CheckAddressAndRangeContained(&mp, (MEMPOOL_SIZE_TYPE)array, 32);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_MemPool_Alloc_SizeZero_ReturnsNULL(void)
{
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, 0, 0x0A);
  TEST_ASSERT_EQUAL(NULL, array);
}

void test_MemPool_Alloc_SizeTooBig_ReturnsNULL(void)
{
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, (MEMPOOL_BLOCK_SIZE*MEMPOOL_MAX_NUM_BLOCKS+1), 0x0A);
  TEST_ASSERT_EQUAL(NULL, array);
}

void test_MemPool_Alloc_AllocOneBlock(void)
{
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, 32, 0x0A);
  (void) array;
  TEST_ASSERT_EQUAL(0x0A, mp.blocks[0]);
}

void test_MemPool_Alloc_AllocTwoBlocks_TwoDifferentIdentifiers_TwoRequest(void)
{
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, 32, 0x0A);
  array = mp.alloc(&mp, 32, 0x0B);
  (void) array;
  TEST_ASSERT_EQUAL(0x0A, mp.blocks[0]);
  TEST_ASSERT_EQUAL(0x0B, mp.blocks[1]);
}

void test_MemPool_Alloc_AllocTwoBlocks_OneRequest(void)
{
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, 512, 0x0A);
  (void) array;
  TEST_ASSERT_EQUAL(0x0A, mp.blocks[0]);
}

void test_MemPool_Alloc_AllocThreeBlocks(void)
{
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, 32, 0x0A);
  array = mp.alloc(&mp, 32, 0x0B);
  array = mp.alloc(&mp, 32, 0x0A);
  (void) array;
  TEST_ASSERT_EQUAL(0x0A, mp.blocks[0]);
  TEST_ASSERT_EQUAL(0x0B, mp.blocks[1]);
  TEST_ASSERT_EQUAL(0x0A, mp.blocks[2]);
}

void test_MemPool_Alloc_AllocAllBlocks_ReturnsPointer(void)
{
  Std_ReturnType retVal = E_NOT_OK;
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, MEMPOOL_SIZE, 0x0A);
  int i;
  for(i = 0; i<mp.numTotalBlocks; i++)
  {
    TEST_ASSERT_EQUAL(0x0A, mp.blocks[i]);
  }
  retVal = MemPool_CheckAddressAndRangeContained(&mp, (MEMPOOL_ADDR_TYPE)array, MEMPOOL_SIZE);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_MemPool_Alloc_AllocTooManyBlocks_ReturnsNull(void)
{
  Std_ReturnType retVal = E_NOT_OK;
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  array = mp.alloc(&mp, MEMPOOL_SIZE, 0x0A);
  array = mp.alloc(&mp, 32, 0x0B);
  int i;
  for(i = 0; i<mp.numTotalBlocks; i++)
  {
    TEST_ASSERT_EQUAL(0x0A, mp.blocks[i]);
  }
  retVal = MemPool_CheckAddressAndRangeContained(&mp, (MEMPOOL_ADDR_TYPE)array, MEMPOOL_SIZE);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_Alloc_AllocAllBlocks_AllPointersInMemPool(void)
{
  Std_ReturnType retVal = E_NOT_OK;
  MemPool mp = testHelper_MemPool_Create();
  uint8* array;
  uint8 i;

  for(i = 0; i<mp.numTotalBlocks; i++)
  {
    array = mp.alloc(&mp, 32, 0x0A);
    TEST_ASSERT_EQUAL(0x0A, mp.blocks[i]);
    retVal = MemPool_CheckAddressAndRangeContained(&mp, (MEMPOOL_ADDR_TYPE)array, 32);
    TEST_ASSERT_EQUAL(E_OK, retVal);
  }
}

void test_MemPool_Alloc_AllocAllBlocks_AllPointersDifferent(void)
{
  Std_ReturnType retVal = E_NOT_OK;
  MemPool mp = testHelper_MemPool_Create();
  uint8* arrayOfAddr[mp.numTotalBlocks];
  int i;

  for(i = 0; i < mp.numTotalBlocks; i++)
  {
    arrayOfAddr[i] = mp.alloc(&mp, 32, i);
    retVal = MemPool_CheckAddressAndRangeContained(&mp, (MEMPOOL_ADDR_TYPE)arrayOfAddr[i], 32);
  }
  TEST_ASSERT_EQUAL(E_OK, retVal);
}


