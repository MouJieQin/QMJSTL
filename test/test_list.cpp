#include <iostream>
#include <list>

#include "../QMJSTL/list_qmj.h"
#include "test_create_data.h"

namespace qmj
{
namespace test
{

template <typename value_type_>
class Test_list
    : public Test_list_base<std::list<value_type_>,qmj::list<value_type_>>
{
  public:
    typedef value_type_ value_type;
    typedef Test_list_base<std::list<value_type>, qmj::list<value_type>> base_type;
    Test_list() {}
    ~Test_list() {}

    void test_erase()
    {
        size_t len = qmj::distance(this->std_con.begin(), this->std_con.end());
        size_t erase_first = len / 3;
        size_t erase_end = erase_first * 3;
        this->std_con.erase(advance(this->std_con.begin(), erase_first),
                                  advance(this->std_con.begin(), erase_end));
        this->qmj_con.erase(advance(this->qmj_con.begin(), erase_first),
                                  advance(this->qmj_con.begin(), erase_end));
        EXPECT_TRUE(this->is_equal()) << "qmj::list is not equal to std::list after erase";
    }

    void test_insert()
    {
        this->std_con.insert(this->std_con.begin(), 10, value_type());
        this->qmj_con.insert(this->qmj_con.begin(), 10, value_type());
        EXPECT_TRUE(this->is_equal()) << "qmj::list is not equal to std::list after insert";
    }

    void test_splice()
    {
        std::list<value_type> std_splice(this->std_con);
        qmj::list<value_type> qmj_splice(this->qmj_con);
        size_t len = qmj::distance(this->std_con.begin(), this->std_con.end());
        size_t splice_first = len / 4;
        size_t splice_end = splice_first * 3;
        this->std_con.splice(advance(this->std_con.begin(), splice_first),
                                   std_splice, advance(std_splice.begin(), splice_first),
                                   advance(std_splice.begin(), splice_end));
        this->qmj_con.splice(advance(this->qmj_con.begin(), splice_first),
                                   qmj_splice, advance(qmj_splice.begin(), splice_first),
                                   advance(qmj_splice.begin(), splice_end));

        EXPECT_TRUE(this->is_equal()) << "qmj::list is not equal to std::list after splice";
        EXPECT_TRUE(is_equal(std_splice, qmj_splice)) << "std_splice is not equal to qmj_splice after splice";
    }

    void test_push_back()
    {
        this->std_con.push_back(value_type());
        this->qmj_con.push_back(value_type());
        EXPECT_TRUE(this->is_equal()) << "qmj::list is not equal to std::list after push_back";
        
    }

      void test_push_front()
    {
        this->std_con.push_front(value_type());
        this->qmj_con.push_front(value_type());
        EXPECT_TRUE(this->is_equal()) << "qmj::list is not equal to std::list after push_front";
        
    }
};

class Test_list_int : public Test_list<int>
{
};

class Test_list_string : public Test_list<std::string>
{
};

class Test_list_pair : public Test_list<std::pair<std::string, int>>
{
};

TEST_F(Test_list_int, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_list_int, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_list_int, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_list_int, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_list_int, Test_push_font)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_list_int, Test_remove_if)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_remove_if();
}

TEST_F(Test_list_int, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_list_int, Test_splice)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_splice();
}

TEST_F(Test_list_int, Test_push_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_list_int, Test_push_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
}

TEST_F(Test_list_int, Test_sort_merge_unique)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_sort_merge_unique();
}

TEST_F(Test_list_int, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_insert();
    test_pop_front();
    test_push_front();
    test_remove_if();
    test_resize();
    test_splice();
    test_push_back();
    test_push_front();
    test_sort_merge_unique();
}

TEST_F(Test_list_string, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_list_string, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_list_string, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_list_string, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_list_string, Test_push_font)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_list_string, Test_remove_if)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_remove_if();
}

TEST_F(Test_list_string, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_list_string, Test_splice)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_splice();
}

TEST_F(Test_list_string, Test_push_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_list_string, Test_push_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
}

TEST_F(Test_list_string, Test_sort_merge_unique)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_sort_merge_unique();
}

TEST_F(Test_list_string, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_insert();
    test_pop_front();
    test_push_front();
    test_remove_if();
    test_resize();
    test_splice();
    test_push_back();
    test_push_front();
    test_sort_merge_unique();
}

TEST_F(Test_list_pair, Test_assign)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
}

TEST_F(Test_list_pair, Test_erase)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_erase();
}

TEST_F(Test_list_pair, Test_insert)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_insert();
}

TEST_F(Test_list_pair, Test_pop_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_pop_front();
}

TEST_F(Test_list_pair, Test_push_font)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_list_pair, Test_remove_if)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_remove_if();
}

TEST_F(Test_list_pair, Test_resize)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_resize();
}

TEST_F(Test_list_pair, Test_splice)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_splice();
}

TEST_F(Test_list_pair, Test_push_front)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_front();
}

TEST_F(Test_list_pair, Test_push_back)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_push_back();
}

TEST_F(Test_list_pair, Test_sort_merge_unique)
{
    ASSERT_TRUE(load_data()) << "load data error";
    test_sort_merge_unique();
}

TEST_F(Test_list_pair, Test_all)
{
    ASSERT_TRUE(reset_data(TEST_DATASIZE)) << "reset data error";
    test_assign();
    test_erase();
    test_insert();
    test_pop_front();
    test_push_front();
    test_remove_if();
    test_resize();
    test_splice();
    test_push_back();
    test_push_front();
    test_sort_merge_unique();
}

} // namespace test
} // namespace qmj