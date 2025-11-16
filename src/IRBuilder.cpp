#include "IRBuilder.hpp"
#include "Module.hpp"
#include <memory>

namespace llvm {

auto IRBuilder::genLLVMReg() -> std::string {
  static uint64_t counter = 0;
  return std::to_string(++counter);
}

auto IRBuilder::CreateBinaryOp(Value *LHS, Value *RHS, const std::string &name,
                               const std::string &op_name) -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto bin_op = std::make_unique<BinaryOperator>(actual_name, LHS->getType(),
                                                 LHS, RHS, op_name);
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateUnaryOp(Value *operand, const std::string &name,
                              const std::string &op_name) -> UnaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto unary_op = std::make_unique<UnaryOperator>(actual_name, op_name,
                                                  operand->getType(), operand);
  auto result = unary_op.get();
  insert_block_->addInstruction(std::move(unary_op));
  return result;
}

auto IRBuilder::CreateICmp(Value *LHS, Value *RHS, const std::string &name,
                           const std::string &pred) -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto icmp = std::make_unique<ICmpInst>(actual_name, pred,
                                         context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

IRBuilder::IRBuilder(LLVMContext *context)
    : context_(context), insert_block_(nullptr) {}

void IRBuilder::SetInsertPoint(BasicBlock *point) { insert_block_ = point; }

auto IRBuilder::GetInsertBlock() -> BasicBlock * { return insert_block_; }

auto IRBuilder::GetInsertFunction() -> Function * {
  if (insert_block_) {
    return insert_block_->getParent();
  }
  return nullptr;
}

auto IRBuilder::CreateAdd(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "add");
}

auto IRBuilder::CreateSub(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "sub");
}

auto IRBuilder::CreateMul(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "mul");
}

auto IRBuilder::CreateSDiv(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "sdiv");
}

auto IRBuilder::CreateUDiv(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "udiv");
}

auto IRBuilder::CreateSRem(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "srem");
}

auto IRBuilder::CreateURem(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "urem");
}

auto IRBuilder::CreateShl(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "shl");
}

auto IRBuilder::CreateAShr(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "ashr");
}

auto IRBuilder::CreateLShr(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "lshr");
}

auto IRBuilder::CreateAnd(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "and");
}

auto IRBuilder::CreateOr(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "or");
}

auto IRBuilder::CreateXor(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  return CreateBinaryOp(LHS, RHS, name, "xor");
}

auto IRBuilder::CreateNeg(Value *operand, const std::string &name)
    -> UnaryOperator * {
  return CreateUnaryOp(operand, name, "neg");
}

auto IRBuilder::CreateNot(Value *operand, const std::string &name)
    -> UnaryOperator * {
  return CreateUnaryOp(operand, name, "not");
}

auto IRBuilder::CreateAlloca(Type *type, const std::string &name)
    -> AllocaInst * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto alloca = std::make_unique<AllocaInst>(actual_name, type);
  auto result = alloca.get();
  insert_block_->addInstruction(std::move(alloca));
  return result;
}

auto IRBuilder::CreateLoad(Type *type, Value *ptr, const std::string &name)
    -> LoadInst * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto load = std::make_unique<LoadInst>(actual_name, type, ptr);
  auto result = load.get();
  insert_block_->addInstruction(std::move(load));
  return result;
}

auto IRBuilder::CreateStore(Value *value, Value *ptr) -> StoreInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto store = std::make_unique<StoreInst>(value->getType(), ptr, value);
  auto result = store.get();
  insert_block_->addInstruction(std::move(store));
  return result;
}

auto IRBuilder::CreateRet(Value *value) -> ReturnInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto ret = std::make_unique<ReturnInst>(value);
  auto result = ret.get();
  insert_block_->addInstruction(std::move(ret));
  return result;
}

auto IRBuilder::CreateBr(BasicBlock *dest) -> BrInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto br = std::make_unique<BrInst>(dest);
  auto result = br.get();
  insert_block_->addInstruction(std::move(br));
  return result;
}

