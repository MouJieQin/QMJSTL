#include <iostream>
#include <unordered_set>

#include "../QMJSTL/unordered_set_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{
    
template <typename value_type>
class Test_unordered_set
    : public Test_set_map_base<
          std::unordered_set<value_type>, qmj::unordered_set<value_type>>
{
};

class Test_unordered_set_int : public Test_unordered_set<int>
{
};

class Test_unordered_set_string : public Test_unordered_set<std::string>
{
};

class Test_unordered_set_pair
    : public Test_set_map_base<
          std::unordered_set<std::pair<std::string, int>, hash_pair>,
          qmj::unordered_set<std::pair<std::string, int>, hash_pair>>
{
};

TEST_F(Test_unordered_set_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_set_int, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_set_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_set_int, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_set_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_set_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_unordered_set_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_set_string, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_set_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_set_string, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_set_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_set_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_unordered_set_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_unordered_set_pair, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_unordered_set_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_unordered_set_pair, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_unordered_set_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_unordered_set_pair, Test_all)
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