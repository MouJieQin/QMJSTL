#include <iostream>
#include <unordered_set>

#include "../QMJSTL/unordered_set_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{

template <typename value_type>
class Test_unordered_multiset
    : public Test_set_map_base<
          std::unordered_multiset<value_type>, qmj::unordered_multiset<value_type>>
{
};

class Test_unordered_multiset_int : public Test_unordered_multiset<int>
{
};

class Test_unordered_multiset_string : public Test_unordered_multiset<std::string>
{
};

class Test_unordered_multiset_pair
    : public Test_set_map_base<
          std::unordered_multiset<std::pair<std::string, int>, hash_pair>,
          qmj::unordered_multiset<std::pair<std::string, int>, hash_pair>>
{
};

TEST_F(Test_unordered_multiset_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_multiset_int, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_multiset_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_multiset_int, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_multiset_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_multiset_int, Test_emplace_hint)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_emplace_hint();
}

TEST_F(Test_unordered_multiset_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_unordered_multiset_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_multiset_string, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_unordered_multiset_string, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_multiset_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_multiset_string, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_multiset_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_multiset_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_unordered_multiset_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_multiset_pair, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_unordered_multiset_pair, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_multiset_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_multiset_pair, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_multiset_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_multiset_pair, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

} // namespace test
} // namespace qmj