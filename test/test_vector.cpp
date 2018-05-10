#include <iostream>

#include "../QMJSTL/vector_qmj.h"
#include "notMapData.h"

namespace qmj
{
namespace test
{

template <typename value_type>
class Test_vector
    : public Test_rand_container<std::vector<value_type>, qmj::vector<value_type>>
{
};

class Test_vector_int : public Test_vector<int>
{
};

class Test_vector_string : public Test_vector<std::string>
{
};

class Test_vector_pair : public Test_vector<std::pair<std::string, int>>
{
};

TEST_F(Test_vector_int, Test_copy)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_vector_int, Test_push_back)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_back();
}

TEST_F(Test_vector_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_vector_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_vector_int, Test_pop_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_back();
}

TEST_F(Test_vector_int, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_vector_int, Test_all)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
    test_erase();
    test_insert();
    test_pop_back();
    test_resize();
}

TEST_F(Test_vector_string, Test_copy)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_vector_string, Test_push_back)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_back();
}

TEST_F(Test_vector_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_vector_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_vector_string, Test_pop_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_back();
}

TEST_F(Test_vector_string, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_vector_string, Test_all)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
    test_erase();
    test_insert();
    test_pop_back();
    test_resize();
}

TEST_F(Test_vector_pair, Test_copy)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_vector_pair, Test_push_back)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_back();
}

TEST_F(Test_vector_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_vector_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_vector_pair, Test_pop_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_back();
}

TEST_F(Test_vector_pair, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_vector_pair, Test_all)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
    test_erase();
    test_insert();
    test_pop_back();
    test_resize();
}

} // namespace test
} // namespace qmj