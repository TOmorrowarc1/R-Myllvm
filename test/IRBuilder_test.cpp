
#include "Context.hpp"
#include "IRBuilder.hpp"
#include "Module.hpp"
#include "Type.hpp"
#include "Value.hpp"
#include "gtest/gtest.h"

namespace llvm {

// 测试IRBuilder的基本功能
TEST(IRBuilderTest, BasicFunctionalityTest) {
  LLVMContext context;
  Module module("test_module", &context);
  IRBuilder builder(&context);

  // 测试初始状态下获取插入块和函数
  EXPECT_EQ(builder.GetInsertBlock(), nullptr);
  EXPECT_EQ(builder.GetInsertFunction(), nullptr);

  // 创建函数类型和函数
  Type *i32_type = context.getInt32Ty();
  std::vector<Type *> param_types = {};
  FunctionType *func_type = context.getFunctionType(i32_type, param_types);
  Function *function = module.getOrCreateFunction("test_func", func_type);

  // 创建基本块
  BasicBlock *bb = function->createBasicBlock("entry");
  
  // 设置插入点
  builder.SetInsertPoint(bb);
  
  // 测试设置插入点后获取插入块和函数
  EXPECT_EQ(builder.GetInsertBlock(), bb);
  EXPECT_EQ(builder.GetInsertFunction(), function);
}

// 测试算术运算指令创建
TEST(IRBuilderTest, ArithmeticInstructionTest) {
  LLVMContext context;
  Module module("test_module", &context);
  IRBuilder builder(&context);

  // 创建函数类型和函数
  Type *i32_type = context.getInt32Ty();
  std::vector<Type *> param_types = {};
  FunctionType *func_type = context.getFunctionType(i32_type, param_types);
  Function *function = module.getOrCreateFunction("test_func", func_type);

  // 创建基本块并设置插入点
  BasicBlock *bb = function->createBasicBlock("entry");
  builder.SetInsertPoint(bb);

  // 获取类型和常量
  ConstantInt *lhs =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 10);
  ConstantInt *rhs =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 20);

  // 测试加法指令
  BinaryOperator *add_inst = builder.CreateAdd(lhs, rhs, "add_result");
  ASSERT_NE(add_inst, nullptr);
  EXPECT_EQ(add_inst->getName(), "%add_result");
  EXPECT_EQ(add_inst->getOp(), "add");
  EXPECT_EQ(add_inst->getLHS(), lhs);
  EXPECT_EQ(add_inst->getRHS(), rhs);
  EXPECT_EQ(add_inst->getType(), i32_type);

  // 测试减法指令
  BinaryOperator *sub_inst = builder.CreateSub(lhs, rhs, "sub_result");
  ASSERT_NE(sub_inst, nullptr);
  EXPECT_EQ(sub_inst->getName(), "%sub_result");
  EXPECT_EQ(sub_inst->getOp(), "sub");
  EXPECT_EQ(sub_inst->getLHS(), lhs);
  EXPECT_EQ(sub_inst->getRHS(), rhs);
  EXPECT_EQ(sub_inst->getType(), i32_type);

  // 测试乘法指令
  BinaryOperator *mul_inst = builder.CreateMul(lhs, rhs, "mul_result");
  ASSERT_NE(mul_inst, nullptr);
  EXPECT_EQ(mul_inst->getName(), "%mul_result");
  EXPECT_EQ(mul_inst->getOp(), "mul");
  EXPECT_EQ(mul_inst->getLHS(), lhs);
  EXPECT_EQ(mul_inst->getRHS(), rhs);
  EXPECT_EQ(mul_inst->getType(), i32_type);

  // 测试有符号除法指令
  BinaryOperator *sdiv_inst = builder.CreateSDiv(lhs, rhs, "sdiv_result");
  ASSERT_NE(sdiv_inst, nullptr);
  EXPECT_EQ(sdiv_inst->getName(), "%sdiv_result");
  EXPECT_EQ(sdiv_inst->getOp(), "sdiv");
  EXPECT_EQ(sdiv_inst->getLHS(), lhs);
  EXPECT_EQ(sdiv_inst->getRHS(), rhs);
  EXPECT_EQ(sdiv_inst->getType(), i32_type);

  // 测试无符号除法指令
  BinaryOperator *udiv_inst = builder.CreateUDiv(lhs, rhs, "udiv_result");
  ASSERT_NE(udiv_inst, nullptr);
  EXPECT_EQ(udiv_inst->getName(), "%udiv_result");
  EXPECT_EQ(udiv_inst->getOp(), "udiv");
  EXPECT_EQ(udiv_inst->getLHS(), lhs);
  EXPECT_EQ(udiv_inst->getRHS(), rhs);
  EXPECT_EQ(udiv_inst->getType(), i32_type);

  // 测试有符号取模指令
  BinaryOperator *srem_inst = builder.CreateSRem(lhs, rhs, "srem_result");
  ASSERT_NE(srem_inst, nullptr);
  EXPECT_EQ(srem_inst->getName(), "%srem_result");
  EXPECT_EQ(srem_inst->getOp(), "srem");
  EXPECT_EQ(srem_inst->getLHS(), lhs);
  EXPECT_EQ(srem_inst->getRHS(), rhs);
  EXPECT_EQ(srem_inst->getType(), i32_type);

  // 测试无符号取模指令
  BinaryOperator *urem_inst = builder.CreateURem(lhs, rhs, "urem_result");
  ASSERT_NE(urem_inst, nullptr);
  EXPECT_EQ(urem_inst->getName(), "%urem_result");
  EXPECT_EQ(urem_inst->getOp(), "urem");
  EXPECT_EQ(urem_inst->getLHS(), lhs);
  EXPECT_EQ(urem_inst->getRHS(), rhs);
  EXPECT_EQ(urem_inst->getType(), i32_type);
}

