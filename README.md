# QMJSTL
用C++11实现的STL标准库，容器和算法包含但不限于STL
容器实现了几乎所有标准接口,无异常处理

**代码测试环境: vs2015,release,O2优化,64位,i3处理器**

以下为一些数据结构的设计,和某些算法的数学证明
(主要是 ***<<stl源码剖析>>*** 没有写出来的).
未特殊说明,下文中`std/stl`全部指代`vs2015`自带的标准库

## 内存池
分配器在`allocator.h`中,第一级配置器使用`malloc-free`作为分配
释放器.第二级分配器使用内存池,对于请求大于128字节的,第二级分配器
直接调用第一级分配器.这两级配置器的程序照搬自<<stl源码剖析>>的
SGI的某一版实现.

### 优点:使用内存池后能显著提高某些容器的速度,如链表.
### 缺点:该内存池对于小于128 bytes的内存并不归还给系统,
总是保留内存高峰期的使用内存.像一个体温计.

针对这一点该头文件提供两个分配器,一个是qmj容器的默认内存分配器`allocator`,使用二级分配器.
另一个是`simple_allocator`使用一级分配器.这两个分配器都**不符合标准**.
	
qmj::各容器虽然都提供有自定义内存分配器模板参数,但不提供接受分配器的
构造函数,所以不支持动态改变容器的分配器.即使在编译器提供分配器也要
符合allocator_base的接口.

## 红黑树
	
![rb_tree](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/RB_tree%20for%20map.png)

为`qmj::map`和`qmj::set`基类,在文件`rb_tree.h`中,树中所有叶节点和
少子节点(没有左节点或右节点)的空闲指针以及根节点的父节点都指向
名为`NIL`的黑色节点.

1.在插入和删除中始终维护`NIL`的父节点的为空,以作为`NIL`的标识,左节点
指向树中的最大节点,右节点指向最小节点,这可让迭代器递增递减算法
更加简洁高效.

### 维护这一算法的复杂度分析:
由于极值节点都在`NIL`中,`rbt::begin`,`rbt::end()`可为常数时间,虽然
没有增加每次插入删除的复杂度,但会增加`rbt::insert`,`rbt::erase`等
的常数.对比通过`rbt::minimum(root)`,`rbt::maximum(root)`复杂度`O(lgn)`,
来获得最小最大节点来说.如果经常进行插入删除操作,而较少使用迭代器
可能有点得不偿失.但经过测试,`qmj::rbt::insert`,`qnj::rbt::erase`还是比
`std`快.实际上经过测试发现对`qmj::rbt`的遍历并没有提高多少速度,原因在于
根据`Amdahl`定律,相对于`O(n)`复杂度的遍历,`O(l)`的迭代器提取所占的总体
百分比太少.所以我的算法反而复杂了代码.

![set](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/set.png)
	
***对1000000个无重复随机int整数的set对比测试***
	
![multi data](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/set%20multiData.png)
	
***对1000000个随机int整数的set对比测试,其中无重复数据32768个***
	
![multiset](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/multiset.png)

***对1000000个随机有重复整数的multiset对比测试,(图中set test 标题有误)***

2.`qmj::rbt::find`比`std::rbt::find`更快的原因在于`std`使用了
`std::rbt::lower_bound`作为查找内部函数.该操作可能进行额外的递增
/递减操作.
	
## 哈希表

