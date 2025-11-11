# Type 系统概述

Type 系统以 Type 多态类为核心。Type 多态类用于代指一个LLVM Value对象的类型信息，映射到汇编中的内存布局，数据表示。

Type类别所有对象由Context统一管理和分配内存（不过在这个模块本身设计中无需在意这一点）。

Type类继承结构如下：
```
llvm::Type (抽象基类)
  │
  ├── llvm::Int32Type (32位整数类型)
  ├── llvm::Int8Type  (8位整数类型)
  ├── llvm::Int1Type  (1位整数类型)
  │
  ├── llvm::StructType (结构体类型)
  │
  ├── llvm::ArrayType (数组类型)
  │
  ├── llvm::FunctionType (函数类型)
  │
  └── llvm::PointerType (指针类型)
```

## Type 类及其接口

### 0. 基类 `llvm::Type`

`llvm::Type` 是所有类型类的抽象基类，定义了类型系统的基本接口和行为。

**成员**：没有成员。

**接口**：
- `auto isPointerTy() const -> bool` - 判断是否为指针类型
- `auto isEqual(const Type* other_type) const -> bool` - 判断两个类型是否相等
- `auto print() const -> std::string` - 打印类型信息

### 1. 整数类型

整数类型包括 `llvm::Int32Type`、`llvm::Int8Type` 和 `llvm::Int1Type`，它们分别表示32位、8位和1位整数类型。虽然它们继承自同一个基类，但它们是不同的类型。

#### 1.1 `llvm::Int32Type`

**成员**：无特定成员。

**接口**：
- `Int32Type()` - 构造函数

#### 1.2 `llvm::Int8Type`

**成员**：无特定成员。

**接口**：
- `Int8Type()` - 构造函数

#### 1.3 `llvm::Int1Type`

**成员**：无特定成员。

**接口**：
- `Int1Type()` - 构造函数

### 2. 结构体类型 `llvm::StructType`

结构体类型用于表示由多个字段组成的复合类型。

**成员**：
- `std::string name_` - 结构体名称
- `std::vector<Type*> elements_` - 字段类型列表

**接口**：
- `StructType(const std::string& name)` - 构造函数
- `auto getName() const -> const std::string&` - 获取结构体名称
- `void setBody(const std::vector<Type*>& elements)` - 设置结构体字段列表
- `auto getBody() const -> const std::vector<Type*>&` - 获取字段列表引用
- `auto getElementType(unsigned i) const -> Type*` - 获取第i个字段类型
- `auto getNumElements() const -> size_t` - 获取字段数量
- `void setElementType(size_t i, Type* type)` - 更改第i个字段类型

### 3. 数组类型 `llvm::ArrayType`

数组类型用于表示固定大小的元素数组。

**成员**：
- `Type* element_type_` - 元素类型
- `size_t num_elements_` - 数组大小

**接口**：
- `ArrayType(Type* element_type, size_t num_elements)` - 构造函数
- `auto getElementType() const -> Type*` - 获取元素类型
- `auto getNumElements() const -> size_t` - 获取数组大小

### 4. 函数类型 `llvm::FunctionType`

函数类型用于表示函数的签名，包括返回值类型和参数类型列表。

**成员**：
- `Type* return_type_` - 返回值类型
- `std::vector<Type*> param_types_` - 参数类型列表

**接口**：
- `FunctionType(Type* return_type, const std::vector<Type*>& param_types)` - 构造函数
- `auto getReturnType() const -> Type*` - 获取返回值类型
- `auto getParamTypes() const -> const std::vector<Type*>&` - 获取参数类型列表
- `auto getParamType(unsigned i) const -> Type*` - 获取第i个参数类型
- `auto getNumParams() const -> unsigned` - 获取参数数量
- `void setParamType(unsigned i, Type* type)` - 修改第i个参数类型
- `void setReturnType(Type* type)` - 修改返回值类型
- `void addParamType(Type* type)` - 添加参数类型

### 5. 指针类型 `llvm::PointerType`

指针类型采用不透明指针设计，不包含指向的具体类型信息。

**成员**：无成员（不透明指针设计）。

**接口**：
- `PointerType()` - 构造函数
