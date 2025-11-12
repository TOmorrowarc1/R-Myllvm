# 工具类
以下为实现中用到的utils/中的工具类。

## 数组视图 ArrayRef
`ArrayRef`是一个轻量级的不可变数组视图类，提供对数组数据的只读访问。它不拥有数据，只是引用现有的数据。

成员：
``` cpp
template <typename T>
class ArrayRef {
  const T* data_ = nullptr;
  size_t length_ = 0;
};

```

接口及其实现：
```cpp
class ArrayRef {
  public:
    using iterator = const T*;
    using const_iterator = const T*;

    // Constructors
    ArrayRef() = default;
    ArrayRef(const T* data, size_t length) : data(data), Length(length) {}
    ArrayRef(const std::vector<T>& Vec) : data(Vec.data()), Length(Vec.size()) {}
    ArrayRef(const std::initializer_list<T>& List) : data(List.begin()), Length(List.size()) {}

    // Accessors
    size_t size() const { return length_; }
    bool empty() const { return length_ == 0; }
    const T& front() const { assert(!empty()); return data_[0]; }
    const T& back() const { assert(!empty()); return data_[length_ - 1]; }
    const T& operator[](size_t index) const { assert(index < length_); return data_[index]; }
    const T* data() const { return data_; }

    // Iterators
    iterator begin() const { return data_; }
    iterator end() const { return data_ + length_; }
};
```
