#include <iostream>

#include "../QMJSTL/vector_qmj.h"
#include "gtest/gtest.h"
#include "notMapData.h"

namespace qmj
{
namespace test
{

#define TEST_DATASIZE 100000

template <typename value_type_>
class Test_vector
    : public testing::Test,
      public Test_not_map<std::vector<value_type_>, qmj::vector<value_type_>>
{
  public:
    typedef value_type_ value_type;
    typedef Test_not_map<std::vector<value_type>, qmj::vector<value_type>>
        base_type;
    Test_vector() : base_type() {}
    ~Test_vector() {}

    bool load_data()
    {
        return (this->reset_data(TEST_DATASIZE) && this->test_assign());
    }

    void test_erase()
    {
        try
        {
            size_t erase_first = this->std_con.size() / 4;
            size_t erase_end = erase_first * 3;
            this->std_con.erase(this->std_con.begin() + erase_first,
                                this->std_con.begin() + erase_end);
            this->qmj_con.erase(this->qmj_con.begin() + erase_first,
                                this->qmj_con.begin() + erase_end);
        }
        catch (...)
        {
            ASSERT_TRUE(false) << "vector erase error";
        }
        EXPECT_TRUE(this->is_equal())
            << "qmj::vector is not equal to std::vector after erase";
    }

    void test_insert()
    {
        try
        {
            std::vector<value_type> insert_data(10, value_type());
            size_t insert_pos = this->data_size / 4;
            this->std_con.insert(this->std_con.begin() + insert_pos,
                                 insert_data.begin(), insert_data.end());
            this->qmj_con.insert(this->qmj_con.begin() + insert_pos,
                                 insert_data.begin(), insert_data.end());
        }
        catch (...)
        {
            ASSERT_TRUE(false) << "vector insert error";
        }
        EXPECT_TRUE(this->is_equal())
            << "qmj::vector is not equal to std::vector after copy";
    }

    void test_pop_back()
    {
        this->std_con.pop_back();
        this->qmj_con.pop_back();
        EXPECT_TRUE(this->is_equal())
            << "qmj::vector is not equal to std::vector after copy";
    }

    void test_resize()
    {
        const size_t new_size = this->std_con.size() / 2;
        this->std_con.resize(new_size);
        this->qmj_con.resize(new_size);
        EXPECT_TRUE(this->is_equal())
            << "qmj::vector is not equal to std::vector after copy";
    }
};

class Test_vector_int : public Test_vector<int>
{
};

class Test_vector_string : public Test_vector<std::string>
{
};

class Test_vector_pair: public Test_vector<std::pair<std::string,int>>
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