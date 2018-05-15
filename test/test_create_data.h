#ifndef _TEST_CREATE_DATA_
#define _TEST_CREATE_DATA_

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "../QMJSTL/hashfunction.h"

namespace qmj
{
namespace test
{

#define MAXLEN_STRING 15
#define TEST_DATASIZE 10000

template <typename Container>
std::ostream &display(const Container &con)
{
    for (auto const &val : con)
        std::cout << val << " ";
    return std::cout;
}

bool operator<(const std::pair<std::string, int> &left,
               const std::pair<std::string, int> &right)
{
    return left.first < right.first;
}

template <typename STD_container, typename QMJ_container>
bool _is_equal_imple(const STD_container &left, const QMJ_container &right,
                     std::bidirectional_iterator_tag)
{
    return (left.size() == right.size() &&
            std::equal(left.begin(), left.end(), right.begin()));
}

template <typename STD_container, typename QMJ_container>
bool _is_equal_imple(const STD_container &left, const QMJ_container &right,
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

template <typename STD_container, typename QMJ_container>
bool is_equal(const STD_container &left, const QMJ_container &right)
{
    return _is_equal_imple(left, right, qmj::iterator_category(left.begin()));
}

template <typename Container, typename = void>
struct is_unordered : qmj::false_type
{
};

template <typename Container>
struct is_unordered<Container,
                    qmj::void_t<typename Container::local_iterator>>
    : qmj::true_type
{
};

template <typename Container, typename = void>
struct is_multi : qmj::false_type
{
};

template <typename Container>
struct is_multi<Container,
                qmj::void_t<typename Container::is_multi>>
    : qmj::true_type
{
};

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

template <typename key_type, typename value_type>
bool create_data(std::vector<std::pair<key_type, value_type>> &vec, const size_t data_size)
{
    try
    {
        std::vector<key_type> first;
        std::vector<value_type> second;
        if (create_data(first, data_size) == false)
            return false;
        if (create_data(second, data_size) == false)
            return false;
        std::vector<std::pair<key_type, value_type>> temp;
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

template <typename Container, typename = void>
struct is_map_type : qmj::false_type
{
    typedef typename Container::value_type key_type;
    typedef typename Container::value_type mapped_type;
};

template <typename Container>
struct is_map_type<Container, qmj::void_t<typename Container::mapped_type>>
    : qmj::true_type
{
    typedef typename Container::key_type key_type;
    typedef typename Container::mapped_type mapped_type;
};

template <typename STD_container, typename QMJ_container>
class Test_data : public testing::Test
{
  public:
    typedef typename STD_container::value_type value_type;
    typedef typename is_map_type<STD_container>::key_type key_type;
    typedef typename is_map_type<STD_container>::mapped_type mapped_type;

    typedef typename qmj::If<
        qmj::is_same<key_type, value_type>::value, value_type,
        std::pair<key_type, mapped_type>>::type data_value_type;

    typedef typename qmj::If<
        qmj::is_same<key_type, value_type>::value, qmj::false_type,
        qmj::true_type>::type is_map_type;

    typedef typename qmj::If<
        is_unordered<QMJ_container>::value, qmj::true_type,
        qmj::false_type>::type is_unordered_type;

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

    bool is_equal() { return qmj::test::is_equal(std_con, qmj_con); }

    bool is_euqal_special()
    {
        if (std_con.size() != qmj_con.size())
            return false;
        STD_container qmj_cp(qmj_con.begin(), qmj_con.end());
        QMJ_container std_cp(std_con.begin(), std_con.end());
        return (std_con == qmj_cp && qmj_con == std_cp);
    }

  protected:
    size_t data_size = 0;
    std::vector<data_value_type> data;
    STD_container std_con;
    QMJ_container qmj_con;
};

template <typename STD_container, typename QMJ_container>
class Test_line_container : public Test_data<STD_container, QMJ_container>
{
  public:
    typedef Test_data<STD_container, QMJ_container> base_type;
    Test_line_container() : base_type() {}
    ~Test_line_container() {}

    bool load_data()
    {
        return (this->reset_data(TEST_DATASIZE) && this->test_assign());
    }

    bool test_assign()
    {
        try
        {
            this->std_con.assign(this->data.begin(), this->data.end());
            this->qmj_con.assign(this->data.begin(), this->data.end());
        }
        catch (...)
        {
            EXPECT_TRUE(false) << "copy/assign error";
            return false;
        }
        bool result = this->is_equal();
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
};

template <typename STD_container, typename QMJ_container>
class Test_rand_container : public Test_line_container<STD_container, QMJ_container>
{
  public:
    typedef typename STD_container::value_type value_type;
    typedef Test_line_container<STD_container, QMJ_container> base_type;
    Test_rand_container() : base_type() {}
    ~Test_rand_container() {}

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

template <typename STD_container, typename QMJ_container>
class Test_list_base
    : public Test_line_container<STD_container, QMJ_container>
{
  public:
    typedef typename STD_container::value_type value_type;
    typedef Test_line_container<STD_container, QMJ_container> base_type;

    Test_list_base() {}

    ~Test_list_base() {}

    void test_pop_front()
    {
        this->std_con.pop_front();
        this->qmj_con.pop_front();
        EXPECT_TRUE(this->is_equal()) << "qmj::Container is not equal to std::Container after pop_front";
    }

    void test_push_front()
    {
        this->std_con.push_front(value_type());
        this->qmj_con.push_front(value_type());
        EXPECT_TRUE(this->is_equal()) << "qmj::Container is not equal to std::Container after push_front";
    }

    auto get_pred(int)
    {
        return [](const value_type &val) { return (val & 1); };
    }

    auto get_pred(std::string)
    {
        return [](const std::string &val) { return (val.size() & 1); };
    }

    auto get_pred(std::pair<std::string, int>)
    {
        return [](const std::pair<std::string, int> &val) { return (val.second & 1); };
    }

    void test_remove_if()
    {
        auto pred = get_pred(value_type());
        this->std_con.remove_if(pred);
        this->qmj_con.remove_if(pred);
        EXPECT_TRUE(this->is_equal()) << "qmj::Container is not equal to std::Container after remove_if";
    }

    void test_sort_merge_unique()
    {
        this->std_con.sort();
        this->qmj_con.sort();
        EXPECT_TRUE(this->is_equal()) << "qmj::Container is not equal to std::Container after sort";
        STD_container std_cp(this->std_con);
        QMJ_container qmj_cp(this->qmj_con);
        this->std_con.merge(std_cp);
        this->qmj_con.merge(qmj_cp);
        EXPECT_TRUE(this->is_equal()) << "qmj::Container is not equal to std::Container after merge";
        EXPECT_TRUE(is_equal(std_cp, qmj_cp)) << "qmj_cp is not equal to std_cp after merge";

        this->std_con.unique();
        this->qmj_con.unique();
        EXPECT_TRUE(this->is_equal()) << "qmj::Container is not equal to std::Container after unique";
    }
};

struct hash_pair
{
    size_t operator()(const std::pair<std::string, int> &pr) const
    {
        return _QMJ hash<int>()(pr.second);
    }
};

template <typename STD_container, typename QMJ_container>
class Test_set_map_base : public Test_data<STD_container, QMJ_container>
{
  public:
    enum
    {
        is_multi = QMJ_container::is_multi
    };
    typedef Test_data<STD_container, QMJ_container> base_type;
    typedef typename base_type::value_type value_type;
    typedef typename base_type::key_type key_type;
    typedef typename base_type::is_map_type is_map_type;
    typedef typename base_type::is_unordered_type is_unordered_type;

    Test_set_map_base() : base_type() {}
    ~Test_set_map_base() {}

    template <bool is_unordered = is_unordered_type::value,
              bool multi = is_multi>
    typename qmj::enable_if_t<!is_unordered, bool>
    is_equal(const STD_container &left, const QMJ_container &right)
    {
        return base_type::is_equal();
    }

    template <bool is_unordered = is_unordered_type::value,
              bool multi = is_multi>
    typename qmj::enable_if_t<is_unordered & !is_multi, bool>
    is_equal(const STD_container &left, const QMJ_container &right)
    {
        if (left.size() != right.size())
            return false;
        for (const value_type &val : left)
            if (right.find(get_key(val)) == right.end())
                return false;
        return true;
    }

    template <bool is_unordered = is_unordered_type::value,
              bool multi = is_multi>
    typename qmj::enable_if_t<is_unordered && multi, bool>
    is_equal(const STD_container &left, const QMJ_container &right)
    {
        return base_type::is_euqal_special();
    }

    bool is_equal()
    {
        return is_equal(this->std_con, this->qmj_con);
    }

    bool load_data()
    {
        if (!this->reset_data(TEST_DATASIZE))
            return false;
        try
        {
            this->std_con.insert(this->data.begin(), this->data.end());
            this->qmj_con.insert(this->data.begin(), this->data.end());
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    void test_assign()
    {
        ASSERT_TRUE(this->reset_data(TEST_DATASIZE)) << "reset data error";
        STD_container std_assign(this->data.begin(), this->data.end());
        QMJ_container qmj_assign(this->data.begin(), this->data.end());
        this->std_con = std_assign;
        this->qmj_con = qmj_assign;
        EXPECT_TRUE(this->is_equal()) << "qmj::container is not euqal to std::container after assign";
    }

    void test_insert()
    {
        this->std_con.insert(this->data.begin(), this->data.end());
        this->std_con.insert(this->data.begin(), this->data.end());
        this->qmj_con.insert(this->data.begin(), this->data.end());
        this->qmj_con.insert(this->data.begin(), this->data.end());
        EXPECT_TRUE(this->is_equal()) << "not equal after insert";
    }

    template <bool is_map = is_map_type::value>
    qmj::enable_if_t<!is_map, const key_type>
    get_key(const value_type &val) const
    {
        return val;
    }

    template <bool is_map = is_map_type::value>
    qmj::enable_if_t<is_map, const key_type>
    get_key(const value_type &val) const
    {
        return val.first;
    }

    void test_count()
    {
        for (const value_type &val : this->data)
        {
            ASSERT_EQ(this->std_con.count(get_key(val)), this->qmj_con.count(get_key(val)))
                << "count not equlal ";
        }
    }

    void test_find()
    {
        for (const value_type &val : this->data)
        {
            auto std_find = this->std_con.find(get_key(val));
            auto qmj_find = this->qmj_con.find(get_key(val));
            if (std_find == this->std_con.end())
                ASSERT_EQ(qmj_find, this->qmj_con.end())
                    << "find not equlal ";
            else
                ASSERT_EQ(*std_find, *qmj_find)
                    << "find not equlal ";
        }
    }

    template <bool multi = is_multi>
    typename qmj::enable_if_t<multi, void>
    test_erase()
    {
        for (size_t i = 0; i != this->data.size() / 3; ++i)
        {
            size_t std_count = this->std_con.erase(get_key(this->data[i]));
            size_t qmj_count = this->qmj_con.erase(get_key(this->data[i]));
            ASSERT_EQ(std_count, qmj_count) << "count not equal in erase";
        }
    }

    template <bool multi = is_multi>
    typename qmj::enable_if_t<!multi, void> test_erase()
    {
        for (size_t i = 0; i != this->data.size() / 3; ++i)
        {
            size_t std_count = this->std_con.erase(get_key(this->data[i]));
            size_t qmj_count = this->qmj_con.erase(get_key(this->data[i]));
            ASSERT_EQ(std_count, qmj_count) << "count not equal in erase";
        }

        for (size_t i = this->data.size() / 3; i != this->data.size() / 2; ++i)
        {
            auto std_pos = this->std_con.find(get_key(this->data[i]));
            auto qmj_pos = this->qmj_con.find(get_key(this->data[i]));
            if (std_pos != this->std_con.end())
                this->std_con.erase(std_pos);
            if (qmj_pos != this->qmj_con.end())
                this->qmj_con.erase(qmj_pos);
        }

        EXPECT_TRUE(this->is_equal()) << "not equal after erase";
    }

    void test_emplace_hint()
    {
        for (size_t i = 0; i != this->data.size() / 4; ++i)
        {
            this->std_con.emplace_hint(advance(
                                           this->std_con.begin(), i),
                                       this->data[i]);
            this->qmj_con.emplace_hint(advance(
                                           this->qmj_con.begin(), i),
                                       this->data[i]);
        }

        EXPECT_TRUE(this->is_equal()) << "not equal after emplace_hint";
    }

    void test_equal_range()
    {
        test_insert();
        for (size_t i = 0; i != this->data.size(); ++i)
        {
            auto std_range = this->std_con.equal_range(get_key(this->data[i]));
            auto qmj_range = this->qmj_con.equal_range(get_key(this->data[i]));
            ASSERT_EQ(qmj::distance(std_range.first, std_range.second),
                      qmj::distance(qmj_range.first, qmj_range.second))
                << "not equal in equal range";
        }
    }
};

template <typename STD_container, typename QMJ_container>
class Test_stack_queue_base : public Test_data<STD_container, QMJ_container>
{
  public:
    typedef Test_data<STD_container, QMJ_container> base_type;
    typedef typename base_type::value_type value_type;

    template <typename Container, typename = void, typename = qmj::void_t<decltype(std::declval<Container>().top())>>
    const value_type &get_value(const Container &con)
    {
        return con.top();
    }

    template <typename Container, typename = qmj::void_t<decltype(std::declval<Container>().front())>>
    const value_type &get_value(const Container &con)
    {
        return con.front();
    }

    bool load_data()
    {
        return (this->reset_data(TEST_DATASIZE) && test_assign());
    }

    bool is_equal()
    {
        if (this->std_con.size() != this->qmj_con.size())
            return false;
        STD_container std_cp(this->std_con);
        QMJ_container qmj_cp(this->qmj_con);
        for (size_t i = 0; i != std_cp.size(); ++i)
        {
            if (get_value(std_cp) != get_value(qmj_cp))
                return false;
            std_cp.pop();
            qmj_cp.pop();
        }
        return true;
    }

    bool test_assign()
    {
        STD_container std_assign;
        QMJ_container qmj_assign;
        for (const value_type &val : this->data)
        {
            std_assign.push(val);
            qmj_assign.push(val);
        }
        this->std_con = std_assign;
        this->qmj_con = qmj_assign;
        bool result = is_equal();
        EXPECT_TRUE(result) << "not equal after assign";
        return result;
    }

    void test_push()
    {
        for (const value_type &val : this->data)
        {
            this->std_con.push(val);
            this->qmj_con.push(val);
        }
        EXPECT_TRUE(is_equal()) << "not eaul after push";
    }

    void test_pop()
    {
        for (size_t i = 0; i != this->std_con.size() / 3; ++i)
        {
            this->std_con.pop();
            this->qmj_con.pop();
        }

        EXPECT_TRUE(is_equal()) << "not eaul after pop";
    }

    void test_top()
    {
        EXPECT_EQ(this->std_con.top(), this->qmj_con.top());
    }
};

} // namespace test
} // namespace qmj

#endif //_TEST_CREATE_DATA_