auto IRBuilder::CreateCondBr(Value *cond, BasicBlock *then_bb,
                             BasicBlock *else_bb) -> ConBrInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto cond_br = std::make_unique<ConBrInst>(cond, then_bb, else_bb);
  auto result = cond_br.get();
  insert_block_->addInstruction(std::move(cond_br));
  return result;
}

auto IRBuilder::CreateICmpEQ(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "eq");
}

auto IRBuilder::CreateICmpNE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "ne");
}

auto IRBuilder::CreateICmpSLT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "slt");
}

auto IRBuilder::CreateICmpSLE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "sle");
}

auto IRBuilder::CreateICmpSGT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "sgt");
}

auto IRBuilder::CreateICmpSGE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "sge");
}

auto IRBuilder::CreateICmpULT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "ult");
}

auto IRBuilder::CreateICmpULE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "ule");
}

auto IRBuilder::CreateICmpUGT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "ugt");
}

auto IRBuilder::CreateICmpUGE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  return CreateICmp(LHS, RHS, name, "uge");
}

auto IRBuilder::CreatePHI(Type *type, const std::string &name) -> PHINode * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto phi = std::make_unique<PHINode>(actual_name, type);
  auto result = phi.get();
  insert_block_->addInstruction(std::move(phi));
  return result;
}

auto IRBuilder::CreateCall(Function *func, const std::vector<Value *> &args,
                           const std::string &name) -> CallInst * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto call = std::make_unique<CallInst>(actual_name, func, args);
  auto result = call.get();
  insert_block_->addInstruction(std::move(call));
  return result;
}

auto IRBuilder::CreatePtrToInt(Type *type, Value *ptr, const std::string &name)
    -> PtrToIntInst * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  auto ptr_to_int = std::make_unique<PtrToIntInst>(actual_name, type, ptr);
  auto result = ptr_to_int.get();
  insert_block_->addInstruction(std::move(ptr_to_int));
  return result;
}

auto IRBuilder::CreateGEP(Type *type, Value *ptr,
                          const std::vector<Value *> &indices,
                          const std::string &name) -> GetElementPtrInst * {
  if (!insert_block_) {
    return nullptr;
  }

  std::string actual_name = name.empty() ? genLLVMReg() : name;
  Type *pointer_type = context_->getPointerType();

  auto gep = std::make_unique<GetElementPtrInst>(actual_name, pointer_type,
                                                 type, ptr, indices);
  auto result = gep.get();
  insert_block_->addInstruction(std::move(gep));
  return result;
}

auto IRBuilder::CreateMemCpy(Value *dest, Value *src, Value *size,
                             bool is_violatile) -> CallInst * {
  if (!insert_block_) {
    return nullptr;
  }

  // 获取当前函数所属的Module
  Function *current_func = GetInsertFunction();
  if (!current_func) {
    return nullptr;
  }

  Module *module = current_func->getParent();
  if (!module) {
    return nullptr;
  }

  // 检测size类型是否为i32整数类型
  if (size->getType() != context_->getInt32Ty()) {
    return nullptr;
  }

  // 创建memcpy函数类型：void (ptr, ptr, i32, i1)
  Type *void_type = context_->getVoidTy();
  Type *ptr_type = context_->getPointerType();
  Type *i1_type = context_->getInt1Ty();

  std::vector<Type *> param_types = {ptr_type, ptr_type, context_->getInt32Ty(), i1_type};
  FunctionType *memcpy_type = context_->getFunctionType(void_type, param_types);

  // 获取或创建memcpy函数声明
  Function *memcpy_func =
      module->getOrCreateFunction("llvm.memcpy.p0.p0.i32", memcpy_type);

  // 创建函数调用参数
  std::vector<Value *> args;
  args.push_back(dest); // 目标地址
  args.push_back(src);  // 源地址
  args.push_back(size); // 拷贝大小

  // 创建is_violatile参数的常量
  ConstantInt *volatile_const = context_->getIntConstant(
      static_cast<IntegerType *>(context_->getInt1Ty()), is_violatile ? 1 : 0);
  args.push_back(volatile_const); // 是否易失

  // 创建函数调用指令
  return CreateCall(memcpy_func, args, "");
}

} // namespace llvm