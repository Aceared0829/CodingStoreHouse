export module TArray;
import<initializer_list>;
import<iostream>;
import<algorithm>;
import<memory>;
import<print>;
import MIterator;
// Vector类模板，支持动态数组功能

template <class T>
class TArray
{
public:
	// 构造函数，指定初始大小
	TArray(int sz);
	// 拷贝构造函数
	TArray(const TArray& v);
	// 使用初始化列表构造
	TArray(std::initializer_list<T> vec);
	// 析构函数
	~TArray();

	// 拷贝赋值运算符
	auto operator=(const TArray& v) -> TArray&;
	// 判断两个Vector是否相等
	auto operator==(const TArray& v) -> bool;
	// 判断两个Vector是否不等
	auto operator!=(const TArray& v) -> bool;
	// 下标访问运算符
	auto operator[](int i) const -> T&;
	// 下标访问运算符（只读）
	auto Begin() const -> MIterator<T> { return MIterator<T>(Elum.get()); }
	auto End() const->MIterator<T> { return MIterator<T>(Elum.get() + Sz); }
	//两个迭代器相加
	auto operator+(const TArray& v) -> TArray;
	//两个迭代器相加
	auto operator+=(const TArray& v) -> TArray&;
	// 返回首地址（可修改）
	friend auto operator<<(std::ostream& os, const TArray& s) -> std::ostream&
	{
		for (auto i = 0; i < s.Sz; i++)
			os << s.Elum[i] << " ";
		return os;
	}
	// 获取元素数量
	auto Size() const -> int; //获取元素数量
	// 获取容器最大容量
	auto MaxSize() const -> int; //获取容器大小
	// 设置容器最大容量
	auto SetMaxSize(int maxsz) -> void; //设置容器大小
	// 升序排序
	auto SortSmallToBig() -> void; //升序排序
	// 降序排序
	auto SortBigToSmall() -> void; //降序排序
	// 在末尾添加元素
	auto Add(T elum) -> void; //在末尾添加元素
	// 在末尾插入元素（等价于Add）
	auto Emplace(T elum) -> void; //在末尾插入元素
	// 在指定位置后代替元素
	auto Emplace(T elum, int i) -> void; //在指定位置后代替元素
	// 在指定位置插入元素
	auto Insert(T elum, int i) -> void; //在指定位置插入元素
	// 在指定位置插入多个元素
	auto Insert(T elum, int i, int n) -> void; //在指定位置插入多个元素
	// 移除某个元素
	auto RemoveElum(T elum) -> void; //移除某个元素
	// 清空容器并释放元素
	auto Empty() -> void; //清空容器
	// 重置容器，释放内存
	auto Reset() -> void; //重置容器
	// 清空容器但不释放内存
	auto Clear() -> void; //清空容器,不释放内存
	// 获取当前剩余容量
	auto GetCapacity() const -> int; //获取当前容量
	// 设置当前容量
	auto GetCapacity(int s) -> void; //设置当前容量
	// 获取第一个元素指针
	auto GetFront() const -> T*;
	// 获取最后一个元素指针
	auto GetBack() const -> T*;
	// 移动赋值
	auto Move(TArray& vec) -> TArray&;
	// 用初始化列表移动赋值
	auto Move(std::initializer_list<T> vec) -> TArray&;
	// 检查数组是否有效
	auto IsValid() const -> bool; //查看数组是否可用
	// 交换两个Vector内容
	auto Swap(TArray& vec) -> TArray&;
	//打印所有元素
	auto PrintAllElements() -> void;
private:
	std::unique_ptr<T[]> Elum; // 元素存储的智能指针
	int Sz; // 当前元素数量
	int MaxSz; // 最大容量
};

// 构造函数，分配2倍于sz的空间并初始化为0
export template <class T>
TArray<T>::TArray(int sz) :Sz(sz), MaxSz(2 * sz)
{
	Elum = std::make_unique<T[]>(MaxSz); // 使用make_unique分配空间
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
}

// 拷贝构造函数，深拷贝
export template <class T>
TArray<T>::TArray(const TArray& v) :Sz(v.Sz), MaxSz(v.MaxSz)
{
	Elum = std::make_unique<T[]>(MaxSz); // 使用make_unique分配空间
	for (auto i = 0; i < Sz; i++)
		Elum[i] = v.Elum[i];
}

// 初始化列表构造函数
export template <class T>
TArray<T>::TArray(std::initializer_list<T> vec) :Sz(vec.size()), MaxSz(2 * Sz)
{
	Elum = std::make_unique<T[]>(MaxSz); // 使用make_unique分配空间
	int i = 0;
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		Elum[i] = *it;
		i++;
	}
}

// 析构函数，智能指针自动释放内存
export template <class T>
TArray<T>::~TArray()
{
	// 无需显式delete，智能指针自动清理
}

