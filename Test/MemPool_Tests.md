# basic design goals.
A mempool is often tracked through the use of a linkedlist, but this is problematic to me. Why is an array not used?
Arrays offer some benefits, and disadvantages:

### advantages:
1. O(1) allocation and deallocation - fast and predictable.
2. Low overhead - array is basically directly proportional to the number of blocks in the mempool.
3. Arrays are simpler
4. Cache-friendly (locality of reference).

### disadvantages:
1. less flexible - if we want to do variable size of blocks?
    - variable block size is a possible vulnerability to fragmentation
    - contiguity may not be possible as runtime of program continues.
    - 

So lets use an array to start, and move from there -
[0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f] could represent an array of blocks.
blocks will have the following attributes:
{
    char * buffer,
    bool dirty,
}

Honestly, with a known static start address, an index for the block, we just need a dirty bit, startaddress of the block can be calculated by MEMPOOL_START + BLOCK_SIZE*i
So is a struct really useful?

We could keep requestor id in the struct, which is equal to a free/allocated
{
    FF,
    FF,
    FF,
    FF,
    FF,
    FF,
    FF,
}

would be all free, but:
{
    AB,
    AB,
    FA,
    FF
}
would show a mempool where module 'AB' has requested blocks 0 and 1, and module 'FA' has requested block FA. This could be hidden from the requestor, and an additional step in verifying that the freeing of a block is performed by the correct code/software.

So an alloc would look like:
mem_ptr mempool_alloc(char* buffPointer, .word numBytes, .char ID)

And a dealloc would look like
void mempool_dealloc(memptr addr, .char id)

I don't think an object makes the most sense, unless we may have multiple mempools.
multiple mempools is possible, but not necessarily a good idea.
OOP doesn't make the most sense here - unless we want to create different mempools for different memory regions. Which *could* make sense in the case of prioritizing syntactic-recency.
So OOP does make sense.

elements of a mempool would be something like:
    1. poolstartaddr
    2. poolSize
    3. poolBlockSize
    4. poolNumBlocks
    5. poolNumFreeBlocks
    6. array or MemPoolBlocks

elements of a block would be:
    1. identifer of the user of the block - if present the block is used
    so a char array will suffice for the block array.

# MemPool test cases

1. Mempool init
    a. ~~init works~~
        - returns ok
        - unaligned startaddress fails
        - unaligned size fails
        - size of zero fails
        - size is stored in the mempool
        - address is stored in the mempool
        - blockcount is calculated correctly
        - free block count is correct
        - blockarray is populated totally free
        - memory is cleared?
    b. double init - wwhat to do here?
    c. ~~on init, free block count matches expected block count~~
    d. ~~on init, data is all freed~~
    e. on init, confirm that all blocks are in the linked list

2. ~~MemPool_CheckAddressAndRangeContained()~~
    a. Address in mempool + range in mempool -> OK
    b. Address in mempool + range out -> not_ok
    c. address out mempool + range in -> not_ok
    d. address out + range out -> not ok

3. mempool init during runtime?
    a. reinit the pool
    b. all blocks are free after
    c. no data in blocks

4. ~~Allocation tests~~
    a. Allocate a single block
    b. Allocate two blocks
    c. Verify the block allocated is in the pool
    d. allocate all available blocks
    e. allocate all blocks + 1 and it should fail
    f. verify no blocks are duplicates

5. ~~Dealloc/free tests~~
    a. free a block
    b. free the same block twice (should fail)
    c. free a block that doesnt exist (should fail)
    d. free a block that doesnt belong to the mempool

6. ~~edge cases:~~
    a. allocate full pool, free one, allocate again
    b. allocate full pool, init pool, allocate full pool
    c. allocate full pool, init pool, free (should fail)
