## Bloom Filter 设计书

#### 基本知识

Bloom Filter 是一个超大的 bitset，其大小为 m，待散列元素个数为 n，每个元素需要经过 k 个散列函数，并且误判率为 α，则最佳性能参数如下：

> m = log<sub>2</sub><sup>e</sup> * n * log<sub>2</sub><sup>(1/α)</sup>
>
> k = (m / n) * ln2

误判：如果 Bloom Filter 判断一个元素不在集合当中，那么它一定不在集合当中。如果  Bloom Filter 判断一个元素在集合中，那么它 **可能** 在集合当中

#### 设计思路(policy-base + OOP + TDD)

* Bloom Filter 包含哪些模板参数？

  1. 非类型模板参数 std::uint64_t N,  代表待插入的元素个数
  2. 模板模板参数 std::ratio FalsePositiveProbability, 代表误判率

* Bloom Filter 包含哪些 Policies？

  	1. 优化策略 OptimalPolicy：接收 N 和 FalsePositiveProbability 并计算出优化后的 k 和 m。 默认采用上述两个公式进行计算
   	2. 哈希策略 HashPolicy：包含两个维度：哈希对象的类型以及采用何种哈希方式。另外，有一个问题尚未解决：如果用户要制定对应的 hash 函数，他需要知道 k 值吗？我想到有两种策略：
       * 如果用户的 HashObject 中包含了一个 vector，并且用户需要自己填充这个 vector，那么他要知道 k 值的大小。除非他能够描述 hash 函数的生成规律，并由 HashObject 自动生成 k 个 hash 函数
       * 如果用户的做法是计算一个超大的哈希，并将其分割为 k 个部分，那么他只需要指定哈希计算方法以及分割方法即可，剩余部分由 HashPolicy 自动完成

* Bloom Filter 包含哪些成员变量？

  1. 包含 k 和 m，二者均为 const 属性
  2. 包含一个 bitset

* Bloom Filter 需要提供哪些接口

  	1. hash 接口，将给定的元素散列到 bitset 中，类型为 void(T element)
   	2. contain 接口，查询给定的元素是否包含在 bitset 当中, contain(T element)

  