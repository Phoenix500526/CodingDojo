## Bloom Filter 设计书

#### 基本知识

Bloom Filter 是一个超大的 bitset，其大小为 m，待散列元素个数为 n，每个元素需要经过 k 个散列函数，并且误判率为 α，则最佳性能参数如下：

> m = log<sub>2</sub><sup>e</sup> * n * log<sub>2</sub><sup>(1/α)</sup>
>
> k = (m / n) * ln2  = -log<sub>2</sub><sup>α</sup>

误判：如果 Bloom Filter 判断一个元素不在集合当中，那么它一定不在集合当中。如果  Bloom Filter 判断一个元素在集合中，那么它 **可能** 在集合当中

#### 设计思路(policy-base + OOP + TDD)

* Bloom Filter 包含哪些模板参数？

  1. 非类型模板参数 std::uint64_t N,  代表待插入的元素个数
  2. 模板模板参数 std::ratio FalsePositiveProbability, 代表误判率

* Bloom Filter 包含哪些 Policies？
	1. 优化策略 OptimalPolicy：接收 N 和 FalsePositiveProbability 并计算出优化后的 k 和 m。 默认采用上述两个公式进行计算
	2. 哈希策略 HashPolicy：只需要提供一个 hash 函数，而能够接收待哈希对象 key 以及一个哈希种子 hash。

* Bloom Filter 包含哪些成员变量？

  1. bitmap 数组的大小，const m_tableSize
  2. vector<uint8_t\> m_bitmap，作为 bitmap 的底层存储结构。
  3. vector<uint32_t\> m_salt, 用于保存随机值
  4. HashPolicy m_hash 哈希对象，用于计算哈希函数
  5. static constexpr std::uint8_t s_bitPerElement
  6. static constexpr std::uint8_t s_bitMask[s_bitPerElement]

* Bloom Filter 需要提供哪些接口

  3. hash_impl 接口，私有接口，将计算出对应的散列值，返回形式应该是 vector\<std::uint64_t> hash_idx, idx 的大小
  2. insert 接口，将给定的元素散列到 bitset 中，类型为 void(T element)， 提供支持容器的版本
  3. query 接口，查询给定的元素是否包含在 bitset 当中, query(T element)，提供支持容器的版本
  4. clear 接口，可以用于清空一个 Bloom Filter
  5. 由于存在误判率，所以 update 基本不提供删除操作。

  