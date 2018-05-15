#include <iostream>
#include <queue>

#include "../QMJSTL/queue_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{

template <typename value_type>
class Test_queue
    : public Test_stack_queue_base<std::queue<value_type>, qmj::queue<value_type>>
{
  public:
    void test_back()
    {
        EXPECT_EQ(this->std_con.back(), this->qmj_con.back())
            << "not equal of back in queue";
    }

    void test_front()
    {
        EXPECT_EQ(this->std_con.front(), this->qmj_con.front())
            << "not equal of front in queue";
    }
};

class Test_queue_int : public Test_queue<int>
{
};

class Test_queue_string : public Test_queue<std::string>
{
};

class Test_queue_pair : public Test_queue<std::pair<std::string, int>>
{
};

TEST_F(Test_queue_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_queue_int, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_queue_int, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_queue_int, Test_front)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_front();
}

TEST_F(Test_queue_int, Test_back)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_back();
}

TEST_F(Test_queue_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_back();
    test_front();
}

TEST_F(Test_queue_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_queue_string, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_queue_string, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_queue_string, Test_front)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_front();
}

TEST_F(Test_queue_string, Test_back)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_back();
}

TEST_F(Test_queue_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_back();
    test_front();
}

TEST_F(Test_queue_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_queue_pair, Test_push)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push();
}

TEST_F(Test_queue_pair, Test_pop)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_pop();
}

TEST_F(Test_queue_pair, Test_front)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_front();
}

TEST_F(Test_queue_pair, Test_back)
{
    ASSERT_TRUE(load_data()) << "reset data error";
    test_back();
}

TEST_F(Test_queue_pair, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_push();
    test_pop();
    test_back();
    test_front();
}

} // namespace test
} // namespace qmj