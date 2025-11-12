#include "IRBuilder.hpp"
#include "Module.hpp"
#include <memory>

namespace llvm {

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
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "add");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateSub(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "sub");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateMul(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "mul");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateSDiv(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "sdiv");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateUDiv(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "udiv");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateSRem(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "srem");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateURem(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "urem");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateShl(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "shl");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateAShr(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "ashr");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateLShr(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "lshr");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateAnd(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "and");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateOr(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "or");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateXor(Value *LHS, Value *RHS, const std::string &name)
    -> BinaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto bin_op =
      std::make_unique<BinaryOperator>(name, LHS->getType(), LHS, RHS, "xor");
  auto result = bin_op.get();
  insert_block_->addInstruction(std::move(bin_op));
  return result;
}

auto IRBuilder::CreateNeg(Value *operand, const std::string &name)
    -> UnaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto unary_op =
      std::make_unique<UnaryOperator>(name, "neg", operand->getType(), operand);
  auto result = unary_op.get();
  insert_block_->addInstruction(std::move(unary_op));
  return result;
}

auto IRBuilder::CreateNot(Value *operand, const std::string &name)
    -> UnaryOperator * {
  if (!insert_block_) {
    return nullptr;
  }

  auto unary_op =
      std::make_unique<UnaryOperator>(name, "not", operand->getType(), operand);
  auto result = unary_op.get();
  insert_block_->addInstruction(std::move(unary_op));
  return result;
}

auto IRBuilder::CreateAlloca(Type *type, Value *array_size,
                             const std::string &name) -> AllocaInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto alloca = std::make_unique<AllocaInst>(name, type);
  auto result = alloca.get();
  insert_block_->addInstruction(std::move(alloca));
  return result;
}

auto IRBuilder::CreateLoad(Type *type, Value *ptr, const std::string &name)
    -> LoadInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto load = std::make_unique<LoadInst>(name, type, ptr);
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
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "eq", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpNE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "ne", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpSLT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "slt", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpSLE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "sle", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpSGT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "sgt", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpSGE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "sge", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpULT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "ult", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpULE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "ule", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpUGT(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "ugt", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreateICmpUGE(Value *LHS, Value *RHS, const std::string &name)
    -> ICmpInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto icmp =
      std::make_unique<ICmpInst>(name, "uge", context_->getInt1Ty(), LHS, RHS);
  auto result = icmp.get();
  insert_block_->addInstruction(std::move(icmp));
  return result;
}

auto IRBuilder::CreatePHI(Type *type, const std::string &name) -> PHINode * {
  if (!insert_block_) {
    return nullptr;
  }

  auto phi = std::make_unique<PHINode>(name, type);
  auto result = phi.get();
  insert_block_->addInstruction(std::move(phi));
  return result;
}

auto IRBuilder::CreateCall(Function *func, const std::vector<Value *> &args,
                           const std::string &name) -> CallInst * {
  if (!insert_block_) {
    return nullptr;
  }

  auto call = std::make_unique<CallInst>(name, func, args);
  auto result = call.get();
  insert_block_->addInstruction(std::move(call));
  return result;
}

auto IRBuilder::CreateGEP(Type *type, Value *ptr,
                          const std::vector<Value *> &indices,
                          const std::string &name) -> GetElementPtrInst * {
  if (!insert_block_) {
    return nullptr;
  }

  Type *pointer_type = context_->getPointerType();

  auto gep = std::make_unique<GetElementPtrInst>(name, pointer_type, type, ptr,
                                                 indices);
  auto result = gep.get();
  insert_block_->addInstruction(std::move(gep));
  return result;
}

auto IRBuilder::CreateMemCpy(Value *dest, Value *src, uint64_t size,
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

  // 创建memcpy函数类型：void (ptr, ptr, i64, i1)
  Type *void_type = context_->getInt8Ty(); // 使用i8作为void的替代
  Type *ptr_type = context_->getPointerType();
  Type *i64_type = context_->getInt32Ty(); // 使用i32作为i64的替代
  Type *i1_type = context_->getInt1Ty();

  std::vector<Type *> param_types = {ptr_type, ptr_type, i64_type, i1_type};
  FunctionType *memcpy_type = context_->getFunctionType(void_type, param_types);

  // 获取或创建memcpy函数声明
  Function *memcpy_func =
      module->getOrCreateFunction("memcpy.p0.p0.i64", memcpy_type);

  // 创建函数调用参数
  std::vector<Value *> args;
  args.push_back(dest); // 目标地址
  args.push_back(src);  // 源地址

  // 创建size参数的常量
  ConstantInt *size_const =
      context_->getIntConstant(static_cast<IntegerType *>(i64_type), size);
  args.push_back(size_const); // 拷贝大小

  // 创建is_violatile参数的常量
  ConstantInt *volatile_const = context_->getIntConstant(
      static_cast<IntegerType *>(i1_type), is_violatile ? 1 : 0);
  args.push_back(volatile_const); // 是否易失

  // 创建函数调用指令
  return CreateCall(memcpy_func, args, "");
}

} // namespace llvm