#include <deque>
#include <iostream>

#include "../QMJSTL/deque_qmj.h"
#include "notMapData.h"

namespace qmj
{
namespace test
{

template <typename value_type_>
class Test_deque
    : public Test_rand_container<std::deque<value_type_>, qmj::deque<value_type_>>
{
  public:
    typedef value_type_ value_type;
    typedef Test_rand_container<std::deque<value_type_>, qmj::deque<value_type_>>
        base_type;

    Test_deque() : base_type() {}

    ~Test_deque() {}

    void test_pop_front()
    {
        this->std_con.pop_front();
        this->qmj_con.pop_front();
        EXPECT_TRUE(this->is_equal())
            << "qmj::deque is not equal to std::deque after pop font";
    }

    void test_push_front()
    {
        try
        {
            for (const value_type &val : this->data)
            {
                this->std_con.push_front(val);
                this->qmj_con.push_front(val);
            }
        }
        catch (...)
        {
            ASSERT_TRUE(false) << "deque push_front error";
        }
        EXPECT_TRUE(this->is_equal())
            << "qmj::deque is not equal to std::deque after push_front";
    }
};

class Test_deque_int : public Test_deque<int>
{
};

class Test_deque_string : public Test_deque<std::string>
{
};

class Test_deque_pair : public Test_deque<std::pair<std::string, int>>
{
};

TEST_F(Test_deque_int, Test_copy)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_deque_int, Test_push_back)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_back();
}

TEST_F(Test_deque_int, Test_push_front)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_front();
}

TEST_F(Test_deque_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_deque_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_deque_int, Test_pop_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_back();
}

TEST_F(Test_deque_int, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_deque_int, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_deque_int, Test_all)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
    test_push_front();
    test_erase();
    test_insert();
    test_pop_back();
    test_pop_front();
    test_resize();
}

TEST_F(Test_deque_string, Test_copy)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_deque_string, Test_push_back)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_back();
}

TEST_F(Test_deque_string, Test_push_front)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_front();
}

TEST_F(Test_deque_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_deque_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_deque_string, Test_pop_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_back();
}

TEST_F(Test_deque_string, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_deque_string, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_deque_string, Test_all)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
    test_push_front();
    test_erase();
    test_insert();
    test_pop_back();
    test_pop_front();
    test_resize();
}

TEST_F(Test_deque_pair, Test_copy)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_deque_pair, Test_push_back)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_back();
}

TEST_F(Test_deque_pair, Test_push_front)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_push_front();
}

TEST_F(Test_deque_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_deque_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_deque_pair, Test_pop_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_back();
}

TEST_F(Test_deque_pair, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_deque_pair, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_deque_pair, Test_all)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
    test_push_front();
    test_erase();
    test_insert();
    test_pop_back();
    Test_deque_pair_Test_pop_front_Test();
    test_resize();
}

} // namespace test
} // namespace qmj