#include <iostream>
#include <set>

#include "../QMJSTL/set_qmj.h"
#include "notMapData.h"

namespace qmj
{
namespace test
{

template <typename value_type>
class Test_multiset
    : public Test_set_map_base<
          std::multiset<value_type>, qmj::multiset<value_type>>
{
};

class Test_multiset_int : public Test_multiset<int>
{
};

class Test_multiset_string : public Test_multiset<std::string>
{
};

class Test_multiset_pair:public Test_multiset<std::pair<std::string,int>>
{
};

TEST_F(Test_multiset_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_multiset_int, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_multiset_int, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_multiset_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_multiset_int, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_multiset_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_multiset_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_multiset_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_multiset_string, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_multiset_string, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_multiset_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_multiset_string, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_multiset_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_multiset_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_equal_range();
    test_erase();
    test_count();
    test_insert();
    test_find();
}

TEST_F(Test_multiset_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_multiset_pair, Test_equal_range)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_equal_range();
}

TEST_F(Test_multiset_pair, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_multiset_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_multiset_pair, Test_find)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_find();
}

TEST_F(Test_multiset_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_multiset_pair, Test_all)
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