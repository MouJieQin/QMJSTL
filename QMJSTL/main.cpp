#include <iostream>
#include <vector>
//#include<utility>
#include "gtest/gtest.h"
#include "vector_qmj.h"
#include "deque_qmj.h"
//#include"map_qmj.h"

using namespace std;

int main()
{

    qmj::deque<int> deq{1, 2, 3, 4};
    deq.push_back(10);
    for (int i : deq)
        cout << i << " ";
    cout << endl;
}
