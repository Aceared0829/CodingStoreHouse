export module Map;
import <memory>;
import <stdexcept>;
import Pair;
import MIterator;

namespace MSCON
{
	template<class Key, class Value >
	class Map
	{
	public:
		Map(int sz);
		Map();
		Map(const Map& other);
		~Map() = default;
		auto begin()-> MSCON::MIterator<Pair<Key, Value>> { return MIterator<Pair<Key, Value>>(Elum.get()); }
		auto begin() const->MSCON::MIterator<Pair<Key, Value>> { return MIterator<Pair<Key, Value>>(Elum.get()); }
		auto end() -> MSCON::MIterator<Pair<Key, Value>> { return MIterator<Pair<Key, Value>>(Elum.get() + Sz); }
		auto end()const->MSCON::MIterator<Pair<Key, Value>> { return MIterator<Pair<Key, Value>>(Elum.get() + Sz); }
		auto operator=(const Map& other) -> Map&;
		auto operator==(const Map& other) const -> bool;
		auto operator[](const Key& key)->Value&;
		auto Add(const Key& key, const Value& value) -> void;//默认在尾后添加元素
		auto Add(const Key& key, const Value& value, int i) -> void;//在指定位置添加元素
		auto Remove(const Key& key) -> void;//删除指定元素
	private:
		std::unique_ptr<Pair<int, int>[]> Elum; // 元素存储的智能指针
		int Sz; // 当前元素数量
		int MaxSz; // 最大容量
	};

	template <class Key, class Value>
	Map<Key, Value>::Map(int sz)
	{
		Sz = 0;
		MaxSz = sz;
		Elum = std::make_unique<Pair<Key, Value>[]>(MaxSz);
		if (Sz > MaxSz) throw std::out_of_range("Size exceeds maximum size");
		for (int i = 0; i < MaxSz; ++i)
		{
			Elum[i] = Pair<Key, Value>();
		}
	}

	export template <class Key, class Value>
	Map<Key, Value>::Map()
	{
		Sz = 0;
		MaxSz = 10;
		Elum = std::make_unique<Pair<Key, Value>[]>(MaxSz);
	}

	export template <class Key, class Value>
	Map<Key, Value>::Map(const Map& other)
	{
		Sz = other.Sz;
		MaxSz = other.MaxSz;
		Elum = std::make_unique<Pair<Key, Value>[]>(MaxSz);
		for (int i = 0; i < Sz; ++i)
		{
			Elum[i] = other.Elum[i];
		}
	}

	export template <class Key, class Value>
	auto Map<Key, Value>::operator=(const Map& other) -> Map&
	{
		if (this != &other)
		{
			Sz = other.Sz;
			MaxSz = other.MaxSz;
			Elum = std::make_unique<Pair<Key, Value>[]>(MaxSz);
			for (int i = 0; i < Sz; ++i)
			{
				Elum[i] = other.Elum[i];
			}
		}
		return *this;
	}

	export template <class Key, class Value>
	auto Map<Key, Value>::operator==(const Map& other) const -> bool
	{
		if (this->MaxSz!=other.MaxSz||this->Sz!=other.Sz)
			return false;
		for (int i = 0; i < Sz; ++i)
		{
			if (Elum[i] != other.Elum[i])
				return false;
		}
		return true;
	}

	export template <class Key, class Value>
	auto Map<Key, Value>::operator[](const Key& key) -> Value&
	{
		for (int i=0;i<Sz;i++)
		{
			if (Elum[i].First()==key)
			{
				return *Elum[i].Second();
			}
		}
		throw std::out_of_range("Key not found");
	}

	export template <class Key, class Value>
	auto Map<Key, Value>::Add(const Key& key, const Value& value) -> void
	{
		for (auto it=begin();it!=end();++it)
		{
			if (it.First() == key)
			{
				throw std::runtime_error("Key already exists");
			}
		}
		if (Sz == MaxSz)
		{
			MaxSz *= 2;
			auto newElum = std::make_unique<Pair<Key, Value>[]>(MaxSz);
			for (int i = 0; i < Sz; ++i)
			{
				newElum[i] = Elum[i];
			}
			Elum = std::move(newElum);
		}
		else
		{
			Elum[Sz].First() = key;
			Elum[Sz].Second() = value;
			Sz++;
		}
	}

	template <class Key, class Value>
	auto Map<Key, Value>::Add(const Key& key, const Value& value, int i) -> void
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (it.First() == key)
			{
				throw std::runtime_error("Key already exists");
			}
		}
		if (Sz==MaxSz)
		{
			MaxSz *= 2;
			auto newElum = std::make_unique<Pair<Key, Value>[]>(MaxSz);
			for (int j = 0; j < Sz; ++j)
			{
				newElum[j] = Elum[j];
			}
			Elum = std::move(newElum);
		}
		else
		{
			for (int j = Sz; j > i; --j)
			{
				Elum[j] = Elum[j - 1];
			}
			Elum[i].First() = key;
			Elum[i].Second() = value;
			Sz++;
		}
	}

	template <class Key, class Value>
	auto Map<Key, Value>::Remove(const Key& key) -> void
	{
		for (auto& i:this)
		{
			
		}
	}
}
