#include <iostream>
#include <queue>

#include "../QMJSTL/queue_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{

template <typename value_type>
class Test_priority_queue
    : public Test_stack_queue_base<std::priority_queue<value_type>, qmj::priority_queue<value_type>>
{
};

class Test_priority_queue_int : public Test_priority_queue<int>
{
};

class Test_priority_queue_string : public Test_priority_queue<std::string>
{
};

class Test_priority_queue_pair : public Test_priority_queue<std::pair<std::string,int>>
{
};

TEST_F(Test_priority_queue_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_priority_queue_int, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_priority_queue_int, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_priority_queue_int, Test_top)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_top();
}

TEST_F(Test_priority_queue_int, Test_all)
{
     ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_top();
}

TEST_F(Test_priority_queue_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}


TEST_F(Test_priority_queue_string, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_priority_queue_string, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_priority_queue_string, Test_top)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_top();
}

TEST_F(Test_priority_queue_string, Test_all)
{
     ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_top();
}

TEST_F(Test_priority_queue_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_priority_queue_pair, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_priority_queue_pair, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_priority_queue_pair, Test_top)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_top();
}

TEST_F(Test_priority_queue_pair, Test_all)
{
     ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_top();
}


} // namespace test
} // namespace qmj