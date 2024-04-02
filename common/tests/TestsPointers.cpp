#include "UniquePtr.h"
#include "SharedPtr.h"

#include "gtest/gtest.h"

HeapAllocator heapAllocator;

struct FloatFixture : public ::testing::TestWithParam<float>{};

INSTANTIATE_TEST_SUITE_P(UniquePtr, FloatFixture, testing::Values(
        0.f, 1.f, 5.f, 99999.11111f
));

TEST_P(FloatFixture, UniquePtrConstructorAndDereference)
{
    auto value = GetParam();

    UniquePtr<float> uniquePtr(new float(value));

    EXPECT_FLOAT_EQ(*uniquePtr, value);
    EXPECT_FLOAT_EQ(*(uniquePtr.Get()), value);
}

TEST_P(FloatFixture, UniquePtrArrowOperator)
{
    struct TestStruct
    {
        float value;
        explicit TestStruct(float val) : value(val) {}
    };

    auto value = GetParam();
    UniquePtr<TestStruct> ptr(new TestStruct(value));
    EXPECT_EQ(ptr->value, value);
}

TEST_P(FloatFixture, UniquePtrCast)
{
    auto value = GetParam();

    UniquePtr<float> uniquePtr = MakeUnique<float>(heapAllocator, value);

    EXPECT_FLOAT_EQ(*uniquePtr, value);
    EXPECT_FLOAT_EQ(*(uniquePtr.Get()), value);
}

TEST_P(FloatFixture, UniquePtrMakeUnique)
{
    auto value = GetParam();

    UniquePtr<float> uniquePtr = MakeUnique<float>(heapAllocator, value);

    EXPECT_FLOAT_EQ(*uniquePtr, value);
    EXPECT_FLOAT_EQ(*(uniquePtr.Get()), value);
}

TEST_P(FloatFixture, SharedPtrConstructorAndDereference)
{
    auto value = GetParam();

    SharedPtr<float> sharedPtr(new float(value));

    EXPECT_FLOAT_EQ(*sharedPtr, value);
    EXPECT_FLOAT_EQ(*(sharedPtr.Get()), value);

    EXPECT_EQ(sharedPtr.SharedCount(), 1);
}

TEST_P(FloatFixture, SharedPtrCopyConstructor)
{
    auto param = GetParam();
    auto* floatPtr = new float(param);

    SharedPtr<float> ptr(floatPtr);
    SharedPtr<float> ptrCopy(ptr);

    EXPECT_EQ(ptr.Get(), floatPtr);
    EXPECT_EQ(ptrCopy.Get(), floatPtr);
    EXPECT_EQ(*ptr.Get(), param);
    EXPECT_EQ(*ptrCopy.Get(), param);

    EXPECT_EQ(ptr.SharedCount(), 2);
    EXPECT_EQ(ptrCopy.SharedCount(), 2);
}

TEST_P(FloatFixture, SharedPtrCopyAssignmentOperator)
{
    auto param = GetParam();
    auto* floatPtr = new float(param);

    SharedPtr<float> ptr(floatPtr);

    // Self assignment.
    ptr = ptr;
    EXPECT_EQ(ptr.Get(), floatPtr);
    EXPECT_EQ(ptr.SharedCount(), 1);

    SharedPtr<float> ptr2;
    ptr2 = ptr;

    EXPECT_EQ(*ptr2, param);
    EXPECT_EQ(ptr2.Get(), ptr.Get());
    EXPECT_EQ(ptr2.Get(), floatPtr);
    EXPECT_EQ(ptr.SharedCount(), 2);
    EXPECT_EQ(ptr2.SharedCount(), 2);
}

TEST_P(FloatFixture, SharedPtrDestructor)
{
    auto param = GetParam();
    auto* floatPtr = new float(param);

    {
        SharedPtr<float> ptr(floatPtr);

        // Expect that the value is the same.
        EXPECT_FLOAT_EQ(*floatPtr, param);

        {
            SharedPtr<float> ptr2(ptr);
            EXPECT_EQ(ptr.SharedCount(), 2);
            EXPECT_EQ(ptr2.SharedCount(), 2);
        } // Call the ptr2 destructor.

        EXPECT_EQ(ptr.SharedCount(), 1);
    } // Call the ptr destructor.
}

TEST_P(FloatFixture, SharedPtrArrowOperator)
{
    struct TestStruct
    {
        float value;
        explicit TestStruct(float val) : value(val) {}
    };

    auto value = GetParam();
    SharedPtr<TestStruct> ptr(new TestStruct(value));
    EXPECT_EQ(ptr->value, value);

    SharedPtr<TestStruct> ptr2(ptr);
    EXPECT_EQ(ptr2->value, value);
    EXPECT_EQ(ptr2->value, ptr->value);
}

TEST_P(FloatFixture, SharedPtrMakeShared)
{
    auto value = GetParam();

    SharedPtr<float> sharedPtr = MakeShared(value);

    EXPECT_FLOAT_EQ(*sharedPtr, value);
    EXPECT_FLOAT_EQ(*(sharedPtr.Get()), value);

    EXPECT_EQ(sharedPtr.SharedCount(), 1);
}