![hashtable](https://github.com/MouJieQin/QMJSTL/blob/master/image/hashtable/hashtable.png)

***图中的桶是连续内存***

`qmj::unordered_map`和`qmj::unordered_set`的基类,在文件`hashtable.h`中.
对于`qmj::hashtable::equal_range(key)`,该函数返回表中关键字`key`所在
的区间迭代器,我使用的算法是在调用该算法时才改变内部结构,让在一个桶中
具有关键字`key`的节点连在一起.这样做的优点是加速插入,缺点是如果多次
调用该算法,该算法每次都会遍历该关键字所在桶中的每一个节点.
例如比对两个允许相同关键字的`hashtable`内的元素是否相等时就会多次调用
`equal_range`.

![unordered_set](https://github.com/MouJieQin/QMJSTL/blob/master/image/hashtable/unordered_set.png)
	
***以上测试unordered_set无重复数据***
	
![unordered_set multi data](https://github.com/MouJieQin/QMJSTL/blob/master/image/hashtable/unordered_set%20multiData.png)
	
***以上测试unordered_set有3/4重复数据***
	
![unordered_multiset](https://github.com/MouJieQin/QMJSTL/blob/master/image/hashtable/unordered_set.png)
	
***以上测试unordered_multiset***
	
`qmj::unorder_multi(set/map)::operator==` 和`access`(遍历)慢很多,`operator==`慢
一是因为遍历慢,二是因为`equal_range`每次都要遍历桶中全部元素.
`operator==`使用了`qmj::is_permutation`,该算法确定第一个区间
是否是第二个区间的排列组合,复杂度`O(n^2)`.隐藏在其中的常数非常大,
如果直接使用该算法是没有用处的.但是`hashtable::equal_range`是常数时间,
所以`operator==`对两个`hashtable::equal_range`里的元素区间进行比较,
一般桶里的元素数目为常数,所以整个算法复杂度为`O(n)`.

### 改进遍历速度的方法

维护一个节点指针`head`,初始化为`nullptr`,每当一个空桶插入元素,就让新插入
的元素的`next`指针指向`head`,然后让`head`指向新插入的元素.删除时如果一个桶
变为空,更新`head`指向删除元素的`next`.

这样各个节点将会变成单向链表,如图
	
![hashtable_qmj](https://github.com/MouJieQin/QMJSTL/blob/master/image/hashtable/qmj_hashtable.png)
	
但这会造成一个问题,无法直接判断一个桶中的`local_iterator`是否走到尽头,
对于`equal_range`将造成阻碍.

1.直接对当前迭代器所指元素调用hash函数来判断元素是否在目标桶中,
但hash函数开销比较大.

2.添加哨兵节点,`next==nullptr`,`next_bucket`指向了下一个桶,如图
	
![hashtable1_qmj](https://github.com/MouJieQin/QMJSTL/blob/master/image/hashtable/qmj_hashtable1.png)
	
3.令每个桶存储一个结构体而不是一个指针,其中包含节点指针和桶元素计数器,无图.

文件中的`qmj::hashtable`并未使用上述3个算法
	
## vector

在文件`vector_qmj.h`中

![vector](https://github.com/MouJieQin/QMJSTL/blob/master/image/vector/vector.png)

比`std::vector`快的原因在于`std::vector`使用`new`对于预分配的内存
会全部进行构造,而`qmj::vector`只构造`qmj::vector::size`大小的内存

## list/forward_list/slist

1.`slist`不是单向链表,`list`继承自`slist`,不同之处在于`slist::splice`
复杂度`O(1)`,`slist::size()`复杂度`O(n)`.而`qmj::list::splice`复杂度`O(n)`,
`qmj::list::size`复杂度`O(1)`.

2.如果使用一级配置器,链表类容器`qmj`相对于`std`没有明显效率改善,使用内存池
分配器后,效率有质的改变.如图

![forward_list](https://github.com/MouJieQin/QMJSTL/blob/master/image/list/forward_list.png)

![list](https://github.com/MouJieQin/QMJSTL/blob/master/image/list/list.png)

3.`list/slist`使用回溯归并排序`iterator merge_sort(first,last,len,cmp)`,
返回值为排好序区间的首迭代器.`forward_list`也使用了归并排序
	
## deque

![std::deque](https://github.com/MouJieQin/QMJSTL/blob/master/image/deque/std__deque.png)

`std::deque`架构较为复杂,其中`map`是指向`block`(区块)的映射表,也就是指针的指针,
根据单个`value_type`的数据大小来指定区块所存元素个数.当需要在首尾插入数据时,就将
`map`的首尾指针指向区块地址.

![qmj::deque](https://github.com/MouJieQin/QMJSTL/blob/master/image/deque/qmj__deuqe.png)

`qmj::deque`限定`map`的指针映射一个数据元素而不是一个区块,如果需要在中间插入元素
`qmj::deque`能获得更高的性能,因为`qmj::deque`只需要使用`memcpy`移动`map`中的指针,
而`std::deque`需要对元素进行赋值之外,迭代器的遍历涉及的控制流和计算也会拖慢速度.
对比`qmj::deque`和`std::deque`.

1.当单个元素的大小较大时,如果`qmj::deque`不使用内存池配置器,`qmj::deque`在首尾插入
元素没有明显提高,但如果使用内存池分配器,速度会快上很多.

![deque-not pod](https://github.com/MouJieQin/QMJSTL/blob/master/image/deque/deque-not%20pod.png)
***deque 非POD数据测试***

2.当元素较小时,如果`qmj::deque`不使用内存池配置器,为一个较小的元素单独分配和释放内存,
花费的开销太大,速度比`std::deque`慢上很多.(使用内存池配置器后又会快上很多)

3.在中间插入和删除元素或者排序,`qmj::deque`要比`std::deque`快.对于排序,特意为
`qmj::deque_iterator`设计了`iter_swap`,因为`qmj::deque`只用交换`map`中的指针.

当元素大小较小时,不使用内存池分配器的`qmj::deque`处于劣势,所以`qmj::deque`采用了双层架构,
当元素大小较小或为`POD`类型时的内存布局采用类似`qmj::vector`布局,直接使用了`qmj::vector`的迭代器.
数据移动直接使用`memcpy`,与`qmj::vector`的不同之处只在`qmj::deque`在预分配内存的中间开始使用而不是头部.

![deque-int(POD)](https://github.com/MouJieQin/QMJSTL/blob/master/image/deque/deque-%20int.png)
***deque int数据类型测试***

对于`qmj::deque`的使用和`std::deque`一致,`qmj::deque`会自动检测,如果元素小于一个指针的大小
或者该元素为`POD`类型并且小于10个指针的大小时会自动调用`qmj::vector`模型的`deque`.
怎么检测在后面的`type_traits`会说明.但对于数据较大的`POD`类型数据使用`qmj::vector`排序会
较慢.使用`qmj::deque<value_type,Alloc,value_type>`模式会强制调用映射模型的`qmj::deque`.


## priority_queue/fib_heap

`binary_heap/fib_heap`在文件`heap.h`中,`qmj::priority_queue`在文件`queue_qmj.h`中

### binary_heap

`std::priority_queue`包含一个容器,主体算法都来自`algorithm.std::pop_heap`,
`std::push_heap`.我也写了同名算法,不过在`qmj::binary_heap`里重新写了一遍.
与`std`的不同之处在于`pop_heap`算法.

`std::pop_heap`算法将`top(root)`元素移出后,通过迭代下溯从子树元素提取元素填补洞口,
最后在叶子节点会空出一个洞口,然后以该洞口洞号开始为尾端元素上溯使用`std::push_heap`
找到合适洞口.

`qmj::pop_heap`算法使用的`heapify`(堆维护)算法则直接在下溯中为尾端元素找到洞口,
而没有再使用一次`push_heap`上溯.但跌代体中会多出一个控制流.

`pop_heap`算法分析,因为尾端元素原本就是叶节点,所以它的优先权重较小,更有可能就在底层
找到洞口,而且由于二叉堆是满二叉树,大部分数据就在底层(2^n-1个元素的满二叉树的叶节点
占总体元素一半),所以综合来看`std::pop_heap`堆更合理.

![priority_queue](https://github.com/MouJieQin/QMJSTL/blob/master/image/heap/priority_queue.png)

经过测试,对于随机数据,两者速度没有明显差异.但对于具有较多重复
元素的数据,`qmj::priority_queue`(继承自`qmj::binary_heap`)快10%左右.

对于随机数据`qmj::make_heap比std::make_heap`快20%,对于完全重复或降序数据快80%.
增序无较大差异.

![make_heap](https://github.com/MouJieQin/QMJSTL/blob/master/image/heap/make_heap.png)

对于随机数据`qmj::sort_heap`和`std::sort_heap`无明显速度差异,对于有序数据(增序/降序)
`qmj::sort_heap`要快,而对于重复数据.如图

![heap_sort](https://github.com/MouJieQin/QMJSTL/blob/master/image/heap/heap_sort.png)

对重复或增序数据较快的原因在于改进的`heapify`(堆维护算法)复杂对变成了`O(1)`

### fib_heap

斐波那契堆主要用于优化后面的图算法,和`binary_heap`相比,隐藏在其中的常数非常大.
是其9倍左右.
	
![fib_heap](https://github.com/MouJieQin/QMJSTL/blob/master/image/heap/fib_heap.png)


## algorithm

### power

`qmj::power(x,n,fn2)`在文件`numeric_qmj.h`中,是非标准算法.
该算法返回对`x`进行`fn2`操作的`n`幂次方,`fn2`要满足结合律而
不需满足交换律,其中`n`必须是非负整数,复杂度`O(lg(n))`.

		`template<typename value_type,
		typename UnsignInteger,
		typename Fn2>
		value_type power(value_type x, UnsignInteger n, const Fn2& fn2)
	{//N=(2^a)*b
		if (n <= 0)
			return (x);
		for (; !(n & 1);)
		{//循环a次
			n >>= 1;
			x = fn2(x, x);
		}
		value_type result = x;//result=X^(2a),n==b
		for (n >>= 1; n != 0; n >>= 1)
		{
			x = fn2(x, x);
			if (n & 1)
				result = fn2(result, x);
		}
		return (result);
	}`
	
![power](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/power.gif)


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
