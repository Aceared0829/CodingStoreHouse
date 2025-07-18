export module MIterator;
import <iterator>;
export namespace MSCON
{
    template<class T>
    class MIterator {
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        MIterator(pointer ptr) : ptr_(ptr) {}
        auto operator*() const->value_type { return *ptr_; }
		auto operator->() ->pointer{ return ptr_; }
		auto operator++() -> MIterator& ;
        auto operator==(const MIterator& other) const -> bool;
        auto operator!=(const MIterator& other) const -> bool;
        auto operator--() -> MIterator&;
        auto operator--(int) -> MIterator;
        auto operator+=(difference_type n) -> MIterator&;
        auto operator-=(difference_type n) -> MIterator&;
        auto operator+(difference_type n) const -> MIterator ;
        auto operator-(difference_type n) const -> MIterator ;
        auto operator-(const MIterator& other) const -> difference_type ;
        auto operator[](difference_type n) const -> reference ;
        auto operator<(const MIterator& other) const -> bool ;
        auto operator>(const MIterator& other) const -> bool ;
        auto operator<=(const MIterator& other) const -> bool ;
        auto operator>=(const MIterator& other) const -> bool ;
    private:
        pointer ptr_;
    };

    template <class T>
    auto MIterator<T>::operator++() -> MIterator&
    {
	    ++ptr_;
	    return *this;
    }

    template <class T>
    auto MIterator<T>::operator==(const MIterator& other) const -> bool
    {
	    return ptr_ == other.ptr_;
    }

    template <class T>
    auto MIterator<T>::operator!=(const MIterator& other) const -> bool
    {
	    return !(*this == other);
    }

    template <class T>
    auto MIterator<T>::operator--() -> MIterator&
    {
	    --ptr_;
	    return *this;
    }

    template <class T>
    auto MIterator<T>::operator--(int) -> MIterator
    {
	    MIterator tmp = *this;
	    --(*this);
	    return tmp;
    }

    template <class T>
    auto MIterator<T>::operator+=(difference_type n) -> MIterator&
    {
	    ptr_ += n;
	    return *this;
    }

    template <class T>
    auto MIterator<T>::operator-=(difference_type n) -> MIterator&
    {
	    ptr_ -= n;
	    return *this;
    }

    template <class T>
    auto MIterator<T>::operator+(difference_type n) const -> MIterator
    {
	    MIterator tmp = *this;
	    return tmp += n;
    }

    template <class T>
    auto MIterator<T>::operator-(difference_type n) const -> MIterator
    {
	    MIterator tmp = *this;
	    return tmp -= n;
    }

    template <class T>
    auto MIterator<T>::operator-(const MIterator& other) const -> difference_type
    {
	    return ptr_ - other.ptr_;
    }

    template <class T>
    auto MIterator<T>::operator[](difference_type n) const -> reference
    {
	    return *(ptr_ + n);
    }

    template <class T>
    auto MIterator<T>::operator<(const MIterator& other) const -> bool
    {
	    return ptr_ < other.ptr_;
    }

    template <class T>
    auto MIterator<T>::operator>(const MIterator& other) const -> bool
    {
	    return other < *this;
    }

    template <class T>
    auto MIterator<T>::operator<=(const MIterator& other) const -> bool
    {
	    return !(other < *this);
    }

    template <class T>
    auto MIterator<T>::operator>=(const MIterator& other) const -> bool
    {
	    return !(*this < other);
    }
}