// 测试位运算指令创建
TEST(IRBuilderTest, BitwiseInstructionTest) {
  LLVMContext context;
  Module module("test_module", &context);
  IRBuilder builder(&context);

  // 创建函数类型和函数
  Type *i32_type = context.getInt32Ty();
  std::vector<Type *> param_types = {};
  FunctionType *func_type = context.getFunctionType(i32_type, param_types);
  Function *function = module.getOrCreateFunction("test_func", func_type);

  // 创建基本块并设置插入点
  BasicBlock *bb = function->createBasicBlock("entry");
  builder.SetInsertPoint(bb);

  // 获取类型和常量
  ConstantInt *lhs =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 0x0F);
  ConstantInt *rhs =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 0xF0);

  // 测试左移指令
  BinaryOperator *shl_inst = builder.CreateShl(lhs, rhs, "shl_result");
  ASSERT_NE(shl_inst, nullptr);
  EXPECT_EQ(shl_inst->getName(), "%shl_result");
  EXPECT_EQ(shl_inst->getOp(), "shl");
  EXPECT_EQ(shl_inst->getLHS(), lhs);
  EXPECT_EQ(shl_inst->getRHS(), rhs);
  EXPECT_EQ(shl_inst->getType(), i32_type);

  // 测试算术右移指令
  BinaryOperator *ashr_inst = builder.CreateAShr(lhs, rhs, "ashr_result");
  ASSERT_NE(ashr_inst, nullptr);
  EXPECT_EQ(ashr_inst->getName(), "%ashr_result");
  EXPECT_EQ(ashr_inst->getOp(), "ashr");
  EXPECT_EQ(ashr_inst->getLHS(), lhs);
  EXPECT_EQ(ashr_inst->getRHS(), rhs);
  EXPECT_EQ(ashr_inst->getType(), i32_type);

  // 测试逻辑右移指令
  BinaryOperator *lshr_inst = builder.CreateLShr(lhs, rhs, "lshr_result");
  ASSERT_NE(lshr_inst, nullptr);
  EXPECT_EQ(lshr_inst->getName(), "%lshr_result");
  EXPECT_EQ(lshr_inst->getOp(), "lshr");
  EXPECT_EQ(lshr_inst->getLHS(), lhs);
  EXPECT_EQ(lshr_inst->getRHS(), rhs);
  EXPECT_EQ(lshr_inst->getType(), i32_type);

  // 测试按位与指令
  BinaryOperator *and_inst = builder.CreateAnd(lhs, rhs, "and_result");
  ASSERT_NE(and_inst, nullptr);
  EXPECT_EQ(and_inst->getName(), "%and_result");
  EXPECT_EQ(and_inst->getOp(), "and");
  EXPECT_EQ(and_inst->getLHS(), lhs);
  EXPECT_EQ(and_inst->getRHS(), rhs);
  EXPECT_EQ(and_inst->getType(), i32_type);

  // 测试按位或指令
  BinaryOperator *or_inst = builder.CreateOr(lhs, rhs, "or_result");
  ASSERT_NE(or_inst, nullptr);
  EXPECT_EQ(or_inst->getName(), "%or_result");
  EXPECT_EQ(or_inst->getOp(), "or");
  EXPECT_EQ(or_inst->getLHS(), lhs);
  EXPECT_EQ(or_inst->getRHS(), rhs);
  EXPECT_EQ(or_inst->getType(), i32_type);

  // 测试按位异或指令
  BinaryOperator *xor_inst = builder.CreateXor(lhs, rhs, "xor_result");
  ASSERT_NE(xor_inst, nullptr);
  EXPECT_EQ(xor_inst->getName(), "%xor_result");
  EXPECT_EQ(xor_inst->getOp(), "xor");
  EXPECT_EQ(xor_inst->getLHS(), lhs);
  EXPECT_EQ(xor_inst->getRHS(), rhs);
  EXPECT_EQ(xor_inst->getType(), i32_type);
}

