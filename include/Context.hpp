#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Utils/ArrayRef.hpp"

namespace llvm {

// 前向声明
class Type;
class IntegerType;
class StructType;
class ArrayType;
class FunctionType;
class PointerType;
class IntegerType;
class ConstantInt;

class FuncComparator {
public:
  auto operator()(const std::pair<Type *, llvm::ArrayRef<Type *>> &lhs,
                  const std::pair<Type *, llvm::ArrayRef<Type *>> &rhs) const
      -> bool {
    if (lhs.first != rhs.first) {
      return lhs.first < rhs.first;
    }
    llvm::ArrayRefComparator<Type *> arrayRefComp;
    return arrayRefComp(lhs.second, rhs.second);
  }
};

// LLVM上下文类，用于管理LLVM IR中类型的创建和销毁
class LLVMContext {
public:
  // 构造函数
  LLVMContext() = default;

  // 获取或创建32位整数类型对象
  auto getInt32Ty() -> IntegerType *;

  // 获取或创建8位整数类型对象
  auto getInt8Ty() -> IntegerType *;

  // 获取或创建1位整数类型对象
  auto getInt1Ty() -> IntegerType *;

  // 获取或创建结构体类型对象
  auto getStructType(const std::string &name) -> StructType *;

  // 获取或创建数组类型对象
  auto getArrayType(Type *element_type, int32_t length) -> ArrayType *;

  // 获取或创建函数类型对象
  auto getFunctionType(Type *return_type,
                       const std::vector<Type *> &param_types)
      -> FunctionType *;

  // 获取指针类型对象
  auto getPointerType() -> PointerType *;

  // 获取或创建整数常量对象
  auto getIntConstant(IntegerType *type, int64_t value) -> ConstantInt *;

private:
  // 存储已创建的整数类对象，避免重复创建，通过位数索引
  std::map<int32_t, std::unique_ptr<IntegerType>> integer_types_;

  // 存储已创建的Type Struct对象，避免重复创建，通过名称索引
  std::map<std::string, std::unique_ptr<StructType>> struct_types_;

  // 存储已创建的数组类对象，避免重复创建，通过元素类型和长度索引
  std::map<std::pair<Type *, int32_t>, std::unique_ptr<ArrayType>> array_types_;

  // 指针类型单例对象
  std::unique_ptr<PointerType> pointer_type_;

  // 存储已创建的函数类对象，避免重复创建，通过返回类型和参数类型列表索引
  std::map<std::pair<Type *, ArrayRef<Type *>>, std::unique_ptr<FunctionType>,
           FuncComparator>
      function_types_;

  // 存储已创建的整数常量对象，避免重复创建，通过类型与整数值索引
  std::map<std::pair<IntegerType *, int64_t>, std::unique_ptr<ConstantInt>>
      int_constants_;
};
} // namespace llvm
