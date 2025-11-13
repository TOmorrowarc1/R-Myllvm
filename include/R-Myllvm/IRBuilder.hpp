#pragma once

#include <string>
#include <vector>

#include "Context.hpp"
#include "Value.hpp"

namespace llvm {

// 前向声明
class LLVMContext;
class BasicBlock;
class Function;
class Module;

// IRBuilder 类，用于创建和插入 LLVM IR 指令
class IRBuilder {
private:
  LLVMContext *context_;     // 关联上下文指针
  BasicBlock *insert_block_; // 当前插入点基本块指针

  auto genLLVMReg() -> std::string; // 生成唯一寄存器名称
  auto CreateBinaryOp(Value *LHS, Value *RHS, const std::string &name,
                      const std::string &op_name)
      -> BinaryOperator *; // 创建二元运算的辅助函数
  auto CreateUnaryOp(Value *operand, const std::string &name,
                     const std::string &op_name)
      -> UnaryOperator *; // 创建一元运算的辅助函数
  auto CreateICmp(Value *LHS, Value *RHS, const std::string &name,
                  const std::string &pred)
      -> ICmpInst *; // 创建整数比较的辅助函数

public:
  // 构造函数，创建 IR 构建器
  IRBuilder(LLVMContext *context);

  // 位置设计接口
  void SetInsertPoint(BasicBlock *point); // 设置插入点
  auto GetInsertBlock() -> BasicBlock *;  // 获取当前插入块
  auto GetInsertFunction() -> Function *; // 获取当前函数

  // 指令创建接口
  // 算术运算指令
  auto CreateAdd(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateSub(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateMul(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateSDiv(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateUDiv(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateSRem(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateURem(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateShl(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateAShr(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateLShr(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateAnd(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateOr(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;
  auto CreateXor(Value *LHS, Value *RHS, const std::string &name = "")
      -> BinaryOperator *;

  // 一元运算指令
  auto CreateNeg(Value *operand, const std::string &name = "")
      -> UnaryOperator *;
  auto CreateNot(Value *operand, const std::string &name = "")
      -> UnaryOperator *;

  // 内存访问指令
  auto CreateAlloca(Type *type, const std::string &name = "") -> AllocaInst *;
  auto CreateLoad(Type *type, Value *ptr, const std::string &name = "")
      -> LoadInst *;
  auto CreateStore(Value *value, Value *ptr) -> StoreInst *;

  // 控制流指令
  auto CreateRet(Value *value) -> ReturnInst *;
  auto CreateBr(BasicBlock *dest) -> BrInst *;
  auto CreateCondBr(Value *cond, BasicBlock *then_bb, BasicBlock *else_bb)
      -> ConBrInst *;

  // 比较指令
  auto CreateICmpEQ(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpNE(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpSLT(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpSLE(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpSGT(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpSGE(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpULT(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpULE(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpUGT(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;
  auto CreateICmpUGE(Value *LHS, Value *RHS, const std::string &name = "")
      -> ICmpInst *;

  // PHI节点
  auto CreatePHI(Type *type, const std::string &name = "") -> PHINode *;

  // 其他指令
  auto CreateCall(Function *func, const std::vector<Value *> &args,
                  const std::string &name = "") -> CallInst *;
  auto CreateGEP(Type *type, Value *ptr, const std::vector<Value *> &indices,
                 const std::string &name = "") -> GetElementPtrInst *;
  auto CreateMemCpy(Value *dest, Value *src, uint64_t size, bool is_violatile)
      -> CallInst *;
};

} // namespace llvm