// 测试一元运算指令创建
TEST(IRBuilderTest, UnaryInstructionTest) {
  LLVMContext context;
  Module module("test_module", &context);
  IRBuilder builder(&context);

  // 创建函数类型和函数
  Type *i32_type = context.getInt32Ty();
  std::vector<Type *> param_types = {};
  FunctionType *func_type = context.getFunctionType(i32_type, param_types);
  Function *function = module.getOrCreateFunction("test_func", func_type);

  // 创建基本块并设置插入点
  BasicBlock *bb = function->createBasicBlock("entry");
  builder.SetInsertPoint(bb);

  // 获取类型和常量
  ConstantInt *operand =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 42);

  // 测试取负指令
  UnaryOperator *neg_inst = builder.CreateNeg(operand, "neg_result");
  ASSERT_NE(neg_inst, nullptr);
  EXPECT_EQ(neg_inst->getName(), "%neg_result");
  EXPECT_EQ(neg_inst->getOp(), "neg");
  EXPECT_EQ(neg_inst->getOperand(), operand);
  EXPECT_EQ(neg_inst->getType(), i32_type);

  // 测试按位取反指令
  UnaryOperator *not_inst = builder.CreateNot(operand, "not_result");
  ASSERT_NE(not_inst, nullptr);
  EXPECT_EQ(not_inst->getName(), "%not_result");
  EXPECT_EQ(not_inst->getOp(), "not");
  EXPECT_EQ(not_inst->getOperand(), operand);
  EXPECT_EQ(not_inst->getType(), i32_type);
}

// 测试内存访问指令创建
TEST(IRBuilderTest, MemoryAccessInstructionTest) {
  LLVMContext context;
  Module module("test_module", &context);
  IRBuilder builder(&context);

  // 创建函数类型和函数
  Type *i32_type = context.getInt32Ty();
  std::vector<Type *> param_types = {};
  FunctionType *func_type = context.getFunctionType(i32_type, param_types);
  Function *function = module.getOrCreateFunction("test_func", func_type);

  // 创建基本块并设置插入点
  BasicBlock *bb = function->createBasicBlock("entry");
  builder.SetInsertPoint(bb);

  // 获取类型
  PointerType *ptr_type = context.getPointerType();

  // 测试内存分配指令
  AllocaInst *alloca_inst = builder.CreateAlloca(i32_type, "ptr");
  ASSERT_NE(alloca_inst, nullptr);
  EXPECT_EQ(alloca_inst->getName(), "%ptr");
  EXPECT_TRUE(alloca_inst->getType()->isPointerTy());

  // 测试加载指令
  LoadInst *load_inst =
      builder.CreateLoad(i32_type, alloca_inst, "load_result");
  ASSERT_NE(load_inst, nullptr);
  EXPECT_EQ(load_inst->getName(), "%load_result");
  EXPECT_EQ(load_inst->getType(), i32_type);
  EXPECT_EQ(load_inst->getPtr(), alloca_inst);

  // 测试存储指令
  ConstantInt *value =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 100);
  StoreInst *store_inst = builder.CreateStore(value, alloca_inst);
  ASSERT_NE(store_inst, nullptr);
  EXPECT_EQ(store_inst->getType(), i32_type);
  EXPECT_EQ(store_inst->getValue(), value);
  EXPECT_EQ(store_inst->getPtr(), alloca_inst);
}

