#include <iostream>
#include <forward_list>

#include "../QMJSTL/forward_list_qmj.h"
#include "notMapData.h"

namespace qmj
{
namespace test
{

template <typename value_type>
class Test_forward_list : public Test_list_base<std::forward_list<value_type>,
                                         qmj::forward_list<value_type>>
{
  public:
    typedef Test_list_base<std::forward_list<value_type>,
                           qmj::forward_list<value_type>>
        base_type;
    Test_forward_list() : base_type() {}

    ~Test_forward_list() {}

    void test_erase_after()
    {
        size_t len = qmj::distance(this->std_con.begin(), this->std_con.end());
        size_t erase_first = len / 3;
        size_t erase_end = erase_first * 3;
        this->std_con.erase_after(advance(this->std_con.begin(), erase_first),
                                  advance(this->std_con.begin(), erase_end));
        this->qmj_con.erase_after(advance(this->qmj_con.begin(), erase_first),
                                  advance(this->qmj_con.begin(), erase_end));
        EXPECT_TRUE(this->is_equal()) << "qmj::forward is not equal to std::forward_list after erase_after";
    }

    void test_insert_after()
    {
        this->std_con.insert_after(this->std_con.begin(), 10, value_type());
        this->qmj_con.insert_after(this->qmj_con.begin(), 10, value_type());
        EXPECT_TRUE(this->is_equal()) << "qmj::forward is not equal to std::forward_list after insert_after";
    }

    void test_splice_after()
    {
        std::forward_list<value_type> std_splice(this->std_con);
        qmj::forward_list<value_type> qmj_splice(this->qmj_con);
        size_t len = qmj::distance(this->std_con.begin(), this->std_con.end());
        size_t splice_first = len / 4;
        size_t splice_end = splice_first * 3;
        this->std_con.splice_after(advance(this->std_con.begin(), splice_first),
                                   std_splice, advance(std_splice.begin(), splice_first),
                                   advance(std_splice.begin(), splice_end));
        this->qmj_con.splice_after(advance(this->qmj_con.begin(), splice_first),
                                   qmj_splice, advance(qmj_splice.begin(), splice_first),
                                   advance(qmj_splice.begin(), splice_end));
        EXPECT_TRUE(this->is_equal()) << "qmj::forward is not equal to std::forward_list after splice_after";
        EXPECT_TRUE(is_equal(std_splice, qmj_splice)) << "std_splice is not equal to qmj_splice after splice_after";
    }
};

class Test_forward_list_int : public Test_forward_list<int>
{
};

class Test_forward_list_string : public Test_forward_list<std::string>
{
};

class Test_forward_list_pair : public Test_forward_list<std::pair<std::string, int>>
{
};

TEST_F(Test_forward_list_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_forward_list_int, Test_erase_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase_after();
}

TEST_F(Test_forward_list_int, Test_insert_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert_after();
}

TEST_F(Test_forward_list_int, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_forward_list_int, Test_push_font)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_forward_list_int, Test_remove_if)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_remove_if();
}

TEST_F(Test_forward_list_int, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_forward_list_int, Test_splice_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_splice_after();
}

TEST_F(Test_forward_list_int, Test_sort_merge_unique)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_sort_merge_unique();
}

TEST_F(Test_forward_list_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase_after();
    test_insert_after();
    test_pop_front();
    test_push_front();
    test_remove_if();
    test_resize();
    test_splice_after();
    test_sort_merge_unique();
}

TEST_F(Test_forward_list_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_forward_list_string, Test_erase_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase_after();
}

TEST_F(Test_forward_list_string, Test_insert_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert_after();
}

TEST_F(Test_forward_list_string, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_forward_list_string, Test_push_font)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_forward_list_string, Test_remove_if)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_remove_if();
}

TEST_F(Test_forward_list_string, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_forward_list_string, Test_splice_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_splice_after();
}

TEST_F(Test_forward_list_string, Test_sort_merge_unique)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_sort_merge_unique();
}

TEST_F(Test_forward_list_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase_after();
    test_insert_after();
    test_pop_front();
    test_push_front();
    test_remove_if();
    test_resize();
    test_splice_after();
    test_sort_merge_unique();
}

TEST_F(Test_forward_list_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_forward_list_pair, Test_erase_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase_after();
}

TEST_F(Test_forward_list_pair, Test_insert_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert_after();
}

TEST_F(Test_forward_list_pair, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_forward_list_pair, Test_push_font)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_forward_list_pair, Test_remove_if)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_remove_if();
}

TEST_F(Test_forward_list_pair, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_forward_list_pair, Test_splice_after)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_splice_after();
}

TEST_F(Test_forward_list_pair, Test_sort_merge_unique)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_sort_merge_unique();
}

TEST_F(Test_forward_list_pair, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase_after();
    test_insert_after();
    test_pop_front();
    test_push_front();
    test_remove_if();
    test_resize();
    test_splice_after();
    test_sort_merge_unique();
}

} // namespace test
} // namespace qmj