#include "Type.hpp"
#include <cassert>
#include <typeinfo>

namespace llvm {

// VoidType 实现
auto VoidType::isEqual(const Type *other_type) const -> bool {
  return dynamic_cast<const VoidType *>(other_type) != nullptr;
}

// Int32Type 实现
auto Int32Type::isEqual(const Type *other_type) const -> bool {
  return dynamic_cast<const Int32Type *>(other_type) != nullptr;
}

// Int8Type 实现
auto Int8Type::isEqual(const Type *other_type) const -> bool {
  return dynamic_cast<const Int8Type *>(other_type) != nullptr;
}

// Int1Type 实现
auto Int1Type::isEqual(const Type *other_type) const -> bool {
  return dynamic_cast<const Int1Type *>(other_type) != nullptr;
}

// StructType 实现
StructType::StructType(const std::string &name) : name_("%struct." + name) {}

auto StructType::getName() const -> const std::string & { return name_; }

void StructType::setBody(const std::vector<Type *> &elements) {
  this->elements_ = elements;
}

auto StructType::getBody() const -> const std::vector<Type *> & {
  return elements_;
}

auto StructType::getElementType(unsigned i) const -> Type * {
  assert(i < elements_.size() && "Index out of bounds");
  return elements_[i];
}

auto StructType::getNumElements() const -> size_t { return elements_.size(); }

void StructType::setElementType(size_t i, Type *type) {
  assert(i < elements_.size() && "Index out of bounds");
  elements_[i] = type;
}

auto StructType::isEqual(const Type *other_type) const -> bool {
  const StructType *other_struct = dynamic_cast<const StructType *>(other_type);
  if (!other_struct) {
    return false;
  }

  if (name_ != other_struct->name_ ||
      elements_.size() != other_struct->elements_.size()) {
    return false;
  }

  for (size_t i = 0; i < elements_.size(); ++i) {
    if (!elements_[i]->isEqual(other_struct->elements_[i])) {
      return false;
    }
  }

  return true;
}

auto StructType::print() const -> std::string { return name_; }

auto StructType::printDef() const -> std::string {
  std::string result = name_ + " = type { ";
  for (size_t i = 0; i < elements_.size(); ++i) {
    result += elements_[i]->print();
    if (i < elements_.size() - 1) {
      result += ", ";
    }
  }
  result += " }";
  return result;
}

// ArrayType 实现
ArrayType::ArrayType(Type *element_type, size_t num_elements)
    : element_type_(element_type), num_elements_(num_elements) {}

auto ArrayType::getElementType() const -> Type * { return element_type_; }

auto ArrayType::getNumElements() const -> size_t { return num_elements_; }

auto ArrayType::isEqual(const Type *other_type) const -> bool {
  const ArrayType *other_array = dynamic_cast<const ArrayType *>(other_type);
  if (!other_array) {
    return false;
  }

  return num_elements_ == other_array->num_elements_ &&
         element_type_->isEqual(other_array->element_type_);
}

auto ArrayType::print() const -> std::string {
  std::string result = "[";
  result += element_type_->print();
  result += " x " + std::to_string(num_elements_) + "]";
  return result;
}

// FunctionType 实现
FunctionType::FunctionType(Type *return_type,
                           const std::vector<Type *> &param_types)
    : return_type_(return_type), param_types_(param_types) {}

auto FunctionType::getReturnType() const -> Type * { return return_type_; }

auto FunctionType::getParamTypes() const -> const std::vector<Type *> & {
  return param_types_;
}

auto FunctionType::getParamType(unsigned i) const -> Type * {
  assert(i < param_types_.size() && "Index out of bounds");
  return param_types_[i];
}

auto FunctionType::getNumParams() const -> unsigned {
  return static_cast<unsigned>(param_types_.size());
}

void FunctionType::setParamType(unsigned i, Type *type) {
  assert(i < param_types_.size() && "Index out of bounds");
  param_types_[i] = type;
}

void FunctionType::setReturnType(Type *type) { return_type_ = type; }

void FunctionType::addParamType(Type *type) { param_types_.push_back(type); }

auto FunctionType::isEqual(const Type *other_type) const -> bool {
  const FunctionType *other_func =
      dynamic_cast<const FunctionType *>(other_type);
  if (!other_func) {
    return false;
  }

  if (!return_type_->isEqual(other_func->return_type_) ||
      param_types_.size() != other_func->param_types_.size()) {
    return false;
  }

  for (size_t i = 0; i < param_types_.size(); ++i) {
    if (!param_types_[i]->isEqual(other_func->param_types_[i])) {
      return false;
    }
  }

  return true;
}

auto FunctionType::print() const -> std::string {
  std::string result = return_type_->print();
  result += " (";
  for (size_t i = 0; i < param_types_.size(); ++i) {
    result += param_types_[i]->print();
    if (i < param_types_.size() - 1) {
      result += ", ";
    }
  }
  result += ")";
  return result;
}

// PointerType 实现
auto PointerType::isEqual(const Type *other_type) const -> bool {
  return dynamic_cast<const PointerType *>(other_type) != nullptr;
}

} // namespace llvm