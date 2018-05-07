#ifndef _NOT_MAP_DATA_CREATE_
#define _NOT_MAP_DATA_CREATE_

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace qmj
{
namespace test
{

#define MAXLEN_STRING 15
#define TEST_DATASIZE 100000

template <typename ContainerA, typename ContainerB>
bool _is_equal_imple(const ContainerA &left, const ContainerB &right,
                     std::bidirectional_iterator_tag)
{
    return (left.size() == right.size() &&
            std::equal(left.begin(), left.end(), right.begin()));
}

template <typename ContainerA, typename ContainerB>
bool _is_equal_imple(const ContainerA &left, const ContainerB &right,
                     std::forward_iterator_tag)
{
    try
    {
        return ((left.empty() && right.empty()) || std::equal(left.begin(),
                                                              left.end(), right.begin()));
    }
    catch (...)
    {
        return false;
    }
    return false;
}

template <typename ContainerA, typename ContainerB>
bool is_equal(const ContainerA &left, const ContainerB &right)
{
    return _is_equal_imple(left, right, qmj::iterator_category(left.begin()));
}

template <typename Iter, typename Dif>
Iter advance(Iter iter, Dif dif)
{
    std::advance(iter, dif);
    return iter;
}

bool create_data(std::vector<int> &vec, const size_t data_size)
{
    std::vector<int> temp;
    try
    {
        temp.reserve(data_size);
        for (size_t i = 0; i != data_size; ++i)
            temp.push_back(i);
    }
    catch (...)
    {
        return false;
    }
    std::random_shuffle(temp.begin(), temp.end());
    vec.swap(temp);
    return true;
}

bool create_data(std::vector<std::string> &vec, const size_t data_size)
{
    try
    {
        std::vector<std::string> temp;
        temp.reserve(data_size);
        std::vector<char> rand_char;
        const size_t one_circul = ((1 + MAXLEN_STRING) * MAXLEN_STRING / 2);
        const size_t len_char = (data_size / MAXLEN_STRING + 1) * one_circul;
        rand_char.reserve(len_char);
        char c = 'a';
        for (size_t i = 0; i != len_char; ++i)
        {
            if (c > 'z')
                c = 'a';
            rand_char.push_back(c++);
        }
        std::random_shuffle(rand_char.begin(), rand_char.end());
        size_t cur_len = 1;
        size_t k = 0;
        for (size_t i = 0; i != data_size; ++i)
        {
            std::string cur_str = "";
            for (size_t j = 0; j != cur_len; ++j)
                cur_str += rand_char[k++];
            temp.push_back(std::move(cur_str));
            if (++cur_len > MAXLEN_STRING)
                cur_len = 1;
        }
        std::random_shuffle(temp.begin(), temp.end());
        vec.swap(temp);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool create_data(std::vector<std::pair<std::string, int>> &vec,
                 const size_t data_size)
{
    try
    {
        std::vector<std::string> first;
        std::vector<int> second;
        if (create_data(first, data_size) == false)
            return false;
        if (create_data(second, data_size) == false)
            return false;
        std::vector<std::pair<std::string, int>> temp;
        for (size_t i = 0; i != data_size; ++i)
            temp.emplace_back(first[i], second[i]);
        vec.swap(temp);
    }
    catch (...)
    {
        return false;
    }
    return true;
}

template <typename STD_container, typename QMJ_container>
class Test_not_map_base : public testing::Test
{
  public:
    typedef typename STD_container::value_type value_type;
    Test_not_map_base() : data_size(0) {}
    ~Test_not_map_base() {}

    bool reset_data(const size_t new_size)
    {
        if (new_size == data_size)
        {
            std::random_shuffle(data.begin(), data.end());
            return true;
        }
        data_size = new_size;
        return create_data(data, new_size);
    }

    bool load_data()
    {
        return (this->reset_data(TEST_DATASIZE) && this->test_assign());
    }

    bool is_equal() { return qmj::test::is_equal(std_con, qmj_con); }

    void test_clear()
    {
        std_con.clear();
        qmj_con.clear();
        EXPECT_TRUE(is_equal())
            << "qmj::Container is not equal to std::Container after clear";
    }

    bool test_assign()
    {
        try
        {
            std_con.assign(data.begin(), data.end());
            qmj_con.assign(data.begin(), data.end());
        }
        catch (...)
        {
            EXPECT_TRUE(false) << "copy/assign error";
            return false;
        }
        bool result = is_equal();
        EXPECT_TRUE(result)
            << "qmj::Container is not equal to std::Container after copy/assign";
        return result;
    }

    void test_resize()
    {
        size_t len = qmj::distance(this->std_con.begin(), this->std_con.end());
        size_t new_size = len / 4;
        this->std_con.resize(new_size);
        this->qmj_con.resize(new_size);
        EXPECT_TRUE(this->is_equal()) << "qmj::forward is not equal to std::forward_list after resize";
    }

  protected:
    size_t data_size;
    std::vector<value_type> data;
    STD_container std_con;
    QMJ_container qmj_con;
};

template <typename STD_container, typename QMJ_container>
class Test_not_map : public Test_not_map_base<STD_container, QMJ_container>
{
  public:
    typedef typename STD_container::value_type value_type;
    typedef Test_not_map_base<STD_container, QMJ_container> base_type;
    Test_not_map() : base_type() {}
    ~Test_not_map() {}

    void test_erase()
    {
        try
        {
            size_t erase_first = this->std_con.size() / 4;
            size_t erase_end = erase_first * 3;
            this->std_con.erase(advance(this->std_con.begin(), erase_first),
                                advance(this->std_con.begin(), erase_end));
            this->qmj_con.erase(advance(this->qmj_con.begin(), erase_first),
                                advance(this->qmj_con.begin(), erase_end));
        }
        catch (...)
        {
            ASSERT_TRUE(false) << "Container erase error";
        }
        EXPECT_TRUE(this->is_equal())
            << "qmj::Container is not equal to std::Container after erase";
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
            ASSERT_TRUE(false) << "Container insert error";
        }
        EXPECT_TRUE(this->is_equal())
            << "qmj::Container is not equal to std::Container after insert";
    }

    void test_push_back()
    {
        try
        {
            for (const value_type &val : this->data)
            {
                this->std_con.push_back(val);
                this->qmj_con.push_back(val);
            }
        }
        catch (...)
        {
            ASSERT_TRUE(false) << "Container push_back error";
        }
        EXPECT_TRUE(this->is_equal())
            << "qmj::Container is not equal to std::Container after push_back";
    }

    void test_pop_back()
    {
        this->std_con.pop_back();
        this->qmj_con.pop_back();
        EXPECT_TRUE(this->is_equal())
            << "qmj::Container is not equal to std::Container after pop_back";
    }

};

} // namespace qmj
} // namespace test
#endif //_NOT_MAP_DATA_CREATE_