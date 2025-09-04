#include "gtest/gtest.h"
#include <stdint.h>

extern "C"
{
    #include "containers/queue.h"
}

static constexpr unsigned BUFFER_LENGTH = 4;

class QueueTest : public testing::Test
{
protected:
    queue_t queue;
    void SetUp() override
    {
        uint8_t buffer[BUFFER_LENGTH];
        queue_create(&queue, buffer, BUFFER_LENGTH, sizeof(uint8_t));
    }
};

TEST_F(QueueTest, EmptyWhenCreated)
{
    ASSERT_TRUE(queue_is_empty(&queue));
}

TEST_F(QueueTest, NotEmptyAfterPushing)
{
    uint8_t elem = 42;
    queue_push(&queue, &elem);
    ASSERT_FALSE(queue_is_empty(&queue));
}

TEST_F(QueueTest, RetrievePushedElement)
{
    uint8_t elem = 42;
    queue_push(&queue, &elem);

    uint8_t retrieved;
    queue_pop(&queue, &retrieved);

    ASSERT_EQ(retrieved, elem);
}

TEST_F(QueueTest, RetrieveMultiplePushedElements)
{
    uint8_t elem = 42;
    queue_push(&queue, &elem);
    elem = 9;
    queue_push(&queue, &elem);

    uint8_t retrieved;
    queue_pop(&queue, &retrieved);
    ASSERT_EQ(retrieved, 42);
    queue_pop(&queue, &retrieved);
    ASSERT_EQ(retrieved, 9);
}


TEST_F(QueueTest, NotFullWhenCreated)
{
    ASSERT_FALSE(queue_is_full(&queue));
}


TEST_F(QueueTest, FullIfLengthElementsPushed)
{
    for (int i = 0; i < BUFFER_LENGTH; i++)
    {
        uint8_t elem = i;
        queue_push(&queue, &elem);
    }

    ASSERT_TRUE(queue_is_full(&queue));
}

TEST_F(QueueTest, CanPushWhenEmpty)
{
    uint8_t elem = 42;
    ASSERT_TRUE(queue_push(&queue, &elem));
}

TEST_F(QueueTest, CannotPushWhenFull)
{
    for (int i = 0; i < BUFFER_LENGTH; i++)
    {
        uint8_t elem = i;
        queue_push(&queue, &elem);
    }

    uint8_t elem = 42;
    ASSERT_FALSE(queue_push(&queue, &elem));
}

TEST_F(QueueTest, CanPopWhenNotEmpty)
{
    uint8_t elem = 42;
    queue_push(&queue, &elem);

    uint8_t retrieved;
    ASSERT_TRUE(queue_pop(&queue, &elem));
}

TEST_F(QueueTest, CannotPopWhenEmpty)
{
    uint8_t elem;
    ASSERT_FALSE(queue_pop(&queue, &elem));
}

TEST_F(QueueTest, GetSize)
{
    uint8_t elem = 42;
    queue_push(&queue, &elem);
    ASSERT_EQ(queue_size(&queue), 1);
    queue_push(&queue, &elem);
    ASSERT_EQ(queue_size(&queue), 2);
}

TEST_F(QueueTest, CanPeekWithoutPopping)
{
    uint8_t elem = 42;
    queue_push(&queue, &elem);
    ASSERT_EQ(queue_size(&queue), 1);
    uint8_t retrieved;
    queue_peek(&queue, &retrieved);
    ASSERT_EQ(retrieved, elem);
    ASSERT_EQ(queue_size(&queue), 1);
}

TEST(QueueTestBuffer, DoesNotExceedBuffer)
{
    queue_t queue;
    uint8_t protected_begin = 0xAA;
    uint8_t protected_end = 0xBB;
    uint8_t protected_buffer[BUFFER_LENGTH + 2];
    protected_buffer[0] = protected_begin;
    protected_buffer[(BUFFER_LENGTH + 2) - 1] = protected_end;

    queue_create(&queue, &protected_buffer[1], BUFFER_LENGTH, sizeof(uint8_t));

    // Fill the buffer
    uint8_t elem = 42;
    for (int i = 0; i < BUFFER_LENGTH; i++)
    {
        queue_push(&queue, &elem);
    }

    // Pop an element to make room, then push again
    uint8_t retrieved;
    queue_pop(&queue, &retrieved);
    queue_push(&queue, &elem);

    ASSERT_EQ(protected_buffer[0], protected_begin);
    ASSERT_EQ(protected_buffer[(BUFFER_LENGTH + 2) - 1], protected_end);
}


