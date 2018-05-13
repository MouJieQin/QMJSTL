#include <iostream>
#include <unordered_map>

#include "../QMJSTL/unordered_map_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{

template <typename key_type, typename value_type>
class Test_unordered_map
    : public Test_set_map_base<
          std::unordered_map<key_type, value_type>,
          qmj::unordered_map<key_type, value_type>>
{
};

class Test_unordered_map_int : public Test_unordered_map<int,int>
{
};

class Test_unordered_map_string : public Test_unordered_map<std::string, int>
{
};

class Test_unordered_map_pair
    : public Test_set_map_base<
          std::unordered_map<std::pair<std::string, int>, int, hash_pair>,
          qmj::unordered_map<std::pair<std::string, int>, int, hash_pair>>
{
};

TEST_F(Test_unordered_map_int, Test_assign)
{
   ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
   test_assign();
}

TEST_F(Test_unordered_map_int, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_map_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_map_int, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_map_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_map_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_unordered_map_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_map_string, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_map_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_map_string, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_map_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_map_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_unordered_map_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_map_pair, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_map_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_map_pair, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_map_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_map_pair, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

} // namespace test
} // namespace qmj