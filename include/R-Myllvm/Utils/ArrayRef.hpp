#pragma once

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <vector>

namespace llvm {

template <typename T> class ArrayRef {
public:
  using iterator = const T *;
  using const_iterator = const T *;

  // Constructors
  ArrayRef() = default;
  ArrayRef(const T *data, size_t length) : data_(data), length_(length) {}
  ArrayRef(const std::vector<T> &Vec)
      : data_(Vec.data()), length_(Vec.size()) {}
  ArrayRef(const std::initializer_list<T> &List)
      : data_(List.begin()), length_(List.size()) {}

  // Accessors
  size_t size() const { return length_; }
  bool empty() const { return length_ == 0; }
  const T &front() const {
    assert(!empty());
    return data_[0];
  }
  const T &back() const {
    assert(!empty());
    return data_[length_ - 1];
  }
  const T &operator[](size_t index) const {
    assert(index < length_);
    return data_[index];
  }
  const T *data() const { return data_; }

  // Iterators
  iterator begin() const { return data_; }
  iterator end() const { return data_ + length_; }

private:
  const T *data_ = nullptr;
  size_t length_ = 0;
};

// ArrayRef比较函数，用于在map中使用
template <typename T> struct ArrayRefComparator {
  bool operator()(const ArrayRef<T> &lhs, const ArrayRef<T> &rhs) const {
    if (lhs.size() != rhs.size()) {
      return lhs.size() < rhs.size();
    }

    for (size_t i = 0; i < lhs.size(); ++i) {
      if (lhs[i] != rhs[i]) {
        return lhs[i] < rhs[i];
      }
    }

    return false;
  }
};

} // namespace llvm