#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Type.hpp"

namespace llvm {

// 前向声明
class Value;
class User;
class Constant;
class GlobalVariable;
class Function;
class BasicBlock;
class Instruction;
class Argument;
class Module;

// 抽象基类 Value
class Value {
protected:
  std::vector<User *> users_;

public:
  virtual ~Value() = default;

  // 获取值的类型信息
  virtual auto getType() const -> Type * = 0;

  // 获取值的名称
  virtual auto getName() const -> std::string = 0;

  // 打印值信息
  virtual auto print() const -> std::string = 0;

  // 添加使用该值的用户
  void addUser(User *user);

  // 获取使用该值的用户列表
  const std::vector<User *> &getUsers() const;
};

// 中间基类 User
class User : public Value {
protected:
  std::vector<Value *> operands_; // 操作数列表，用于生成 DFG

public:
  virtual ~User() = default;

  // 添加操作数
  void addOperand(Value *operand);

  // 获取操作数列表
  const std::vector<Value *> &getOperands() const;
};

// 全局变量
class GlobalVariable : public Value {
private:
  std::string name_;        // 变量名称
  Type *type_;              // 变量类型
  Constant *initial_value_; // 变量初始值，可以为空表示未初始化
  bool is_constant_;        // 是否为常量

public:
  GlobalVariable(const std::string &name, Type *type, Constant *initial_value, bool is_constant = false);

  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto getInitialValue() const -> Constant *;
  auto isConstant() const -> bool;
  auto print() const -> std::string override;
};

// 函数
class Function : public Value {
private:
  std::string name_;        // 函数名称
  FunctionType *func_type_; // 函数类型
  std::vector<std::unique_ptr<BasicBlock>>
      basic_blocks_; // 函数体内的基本块列表
  std::unordered_map<std::string, size_t>
      bb_name_map_; // 基本块名称到索引的映射，用于防止重名
  std::vector<std::unique_ptr<Argument>> arguments_; // 函数参数列表
  bool is_defined_; // 函数是否已定义（有函数体）
  Module *parent_;  // 所属模块

public:
  Function(const std::string &name, FunctionType *func_type, Module *parent);
  ~Function() override = default;

  void addBasicBlock(std::unique_ptr<BasicBlock> &&bb);
  auto createBasicBlock(const std::string &name) -> BasicBlock *;
  const std::vector<BasicBlock *> getBasicBlocks() const;
  auto getBBbyIndex(size_t index) const -> BasicBlock *;

  void addArgument(std::unique_ptr<Argument> &&arg);
  void setArguments(std::vector<std::unique_ptr<Argument>> &&args);
  const std::vector<Argument *> getArguments() const;
  auto getArgByIndex(size_t index) const -> Argument *;

  auto isDefined() const -> bool;
  auto getType() const -> FunctionType * override;
  auto getParent() const -> Module *;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 基本块
class BasicBlock : public Value {
private:
  std::string name_;                                       // 基本块名称
  Function *parent_;                                       // 所属函数
  std::vector<std::unique_ptr<Instruction>> instructions_; // 基本块内的指令

public:
  BasicBlock(const std::string &name, Function *parent);

  void addInstruction(std::unique_ptr<Instruction> &&inst);
  auto getInstructions() const -> const std::vector<Instruction *>;
  auto isTerminated() const -> bool;
  auto getTerminator() const -> Instruction *;

  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto getParent() const -> Function *;
  auto print() const -> std::string override;
};

// 指令基类
class Instruction : public User {
public:
  virtual ~Instruction() = default;
};

// 二元运算指令
class BinaryOperator : public Instruction {
private:
  std::string name_; // 指令对应Value（结果寄存器）的名称
  std::string op_;   // 操作码（如 "add", "sub" 等）
  Type *type_;       // 结果类型
  Value *lhs_;       // 左操作数
  Value *rhs_;       // 右操作数

public:
  BinaryOperator(const std::string &name, Type *type, Value *lhs, Value *rhs,
                 const std::string &op);

  auto getLHS() const -> Value *;
  auto getRHS() const -> Value *;
  auto getOp() const -> std::string;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 一元运算指令
class UnaryOperator : public Instruction {
private:
  std::string name_; // 指令对应Value（结果寄存器）的名称
  std::string op_;   // 操作码（如 "neg", "not" 等）
  Type *type_;       // 结果类型
  Value *operand_;   // 操作数

public:
  UnaryOperator(const std::string &name, const std::string &op, Type *type,
                Value *operand);

  auto getOperand() const -> Value *;
  auto getOp() const -> std::string;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 加载指令
class LoadInst : public Instruction {
private:
  std::string name_; // 指令对应 Value（结果寄存器）的名称
  Type *type_;       // 结果类型
  Value *ptr_;       // 指向加载地址的指针

public:
  LoadInst(const std::string &name, Type *type, Value *ptr);

  auto getPtr() const -> Value *;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 存储指令
class StoreInst : public Instruction {
private:
  Type *type_;   // 存储值的类型
  Value *ptr_;   // 指向存储地址的指针
  Value *value_; // 要存储的值

public:
  StoreInst(Type *type, Value *ptr, Value *value);

