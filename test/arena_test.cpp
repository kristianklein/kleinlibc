#include "gtest/gtest.h"
#include <stdint.h>
#include <iostream>

extern "C"
{
    #include "kleinlibc/arena.h"
}

#define BUFFER_SIZE 128U
#define PLATFORM_ALIGNMENT alignof(max_align_t)
#define PADDED_SIZE(size, alignment) (((uintptr_t)(size) + ((alignment) - 1)) & ~((uintptr_t)((alignment) - 1)))

class ArenaTest : public testing::Test
{
protected:
    uint8_t buffer[BUFFER_SIZE];    
    arena_t arena;

    void SetUp() override
    {
        arena_init(&arena, buffer, sizeof(buffer));
    }
};

TEST_F(ArenaTest, EmptyAfterInit)
{
    ASSERT_EQ(BUFFER_SIZE, arena_available(&arena));
}

TEST_F(ArenaTest, SpaceUsedAfterAlloc)
{
    uint8_t* ptr = (uint8_t*)arena_alloc(&arena, BUFFER_SIZE/2);

    ASSERT_EQ(BUFFER_SIZE/2, arena_available(&arena));
}

struct padded_struct
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint8_t d;
    // 24-bit padding (total size = 16 bytes)
};

TEST_F(ArenaTest, CanAllocatePaddedStruct)
{
    struct padded_struct *a = (struct padded_struct*)arena_alloc(&arena, sizeof(struct padded_struct));

    ASSERT_EQ(BUFFER_SIZE - PADDED_SIZE(sizeof(padded_struct), PLATFORM_ALIGNMENT), arena_available(&arena));
}

struct __attribute__((packed)) packed_struct
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint8_t d;
    // No padding! (total size = 13 bytes)
};

TEST_F(ArenaTest, CanAllocatePackedStruct)
{
    struct packed_struct *a = (struct packed_struct*)arena_alloc(&arena, sizeof(struct packed_struct));

    ASSERT_EQ(BUFFER_SIZE - PADDED_SIZE(sizeof(struct packed_struct), PLATFORM_ALIGNMENT), arena_available(&arena));
}

TEST_F(ArenaTest, CorrectAlignmentAfterUnalignedAllocation)
{
    // 9-byte allocation
    struct packed_struct *a = (struct packed_struct*)arena_alloc(&arena, sizeof(struct packed_struct));

    // 4-byte allocation
    uint32_t *b = (uint32_t*)arena_alloc(&arena, sizeof(uint32_t));

    size_t a_size_padded = PADDED_SIZE(sizeof(struct packed_struct), PLATFORM_ALIGNMENT);
    size_t b_size_padded = PADDED_SIZE(sizeof(uint32_t), PLATFORM_ALIGNMENT);

    ASSERT_EQ(BUFFER_SIZE - a_size_padded - b_size_padded, arena_available(&arena));
}

TEST_F(ArenaTest, SingleAllocationFailsIfLargerThanBuffer)
{
    void* ptr = arena_alloc(&arena, BUFFER_SIZE + 1);

    ASSERT_EQ((void*)0, ptr);
}

TEST_F(ArenaTest, AllocationFailsIfBufferFull)
{
    void* success = arena_alloc(&arena, BUFFER_SIZE);
    void* fail = arena_alloc(&arena, BUFFER_SIZE/2 + 1);

    ASSERT_EQ((void*)buffer, success);
    ASSERT_EQ((void*)0, fail);
}

TEST_F(ArenaTest, CorrectPaddingWithCustomAlignment)
{
    arena_set_alignment(&arena, 8U);
    arena_alloc(&arena, 4U);

    ASSERT_EQ(BUFFER_SIZE - 8U, arena_available(&arena));
    
    arena_alloc(&arena, 1U);
    ASSERT_EQ(BUFFER_SIZE - 16U, arena_available(&arena));
}

TEST_F(ArenaTest, EmptyAfterFreeAll)
{
    arena_alloc(&arena, 8U);
    arena_alloc(&arena, 16U);
    arena_alloc(&arena, 32U);

    ASSERT_GT(BUFFER_SIZE, arena_available(&arena));

    arena_free_all(&arena);

    ASSERT_EQ(BUFFER_SIZE, arena_available(&arena));
}