// 测试比较指令创建
TEST(IRBuilderTest, ComparisonInstructionTest) {
  LLVMContext context;
  Module module("test_module", &context);
  IRBuilder builder(&context);

  // 创建函数类型和函数
  Type *i32_type = context.getInt32Ty();
  std::vector<Type *> param_types = {};
  FunctionType *func_type = context.getFunctionType(i32_type, param_types);
  Function *function = module.getOrCreateFunction("test_func", func_type);

  // 创建基本块并设置插入点
  BasicBlock *bb = function->createBasicBlock("entry");
  builder.SetInsertPoint(bb);

  // 获取类型和常量
  Type *i1_type = context.getInt1Ty();
  ConstantInt *lhs =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 10);
  ConstantInt *rhs =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 20);

  // 测试等于比较指令
  ICmpInst *icmp_eq_inst = builder.CreateICmpEQ(lhs, rhs, "icmp_eq_result");
  ASSERT_NE(icmp_eq_inst, nullptr);
  EXPECT_EQ(icmp_eq_inst->getName(), "%icmp_eq_result");
  EXPECT_EQ(icmp_eq_inst->getPredicate(), "eq");
  EXPECT_EQ(icmp_eq_inst->getType(), i1_type);
  EXPECT_EQ(icmp_eq_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_eq_inst->getRHS(), rhs);

  // 测试不等于比较指令
  ICmpInst *icmp_ne_inst = builder.CreateICmpNE(lhs, rhs, "icmp_ne_result");
  ASSERT_NE(icmp_ne_inst, nullptr);
  EXPECT_EQ(icmp_ne_inst->getName(), "%icmp_ne_result");
  EXPECT_EQ(icmp_ne_inst->getPredicate(), "ne");
  EXPECT_EQ(icmp_ne_inst->getType(), i1_type);
  EXPECT_EQ(icmp_ne_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_ne_inst->getRHS(), rhs);

  // 测试有符号小于比较指令
  ICmpInst *icmp_slt_inst = builder.CreateICmpSLT(lhs, rhs, "icmp_slt_result");
  ASSERT_NE(icmp_slt_inst, nullptr);
  EXPECT_EQ(icmp_slt_inst->getName(), "%icmp_slt_result");
  EXPECT_EQ(icmp_slt_inst->getPredicate(), "slt");
  EXPECT_EQ(icmp_slt_inst->getType(), i1_type);
  EXPECT_EQ(icmp_slt_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_slt_inst->getRHS(), rhs);

  // 测试有符号小于等于比较指令
  ICmpInst *icmp_sle_inst = builder.CreateICmpSLE(lhs, rhs, "icmp_sle_result");
  ASSERT_NE(icmp_sle_inst, nullptr);
  EXPECT_EQ(icmp_sle_inst->getName(), "%icmp_sle_result");
  EXPECT_EQ(icmp_sle_inst->getPredicate(), "sle");
  EXPECT_EQ(icmp_sle_inst->getType(), i1_type);
  EXPECT_EQ(icmp_sle_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_sle_inst->getRHS(), rhs);

  // 测试有符号大于比较指令
  ICmpInst *icmp_sgt_inst = builder.CreateICmpSGT(lhs, rhs, "icmp_sgt_result");
  ASSERT_NE(icmp_sgt_inst, nullptr);
  EXPECT_EQ(icmp_sgt_inst->getName(), "%icmp_sgt_result");
  EXPECT_EQ(icmp_sgt_inst->getPredicate(), "sgt");
  EXPECT_EQ(icmp_sgt_inst->getType(), i1_type);
  EXPECT_EQ(icmp_sgt_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_sgt_inst->getRHS(), rhs);

  // 测试有符号大于等于比较指令
  ICmpInst *icmp_sge_inst = builder.CreateICmpSGE(lhs, rhs, "icmp_sge_result");
  ASSERT_NE(icmp_sge_inst, nullptr);
  EXPECT_EQ(icmp_sge_inst->getName(), "%icmp_sge_result");
  EXPECT_EQ(icmp_sge_inst->getPredicate(), "sge");
  EXPECT_EQ(icmp_sge_inst->getType(), i1_type);
  EXPECT_EQ(icmp_sge_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_sge_inst->getRHS(), rhs);

  // 测试无符号小于比较指令
  ICmpInst *icmp_ult_inst = builder.CreateICmpULT(lhs, rhs, "icmp_ult_result");
  ASSERT_NE(icmp_ult_inst, nullptr);
  EXPECT_EQ(icmp_ult_inst->getName(), "%icmp_ult_result");
  EXPECT_EQ(icmp_ult_inst->getPredicate(), "ult");
  EXPECT_EQ(icmp_ult_inst->getType(), i1_type);
  EXPECT_EQ(icmp_ult_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_ult_inst->getRHS(), rhs);

  // 测试无符号小于等于比较指令
  ICmpInst *icmp_ule_inst = builder.CreateICmpULE(lhs, rhs, "icmp_ule_result");
  ASSERT_NE(icmp_ule_inst, nullptr);
  EXPECT_EQ(icmp_ule_inst->getName(), "%icmp_ule_result");
  EXPECT_EQ(icmp_ule_inst->getPredicate(), "ule");
  EXPECT_EQ(icmp_ule_inst->getType(), i1_type);
  EXPECT_EQ(icmp_ule_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_ule_inst->getRHS(), rhs);

  // 测试无符号大于比较指令
  ICmpInst *icmp_ugt_inst = builder.CreateICmpUGT(lhs, rhs, "icmp_ugt_result");
  ASSERT_NE(icmp_ugt_inst, nullptr);
  EXPECT_EQ(icmp_ugt_inst->getName(), "%icmp_ugt_result");
  EXPECT_EQ(icmp_ugt_inst->getPredicate(), "ugt");
  EXPECT_EQ(icmp_ugt_inst->getType(), i1_type);
  EXPECT_EQ(icmp_ugt_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_ugt_inst->getRHS(), rhs);

  // 测试无符号大于等于比较指令
  ICmpInst *icmp_uge_inst = builder.CreateICmpUGE(lhs, rhs, "icmp_uge_result");
  ASSERT_NE(icmp_uge_inst, nullptr);
  EXPECT_EQ(icmp_uge_inst->getName(), "%icmp_uge_result");
  EXPECT_EQ(icmp_uge_inst->getPredicate(), "uge");
  EXPECT_EQ(icmp_uge_inst->getType(), i1_type);
  EXPECT_EQ(icmp_uge_inst->getLHS(), lhs);
  EXPECT_EQ(icmp_uge_inst->getRHS(), rhs);
}