// 拷贝赋值运算符
export template<class T>
auto TArray<T>::operator=(const TArray& v) -> TArray&
{
	if (this != &v)
	{
		Sz = v.Sz;
		MaxSz = v.MaxSz;
		Elum = std::make_unique<T[]>(MaxSz); // 使用make_unique分配空间
		for (auto i = 0; i < Sz; i++)
			Elum[i] = v.Elum[i];
	}
	return *this;
}

// 判断两个Vector是否相等
export template<class T>
auto TArray<T>::operator==(const TArray& v) -> bool
{
	if (this != &v)
	{
		if (Sz != v.Sz)
			return false;
		for (auto i = 0; i < Sz; i++)
			if (Elum[i] != v.Elum[i])
				return false;
	}
	return true;
}

// 判断两个Vector是否不等
export template <class T>
auto TArray<T>::operator!=(const TArray& v) -> bool
{ return !(this == v); }

// 下标访问运算符，带越界检查
export template <class T>
auto TArray<T>::operator[](int i) const -> T&
{
	if (i < 0 || i >= Sz)
		throw std::out_of_range("Index out of range");
	return Elum[i];
}

export template <class T>
auto TArray<T>::operator+(const TArray& v) -> TArray
{
	TArray<T> result(Sz + v.Sz);
	for (auto i = 0; i < Sz; i++)
		result.Elum[i] = Elum[i];
	for (auto i = 0; i < v.Sz; i++)
		result.Elum[Sz + i] = v.Elum[i];
	return result;
}

export template <class T>
auto TArray<T>::operator+=(const TArray& v) -> TArray&
{
	if (this != &v)
	{
		if (Sz + v.Sz > MaxSz)
		{
			MaxSz = Sz + v.Sz;
			auto newElum = std::make_unique<T[]>(MaxSz); // 使用make_unique分配空间
			for (auto i = 0; i < Sz; i++)
				newElum[i] = Elum[i];
			Elum = std::move(newElum); // 转移所有权
		}
		for (auto i = 0; i < v.Sz; i++)
			Elum[Sz + i] = v.Elum[i];
		Sz += v.Sz;
	}
	return *this;
}

template <class T>
auto TArray<T>::Size() const -> int
{
	return Sz;
}

template <class T>
auto TArray<T>::MaxSize() const -> int
{
	return MaxSz;
}

// 输出运算符重载，打印所有元素
export template <class T>
auto operator<<(std::ostream& os, const TArray<T>& s) -> std::ostream&
{
	for (auto i = 0; i < s.Sz; i++)
		os << s.Elum[i] << " ";
	return os;
}

// 设置最大容量
export template<class T>
auto TArray<T>::SetMaxSize(int maxsize) -> void
{
	if (maxsize < Sz)
		throw std::runtime_error("The Max Size must more than Size!!!");
	MaxSz = maxsize;
}

// 升序排序
export template<class T>
auto TArray<T>::SortSmallToBig()->void
{
	std::sort(this->begin(), this->end());
}

// 降序排序
export template<class T>
auto TArray<T>::SortBigToSmall()->void
{
	std::sort(this->begin(), this->end(), std::greater<T>());
}

// 在末尾添加元素，必要时扩容
export template<class T>
auto TArray<T>::Add(T elum)->void
{
	if(Sz == MaxSz)
	{
		MaxSz *= 2;
		auto newElum = std::make_unique<T[]>(MaxSz); // 使用make_unique分配空间
		for (auto i = 0; i < Sz; i++)
			newElum[i] = Elum[i];
		Elum = std::move(newElum); // 转移所有权
	}
	Elum[Sz] = elum;
	Sz++;
}

// 在末尾插入元素（等价于Add）
export template<class T>
auto TArray<T>::Emplace(T elum)->void
{
	TArray<T>::Add(elum);
}

// 在指定位置代替元素
export template<class T>
auto TArray<T>::Emplace(T elum, int i)->void
{
	if (i >= Sz || i < 0)
	{
		throw std::out_of_range("Index out of range");
	}
	Elum[i] = elum;
}

// 移除某个元素（移除容器内所有匹配的元素）
export template<class T>
auto TArray<T>::RemoveElum(T elum)->void
{
	TArray<T> NewVector = *this;
	this->Reset();
	for (auto i = 0; i < NewVector.Sz; i++)
	{
		if (NewVector.Elum[i] != elum)
		{
			this->Add(NewVector.Elum[i]);
		}
	}
}

// 在指定位置插入元素，必要时扩容
export template<class T>
auto TArray<T>::Insert(T elum, int i)->void
{
	if (i >= Sz || i < 0)
	{
		throw std::out_of_range("Index out of range");
	}
	if (Sz == MaxSz)
	{
		MaxSz *= 2;
		auto newElum = std::make_unique<T[]>(MaxSz); // 使用make_unique分配空间
		for (auto j = 0; j < Sz; j++)
			newElum[j] = Elum[j];
		Elum = std::move(newElum); // 转移所有权
	}
	for (auto j = Sz; j > i; j--)
	{
		Elum[j] = Elum[j - 1];
	}
	Sz++;
	Elum[i] = elum;
}

