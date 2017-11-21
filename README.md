# QMJSTL
用C++11实现的STL标准库，容器和算法包含但不限于STL

	**以下为一些数据结构的设计,和某些算法的数学证明(主要是<<stl源码剖析>>没有写出来的).
	代码测试环境: vs2015,O2优化,64位,i3处理器,未特殊说明,下文中std/stl全部指代
	vs2015自带的标准库**

***内存池***:

	分配器在'allocator.h'中,第一级配置器使用malloc-free作为分配
	释放器.第二级分配器使用内存池,对于请求大于128字节的,第二级分配器
	直接调用第一级分配器.这两级配置器的程序照搬自<<stl源码剖析>>的
	SGI的某一版实现.
	
	优点:使用内存池后能显著提高某些容器的速度,如链表.
	缺点:该内存池对于小于128 bytes的内存并不归还给系统,
	总是保留内存高峰期的使用内存.像一个体温计.针对这一点该头文件
	提供两个分配器,一个是qmj容器的默认内存分配器allocator,使用二级分配器.
	另一个是simple_allocator使用一级分配器.这两个分配器都**不符合标准**.
	
	qmj::各容器虽然都提供有自定义内存分配器模板参数,但不提供接受分配器的
	构造函数,所以不支持动态改变容器的分配器.即使在编译器提供分配器也要
	符合allocator_base的接口.

***红黑树***:

	![rb_tree](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/RB_tree%20for%20map.png)

	为qmj::map和qmj::set基类,在文件'rb_tree.h'中,树中所有叶节点和
	少子节点(没有左节点或右节点)的空闲指针以及根节点的父节点都指向
	名为'NIL'的黑色节点.
	
	1.在插入和删除中始终维护NIL的父节点的为空,以作为NIL的标识,左节点
	指向树中的最大节点,右节点指向最小节点,这可让迭代器递增递减算法
	更加简洁高效.
	
	维护这一算法的复杂度分析:
	由于极值节点都在NIL中,rbt::begin(),rbt::end()可为常数时间,虽然
	没有增加每次插入删除的复杂度,但会增加rbt::insert(),rbt::erase()等
	的常数.对比通过rbt::minimum(root),rbt::maximum(root)复杂度O(lgn),
	来获得最小最大节点来说.如果经常进行插入删除操作,而较少使用迭代器
	可能有点得不偿失.但经过测试,qmj::rbt::insert,qnj::rbt::erase还是比
	std快.实际上经过测试发现对qmj::rbt的遍历并没有提高多少速度,原因在于
	根据Amdahl定律,相对于O(n)复杂度的遍历,O(l)的迭代器提取所占的总体
	百分比太少.所以我的算法反而复杂了代码.
	
	![set](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/set.png)
	
	对1000000个无重复随机int整数的set对比测试
	
	![multi data](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/set%20multiData.png)
	
	对1000000个随机int整数的set对比测试,其中无重复数据32768个
	
	![multiset](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/multiset.png)
	
	对1000000个随机有重复整数的multiset对比测试,(图中set test 标题有误)
	
	2.qmj::rbt::find()比std::rbt::find()更快的原因在于std使用了
	std::rbt::lower_bound作为查找内部函数.该操作可能进行额外的递增
	/递减操作.
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
