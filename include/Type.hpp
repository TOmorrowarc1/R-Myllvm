#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace llvm {

// 前向声明
class Type;
class VoidType;
class Int32Type;
class Int8Type;
class Int1Type;
class StructType;
class ArrayType;
class FunctionType;
class PointerType;

// 抽象基类 Type
class Type {
public:
  virtual ~Type() = default;

  // 判断是否为指针类型
  virtual auto isPointerTy() const -> bool = 0;

  // 判断是否为聚合类型（结构体或数组）
  virtual auto isAggregateTy() const -> bool = 0;

  // 判断两个类型是否相等
  virtual auto isEqual(const Type *other_type) const -> bool = 0;

  // 打印类型信息
  virtual auto print() const -> std::string = 0;
};

// Void类型
class VoidType : public Type {
public:
  VoidType() = default;

  auto isPointerTy() const -> bool override { return false; }

  auto isAggregateTy() const -> bool override { return false; }

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override { return "void"; }
};

// 整数类型基类
class IntegerType : public Type {
public:
  virtual ~IntegerType() = default;

  auto isPointerTy() const -> bool override { return false; }

  auto isAggregateTy() const -> bool override { return false; }
};

// 32位整数类型
class Int32Type : public IntegerType {
public:
  Int32Type() = default;

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override { return "i32"; }
};

// 8位整数类型
class Int8Type : public IntegerType {
public:
  Int8Type() = default;

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override { return "i8"; }
};

// 1位整数类型
class Int1Type : public IntegerType {
public:
  Int1Type() = default;

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override { return "i1"; }
};

// 结构体类型
class StructType : public Type {
private:
  std::string name_;
  std::vector<Type *> elements_;

public:
  StructType(const std::string &name);

  auto getName() const -> const std::string &;

  void setBody(const std::vector<Type *> &elements);

  auto getBody() const -> const std::vector<Type *> &;

  auto getElementType(unsigned i) const -> Type *;

  auto getNumElements() const -> size_t;

  void setElementType(size_t i, Type *type);

  auto isPointerTy() const -> bool override { return false; }

  auto isAggregateTy() const -> bool override { return true; }

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override;

  auto printDef() const -> std::string;
};

// 数组类型
class ArrayType : public Type {
private:
  Type *element_type_;
  size_t num_elements_;

public:
  ArrayType(Type *element_type, size_t num_elements);

  auto getElementType() const -> Type *;

  auto getNumElements() const -> size_t;

  auto isPointerTy() const -> bool override { return false; }

  auto isAggregateTy() const -> bool override { return true; }

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override;
};

// 函数类型
class FunctionType : public Type {
private:
  Type *return_type_;
  std::vector<Type *> param_types_;

public:
  FunctionType(Type *return_type, const std::vector<Type *> &param_types);

  auto getReturnType() const -> Type *;

  auto getParamTypes() const -> const std::vector<Type *> &;

  auto getParamType(unsigned i) const -> Type *;

  auto getNumParams() const -> unsigned;

  void setParamType(unsigned i, Type *type);

  void setReturnType(Type *type);

  void addParamType(Type *type);

  auto isPointerTy() const -> bool override { return false; }

  auto isAggregateTy() const -> bool override { return false; }

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override;
};

// 指针类型
class PointerType : public Type {
public:
  PointerType() = default;

  auto isPointerTy() const -> bool override { return true; }

  auto isAggregateTy() const -> bool override { return false; }

  auto isEqual(const Type *other_type) const -> bool override;

  auto print() const -> std::string override { return "ptr"; }
};

} // namespace llvm
