#include <flat_tuple.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <type_traits>
#include <string>

using namespace ::testing;

namespace pns
{

struct flat_tuple_test : public Test
{};

struct empty_flat_tuple_test : public flat_tuple_test
{
    using ClassUnderTest = flat_tuple<>;
    ClassUnderTest objectUnderTest;
};

TEST_F(empty_flat_tuple_test, shallBeEmpty)
{
    EXPECT_TRUE(std::is_empty_v<ClassUnderTest>);
}

struct one_POD_elem_flat_tuple_test : public flat_tuple_test
{
    using elem_type = int;
    using ClassUnderTest = flat_tuple<elem_type>;
    ClassUnderTest objectUnderTest;
    const elem_type VALUE1 = 123;
    ClassUnderTest objectUnderTestWithValue{VALUE1};
    const elem_type VALUE2 = 321;
};

TEST_F(one_POD_elem_flat_tuple_test, shallFollowElemTypeProps)
{
    EXPECT_EQ(sizeof(elem_type), sizeof(ClassUnderTest));
    EXPECT_EQ(alignof(elem_type), alignof(ClassUnderTest));
    EXPECT_TRUE((std::is_base_of_v<FlatTupleDetails::ElemAggregate<elem_type, 0u>, ClassUnderTest>));
}

TEST_F(one_POD_elem_flat_tuple_test, shallStoreValue)
{
    EXPECT_EQ(elem_type{}, get<0>(objectUnderTest));
    EXPECT_EQ(VALUE1, get<0>(objectUnderTestWithValue));
    get<0>(objectUnderTestWithValue) = VALUE2;
    EXPECT_EQ(VALUE2, get<0>(objectUnderTestWithValue));
}

struct one_nonPOD_elem_flat_tuple_test : public flat_tuple_test
{
    using elem_type = std::string;
    using ClassUnderTest = flat_tuple<elem_type>;
    ClassUnderTest objectUnderTest;
    const elem_type VALUE1 = "Hello";
    ClassUnderTest objectUnderTestWithValue{VALUE1};
    const elem_type VALUE2 = "World";
};

TEST_F(one_nonPOD_elem_flat_tuple_test, shallFollowElemTypeProps)
{
    EXPECT_EQ(sizeof(elem_type), sizeof(ClassUnderTest));
    EXPECT_EQ(alignof(elem_type), alignof(ClassUnderTest));
    EXPECT_TRUE((std::is_base_of_v<FlatTupleDetails::ElemCompose<elem_type, 0u>, ClassUnderTest>));
}

TEST_F(one_nonPOD_elem_flat_tuple_test, shallStoreValue)
{
    EXPECT_EQ(elem_type{}, get<0>(objectUnderTest));
    EXPECT_EQ(VALUE1, get<0>(objectUnderTestWithValue));
    get<0>(objectUnderTestWithValue) = VALUE2;
    EXPECT_EQ(VALUE2, get<0>(objectUnderTestWithValue));
}

namespace
{
struct Empty1 {};
struct Empty2 {};
}
struct one_empty_elem_flat_tuple_test : public flat_tuple_test
{
    using elem_type = Empty1;
    using ClassUnderTest = flat_tuple<elem_type>;
    ClassUnderTest objectUnderTest;
};

TEST_F(one_empty_elem_flat_tuple_test, shallFollowElemTypeProps)
{
    EXPECT_EQ(sizeof(elem_type), sizeof(ClassUnderTest));
    EXPECT_EQ(alignof(elem_type), alignof(ClassUnderTest));
    EXPECT_TRUE((std::is_base_of_v<FlatTupleDetails::ElemCompose<elem_type, 0u>, ClassUnderTest>));
}

TEST_F(one_empty_elem_flat_tuple_test, shallStoreValue)
{
    get<0>(objectUnderTest) = get<0>(Const(objectUnderTest));
}

struct two_empty_elems_flat_tuple_test : public flat_tuple_test
{
    using elem_type1 = Empty1;
    using elem_type2 = Empty2;
    using ClassUnderTest = flat_tuple<elem_type1, elem_type2>;
    ClassUnderTest objectUnderTest;
    ClassUnderTest objectUnderTest1{elem_type1{}};
    ClassUnderTest objectUnderTest2{elem_type1{}, elem_type2{}};
};

TEST_F(two_empty_elems_flat_tuple_test, shallFollowOneElemTypeProps)
{
    EXPECT_EQ(sizeof(elem_type1), sizeof(ClassUnderTest));
    EXPECT_EQ(alignof(elem_type2), alignof(ClassUnderTest));
    EXPECT_TRUE((std::is_base_of_v<FlatTupleDetails::ElemCompose<elem_type1, 0u>, ClassUnderTest>));
    EXPECT_TRUE((std::is_base_of_v<FlatTupleDetails::ElemCompose<elem_type2, 1u>, ClassUnderTest>));
}

TEST_F(two_empty_elems_flat_tuple_test, shallStoreValue)
{
    get<0>(objectUnderTest) = get<0>(Const(objectUnderTest));
    get<1>(objectUnderTest) = get<1>(Const(objectUnderTest));
}

}
