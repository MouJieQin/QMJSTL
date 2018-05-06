#pragma once
#ifndef _TEST_
#define _TEST_
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct item_time {
  item_time(string name, time_t std, time_t qmj, int num, bool correct = true)
      : name(name), std_time(std), qmj_time(qmj), num(num), correct(correct) {}
  string name;
  time_t std_time;
  time_t qmj_time;
  int num;
  bool correct;
};

int integer_rand(int beg, int end) {
  return static_cast<int>((rand() / static_cast<double>(RAND_MAX)) * (INT_MAX) +
                          INT_MIN / 2 + 1);
}

double double_rand(double beg, double end) {
  return rand() / static_cast<double>(RAND_MAX) * (end - beg) + beg;
}

// time_t start, finished,time_s,total_time_std=0,total_time_qmj=0;
// std::vector<item_time>vt_time;

#define random_data_product(data_size, vt_data, insert_data)      \
  \
{                                                            \
    \
time_t start,                                                     \
        finished, time_s, total_time_std = 0, total_time_qmj = 0; \
    \
std::vector<item_time>                                            \
        vt_time;                                                  \
    \
srand(static_cast<unsigned int>(time(nullptr)));                  \
    \
for(auto i                                                        \
        : data_size) \
{                                         \
      std::vector<int> vt_data;                                   \
      vt_data.reserve(i);                                         \
      std::vector<int> insert_data;                               \
      insert_data.reserve(i);                                     \
      for (int j = 1; j <= i; ++j) {                              \
        vt_data.push_back(integer_rand(-i, i));                   \
        insert_data.push_back(integer_rand(-i, i));               \
      }

#define time_counter(name, std, qmj, num) \
  \
start = clock();                          \
  \
std \
finished = clock();                       \
  \
time_s = finished - start;                \
  \
start = clock();                          \
  \
qmj \
finished = clock();                       \
  \
vt_time.push_back({name, time_s, finished - start, num});

#define print_time(item, first_name, second_name)                              \
  }                                                                            \
  \
cout << "["                                                                    \
     << item << "] test :" << endl;                                            \
  \
for(size_t i = 0; i < vt_time.size(); ++i) \
{                                \
    total_time_std += vt_time[i].std_time;                                     \
    total_time_qmj += vt_time[i].qmj_time;                                     \
    cout << "[" << vt_time[i].name << "]\tdataSize: " << vt_time[i].num        \
         << "\t";                                                              \
    cout << first_name << ": " << vt_time[i].std_time << "ms\t" << second_name \
         << ": " << vt_time[i].qmj_time << "ms"                                \
         << "\t";                                                              \
    cout << "faster: " << vt_time[i].std_time - vt_time[i].qmj_time << "ms";   \
    if (vt_time[i].std_time) {                                                 \
      cout << "\t速度提高： "                                                  \
           << (vt_time[i].std_time - vt_time[i].qmj_time) /                    \
                  static_cast<double>(vt_time[i].std_time) * 100               \
           << "%";                                                             \
    }                                                                          \
    cout << '\n' << endl;                                                      \
  \
}                                                                         \
  \
cout << first_name                                                             \
     << " total time :" << total_time_std << "ms" << endl;                     \
  \
cout << second_name                                                            \
     << " total time :" << total_time_qmj << "ms\n\n"                          \
     << endl;                                                                  \
  \
if(total_time_std) \
{                                                        \
    cout << "速度提高： "                                                      \
         << (total_time_std - total_time_qmj) /                                \
                static_cast<double>(total_time_std) * 100                      \
         << "%\n----------------" << endl;                                     \
  \
}                                                                         \
  \
}

#endif
