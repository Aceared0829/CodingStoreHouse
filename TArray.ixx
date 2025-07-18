export module TArray;
import<initializer_list>;
import<iostream>;
import<algorithm>;
import<memory>;
import<print>;
import MIterator;
// Vector��ģ�壬֧�ֶ�̬���鹦��

template <class T>
class TArray
{
public:
	// ���캯����ָ����ʼ��С
	TArray(int sz);
	// �������캯��
	TArray(const TArray& v);
	// ʹ�ó�ʼ���б���
	TArray(std::initializer_list<T> vec);
	// ��������
	~TArray();

	// ������ֵ�����
	auto operator=(const TArray& v) -> TArray&;
	// �ж�����Vector�Ƿ����
	auto operator==(const TArray& v) -> bool;
	// �ж�����Vector�Ƿ񲻵�
	auto operator!=(const TArray& v) -> bool;
	// �±���������
	auto operator[](int i) const -> T&;
	// �±�����������ֻ����
	auto Begin() const -> MIterator<T> { return MIterator<T>(Elum.get()); }
	auto End() const->MIterator<T> { return MIterator<T>(Elum.get() + Sz); }
	//�������������
	auto operator+(const TArray& v) -> TArray;
	//�������������
	auto operator+=(const TArray& v) -> TArray&;
	// �����׵�ַ�����޸ģ�
	friend auto operator<<(std::ostream& os, const TArray& s) -> std::ostream&
	{
		for (auto i = 0; i < s.Sz; i++)
			os << s.Elum[i] << " ";
		return os;
	}
	// ��ȡԪ������
	auto Size() const -> int; //��ȡԪ������
	// ��ȡ�����������
	auto MaxSize() const -> int; //��ȡ������С
	// ���������������
	auto SetMaxSize(int maxsz) -> void; //����������С
	// ��������
	auto SortSmallToBig() -> void; //��������
	// ��������
	auto SortBigToSmall() -> void; //��������
	// ��ĩβ���Ԫ��
	auto Add(T elum) -> void; //��ĩβ���Ԫ��
	// ��ĩβ����Ԫ�أ��ȼ���Add��
	auto Emplace(T elum) -> void; //��ĩβ����Ԫ��
	// ��ָ��λ�ú����Ԫ��
	auto Emplace(T elum, int i) -> void; //��ָ��λ�ú����Ԫ��
	// ��ָ��λ�ò���Ԫ��
	auto Insert(T elum, int i) -> void; //��ָ��λ�ò���Ԫ��
	// ��ָ��λ�ò�����Ԫ��
	auto Insert(T elum, int i, int n) -> void; //��ָ��λ�ò�����Ԫ��
	// �Ƴ�ĳ��Ԫ��
	auto RemoveElum(T elum) -> void; //�Ƴ�ĳ��Ԫ��
	// ����������ͷ�Ԫ��
	auto Empty() -> void; //�������
	// �����������ͷ��ڴ�
	auto Reset() -> void; //��������
	// ������������ͷ��ڴ�
	auto Clear() -> void; //�������,���ͷ��ڴ�
	// ��ȡ��ǰʣ������
	auto GetCapacity() const -> int; //��ȡ��ǰ����
	// ���õ�ǰ����
	auto GetCapacity(int s) -> void; //���õ�ǰ����
	// ��ȡ��һ��Ԫ��ָ��
	auto GetFront() const -> T*;
	// ��ȡ���һ��Ԫ��ָ��
	auto GetBack() const -> T*;
	// �ƶ���ֵ
	auto Move(TArray& vec) -> TArray&;
	// �ó�ʼ���б��ƶ���ֵ
	auto Move(std::initializer_list<T> vec) -> TArray&;
	// ��������Ƿ���Ч
	auto IsValid() const -> bool; //�鿴�����Ƿ����
	// ��������Vector����
	auto Swap(TArray& vec) -> TArray&;
	//��ӡ����Ԫ��
	auto PrintAllElements() -> void;
private:
	std::unique_ptr<T[]> Elum; // Ԫ�ش洢������ָ��
	int Sz; // ��ǰԪ������
	int MaxSz; // �������
};

// ���캯��������2����sz�Ŀռ䲢��ʼ��Ϊ0
export template <class T>
TArray<T>::TArray(int sz) :Sz(sz), MaxSz(2 * sz)
{
	Elum = std::make_unique<T[]>(MaxSz); // ʹ��make_unique����ռ�
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
}

// �������캯�������
export template <class T>
TArray<T>::TArray(const TArray& v) :Sz(v.Sz), MaxSz(v.MaxSz)
{
	Elum = std::make_unique<T[]>(MaxSz); // ʹ��make_unique����ռ�
	for (auto i = 0; i < Sz; i++)
		Elum[i] = v.Elum[i];
}

// ��ʼ���б��캯��
export template <class T>
TArray<T>::TArray(std::initializer_list<T> vec) :Sz(vec.size()), MaxSz(2 * Sz)
{
	Elum = std::make_unique<T[]>(MaxSz); // ʹ��make_unique����ռ�
	int i = 0;
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		Elum[i] = *it;
		i++;
	}
}

// ��������������ָ���Զ��ͷ��ڴ�
export template <class T>
TArray<T>::~TArray()
{
	// ������ʽdelete������ָ���Զ�����
}

