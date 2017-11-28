# QMJSTL
用C++11实现的STL标准库，容器和算法包含但不限于STL
容器实现了几乎所有标准接口,无异常处理

**代码测试环境: vs2015,release,O2优化,64位,i3处理器**

以下为一些数据结构的设计,和某些算法的数学证明
(主要是 ***<<stl源码剖析>>*** 没有写出来的).
未特殊说明,下文中`std/stl`全部指代`vs2015`自带的标准库

**=============转载请注明出处=============**

## 内存池

[allocator](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/allocator.h)

第一级配置器使用`malloc-free`作为分配
释放器.第二级分配器使用内存池,对于大于128字节的请求,第二级分配器
直接调用第一级分配器.这两级配置器的程序照搬自<<stl源码剖析>>的
SGI的某一版实现.

#### 优点:使用内存池后能显著提高某些容器的速度,如链表.
#### 缺点:该内存池对于小于128 bytes的内存并不归还给系统,总是保留内存高峰期的使用内存.像一个体温计.

针对这一点该头文件提供两个分配器,一个是qmj容器的默认内存分配器`allocator`,使用二级分配器.
另一个是`simple_allocator`使用一级分配器.这两个分配器都**不符合标准**.
	
qmj::各容器虽然都提供有自定义内存分配器模板参数,但不提供接受分配器的
构造函数,所以不支持动态改变容器的分配器.即使在编译器提供分配器也要
符合allocator_base的接口.

## 红黑树
	
