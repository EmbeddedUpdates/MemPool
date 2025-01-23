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
MemPool testSetup_MemPool_Create(void)
{
  MemPool mempool;
  (void)(MemPool_Create(&mempool, MEMPOOL_STARTADDR, MEMPOOL_SIZE));
  return mempool;
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
  MemPool mp = testSetup_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR, 32);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_MemPool_CheckRangeContained_OK_FullRange(void)
{
  MemPool mp = testSetup_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR, MEMPOOL_SIZE);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}

void test_MemPool_CheckRangeContained_StartInLengthOut_NOTOK(void)
{
  MemPool mp = testSetup_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR+4, MEMPOOL_SIZE);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_CheckRangeContained_StartOutLengthIn_NOTOK(void)
{
  MemPool mp = testSetup_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR-10, MEMPOOL_SIZE-2);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

void test_MemPool_CheckRangeContained_StartOutLengthOut_NOTOK(void)
{
  MemPool mp = testSetup_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR-10, MEMPOOL_SIZE+10);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

/* 
  This test covers an edge case where the start is in the range, and the length is huge. 
  In this case an overflow could happen where the end is
*/
void test_MemPool_CheckRangeContained_StartInLengthHuge_OVERFLOW_NOTOK(void)
{
  MemPool mp = testSetup_MemPool_Create();
  Std_ReturnType retVal = MemPool_CheckAddressAndRangeContained(&mp, MEMPOOL_STARTADDR, UINT64_MAX);
  TEST_ASSERT_EQUAL(E_NOT_OK, retVal);
}

