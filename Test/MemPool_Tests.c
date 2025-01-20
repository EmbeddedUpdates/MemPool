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

/* TESTS */
/* Calls ...Create() and returns Ok */
void test_RingBuffer_Create_ReturnsOK(void)
{
  RingBuffer rb;
  Std_ErrorCode retVal = E_NOT_OK;
  retVal = RingBuffer_Create(&rb, 1, RINGBUFFER_MEMPOOL_SIZE);
  TEST_ASSERT_EQUAL(E_OK, retVal);
}