  auto getPtr() const -> Value *;
  auto getValue() const -> Value *;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 内存分配指令
class AllocaInst : public Instruction {
private:
  std::string name_; // 指令对应 Value（结果寄存器）的名称，对应一段栈上空间
  Type *type_; // 分配空间的类型

public:
  AllocaInst(const std::string &name, Type *type);

  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 整数比较指令
class ICmpInst : public Instruction {
private:
  std::string name_;      // 指令对应 Value（结果寄存器）的名称
  std::string predicate_; // 比较谓词（如 "eq", "ne", "slt" 等）
  Type *type_;            // 结果类型（通常为 Int1Type）
  Value *lhs_;            // 左操作数
  Value *rhs_;            // 右操作数

public:
  ICmpInst(const std::string &name, const std::string &predicate, Type *type,
           Value *lhs, Value *rhs);

  auto getLHS() const -> Value *;
  auto getRHS() const -> Value *;
  auto getPredicate() const -> std::string;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 终止指令基类
class TerminatorInst : public Instruction {
public:
  virtual ~TerminatorInst() = default;
};

// 无条件跳转指令
class BrInst : public TerminatorInst {
private:
  BasicBlock *target_; // 跳转目标基本块

public:
  BrInst(BasicBlock *target);

  auto getTarget() const -> BasicBlock *;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 条件分支跳转指令
class ConBrInst : public TerminatorInst {
private:
  Value *condition_;       // 条件值
  BasicBlock *then_block_; // 条件为真时跳转的基本块
  BasicBlock *else_block_; // 条件为假时跳转的基本块

public:
  ConBrInst(Value *condition, BasicBlock *then_block, BasicBlock *else_block);

  auto getCondition() const -> Value *;
  auto getThenBlock() const -> BasicBlock *;
  auto getElseBlock() const -> BasicBlock *;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 返回指令
class ReturnInst : public TerminatorInst {
private:
  Value *return_value_; // 返回值，可以为空表示无返回值

public:
  ReturnInst(Value *return_value);

  auto getReturnValue() const -> Value *;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// PHI节点
class PHINode : public Instruction {
private:
  std::string name_; // 指令对应 Value（结果寄存器）的名称
  Type *type_;       // 结果类型
  std::vector<std::pair<Value *, BasicBlock *>>
      incomings_; // 输入值及其对应的前驱基本块列表

public:
  PHINode(const std::string &name, Type *type);

  void addIncoming(Value *value, BasicBlock *block);
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 函数调用指令
class CallInst : public Instruction {
private:
  std::string name_;   // 指令对应 Value（结果寄存器）的名称
  Function *function_; // 被调用的函数
  std::vector<Value *> args_; // 函数调用参数列表

public:
  CallInst(const std::string &name, Function *function,
           const std::vector<Value *> &args);

  auto getFunction() const -> Function *;
  auto getArgs() const -> const std::vector<Value *> &;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 地址计算指令
class GetElementPtrInst : public Instruction {
private:
  std::string name_; // 指令对应 Value（结果寄存器）的名称
  Type *type_;       // 结果类型（指针类型）
  Type *base_type_;  // 第一个索引指向元素类型
  Value *ptr_;       // 基础指针操作数
  std::vector<Value *> indices_; // 索引操作数列表

public:
  GetElementPtrInst(const std::string &name, Type *type, Type *base_type,
                    Value *ptr, const std::vector<Value *> &indices);

  auto getPtr() const -> Value *;
  auto getIndices() const -> const std::vector<Value *> &;
  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 常量值基类
class Constant : public Value {
public:
  virtual ~Constant() = default;
};

// 整数类型常量
class ConstantInt : public Constant {
private:
  IntegerType *type_; // 整数类型，可能为32或8或1位
  uint32_t value_;    // 整数值

public:
  ConstantInt(IntegerType *type, uint32_t value);

  auto getType() const -> Type * override;
  auto getValue() const -> uint32_t;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 结构体常量
class ConstantStruct : public Constant {
private:
  StructType* type_; // 结构体类型
  std::vector<std::unique_ptr<Constant>> elements_; // 结构体字段常量列表

public:
  ConstantStruct(StructType* type, std::vector<std::unique_ptr<Constant>>&& elements);

  auto getType() const -> StructType*;
  const std::vector<Constant*>& getElements() const;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 数组常量
class ConstantArray : public Constant {
private:
  ArrayType* type_; // 数组类型
  std::vector<std::unique_ptr<Constant>> elements_; // 数组元素常量列表

public:
  ConstantArray(ArrayType* type, std::vector<std::unique_ptr<Constant>>&& elements);

  auto getType() const -> ArrayType*;
  const std::vector<Constant*>& getElements() const;
  auto getName() const -> std::string override;
  auto print() const -> std::string override;
};

// 函数参数
class Argument : public Value {
private:
  std::string name_; // 参数名称
  Type *type_;       // 参数类型
  Function *parent_; // 所属函数

public:
  Argument(const std::string &name, Type *type, Function *parent);

  auto getType() const -> Type * override;
  auto getName() const -> std::string override;
  auto getParent() const -> Function *;
  auto print() const -> std::string override;
};

} // namespace llvm
