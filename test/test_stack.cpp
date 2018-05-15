#include <iostream>
#include <stack>

#include "../QMJSTL/stack_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{

template <typename value_type>
class Test_stack
    : public Test_stack_queue_base<std::stack<value_type>, qmj::stack<value_type>>
{
};

class Test_stack_int : public Test_stack<int>
{
};

class Test_stack_string : public Test_stack<std::string>
{
};

class Test_stack_pair : public Test_stack<std::pair<std::string,int>>
{
};


TEST_F(Test_stack_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_stack_int, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_stack_int, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_stack_int, Test_top)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_top();
}

TEST_F(Test_stack_int, Test_all)
{
     ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_top();
}

TEST_F(Test_stack_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}


TEST_F(Test_stack_string, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_stack_string, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_stack_string, Test_top)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_top();
}

TEST_F(Test_stack_string, Test_all)
{
     ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_top();
}

TEST_F(Test_stack_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_stack_pair, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_stack_pair, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_stack_pair, Test_top)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_top();
}

TEST_F(Test_stack_pair, Test_all)
{
     ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_top();
}


} // namespace test
} // namespace qmj