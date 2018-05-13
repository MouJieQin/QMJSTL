#include <iostream>
#include <unordered_map>

#include "../QMJSTL/unordered_map_qmj.h"
#include "notMapData.h"

namespace qmj
{
namespace test
{

template <typename key_type, typename value_type>
class Test_unordered_multimap
    : public Test_set_map_base<
          std::unordered_multimap<key_type, value_type>,
          qmj::unordered_multimap<key_type, value_type>>
{
};

class Test_unordered_multimap_int : public Test_unordered_multimap<int,int>
{
};

class Test_unordered_multimap_string : public Test_unordered_multimap<std::string, int>
{
};

class Test_unordered_multimap_pair
    : public Test_set_map_base<
          std::unordered_multimap<std::pair<std::string, int>, int, hash_pair>,
          qmj::unordered_multimap<std::pair<std::string, int>, int, hash_pair>>
{
};

TEST_F(Test_unordered_multimap_int, Test_assign)
{
   ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
   test_assign();
}

TEST_F(Test_unordered_multimap_int, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_unordered_multimap_int, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_multimap_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_multimap_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_multimap_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
}

TEST_F(Test_unordered_multimap_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_multimap_string, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_unordered_multimap_string, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_multimap_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_multimap_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_multimap_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
}

TEST_F(Test_unordered_multimap_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_multimap_pair, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_unordered_multimap_pair, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_multimap_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_multimap_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_multimap_pair, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
}

} // namespace test
} // namespace qmj