// ������ֵ�����
export template<class T>
auto TArray<T>::operator=(const TArray& v) -> TArray&
{
	if (this != &v)
	{
		Sz = v.Sz;
		MaxSz = v.MaxSz;
		Elum = std::make_unique<T[]>(MaxSz); // ʹ��make_unique����ռ�
		for (auto i = 0; i < Sz; i++)
			Elum[i] = v.Elum[i];
	}
	return *this;
}

// �ж�����Vector�Ƿ����
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

// �ж�����Vector�Ƿ񲻵�
export template <class T>
auto TArray<T>::operator!=(const TArray& v) -> bool
{ return !(this == v); }

// �±�������������Խ����
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
			auto newElum = std::make_unique<T[]>(MaxSz); // ʹ��make_unique����ռ�
			for (auto i = 0; i < Sz; i++)
				newElum[i] = Elum[i];
			Elum = std::move(newElum); // ת������Ȩ
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

// �����������أ���ӡ����Ԫ��
export template <class T>
auto operator<<(std::ostream& os, const TArray<T>& s) -> std::ostream&
{
	for (auto i = 0; i < s.Sz; i++)
		os << s.Elum[i] << " ";
	return os;
}

// �����������
export template<class T>
auto TArray<T>::SetMaxSize(int maxsize) -> void
{
	if (maxsize < Sz)
		throw std::runtime_error("The Max Size must more than Size!!!");
	MaxSz = maxsize;
}

// ��������
export template<class T>
auto TArray<T>::SortSmallToBig()->void
{
	std::sort(this->begin(), this->end());
}

// ��������
export template<class T>
auto TArray<T>::SortBigToSmall()->void
{
	std::sort(this->begin(), this->end(), std::greater<T>());
}

// ��ĩβ���Ԫ�أ���Ҫʱ����
export template<class T>
auto TArray<T>::Add(T elum)->void
{
	if(Sz == MaxSz)
	{
		MaxSz *= 2;
		auto newElum = std::make_unique<T[]>(MaxSz); // ʹ��make_unique����ռ�
		for (auto i = 0; i < Sz; i++)
			newElum[i] = Elum[i];
		Elum = std::move(newElum); // ת������Ȩ
	}
	Elum[Sz] = elum;
	Sz++;
}

// ��ĩβ����Ԫ�أ��ȼ���Add��
export template<class T>
auto TArray<T>::Emplace(T elum)->void
{
	TArray<T>::Add(elum);
}

// ��ָ��λ�ô���Ԫ��
export template<class T>
auto TArray<T>::Emplace(T elum, int i)->void
{
	if (i >= Sz || i < 0)
	{
		throw std::out_of_range("Index out of range");
	}
	Elum[i] = elum;
}

// �Ƴ�ĳ��Ԫ�أ��Ƴ�����������ƥ���Ԫ�أ�
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

// ��ָ��λ�ò���Ԫ�أ���Ҫʱ����
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
		auto newElum = std::make_unique<T[]>(MaxSz); // ʹ��make_unique����ռ�
		for (auto j = 0; j < Sz; j++)
			newElum[j] = Elum[j];
		Elum = std::move(newElum); // ת������Ȩ
	}
	for (auto j = Sz; j > i; j--)
	{
		Elum[j] = Elum[j - 1];
	}
	Sz++;
	Elum[i] = elum;
}

// ��ָ��λ�ò�����Ԫ��
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

// ����������ͷ�Ԫ��
export template <class T>
auto TArray<T>::Empty() -> void
{
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
	Sz = 0;
}

// �����������ͷ��ڴ�
export template <class T>
auto TArray<T>::Reset() -> void
{
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
	Sz = 0;
	MaxSz = 0;
	Elum.reset(); // ��������ָ��
}

// ������������ͷ��ڴ�
export template<class T>
auto TArray<T>::Clear() -> void
{
	for (auto i = 0; i < Sz; i++)
		Elum[i] = 0;
	Sz = 0;
}

// ��ȡ��ǰʣ������
export template <class T>
auto TArray<T>::GetCapacity() const -> int
{
	return MaxSz - Sz;
}

// ���õ�ǰ����
export template <class T>
auto TArray<T>::GetCapacity(int s) -> void
{
	if (s < Sz)
	{
		throw std::out_of_range("New Capability must more than Size");
	}
	MaxSz = Sz + s;
}

// ��ȡ��һ��Ԫ��ָ��
export template <class T>
auto TArray<T>::GetFront() const -> T*
{
	if (!this->IsValid()) throw std::runtime_error("The Vector you entered isn\'t a Capable Value"); // Escaped apostrophe
	return &Elum[0]; // Return address of the element
}

// ��ȡ���һ��Ԫ��ָ��
export template <class T>
auto TArray<T>::GetBack() const -> T*
{
	if (!this->IsValid()) throw std::runtime_error("The Vector you entered isn\'t a Capable Value"); // Escaped apostrophe
	return &Elum[Sz - 1]; // Return address of the element
}

// �ƶ���ֵ
export template <class T>
auto TArray<T>::Move(TArray& vec) -> TArray&
{
    if (this != &vec)
    {
        Elum = std::move(vec.Elum); // ת������Ȩ
        Sz = vec.Sz;
        MaxSz = vec.MaxSz;
        vec.Sz = 0;
        vec.MaxSz = 0;
    }
    return *this;
}

// �ó�ʼ���б��ƶ���ֵ
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

// ��������Ƿ���Ч
export template <class T>
auto TArray<T>::IsValid() const -> bool
{
	return Sz >= 0 && Sz <= MaxSz && Elum != nullptr; // Added null check for Elum
}

// ��������Vector����
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
