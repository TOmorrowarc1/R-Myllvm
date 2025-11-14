#include "Value.hpp"
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace llvm {

// Value 类实现
void Value::addUser(User *user) { users_.push_back(user); }

const std::vector<User *> &Value::getUsers() const { return users_; }

// User 类实现
void User::addOperand(Value *operand) {
  operands_.push_back(operand);
  operand->addUser(this);
}

const std::vector<Value *> &User::getOperands() const { return operands_; }

// GlobalVariable 类实现
GlobalVariable::GlobalVariable(const std::string &name, Type *type,
                               Constant *initial_value)
    : name_(name), type_(type), initial_value_(initial_value) {}

auto GlobalVariable::getType() const -> Type * { return type_; }

auto GlobalVariable::getName() const -> std::string { return name_; }

auto GlobalVariable::getInitialValue() const -> Constant * {
  return initial_value_;
}

auto GlobalVariable::print() const -> std::string {
  std::string result = "@" + name_ + " = global " + type_->print();
  if (initial_value_) {
    result += " " + initial_value_->print();
  } else {
    result += " 0";
  }
  return result;
}

// Function 类实现
Function::Function(const std::string &name, FunctionType *func_type,
                   Module *parent)
    : name_(name), func_type_(func_type), is_defined_(false), parent_(parent) {}

void Function::addBasicBlock(std::unique_ptr<BasicBlock> &&bb) {
  basic_blocks_.push_back(std::move(bb));
}

auto Function::createBasicBlock(const std::string &name) -> BasicBlock * {
  is_defined_ = true;
  std::string actual_name = name;
  if (bb_name_map_.find(name) != bb_name_map_.end()) {
    bb_name_map_[name]++;
    actual_name = name + "." + std::to_string(bb_name_map_[name]);
  } else {
    bb_name_map_[name] = 0;
  }

  auto bb = std::make_unique<BasicBlock>(actual_name, this);
  auto bb_ptr = bb.get();
  basic_blocks_.push_back(std::move(bb));
  return bb_ptr;
}

const std::vector<BasicBlock *> Function::getBasicBlocks() const {
  std::vector<BasicBlock *> result;
  for (const auto &bb : basic_blocks_) {
    result.push_back(bb.get());
  }
  return result;
}

auto Function::getBBbyIndex(size_t index) const -> BasicBlock * {
  if (index < 0 || index >= basic_blocks_.size()) {
    return nullptr;
  }
  return basic_blocks_[index].get();
}

void Function::addArgument(std::unique_ptr<Argument> &&arg) {
  arguments_.push_back(std::move(arg));
}

void Function::setArguments(std::vector<std::unique_ptr<Argument>> &&args) {
  arguments_.clear();
  for (auto &arg : args) {
    arguments_.push_back(std::move(arg));
  }
}

const std::vector<Argument *> Function::getArguments() const {
  std::vector<Argument *> result;
  for (const auto &arg : arguments_) {
    result.push_back(arg.get());
  }
  return result;
}

auto Function::getArgByIndex(size_t index) const -> Argument * {
  if (index < 0 || index >= arguments_.size()) {
    return nullptr;
  }
  return arguments_[index].get();
}

auto Function::isDefined() const -> bool { return is_defined_; }

auto Function::getType() const -> FunctionType * { return func_type_; }

auto Function::getName() const -> std::string { return name_; }

auto Function::getParent() const -> Module * { return parent_; }

auto Function::print() const -> std::string {
  std::string result = is_defined_ ? "define " : "declare ";
  result += func_type_->getReturnType()->print() + " @" + name_ + "(";

  const auto &param_types = func_type_->getParamTypes();
  for (size_t i = 0; i < param_types.size(); ++i) {
    if (i < arguments_.size()) {
      result += param_types[i]->print() + " %" + arguments_[i]->getName();
    } else {
      result += param_types[i]->print();
    }
    if (i < param_types.size() - 1) {
      result += ", ";
    }
  }

  result += ") {\n";

  for (const auto &bb : basic_blocks_) {
    result += bb->print() + "\n";
  }

  result += "}";
  return result;
}

// BasicBlock 类实现
BasicBlock::BasicBlock(const std::string &name, Function *parent)
    : name_(name), parent_(parent) {}

void BasicBlock::addInstruction(std::unique_ptr<Instruction> &&inst) {
  instructions_.push_back(std::move(inst));
}

const std::vector<Instruction *> BasicBlock::getInstructions() const {
  std::vector<Instruction *> result;
  for (const auto &inst : instructions_) {
    result.push_back(inst.get());
  }
  return result;
}

auto BasicBlock::isTerminated() const -> bool {
  if (instructions_.empty()) {
    return false;
  }
  return dynamic_cast<TerminatorInst *>(instructions_.back().get()) != nullptr;
}

auto BasicBlock::getTerminator() const -> Instruction * {
  if (instructions_.empty()) {
    return nullptr;
  }
  return dynamic_cast<TerminatorInst *>(instructions_.back().get());
}

auto BasicBlock::getType() const -> Type * {
  throw std::runtime_error("BasicBlock cannot be used as a value");
}

auto BasicBlock::getName() const -> std::string { return name_; }

auto BasicBlock::getParent() const -> Function * { return parent_; }

auto BasicBlock::print() const -> std::string {
  std::string result = name_ + ":\n";

  for (const auto &inst : instructions_) {
    result += "  " + inst->print() + "\n";
  }

  return result;
}

// BinaryOperator 类实现
BinaryOperator::BinaryOperator(const std::string &name, Type *type, Value *lhs,
                               Value *rhs, const std::string &op)
    : name_(name), op_(op), type_(type), lhs_(lhs), rhs_(rhs) {
  // 检查操作数类型是否为整数
  auto *lhs_int_type = dynamic_cast<IntegerType *>(lhs->getType());
  auto *rhs_int_type = dynamic_cast<IntegerType *>(rhs->getType());
  if (!lhs_int_type || !rhs_int_type) {
    throw std::runtime_error("BinaryOperator operands must be of integer type");
  }
  // 检查操作数类型是否与结果类型匹配
  if (!lhs->getType()->isEqual(type) || !rhs->getType()->isEqual(type)) {
    throw std::runtime_error(
        "BinaryOperator operand types must match result type");
  }
  addOperand(lhs);
  addOperand(rhs);
}

auto BinaryOperator::getLHS() const -> Value * { return lhs_; }

auto BinaryOperator::getRHS() const -> Value * { return rhs_; }

auto BinaryOperator::getOp() const -> std::string { return op_; }

auto BinaryOperator::getType() const -> Type * { return type_; }

auto BinaryOperator::getName() const -> std::string { return "%" + name_; }

auto BinaryOperator::print() const -> std::string {
  return getName() + " = " + op_ + " " + type_->print() + " " +
         lhs_->getName() + ", " + rhs_->getName();
}

// UnaryOperator 类实现
UnaryOperator::UnaryOperator(const std::string &name, const std::string &op,
                             Type *type, Value *operand)
    : name_(name), op_(op), type_(type), operand_(operand) {
  // 检查操作数类型是否为整数
  auto *operand_int_type = dynamic_cast<IntegerType *>(operand->getType());
  if (!operand_int_type) {
    throw std::runtime_error("UnaryOperator operand must be of integer type");
  }
  // 检查操作数类型是否与结果类型匹配
  if (!operand->getType()->isEqual(type)) {
    throw std::runtime_error(
        "UnaryOperator operand type must match result type");
  }
  addOperand(operand);
}

auto UnaryOperator::getOperand() const -> Value * { return operand_; }

auto UnaryOperator::getOp() const -> std::string { return op_; }

auto UnaryOperator::getType() const -> Type * { return type_; }

auto UnaryOperator::getName() const -> std::string { return "%" + name_; }

auto UnaryOperator::print() const -> std::string {
  return getName() + " = " + op_ + " " + type_->print() + " " +
         operand_->getName();
}

// LoadInst 类实现
LoadInst::LoadInst(const std::string &name, Type *type, Value *ptr)
    : name_(name), type_(type), ptr_(ptr) {
  // 检查指针类型是否为指针
  if (!ptr->getType()->isPointerTy()) {
    throw std::runtime_error(
        "LoadInst pointer operand must be of pointer type");
  }
  addOperand(ptr);
}

auto LoadInst::getPtr() const -> Value * { return ptr_; }

auto LoadInst::getType() const -> Type * { return type_; }

auto LoadInst::getName() const -> std::string { return "%" + name_; }

auto LoadInst::print() const -> std::string {
  return getName() + " = load " + type_->print() + ", ptr " + ptr_->getName();
}

// StoreInst 类实现
StoreInst::StoreInst(Type *type, Value *ptr, Value *value)
    : type_(type), ptr_(ptr), value_(value) {
  // 检查指针类型是否为指针
  if (!ptr->getType()->isPointerTy()) {
    throw std::runtime_error(
        "StoreInst pointer operand must be of pointer type");
  }
  // 检查存储值类型是否与指定类型匹配
  if (!value->getType()->isEqual(type)) {
    throw std::runtime_error("StoreInst value type must match specified type");
  }
  addOperand(ptr);
  addOperand(value);
}

auto StoreInst::getPtr() const -> Value * { return ptr_; }

auto StoreInst::getValue() const -> Value * { return value_; }

auto StoreInst::getType() const -> Type * { return type_; }

auto StoreInst::getName() const -> std::string {
  throw std::runtime_error("StoreInst cannot be used as a value");
}

auto StoreInst::print() const -> std::string {
  return "store " + type_->print() + " " + value_->getName() + ", ptr " +
         ptr_->getName();
}

// AllocaInst 类实现
AllocaInst::AllocaInst(const std::string &name, Type *type)
    : name_(name), type_(type) {}

auto AllocaInst::getType() const -> Type * {
  // 返回 PointerType（产生值为指针）
  static PointerType ptr_type;
  return &ptr_type;
}

auto AllocaInst::getName() const -> std::string { return "%" + name_; }

auto AllocaInst::print() const -> std::string {
  return getName() + " = alloca " + type_->print();
}

// ICmpInst 类实现
ICmpInst::ICmpInst(const std::string &name, const std::string &predicate,
                   Type *type, Value *lhs, Value *rhs)
    : name_(name), predicate_(predicate), type_(type), lhs_(lhs), rhs_(rhs) {
  // 检查操作数类型是否为整数
  auto *lhs_int_type = dynamic_cast<IntegerType *>(lhs->getType());
  auto *rhs_int_type = dynamic_cast<IntegerType *>(rhs->getType());
  if (!lhs_int_type || !rhs_int_type) {
    throw std::runtime_error("ICmpInst operands must be of integer type");
  }
  // 检查操作数类型是否相同
  if (!lhs->getType()->isEqual(rhs->getType())) {
    throw std::runtime_error("ICmpInst operand types must match");
  }
  // 检查结果类型是否为Int1Type
  auto *result_int1_type = dynamic_cast<Int1Type *>(type);
  if (!result_int1_type) {
    throw std::runtime_error("ICmpInst result type must be Int1Type");
  }
  addOperand(lhs);
  addOperand(rhs);
}

auto ICmpInst::getLHS() const -> Value * { return lhs_; }

auto ICmpInst::getRHS() const -> Value * { return rhs_; }

auto ICmpInst::getPredicate() const -> std::string { return predicate_; }

auto ICmpInst::getType() const -> Type * { return type_; }

auto ICmpInst::getName() const -> std::string { return "%" + name_; }

auto ICmpInst::print() const -> std::string {
  return getName() + " = icmp " + predicate_ + " " + lhs_->getType()->print() +
         " " + lhs_->getName() + ", " + rhs_->getName();
}

// BrInst 类实现
BrInst::BrInst(BasicBlock *target) : target_(target) {}

auto BrInst::getTarget() const -> BasicBlock * { return target_; }

auto BrInst::getType() const -> Type * {
  throw std::runtime_error("BrInst cannot be used as a value");
}

auto BrInst::getName() const -> std::string {
  throw std::runtime_error("BrInst cannot be used as a value");
}

auto BrInst::print() const -> std::string {
  return "br label %" + target_->getName();
}

// ConBrInst 类实现
ConBrInst::ConBrInst(Value *condition, BasicBlock *then_block,
                     BasicBlock *else_block)
    : condition_(condition), then_block_(then_block), else_block_(else_block) {
  // 检查条件值是否为i1类型
  auto *condition_int1_type = dynamic_cast<Int1Type *>(condition->getType());
  if (!condition_int1_type) {
    throw std::runtime_error("ConBrInst condition must be of Int1Type");
  }
  addOperand(condition);
}

auto ConBrInst::getCondition() const -> Value * { return condition_; }

auto ConBrInst::getThenBlock() const -> BasicBlock * { return then_block_; }

auto ConBrInst::getElseBlock() const -> BasicBlock * { return else_block_; }

auto ConBrInst::getType() const -> Type * {
  throw std::runtime_error("ConBrInst cannot be used as a value");
}

auto ConBrInst::getName() const -> std::string {
  throw std::runtime_error("ConBrInst cannot be used as a value");
}

auto ConBrInst::print() const -> std::string {
  return "br i1 " + condition_->getName() + ", label %" +
         then_block_->getName() + ", label %" + else_block_->getName();
}

// ReturnInst 类实现
ReturnInst::ReturnInst(Value *return_value) : return_value_(return_value) {
  if (return_value) {
    addOperand(return_value);
  }
}

auto ReturnInst::getReturnValue() const -> Value * { return return_value_; }

auto ReturnInst::getType() const -> Type * {
  throw std::runtime_error("ReturnInst cannot be used as a value");
}

auto ReturnInst::getName() const -> std::string {
  throw std::runtime_error("ReturnInst cannot be used as a value");
}

auto ReturnInst::print() const -> std::string {
  if (return_value_) {
    return "ret " + return_value_->getType()->print() + " " +
           return_value_->getName();
  } else {
    return "ret void";
  }
}

// PHINode 类实现
PHINode::PHINode(const std::string &name, Type *type)
    : name_(name), type_(type) {
  // 类型信息在构造时设置并存储
}

void PHINode::addIncoming(Value *value, BasicBlock *block) {
  // 检查输入值类型是否与PHI节点类型一致
  if (!value->getType()->isEqual(type_)) {
    throw std::runtime_error(
        "PHINode incoming value type must match PHI node type");
  }
  incomings_.emplace_back(value, block);
  addOperand(value);
}

auto PHINode::getType() const -> Type * { return type_; }

auto PHINode::getName() const -> std::string { return "%" + name_; }

auto PHINode::print() const -> std::string {
  std::string result = getName() + " = phi " + getType()->print();

  for (size_t i = 0; i < incomings_.size(); ++i) {
    result += " [ " + incomings_[i].first->getName() + ", %" +
              incomings_[i].second->getName() + " ]";
    if (i < incomings_.size() - 1) {
      result += ", ";
    }
  }

  return result;
}

// CallInst 类实现
CallInst::CallInst(const std::string &name, Function *function,
                   const std::vector<Value *> &args)
    : name_(name), function_(function), args_(args) {
  // 获取函数类型
  auto *func_type = dynamic_cast<FunctionType *>(function->getType());
  if (!func_type) {
    throw std::runtime_error("CallInst function must have a FunctionType");
  }

  // 检查参数数量是否匹配
  const auto &param_types = func_type->getParamTypes();
  if (args.size() != param_types.size()) {
    throw std::runtime_error(
        "CallInst argument count does not match function parameter count");
  }

  // 检查每个参数类型是否匹配
  for (size_t i = 0; i < args.size(); ++i) {
    if (!args[i]->getType()->isEqual(param_types[i])) {
      throw std::runtime_error(
          "CallInst argument type does not match function parameter type");
    }
  }

  for (auto arg : args) {
    addOperand(arg);
  }
}

auto CallInst::getFunction() const -> Function * { return function_; }

auto CallInst::getArgs() const -> const std::vector<Value *> & { return args_; }

auto CallInst::getType() const -> Type * {
  FunctionType *func_type = dynamic_cast<FunctionType *>(function_->getType());
  assert(func_type && "Function must have a FunctionType");
  return func_type->getReturnType();
}

auto CallInst::getName() const -> std::string { return "%" + name_; }

auto CallInst::print() const -> std::string {
  FunctionType *func_type = dynamic_cast<FunctionType *>(function_->getType());
  assert(func_type && "Function must have a FunctionType");

  std::string result;
  std::string type_name;

  if (!name_.empty() && func_type->getReturnType()) {
    result += getName() + " = ";
    type_name = func_type->getReturnType()->print();
  } else {
    type_name = "void";
  }
  result += "call " + type_name + " @" + function_->getName() + "(";

  for (size_t i = 0; i < args_.size(); ++i) {
    result += args_[i]->getType()->print() + " " + args_[i]->getName();
    if (i < args_.size() - 1) {
      result += ", ";
    }
  }

  result += ")";
  return result;
}

// GetElementPtrInst 类实现
GetElementPtrInst::GetElementPtrInst(const std::string &name, Type *type,
                                     Type *base_type, Value *ptr,
                                     const std::vector<Value *> &indices)
    : name_(name), type_(type), base_type_(base_type), ptr_(ptr),
      indices_(indices) {
  if (!ptr->getType()->isPointerTy()) {
    throw std::runtime_error("GEPInst pointer operand must be of pointer type");
  }

  // 检查所有索引是否为整数类型
  for (auto index : indices) {
    auto *index_int_type = dynamic_cast<IntegerType *>(index->getType());
    if (!index_int_type) {
      throw std::runtime_error("GEPInst indices must be of integer type");
    }
  }

  Type *current_type = base_type;
  for (uint32_t i = 0; i < indices.size(); ++i) {
    auto index = indices[i];
    if (i != indices.size() - 1 && !current_type->isAggregateTy()) {
      throw std::runtime_error("GEPInst index applied to non-aggregate type");
    }
    if (auto *const_index = dynamic_cast<ConstantInt *>(index)) {
      size_t idx = const_index->getValue();
      if (auto array_type = dynamic_cast<ArrayType *>(current_type)) {
        if (idx < 0 || idx >= array_type->getNumElements()) {
          throw std::runtime_error("GEPInst array index out of bounds");
        }
        current_type = array_type->getElementType();
      } else if (auto struct_type = dynamic_cast<StructType *>(current_type)) {
        if (idx < 0 || idx >= struct_type->getNumElements()) {
          throw std::runtime_error("GEPInst struct index out of bounds");
        }
        current_type = struct_type->getElementType(idx);
      }
    } else {
      if (i == 0 && dynamic_cast<StructType *>(current_type)) {
        throw std::runtime_error("GEPInst struct first index must be constant");
      }
      break; // 非常量索引，停止检测。
    }
  }

  addOperand(ptr);
  for (auto index : indices) {
    addOperand(index);
  }
}

auto GetElementPtrInst::getPtr() const -> Value * { return ptr_; }

auto GetElementPtrInst::getIndices() const -> const std::vector<Value *> & {
  return indices_;
}

auto GetElementPtrInst::getType() const -> Type * { return type_; }

auto GetElementPtrInst::getName() const -> std::string { return "%" + name_; }

auto GetElementPtrInst::print() const -> std::string {
  std::string result = getName() + " = getelementptr " + base_type_->print() +
                       ", ptr " + ptr_->getName();

  for (auto index : indices_) {
    result += ", " + index->getType()->print() + " " + index->getName();
  }

  return result;
}

// ConstantInt 类实现
ConstantInt::ConstantInt(IntegerType *type, uint32_t value)
    : type_(type), value_(value) {}

auto ConstantInt::getType() const -> Type * { return type_; }

auto ConstantInt::getValue() const -> uint32_t { return value_; }

auto ConstantInt::getName() const -> std::string {
  return std::to_string(value_);
}

auto ConstantInt::print() const -> std::string {
  return std::to_string(value_);
}

// Argument 类实现
Argument::Argument(const std::string &name, Type *type, Function *parent)
    : name_(name), type_(type), parent_(parent) {}

auto Argument::getType() const -> Type * { return type_; }

auto Argument::getName() const -> std::string { return name_; }

auto Argument::getParent() const -> Function * { return parent_; }

auto Argument::print() const -> std::string {
  return type_->print() + " %" + name_;
}

} // namespace llvm