![rb_tree](https://github.com/MouJieQin/QMJSTL/blob/master/image/RB_tree/RB_tree%20for%20map.png)

[rb_tree.h](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/rb_tree.h)

为`qmj::map`和`qmj::set`基类,树中所有叶节点和少子节点
(没有左节点或右节点)的空闲指针以及根节点的父节点都指向名为`NIL`的黑色节点.

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

[hashtable.h](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/hashtable.h)

`qmj::unordered_map`和`qmj::unordered_set`的基类,
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
是否是第二个区间的排列,复杂度`O(n^2)`.隐藏在其中的常数非常大,
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

## AVL_tree

[AVl_tree](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/AVL_tree.h)

`qmj::avl_tree` 将`AVL_tree`的递归回溯插入法优化成循环上溯插入法.`qmj::avl_tree`的插入和删除采用了
和`qmj::rb_tree`相同的模式,先完成插入(删除),然后上溯维护树高.

### `qmj::avl_tree::insert` 优化点:
首先完成节点插入,上溯修复过程在不改变当前节点的高度或完成旋转后退出而不是上溯到根节点.
容易证明旋转后近根节点的高度不会改变其父节点的高度.其`insert`和`delete`操作中旋转操作中的
节点改变操作不同.`insert`的旋转操作更加细化,不证明.

### `qmj::avl_tree::erase`/(亦即`delete`)优化点:
在完成旋转操作或不改变当前节点高度后退出.由于删除操作是我思考出来的,所以主要讲删除操作.

和删除普通二叉搜索树的节点类似,`qmj::avl_tree::erase`面对四种情况.

![avl_tree delete](https://github.com/MouJieQin/QMJSTL/blob/master/image/avl_tree/avl_tree.png)

第一二种情况比较简单,如果要删除的节点的只有一个子节点,就用这个子节点代替它,关键节点为其
父节点,所谓关键节点就是删除节点后可能改变高度的第一个节点.对于没有子节点的节点可以归为
第一种情况中.

对于有左右子树的节点,取其右子树中的最小节点代替它.此时分为三四种情况.
对于右子树中最小节点为其右节点视为情况三.让其右节点代替它.与第一二种
情况的不同之处在于此时关键节点不是删除节点的父节点而是其右节点.

第四种情况较为复杂.其右子树中的最小节点`right_min`不是其右节点.首先让`right_min`的右节点代替
`right_min`,然后让`right_min`代替要删除的节点,此时关键节点为right_min的父节点,注意是`right_min`
的父节点而不一定是要删除节点的右节点.

![avl_tree](https://github.com/MouJieQin/QMJSTL/blob/master/image/avl_tree/avl_tree%20random.png)

`qmj::avl_tree`能够达到和`qmj::multiset`相同的性能,当然,`qmj::avl_tree`没有封装带来的间接性.

## splay_tree

[splay_tree](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/splay_tree.h)

splay_tree适用于需要多次查找最近查找过得元素,因为每次find后会将最接近查找元素的树
中元素上升到根节点,平摊时间为O(lgn).splay_tree占用的内存较小.和双向链表相当.由于不能
保证是平衡树,所以遍历是使用递归可能栈溢出,特别是数据量较大时.我写了一个"一次性"遍历
算法,因为会破坏树的结构,但因为只是用在析构上,所以是否一次性无所谓.具体思想是将栈结构
组织到树上.

	
## vector

[vector](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/vector_qmj.h)

![vector](https://github.com/MouJieQin/QMJSTL/blob/master/image/vector/vector.png)

比`std::vector`快的原因在于`std::vector`使用`new`对于预分配的内存
会全部进行构造,而`qmj::vector`只构造`qmj::vector::size`大小的内存.
算法复杂度,摊还时间为`O(1)`.

## list/forward_list/slist

[list/forward_list/slist](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/list_qmj.h)

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

[deque](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/deque_qmj.h)

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

[binary_heap/fib_heap](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/heap.h)

[queue](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/queue_qmj.h)

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

对重复或增序数据较快的原因在于改进的`heapify`(堆维护算法)复杂度变成了`O(1)`

### fib_heap

斐波那契堆主要用于优化后面的图算法,和`binary_heap`相比,传入小于比较符是最小堆,
隐藏在其中的常数非常大.是其9倍左右.
	
![fib_heap](https://github.com/MouJieQin/QMJSTL/blob/master/image/heap/fib_heap.png)


## algorithm

[algorithm](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/algorithm_qmj.h)

`qmj`没有完成的函数有`equal`,`copy`,`copy_backward`,`fill`等,
原因在于看了`std`源码以后发现这些算法已经优化到了极致,
重写一遍还要造很多型别判定的轮子,可能后面会补上.

### power

[numeric](https://github.com/MouJieQin/QMJSTL/blob/master/QMJSTL/numeric_qmj.h)

`qmj::power(x,n,fn2)`是非标准算法.
该算法返回对`x`进行`fn2`操作的`n`幂次方,`fn2`要满足结合律而
不需满足交换律,其中`n`必须是非负整数,复杂度`O(lg(n))`.

	template<typename value_type,
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
	}
	
**证明假设fn2为乘法操作,其伪代码循环体中的if语句下面的语句应有一个缩进**
	
![power](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/power.gif)

### iter_swap

qmj::iter_swap在文件algorithm.h中.
将两个迭代所指的对象对调,不同于std::iter_swap,
qmj::iter_swap首先判定两个迭代是否是同类类型,
不是同类型直接调用std::iter_swap,是同类类型然后判断
迭代器是否有自定义的成员函数iter_swap,如果没有调用
std,否则调用成员函数.

### rotate

	Assume:
	void mySwap(first1,last1,first2,last2)
	{//_QMJ distance(first1,last1)=_QMJ distance(first2,last2)
		for(;first!=last1;++firs1,++first2)
			_QMJ iter_swap(first1,first2);
	}

	len1=_QMJ distance(first,mid);
	len2=_QMJ distance(mid,last);

#### forward_iterator

![rotate FIter](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/rotate_FIter.png)

	template<typename FIter>inline
		void _rotate_imple(FIter first, FIter middle,
			FIter last, std::forward_iterator_tag)
	{
		for (FIter mid = middle;;)
		{
			_QMJ iter_swap(first++, mid++);
			if (first == middle)
			{
				if (mid == last)
					return;
				middle = mid;
			}
			else if (mid == last)
				mid = middle;
		}
	}

##### 证明:

1.`len1=len2`
`mySwap(first,mid,mid,last)`
完成

2.`len1<len2`
`rotate(first,mid,last)=mySwap(first,mid,mid,mid+len1)+rotate(mid,mid+len1,last)`

3.`len1>len2`
`rotate(first,mid,last)=mySwap(first,first+len2,mid,last)+rotate(first+len2,mid,last)`

##### 复杂度分析:

花费为`mySwap`调用的次数和区间的长度的乘积,
容易证明每次区间的长度为非递增状态,
因为每次调用两个区间的的较短区间长度,然后较长区间要减去
较小区间的长度.
`0<α<1/2`,假设整个区间长度为`n`,某个区间的长度为`αn`,
最多调用`n/(αn)`次,每次调用最多花费αn,复杂度为`n/(αn)*αn=O(n)`.
	
#### bidirectional_iterator

![rotate BIter](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/rotate%20BIter.png)	
	
	template<typename BIter>inline
		void _rotate_imple(BIter first, BIter middle,
			BIter last, std::bidirectional_iterator_tag)
	{
		_QMJ reverse(first, middle);
		_QMJ reverse(middle, last);
		_QMJ reverse(first, last);
	}
	
先完成两个区间的反转,最后对整个区间反转

##### 证明:

1.`len1=len2`
`mySwap(first,mid,mid,last)`
完成

2.`len1<len2`
`rotate(first,mid,last)=mySwap(first,mid,mid,mid+len1)+reverse(mid,last-len1)`

3.`len1>len2`
`rotate(first,mid,last)=mySwap(first,first+len2,mid,last)+reverse(first+len2,mid)`

##### 复杂度:

`O(n)`

对于随机存储迭代器,由于测试 ***<<stl源码剖析>>*** 的速度没有直接使用
双向迭代器的快,所以不证明.
	
### next_permutation/prev_permutation

求序列的下一个(上一个)排列.

> 首先,从最尾端开始往前寻找两个相邻元素,令第一元素为`*i`,第二元素为`*ii`,且
> 满足`*i<*ii`.找到这样一组相邻元素后,再从尾端开始往前检验,找出第一个大于
> `*i`的元素,令为`*j`,将`i,j`元素对调,再将`ii`之后的所有元素颠倒排列,此即所求之
> "下一个"排列组合.
>                              -------- <<stl源码剖析>>
	
![next_permutation](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/next_permutation.png)

#### 证明:

假设一个序列的每一个元素是非递减的,它的最后一个排列一定是非递增的.
这两者之间的排列一定是严格递增的

	0 1 2 3 4	0 1 2 4 3	0 1 3 2 4	0 1 3 4 2	...	4 3 2 1 0

所谓下一个排列亦即重新排列元素,并使整个序列表示的数字增长最小的排列.
要使元素增长最小自然从最低位开始向高位开始查找.
图中区间`S`中的元素由于是严格递减的(从左往右),不可能
重新排列这个区间的元素使序列变大.所以要替换`*i`.
从后向前找到的第一个不小于(对于有重复元素的序列)`*i`的元素`*j`.
根据单调性,`*j`一定是在区间`S`中最接近`*i`的.交换`*i`和`*j`后区间`S`仍然是
严格递减的.再将区间`S`颠倒便得到区间`S`所能组成的最小序列.

### random_shuffle

将接受的区间随机重排,也就是在N!中可能的元素序列中随机选择一种.
qmj并没有重新实现这一个函数.	

	template<typename RIter>inline
	void random_shuffle(RIter first,RIter last)
	{
		_QMJ iter_dif_t<RIter> len=last-first;
		if(len<2)
			return;
		for(_QMJ iter_dif_t<RIter> i=1;i!=len;++i)
			_QMJ iter_swap((first+i),first+(rand()%(i+1)));
	}

#### 证明:

只需证明`len`个元素在每个位置的概率都相同,为`1/len`.

循环不变式:在每次循环体开始之前,序列前`i`个元素在前`i`个位置任一位置的概率都为`1/i`.

初始时:i=1,第一个元素在第一个位置的概率为1/i=1,条件成立.

保持:第i次循环体结束后,前`i+1`个元素在前`i+1`个位置任一的概率都为`1/(i+1)`.
由初始条件可知,前i个元素在前i个位置中任一位置的概率为`1/i`.在循环体
结束后,前`i`个元素中每个元素还在原位置的概率为`(1/i)*(i/(i+1))=1/(i+1)`.
其中`i/(i+1)`为该元素未被选中交换的概率.前`i`个元素中每个元素在第`i+1`个位置的概率
和第`(i+1)`个元素在前`(i+1)`个位置中某个位置的概率都为`1/(i+1)`.
综上,前`i+1`个元素在前`i+1`个位置任一的概率都为`1/(i+1)`.

终止:`i=len`,此时`len`个元素在`len`个位置中任一位置的概率都为`1/len`.

### partial_sort

	template<typename RIter,
		typename Comp>inline
		void partial_sort(RIter first, RIter middle, RIter last,
			const Comp&cmp)
	{
		iter_dif_t<RIter> len = middle - first;
		if (!len)
			return;
		else if (len == 1)
		{
			_QMJ iter_swap(first, _QMJ min_element(first, last, cmp));
			return;
		}
		_QMJ make_heap(first, middle,cmp);
		for (RIter cur = middle; cur != last; ++cur)
			if (cmp(*cur, *first))
			{
				iter_val_t<RIter>val = std::move(*cur);
				*cur = std::move(*first);
				_QMJ _heapify(first, iter_dif_t<RIter>(0), len, std::move(val), cmp);
			}
		_QMJ sort_heap(first, middle,cmp);
	}
	
该函数使序列中的`middle-first`个最小元素以cmp制定的顺序排序置于`[first,middle)`内,
其余`last-middle`个元素安置于`[middle,last)`中,不保证有任何特定顺序.以下假设`cmp`为小于
比较符.
该函数首先对`[first,middle)`造堆,此时其中的最大元素位于`first`,然后遍历`[middle,last)`,
如果有元素小于`*first`,就交换两者,然后维护堆.最后调用堆排序对`[first,middle)`排序.
	
#### 证明:

以下假设`cmp`为小于比较符.

循环不变式:每次循环体开始之前,`*first`是`[first,cur)`个元素中第`middle-first`小的元素.
由堆的性质可知`[first,middle)`是`[first,cur)`中`middle-first`个最小的元素.

初始时:`make_heap(first,middle)`,条件显然成立.

保持:循环体结束后,`*first`是`[first,cur)`个元素中第`middle-first`小的元素.
由堆的性质`[first,middle)`是`[first,cur)`中`middle-first`个最小的元素.
如果`*cur>=*first`什么都不做,循环不变式得到保持.
否则交换`*first`,`*cur`,然后维护堆的性质.此时`*cur`将成为新的第`middle-first`小的元素.
`[first,middle)`是`[first,cur)`中`middle-first`个最小的元素.循环不变式得到保持.

终止:`cur=last`.`*first`是`[first,last)`个元素中第`middle-first`小的元素.
由堆的性质`[first,middle)`是`[first,last)`中`middle-first`个最小的元素.

#### 复杂度分析:

令`k=middle-first,n=last-first`,首先`make_heap`花费`O(k)`,最坏情况下后面循环体中每个元素
都比`*first`小,将花费`O((n-k)*lg(k))`来找出`k`个最小元素.最后`sort_heap`花费`O(k*lgn(k))`,
所以整个算法的复杂度为`O((n-k)*lg(k)+k*lgn(k)+k)=O(n*lg(k))`.

### sort

`qmj::sort` 采用了`introSort`,主体采用循环加递归的快速排序.如果递归恶化(深度过深)会采用
堆排序,如果序列长度低于阈值退出.最后对整个序列进行插入排序.以下是测试结果.

![sort random](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/sort%20random.png)

***随机不重复int数据***

![sort sorted](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/sort%20sorted.png)

***有序(增序/降序)不重复数据****

![sort multi](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/sort%20multi.png)

***完全重复数据***

`qmj::sort`在不重复数据比`std::sort`表现更好的一个原因在于

		template<typename RIter,
		typename Comp>inline
		RIter _mid_partition(RIter first, RIter last,
			const Comp&cmp)
	{
		iter_val_t<RIter> piovt = _get_piovt(
			*first, *(first + ((size_t)(last - first) >> 1)), *(last - 1), cmp);
		for (;; ++first)
		{
			for (; cmp(*first, piovt); ++first)
				;

			for (--last; cmp(piovt, *last); --last)
				;

			if (!(first < last))
				return (first);
			_QMJ iter_swap(first, last);
		}
		return (first);
	}
	
在两个比对`piovt`循环体中没有边界检查,节省了很多常数时间.但这也是`qmj:sort`在面对完全重复数据时
表现不如`std::sort`的原因.`std::sort`的`mid_partition`返回一个`pair`表示`piovt`的所在区间.当数据
完全重复时`std::sort`复杂度为`O(n)`.而`qmj::sort`的`mid_partition`要做很多无用的交换.
经过测试,即使序列有超过一半的重复的元素,但如果序列是随机排列的`qmj::sort`还是比`std::sort`
具有更好的性能.

`for (; cmp(*first, piovt); ++first)`不需要边界检查的原因在于`piovt`是区间中的一个元素.
该循环一定会在越界之前停下来.第二个循环也是如此.而后检查两个迭代器的位置,如果不满足
要求直接返回.而满足要求后会交换两个对象,一旦交换后就很显然不需要担心越界问题了.

#### 复杂度分析:

`qmj::sort`对递归深度有限制,一旦递归恶化(复杂度偏向`O(n^2)`)便调用堆排序.保证了复杂度`O(nlgn)`,
对于最后进行的一次插入排序,也可证明是`O(n)`的.原因在于原序列被划分成了不大于阈值的若干个区间,
每个区间的元素虽然不一定是有序的,但该区间与其他区间是独立的,该区间的元素是整个序列有序后该区间
元素的一个排列.这便保证了插入排序内层循环的花费不会超过阈值,内层循环为常数时间,插入排序复杂度`O(n`).
整个算法的复杂度`O(nlgn)`.

### stable_sort

qmj::stable_sort 采用归并排序和插入排序,当序列长度大于阈值时采用归并排序,
否则使用插入排序.qmj::stable_sort需要使用缓冲区.在有序(升序/降序)或重复元素
较多时比std::stable_sort更快.

qmj::stable_sort接受的比较符对应的有序区间和完全重复元素比std::stable_sort快50%,
逆序快40%,随机数据没有明显提高.

![stable_sort sorted/multi](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/stable_sort%20multi-sorted.png)

***正序或完全重复元素***

![stable_sort reverse](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/stable_sort%20reverse.png)

***逆序***

![stable_sort random](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/stable_sort%20random.png)

***随机不重复数据***

### search_n

	template<typename FIter,
			typename Dif,
			typename value_type,
			typename Pred>inline
			FIter _search_n_imple(FIter first, FIter last,
				Dif n, const value_type&val, const Pred&pred,
				std::random_access_iterator_tag)
		{
			if (n <= 0)
				return (first);
			iter_dif_t<FIter> count = last - first;//size of [first,ast)
			FIter result = first;//may be result
			for (Dif mark = n - 1; mark < count; mark += n)
			{
				if (pred(first[mark], val))
				{//[ret,first+mark] may be satisifyed
					for (Dif cur = 0;; ++cur)
						if (cur == n)
							return (result);
						else if (!pred(result[cur], val))
						{//adjust mark
							mark -= (n - cur - 1);
							break;
						}
				}
				result = first + mark + 1;//update ret
			}
			return (last);
		}
		
![search_n](https://github.com/MouJieQin/QMJSTL/blob/master/image/algorithm/search_n.png)

不同于<<stl源码剖析>>的代码.我使用其代码和`std`进行测试后发现要慢很多.
那`std`一定使用了完全不同的方法,经过一段时间的调试我写出了这个算法.最后和
`std`比对,虽然没有理清源码,但从注释上看它使用了和我一样的算法.我自认为
我的代码比`std`源码更加简洁.

假设`pred`为等于操作符:
因为是随机迭代器,首先记录可能的序列首元素位置为`result`,而后测试这个可能序列
的最后一个元素的下标为`mark`,如果`mark`与`val`不等,表明`result`到`(first+mark)`的元素
不可能为所求结果,跳跃调整`result=first+(mark+1)`,`mark+=n`.重新进行以上测试.
如果`mark`所指向的元素与`val`相等,便开始测试`[result,first+mark]`,如果里面的值全为
`val`便返回`result`.否则记不等于`val`的元素距离`result`的相对距离的为`cur`,
首先调整`mark`到`cur`所指元素到`first`的绝对距离,这里之所以不能远距离跳跃的原因在于
虽然`result`不是符合区间的首元素,但是`[result,first+mark]`(里面的`mark`为未调整前的)
可能具有符合条件的首元素.

#### 复杂度:
期望复杂度可以达到`O(n/k+k)`.其中`n`为序列的长度,`k`为查找的长度.






	
	
	
	
	
	