/* STRUCT TESTS */
struct arbitrary_struct {
    uint8_t a;
    uint16_t b;
    uint32_t c;
    float d;
};

TEST(QueueTestStruct, DoesNotExceedBuffer)
{
    queue_t queue;
    struct arbitrary_struct protected_begin = {.a = 0x00, .b = 0x11, .c = 0x22, .d = 123.0};
    struct arbitrary_struct protected_end = {.a = 0xAA, .b = 0xBB, .c = 0xCC, .d = 321.0};
    struct arbitrary_struct protected_buffer[BUFFER_LENGTH + 2];
    protected_buffer[0] = protected_begin;
    protected_buffer[(BUFFER_LENGTH + 2) - 1] = protected_end;

    queue_create(&queue, &protected_buffer[1], BUFFER_LENGTH, sizeof(struct arbitrary_struct));

    // Fill the buffer
    struct arbitrary_struct elem = {.a = 42, .b = 43, .c = 44, .d = 45.0};
    for (int i = 0; i < BUFFER_LENGTH; i++)
    {
        queue_push(&queue, &elem);
    }

    // Pop an element to make room, then push again
    struct arbitrary_struct retrieved;
    queue_pop(&queue, &retrieved);
    ASSERT_EQ(retrieved.a, elem.a);
    ASSERT_EQ(retrieved.b, elem.b);
    ASSERT_EQ(retrieved.c, elem.c);
    ASSERT_EQ(retrieved.d, elem.d);
    queue_push(&queue, &elem);

    ASSERT_EQ(protected_buffer[0].a, protected_begin.a);
    ASSERT_EQ(protected_buffer[0].b, protected_begin.b);
    ASSERT_EQ(protected_buffer[0].c, protected_begin.c);
    ASSERT_EQ(protected_buffer[0].d, protected_begin.d);
    ASSERT_EQ(protected_buffer[(BUFFER_LENGTH + 2) - 1].a, protected_end.a);
    ASSERT_EQ(protected_buffer[(BUFFER_LENGTH + 2) - 1].b, protected_end.b);
    ASSERT_EQ(protected_buffer[(BUFFER_LENGTH + 2) - 1].c, protected_end.c);
    ASSERT_EQ(protected_buffer[(BUFFER_LENGTH + 2) - 1].d, protected_end.d);
}

TEST(QueueTestStruct, RetrieveStructElementsInOrder)
{
    queue_t queue;
    struct arbitrary_struct buffer[BUFFER_LENGTH];

    queue_create(&queue, buffer, BUFFER_LENGTH, sizeof(struct arbitrary_struct));

    struct arbitrary_struct elem1 = {
        .a = 1,
        .b = 2,
        .c = 3,
        .d = 4.5
    };

    struct arbitrary_struct elem2 = {
        .a = 5,
        .b = 6,
        .c = 7,
        .d = 8.9
    };

    uint32_t struct_size1 = sizeof(elem1);
    uint32_t struct_size2 = sizeof(elem2);
    uint32_t buffer_size = sizeof(buffer);

    queue_push(&queue, &elem1);
    queue_push(&queue, &elem2);

    struct arbitrary_struct retrieved;
    queue_pop(&queue, &retrieved);
    ASSERT_EQ(elem1.a, retrieved.a);
    ASSERT_EQ(elem1.b, retrieved.b);
    ASSERT_EQ(elem1.c, retrieved.c);
    ASSERT_EQ(elem1.d, retrieved.d);

    queue_pop(&queue, &retrieved);
    ASSERT_EQ(elem2.a, retrieved.a);
    ASSERT_EQ(elem2.b, retrieved.b);
    ASSERT_EQ(elem2.c, retrieved.c);
    ASSERT_EQ(elem2.d, retrieved.d);
}