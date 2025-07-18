export module Pair;
import <memory>;
import <iostream>;
namespace MSCON
{
export template<class Key, class Value>
class Pair
{
public:
	Pair(Key Kk, Value Vv) : Kkey(std::make_unique<Key>(Kk)), Vvalue(std::make_unique<Value>(Vv)) {}
	Pair(Key Kk) : Kkey(std::make_unique<Key>(Kk)), Vvalue(nullptr) {}
	Pair(Value Vv) : Kkey(nullptr), Vvalue(std::make_unique<Value>(Vv)) {}
	Pair() : Kkey(nullptr), Vvalue(nullptr) {}
	Pair(const Pair& other) 
		: Kkey(other.Kkey ? std::make_unique<Key>(*other.Kkey) : nullptr),
    Vvalue(other.Vvalue ? std::make_unique<Value>(*other.Vvalue) : nullptr) {}
	~Pair() = default;

	auto operator=(const Pair& other) -> Pair&;
	auto operator==(const Pair& other) const -> bool;
	auto operator!=(const Pair& other) const -> bool;

	friend auto operator<<(std::ostream& os, const Pair& p) -> std::ostream&
	{
		if (p.Kkey)
			os << *p.Kkey;
		else
			os << "null";
		os << " : ";
		if (p.Vvalue)
			os << *p.Vvalue;
		else
			os << "null";
		return os;
	}

	auto First() const -> Key*;
	auto Second() const -> Value*;
	auto Reset() -> void;
	auto Move(Pair& other) -> void;

private:
	std::unique_ptr<Key> Kkey;
	std::unique_ptr<Value> Vvalue;
};

export template <class Key, class Value>
auto Pair<Key, Value>::operator=(const Pair& other) -> Pair&
{
	if (this != &other)
	{
		Kkey = other.Kkey ? std::make_unique<Key>(*other.Kkey) : nullptr;
		Vvalue = other.Vvalue ? std::make_unique<Value>(*other.Vvalue) : nullptr;
	}
	return *this;
}

export template <class Key, class Value>
auto Pair<Key, Value>::operator==(const Pair& other) const -> bool
{
	return ((!Kkey && !other.Kkey) || (Kkey && other.Kkey && *Kkey == *other.Kkey)) &&
     ((!Vvalue && !other.Vvalue) || (Vvalue && other.Vvalue && *Vvalue == *other.Vvalue));
}

export template <class Key, class Value>
auto Pair<Key, Value>::operator!=(const Pair& other) const -> bool
{
	return !(*this == other);
}

export template <class Key, class Value>
auto Pair<Key, Value>::First() const -> Key*
{
	return Kkey ? Kkey.get() : nullptr;
}

export template<class Key, class Value>
auto Pair<Key, Value>::Second() const -> Value*
{
	return Vvalue ? Vvalue.get() : nullptr;
}

export template<class Key, class Value>
auto Pair<Key, Value>::Reset() -> void
{
	Kkey.reset();
	Vvalue.reset();
}

export template<class Key, class Value>
auto Pair<Key, Value>::Move(Pair& other) -> void
{
	Kkey = std::move(other.Kkey);
	Vvalue = std::move(other.Vvalue);
	other.Reset();
}
}
