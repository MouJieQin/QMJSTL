#ifndef _NOT_MAP_DATA_CREATE_
#define _NOT_MAP_DATA_CREATE_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace qmj
{
namespace test
{

#define MAXLEN_STRING 15

template <typename ContainerA, typename ContainerB>
bool operator==(const ContainerA &left, const ContainerB &right)
{
    return (left.size() == right.size() &&
            std::equal(left.begin(), left.end(), right.begin()));
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

template <typename Std_container, typename Qmj_container>
class Test_not_map
{
  public:
    typedef typename Std_container::value_type value_type;
    Test_not_map() : data_size(0) {}
    ~Test_not_map() {}

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

    bool is_equal() { return std_con == qmj_con; }

    void test_clear()
    {
        std_con.clear();
        qmj_con.clear();
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
            EXPECT_TRUE(false) << "vector copy/assign error";
            return false;
        }
        bool result = is_equal();
        EXPECT_TRUE(result)
            << "qmj::vector is not equal to std::vector after copy";
        return result;
    }

    void test_push_back()
    {
        try
        {
            for (const value_type &val : data)
            {
                std_con.push_back(val);
                qmj_con.push_back(val);
            }
        }
        catch (...)
        {
            ASSERT_TRUE(false) << "vector push_back error";
        }
        EXPECT_TRUE(is_equal())
            << "qmj::vector is not equal to std::vector after copy";
    }

  protected:
    size_t data_size;
    std::vector<value_type> data;
    Std_container std_con;
    Qmj_container qmj_con;
};

} // namespace qmj
} // namespace test
#endif //_NOT_MAP_DATA_CREATE_