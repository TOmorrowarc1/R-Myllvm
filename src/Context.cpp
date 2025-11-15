#include "Context.hpp"
#include "Type.hpp"
#include "Value.hpp"

namespace llvm {

auto LLVMContext::getVoidTy() -> VoidType * {
  if (!void_type_) {
    void_type_ = std::make_unique<VoidType>();
  }
  return void_type_.get();
}

auto LLVMContext::getInt32Ty() -> Int32Type * {
  auto it = integer_types_.find(32);
  if (it != integer_types_.end()) {
    return static_cast<Int32Type *>(it->second.get());
  }

  auto type = std::make_unique<Int32Type>();
  auto *result = type.get();
  integer_types_[32] = std::move(type);
  return static_cast<Int32Type *>(result);
}

auto LLVMContext::getInt8Ty() -> Int8Type * {
  auto it = integer_types_.find(8);
  if (it != integer_types_.end()) {
    return static_cast<Int8Type *>(it->second.get());
  }

  auto type = std::make_unique<Int8Type>();
  auto *result = type.get();
  integer_types_[8] = std::move(type);
  return static_cast<Int8Type *>(result);
}

auto LLVMContext::getInt1Ty() -> Int1Type * {
  auto it = integer_types_.find(1);
  if (it != integer_types_.end()) {
    return static_cast<Int1Type *>(it->second.get());
  }

  auto type = std::make_unique<Int1Type>();
  auto *result = type.get();
  integer_types_[1] = std::move(type);
  return static_cast<Int1Type *>(result);
}

auto LLVMContext::getStructType(const std::string &name) -> StructType * {
  auto it = struct_types_.find(name);
  if (it != struct_types_.end()) {
    return it->second.get();
  }

  auto type = std::make_unique<StructType>(name);
  auto *result = type.get();
  struct_types_[name] = std::move(type);
  return result;
}

auto LLVMContext::getArrayType(Type *element_type, int32_t length)
    -> ArrayType * {
  auto key = std::make_pair(element_type, length);
  auto it = array_types_.find(key);
  if (it != array_types_.end()) {
    return it->second.get();
  }

  auto type = std::make_unique<ArrayType>(element_type, length);
  auto *result = type.get();
  array_types_[key] = std::move(type);
  return result;
}

auto LLVMContext::getFunctionType(Type *return_type,
                                  const std::vector<Type *> &param_types)
    -> FunctionType * {
  ArrayRef<Type *> param_types_ref(param_types);
  auto key = std::make_pair(return_type, param_types_ref);
  auto it = function_types_.find(key);
  if (it != function_types_.end()) {
    return it->second.get();
  }

  auto type = std::make_unique<FunctionType>(return_type, param_types);
  auto *result = type.get();
  function_types_[key] = std::move(type);
  return result;
}

auto LLVMContext::getPointerType() -> PointerType * {
  if (!pointer_type_) {
    pointer_type_ = std::make_unique<PointerType>();
  }
  return pointer_type_.get();
}

auto LLVMContext::getIntConstant(IntegerType *type, uint32_t value)
    -> ConstantInt * {
  auto key = std::make_pair(type, value);
  auto it = int_constants_.find(key);
  if (it != int_constants_.end()) {
    return it->second.get();
  }

  auto constant = std::make_unique<ConstantInt>(type, value);
  auto *result = constant.get();
  int_constants_[key] = std::move(constant);
  return result;
}

auto LLVMContext::getStructConstant(StructType *type, std::vector<std::unique_ptr<llvm::Constant>> &&values)
    -> ConstantStruct * {
  // 创建一个临时的常量指针数组用于比较
  std::vector<Constant *> temp_values;
  for (const auto &value : values) {
    temp_values.push_back(value.get());
  }
  
  ArrayRef<Constant *> values_ref(temp_values);
  auto key = std::make_pair(type, values_ref);
  auto it = struct_constants_.find(key);
  if (it != struct_constants_.end()) {
    return it->second.get();
  }

  // 直接使用移动语义，不需要复制
  auto constant = std::make_unique<ConstantStruct>(type, std::move(values));
  auto *result = constant.get();
  struct_constants_[key] = std::move(constant);
  return result;
}

auto LLVMContext::getArrayConstant(ArrayType *type, std::vector<std::unique_ptr<llvm::Constant>> &&values)
    -> ConstantArray * {
  // 创建一个临时的常量指针数组用于比较
  std::vector<Constant *> temp_values;
  for (const auto &value : values) {
    temp_values.push_back(value.get());
  }
  
  ArrayRef<Constant *> values_ref(temp_values);
  auto key = std::make_pair(type, values_ref);
  auto it = array_constants_.find(key);
  if (it != array_constants_.end()) {
    return it->second.get();
  }

  // 直接使用移动语义，不需要复制
  auto constant = std::make_unique<ConstantArray>(type, std::move(values));
  auto *result = constant.get();
  array_constants_[key] = std::move(constant);
  return result;
}

auto LLVMContext::getNullPtrConstant() -> ConstantPointerNull * {
  if (!null_constant_) {
    null_constant_ = std::make_unique<ConstantPointerNull>(getPointerType());
  }
  return null_constant_.get();
}

} // namespace llvm