// 在指定位置插入多个元素
export template<class T>
auto TArray<T>::Insert(T elum, int i, int n) -> void
{
	if (i<0||i>=Sz)
	{
		throw std::out_of_range("Index out of range");
	}
	if (Sz == MaxSz) // Should be Sz + n > MaxSz or similar to ensure space for n elements
	{
		MaxSz = std::max(MaxSz * 2, Sz + n); // Ensure enough space
		auto newElum = std::make_unique<T[]>(MaxSz);
		for (auto j = 0; j < Sz; j++)
			newElum[j] = Elum[j];
		Elum = std::move(newElum);
	}
	// Shift elements to make space for new n elements
	for (auto j = Sz -1; j >= i; --j) // Iterate backwards to avoid overwriting elements prematurely
	{
		Elum[j + n] = Elum[j];
	}
    Sz += n; // Increment size by n
	for (auto j = 0; j < n; ++j)
	{
		Elum[i + j] = elum; // Insert new elements
	}
}

// 清空容器并释放元素
export template <class T>
auto TArray<T>::Empty() -> void
{
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
	Sz = 0;
}

// 重置容器，释放内存
export template <class T>
auto TArray<T>::Reset() -> void
{
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
	Sz = 0;
	MaxSz = 0;
	Elum.reset(); // 重置智能指针
}

// 清空容器但不释放内存
export template<class T>
auto TArray<T>::Clear() -> void
{
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
	Sz = 0;
}

// 获取当前剩余容量
export template <class T>
auto TArray<T>::GetCapacity() const -> int
{
	return MaxSz - Sz;
}

// 设置当前容量
export template <class T>
auto TArray<T>::GetCapacity(int s) -> void
{
	if (s < Sz)
	{
		throw std::out_of_range("New Capability must more than Size");
	}
	MaxSz = Sz + s;
}

// 获取第一个元素指针
export template <class T>
auto TArray<T>::GetFront() const -> T*
{
	if (!this->IsValid()) throw std::runtime_error("The Vector you entered isn\'t a Capable Value"); // Escaped apostrophe
	return &Elum[0]; // Return address of the element
}

// 获取最后一个元素指针
export template <class T>
auto TArray<T>::GetBack() const -> T*
{
	if (!this->IsValid()) throw std::runtime_error("The Vector you entered isn\'t a Capable Value"); // Escaped apostrophe
	return &Elum[Sz - 1]; // Return address of the element
}

// 移动赋值
export template <class T>
auto TArray<T>::Move(TArray& vec) -> TArray&
{
    if (this != &vec)
    {
        Elum = std::move(vec.Elum); // 转移所有权
        Sz = vec.Sz;
        MaxSz = vec.MaxSz;
        vec.Sz = 0;
        vec.MaxSz = 0;
    }
    return *this;
}

// 用初始化列表移动赋值
export template <class T>
auto TArray<T>::Move(std::initializer_list<T> vec) -> TArray&
{
	if (vec.size() > MaxSz) // Check if new size exceeds current MaxSz
	{
        // Option 1: Throw error (as current)
		// throw std::out_of_range("The Vector you entered isn\'t a Capable Value");
        // Option 2: Resize Elum (more like a typical move assignment from list)
        MaxSz = vec.size(); // Or some other strategy like MaxSz = vec.size() * 2;
        Elum = std::make_unique<T[]>(MaxSz);
	}
    // No need to zero out old elements if we are overwriting them anyway
	Sz = vec.size();
	// MaxSz should remain or be adjusted based on new Sz, not just set to Sz.
    // If MaxSz was just set to vec.size() above, this is fine.
    // Otherwise, if MaxSz is larger, it should remain larger.
	int i = 0;
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		Elum[i] = *it;
		i++;
	}
	return *this;
}

// 检查数组是否有效
export template <class T>
auto TArray<T>::IsValid() const -> bool
{
	return Sz >= 0 && Sz <= MaxSz && Elum != nullptr; // Added null check for Elum
}

// 交换两个Vector内容
export template <class T>
auto TArray<T>::Swap(TArray& vec) -> TArray&
{
	if (!vec.IsValid() || !this->IsValid()) // Check both vectors
		throw std::runtime_error("The Vector you entered isn\'t a Capable Value");
	// Use std::swap for members for efficiency and correctness
    std::swap(Elum, vec.Elum);
    std::swap(Sz, vec.Sz);
    std::swap(MaxSz, vec.MaxSz);
	return *this;
}

export template <class T>
auto TArray<T>::PrintAllElements() -> void
{
	for (auto it = 0; it < Sz; it++)
	{
		std::println("The {} is: {}", it + 1, Elum[it]);
	}
}