// 测试其他指令创建
TEST(IRBuilderTest, OtherInstructionTest) {
  LLVMContext context;
  Module module("test_module", &context);
  IRBuilder builder(&context);

  // 创建函数类型和函数
  Type *i32_type = context.getInt32Ty();
  std::vector<Type *> param_types = {};
  FunctionType *func_type = context.getFunctionType(i32_type, param_types);
  Function *function = module.getOrCreateFunction("test_func", func_type);

  // 创建基本块并设置插入点
  BasicBlock *bb = function->createBasicBlock("entry");
  builder.SetInsertPoint(bb);

  // 获取类型
  Type *i1_type = context.getInt1Ty();

  // 测试PHI节点创建
  PHINode *phi_inst = builder.CreatePHI(i32_type, "phi_result");
  ASSERT_NE(phi_inst, nullptr);
  EXPECT_EQ(phi_inst->getName(), "%phi_result");
  EXPECT_EQ(phi_inst->getType(), i32_type);

  // 测试内存拷贝指令
  PointerType *ptr_type = context.getPointerType();
  llvm::IntegerType *integer_type = context.getInt32Ty();
  auto dest = builder.CreateAlloca(integer_type, "dest_ptr");
  auto src = builder.CreateAlloca(integer_type, "src_ptr");
  auto src_result =
      builder.CreateStore(context.getIntConstant(integer_type, 42), dest);

  CallInst *memcpy_inst = builder.CreateMemCpy(dest, src, 64, false);
  ASSERT_NE(memcpy_inst, nullptr);
}

} // namespace llvm