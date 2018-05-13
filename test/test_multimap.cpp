#include <iostream>
#include <map>

#include "../QMJSTL/map_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{

template <typename key_type, typename value_type>
class Test_multimap
    : public Test_set_map_base<
          std::multimap<key_type, value_type>,
          qmj::multimap<key_type, value_type>>
{
};

class Test_multimap_int : public Test_multimap<int, int>
{
};

class Test_multimap_string : public Test_multimap<std::string, int>
{
};

class Test_multimap_pair : public Test_multimap<std::pair<std::string, int>, int>
{
};

TEST_F(Test_multimap_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_multimap_int, Test_emplace_hint)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_emplace_hint();
}

TEST_F(Test_multimap_int, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_multimap_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_multimap_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_multimap_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_emplace_hint();
    test_erase();
    test_count();
    test_insert();
}

TEST_F(Test_multimap_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_multimap_string, Test_emplace_hint)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_emplace_hint();
}

TEST_F(Test_multimap_string, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_multimap_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_multimap_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_multimap_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_emplace_hint();
    test_erase();
    test_count();
    test_insert();
}

TEST_F(Test_multimap_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_multimap_pair, Test_emplace_hint)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_emplace_hint();
}

TEST_F(Test_multimap_pair, Test_count)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_count();
}

TEST_F(Test_multimap_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_multimap_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_multimap_pair, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_emplace_hint();
    test_erase();
    test_count();
    test_insert();
}

} // namespace test
} // namespace qmj