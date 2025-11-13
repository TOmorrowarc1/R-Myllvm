#include "Value.hpp"
#include "gtest/gtest.h"

namespace llvm {

// 测试常量整数
TEST(ValueTest, ConstantIntTest) {
  Int32Type i32_type;
  ConstantInt const_int(&i32_type, 42);

  EXPECT_EQ(const_int.getType(), &i32_type);
  EXPECT_EQ(const_int.getValue(), 42);
  EXPECT_EQ(const_int.getName(), "42");
  EXPECT_EQ(const_int.print(), "42");

  // 测试不同类型的常量
  Int8Type i8_type;
  ConstantInt const_int8(&i8_type, 127);

  EXPECT_EQ(const_int8.getType(), &i8_type);
  EXPECT_EQ(const_int8.getValue(), 127);
  EXPECT_EQ(const_int8.getName(), "127");
  EXPECT_EQ(const_int8.print(), "127");
}

// 测试全局变量
TEST(ValueTest, GlobalVariableTest) {
  Int32Type i32_type;
  ConstantInt initial_value(&i32_type, 100);

  GlobalVariable global_var("my_global", &i32_type, &initial_value);

  EXPECT_EQ(global_var.getType(), &i32_type);
  EXPECT_EQ(global_var.getName(), "my_global");
  EXPECT_EQ(global_var.getInitialValue(), &initial_value);

  // 测试打印
  std::string print_result = global_var.print();
  EXPECT_TRUE(print_result.find("@my_global") != std::string::npos);
  EXPECT_TRUE(print_result.find("global") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("100") != std::string::npos);

  // 测试无初始值的全局变量
  GlobalVariable global_var_no_init("my_global_no_init", &i32_type, nullptr);

  EXPECT_EQ(global_var_no_init.getType(), &i32_type);
  EXPECT_EQ(global_var_no_init.getName(), "my_global_no_init");
  EXPECT_EQ(global_var_no_init.getInitialValue(), nullptr);

  // 测试打印
  std::string print_result_no_init = global_var_no_init.print();
  EXPECT_TRUE(print_result_no_init.find("@my_global_no_init") !=
              std::string::npos);
  EXPECT_TRUE(print_result_no_init.find("global") != std::string::npos);
  EXPECT_TRUE(print_result_no_init.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result_no_init.find("0") != std::string::npos);
}

// 测试函数参数
TEST(ValueTest, ArgumentTest) {
  Int32Type i32_type;

  // 注意：这里我们传入nullptr作为parent，因为在测试环境中我们不需要完整的函数对象
  Argument arg("my_arg", &i32_type, nullptr);

  EXPECT_EQ(arg.getType(), &i32_type);
  EXPECT_EQ(arg.getName(), "my_arg");
  EXPECT_EQ(arg.getParent(), nullptr);

  // 测试打印
  std::string print_result = arg.print();
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("%my_arg") != std::string::npos);
}

// 测试二元运算指令
TEST(ValueTest, BinaryOperatorTest) {
  Int32Type i32_type;
  ConstantInt lhs(&i32_type, 10);
  ConstantInt rhs(&i32_type, 20);

  BinaryOperator bin_op("result", &i32_type, &lhs, &rhs, "add");

  EXPECT_EQ(bin_op.getType(), &i32_type);
  EXPECT_EQ(bin_op.getName(), "%result");
  EXPECT_EQ(bin_op.getOp(), "add");
  EXPECT_EQ(bin_op.getLHS(), &lhs);
  EXPECT_EQ(bin_op.getRHS(), &rhs);

  // 测试操作数列表
  const auto &operands = bin_op.getOperands();
  EXPECT_EQ(operands.size(), 2);
  EXPECT_EQ(operands[0], &lhs);
  EXPECT_EQ(operands[1], &rhs);

  // 测试用户列表
  const auto &lhs_users = lhs.getUsers();
  EXPECT_EQ(lhs_users.size(), 1);
  EXPECT_EQ(lhs_users[0], &bin_op);

  const auto &rhs_users = rhs.getUsers();
  EXPECT_EQ(rhs_users.size(), 1);
  EXPECT_EQ(rhs_users[0], &bin_op);

  // 测试打印
  std::string print_result = bin_op.print();
  EXPECT_TRUE(print_result.find("%result") != std::string::npos);
  EXPECT_TRUE(print_result.find("add") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("10") != std::string::npos);
  EXPECT_TRUE(print_result.find("20") != std::string::npos);
}

// 测试二元运算指令类型检测
TEST(ValueTest, BinaryOperatorTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;
  ConstantInt int32_val(&i32_type, 10);
  ConstantInt int8_val(&i8_type, 20);

  // 使用AllocaInst创建指针值而不是使用ConstantInt
  AllocaInst ptr_val("ptr", &i32_type);

  // 正常情况：两个相同类型的整数操作数
  EXPECT_NO_THROW(
      BinaryOperator("result1", &i32_type, &int32_val, &int32_val, "add"));

  // 异常情况：操作数类型不是整数
  EXPECT_THROW(
      BinaryOperator("result2", &i32_type, &ptr_val, &int32_val, "add"),
      std::runtime_error);
  EXPECT_THROW(
      BinaryOperator("result3", &i32_type, &int32_val, &ptr_val, "add"),
      std::runtime_error);

  // 异常情况：操作数类型与结果类型不匹配
  EXPECT_THROW(
      BinaryOperator("result4", &i8_type, &int32_val, &int32_val, "add"),
      std::runtime_error);
  EXPECT_THROW(
      BinaryOperator("result5", &i32_type, &int8_val, &int8_val, "add"),
      std::runtime_error);
}

// 测试一元运算指令
TEST(ValueTest, UnaryOperatorTest) {
  Int32Type i32_type;
  ConstantInt operand(&i32_type, 10);

  UnaryOperator un_op("result", "neg", &i32_type, &operand);

  EXPECT_EQ(un_op.getType(), &i32_type);
  EXPECT_EQ(un_op.getName(), "%result");
  EXPECT_EQ(un_op.getOp(), "neg");
  EXPECT_EQ(un_op.getOperand(), &operand);

  // 测试操作数列表
  const auto &operands = un_op.getOperands();
  EXPECT_EQ(operands.size(), 1);
  EXPECT_EQ(operands[0], &operand);

  // 测试用户列表
  const auto &operand_users = operand.getUsers();
  EXPECT_EQ(operand_users.size(), 1);
  EXPECT_EQ(operand_users[0], &un_op);

  // 测试打印
  std::string print_result = un_op.print();
  EXPECT_TRUE(print_result.find("%result") != std::string::npos);
  EXPECT_TRUE(print_result.find("neg") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("10") != std::string::npos);
}

// 测试一元运算指令类型检测
TEST(ValueTest, UnaryOperatorTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;
  ConstantInt int32_val(&i32_type, 10);
  ConstantInt int8_val(&i8_type, 20);

  // 使用AllocaInst创建指针值而不是使用ConstantInt
  AllocaInst ptr_val("ptr", &i32_type);

  // 正常情况：整数类型的操作数
  EXPECT_NO_THROW(UnaryOperator("result1", "neg", &i32_type, &int32_val));

  // 异常情况：操作数类型不是整数
  EXPECT_THROW(UnaryOperator("result2", "neg", &i32_type, &ptr_val),
               std::runtime_error);

  // 异常情况：操作数类型与结果类型不匹配
  EXPECT_THROW(UnaryOperator("result3", "neg", &i8_type, &int32_val),
               std::runtime_error);
  EXPECT_THROW(UnaryOperator("result4", "neg", &i32_type, &int8_val),
               std::runtime_error);
}

// 测试加载指令
TEST(ValueTest, LoadInstTest) {
  Int32Type i32_type;
  PointerType ptr_type;

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);

  LoadInst load_inst("result", &i32_type, &alloca_ptr);

  EXPECT_EQ(load_inst.getType(), &i32_type);
  EXPECT_EQ(load_inst.getName(), "%result");
  EXPECT_EQ(load_inst.getPtr(), &alloca_ptr);

  // 测试操作数列表
  const auto &operands = load_inst.getOperands();
  EXPECT_EQ(operands.size(), 1);
  EXPECT_EQ(operands[0], &alloca_ptr);

  // 测试用户列表
  const auto &ptr_users = alloca_ptr.getUsers();
  EXPECT_EQ(ptr_users.size(), 1);
  EXPECT_EQ(ptr_users[0], &load_inst);

  // 测试打印
  std::string print_result = load_inst.print();
  EXPECT_TRUE(print_result.find("%result") != std::string::npos);
  EXPECT_TRUE(print_result.find("load") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("%ptr") != std::string::npos);
}

// 测试加载指令类型检测
TEST(ValueTest, LoadInstTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);
  ConstantInt int32_val(&i32_type, 42);

  // 正常情况：指针类型的操作数
  EXPECT_NO_THROW(LoadInst("result1", &i32_type, &alloca_ptr));

  // 异常情况：操作数类型不是指针
  EXPECT_THROW(LoadInst("result2", &i32_type, &int32_val), std::runtime_error);
}

// 测试存储指令
TEST(ValueTest, StoreInstTest) {
  Int32Type i32_type;
  PointerType ptr_type;
  ConstantInt value(&i32_type, 42);

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);

  StoreInst store_inst(&i32_type, &alloca_ptr, &value);

  EXPECT_EQ(store_inst.getType(), &i32_type);
  EXPECT_EQ(store_inst.getPtr(), &alloca_ptr);
  EXPECT_EQ(store_inst.getValue(), &value);

  // 测试操作数列表
  const auto &operands = store_inst.getOperands();
  EXPECT_EQ(operands.size(), 2);
  EXPECT_EQ(operands[0], &alloca_ptr);
  EXPECT_EQ(operands[1], &value);

  // 测试用户列表
  const auto &ptr_users = alloca_ptr.getUsers();
  EXPECT_EQ(ptr_users.size(), 1);
  EXPECT_EQ(ptr_users[0], &store_inst);

  const auto &value_users = value.getUsers();
  EXPECT_EQ(value_users.size(), 1);
  EXPECT_EQ(value_users[0], &store_inst);

  // 测试打印
  std::string print_result = store_inst.print();
  EXPECT_TRUE(print_result.find("store") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("42") != std::string::npos);
  EXPECT_TRUE(print_result.find("%ptr") != std::string::npos);
}

// 测试存储指令类型检测
TEST(ValueTest, StoreInstTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);
  ConstantInt int32_val(&i32_type, 42);
  ConstantInt int8_val(&i8_type, 20);

  // 正常情况：指针类型的指针和匹配的值类型
  EXPECT_NO_THROW(StoreInst(&i32_type, &alloca_ptr, &int32_val));

  // 异常情况：指针类型不是指针
  EXPECT_THROW(StoreInst(&i32_type, &int32_val, &int32_val),
               std::runtime_error);

  // 异常情况：值类型与指定类型不匹配
  EXPECT_THROW(StoreInst(&i32_type, &alloca_ptr, &int8_val),
               std::runtime_error);
  EXPECT_THROW(StoreInst(&i8_type, &alloca_ptr, &int32_val),
               std::runtime_error);
}

// 测试内存分配指令
TEST(ValueTest, AllocaInstTest) {
  Int32Type i32_type;

  AllocaInst alloca_inst("ptr", &i32_type);

  EXPECT_EQ(alloca_inst.getName(), "%ptr");

  // 测试类型 - 应该是指针类型
  Type *alloca_type = alloca_inst.getType();
  EXPECT_TRUE(alloca_type->isPointerTy());
  EXPECT_FALSE(alloca_type->isAggregateTy());

  // 测试打印
  std::string print_result = alloca_inst.print();
  EXPECT_TRUE(print_result.find("%ptr") != std::string::npos);
  EXPECT_TRUE(print_result.find("alloca") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
}

// 测试内存分配指令 - 数组类型
TEST(ValueTest, AllocaInstArrayTest) {
  Int32Type i32_type;
  ArrayType array_type(&i32_type, 5); // 创建包含5个i32元素的数组类型

  AllocaInst alloca_inst("array_ptr", &array_type);

  EXPECT_EQ(alloca_inst.getName(), "%array_ptr");

  // 测试类型 - 应该是指针类型
  Type *alloca_type = alloca_inst.getType();
  EXPECT_TRUE(alloca_type->isPointerTy());

  // 测试打印
  std::string print_result = alloca_inst.print();
  EXPECT_TRUE(print_result.find("%array_ptr") != std::string::npos);
  EXPECT_TRUE(print_result.find("alloca") != std::string::npos);
  EXPECT_TRUE(print_result.find("[i32 x 5]") != std::string::npos);
}

// 测试内存分配指令 - 结构体类型
TEST(ValueTest, AllocaInstStructTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  StructType struct_type("Person");

  // 设置结构体成员
  std::vector<Type *> elements = {&i32_type, &i8_type, &i32_type};
  struct_type.setBody(elements);

  AllocaInst alloca_inst("person_ptr", &struct_type);

  EXPECT_EQ(alloca_inst.getName(), "%person_ptr");

  // 测试类型 - 应该是指针类型
  Type *alloca_type = alloca_inst.getType();
  EXPECT_TRUE(alloca_type->isPointerTy());

  // 测试打印
  std::string print_result = alloca_inst.print();
  EXPECT_TRUE(print_result.find("%person_ptr") != std::string::npos);
  EXPECT_TRUE(print_result.find("alloca") != std::string::npos);
  EXPECT_TRUE(print_result.find("%struct.Person") != std::string::npos);
}

// 测试整数比较指令
TEST(ValueTest, ICmpInstTest) {
  Int32Type i32_type;
  Int1Type i1_type;
  ConstantInt lhs(&i32_type, 10);
  ConstantInt rhs(&i32_type, 20);

  ICmpInst icmp_inst("result", "eq", &i1_type, &lhs, &rhs);

  EXPECT_EQ(icmp_inst.getType(), &i1_type);
  EXPECT_EQ(icmp_inst.getName(), "%result");
  EXPECT_EQ(icmp_inst.getPredicate(), "eq");
  EXPECT_EQ(icmp_inst.getLHS(), &lhs);
  EXPECT_EQ(icmp_inst.getRHS(), &rhs);

  // 测试操作数列表
  const auto &operands = icmp_inst.getOperands();
  EXPECT_EQ(operands.size(), 2);
  EXPECT_EQ(operands[0], &lhs);
  EXPECT_EQ(operands[1], &rhs);

  // 测试用户列表
  const auto &lhs_users = lhs.getUsers();
  EXPECT_EQ(lhs_users.size(), 1);
  EXPECT_EQ(lhs_users[0], &icmp_inst);

  const auto &rhs_users = rhs.getUsers();
  EXPECT_EQ(rhs_users.size(), 1);
  EXPECT_EQ(rhs_users[0], &icmp_inst);

  // 测试打印
  std::string print_result = icmp_inst.print();
  EXPECT_TRUE(print_result.find("%result") != std::string::npos);
  EXPECT_TRUE(print_result.find("icmp") != std::string::npos);
  EXPECT_TRUE(print_result.find("eq") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("10") != std::string::npos);
  EXPECT_TRUE(print_result.find("20") != std::string::npos);
}

// 测试整数比较指令类型检测
TEST(ValueTest, ICmpInstTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  Int1Type i1_type;
  PointerType ptr_type;
  ConstantInt int32_val1(&i32_type, 10);
  ConstantInt int32_val2(&i32_type, 20);
  ConstantInt int8_val1(&i8_type, 5);
  ConstantInt int8_val2(&i8_type, 15);

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);

  // 正常情况：两个相同类型的整数操作数，结果类型为Int1Type
  EXPECT_NO_THROW(
      ICmpInst("result1", "eq", &i1_type, &int32_val1, &int32_val2));
  EXPECT_NO_THROW(ICmpInst("result2", "ne", &i1_type, &int8_val1, &int8_val2));

  // 异常情况：操作数类型不是整数
  EXPECT_THROW(ICmpInst("result3", "eq", &i1_type, &alloca_ptr, &int32_val1),
               std::runtime_error);
  EXPECT_THROW(ICmpInst("result4", "eq", &i1_type, &int32_val1, &alloca_ptr),
               std::runtime_error);

  // 异常情况：操作数类型不相同
  EXPECT_THROW(ICmpInst("result5", "eq", &i1_type, &int32_val1, &int8_val1),
               std::runtime_error);
  EXPECT_THROW(ICmpInst("result6", "eq", &i1_type, &int8_val1, &int32_val1),
               std::runtime_error);

  // 异常情况：结果类型不是Int1Type
  EXPECT_THROW(ICmpInst("result7", "eq", &i32_type, &int32_val1, &int32_val2),
               std::runtime_error);
  EXPECT_THROW(ICmpInst("result8", "eq", &i8_type, &int8_val1, &int8_val2),
               std::runtime_error);
}

// 测试条件分支指令
TEST(ValueTest, ConBrInstTest) {
  Int32Type i32_type;
  Int1Type i1_type;
  ConstantInt condition(&i1_type, 1);

  // 创建模拟的基本块
  FunctionType func_type(&i32_type, {});
  Function function("test_func", &func_type, nullptr);
  BasicBlock then_block("then", &function);
  BasicBlock else_block("else", &function);

  ConBrInst con_br(&condition, &then_block, &else_block);

  EXPECT_EQ(con_br.getCondition(), &condition);
  EXPECT_EQ(con_br.getThenBlock(), &then_block);
  EXPECT_EQ(con_br.getElseBlock(), &else_block);

  // 测试操作数列表
  const auto &operands = con_br.getOperands();
  EXPECT_EQ(operands.size(), 1);
  EXPECT_EQ(operands[0], &condition);

  // 测试用户列表
  const auto &condition_users = condition.getUsers();
  EXPECT_EQ(condition_users.size(), 1);
  EXPECT_EQ(condition_users[0], &con_br);

  // 测试打印
  std::string print_result = con_br.print();
  EXPECT_TRUE(print_result.find("br i1") != std::string::npos);
  EXPECT_TRUE(print_result.find("1") != std::string::npos);
  EXPECT_TRUE(print_result.find("label %then") != std::string::npos);
  EXPECT_TRUE(print_result.find("label %else") != std::string::npos);

  // 测试getName和getType方法抛出异常
  EXPECT_THROW(con_br.getName(), std::runtime_error);
  EXPECT_THROW(con_br.getType(), std::runtime_error);
}

// 测试条件分支指令类型检测
TEST(ValueTest, ConBrInstTypeCheckTest) {
  Int32Type i32_type;
  Int1Type i1_type;
  PointerType ptr_type;
  ConstantInt i1_val(&i1_type, 1);
  ConstantInt i32_val(&i32_type, 42);

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);

  // 创建模拟的基本块
  FunctionType func_type(&i32_type, {});
  Function function("test_func", &func_type, nullptr);
  BasicBlock then_block("then", &function);
  BasicBlock else_block("else", &function);

  // 正常情况：条件值为i1类型
  EXPECT_NO_THROW(ConBrInst(&i1_val, &then_block, &else_block));

  // 异常情况：条件值不是i1类型
  EXPECT_THROW(ConBrInst(&i32_val, &then_block, &else_block),
               std::runtime_error);
  EXPECT_THROW(ConBrInst(&alloca_ptr, &then_block, &else_block),
               std::runtime_error);
}

// 测试无条件分支指令
TEST(ValueTest, BrInstTest) {
  Int32Type i32_type;

  // 创建模拟的基本块
  FunctionType func_type(&i32_type, {});
  Function function("test_func", &func_type, nullptr);
  BasicBlock target_block("target", &function);

  BrInst br(&target_block);

  EXPECT_EQ(br.getTarget(), &target_block);

  // 测试打印
  std::string print_result = br.print();
  EXPECT_TRUE(print_result.find("br label %target") != std::string::npos);

  // 测试getName和getType方法抛出异常
  EXPECT_THROW(br.getName(), std::runtime_error);
  EXPECT_THROW(br.getType(), std::runtime_error);
}

// 测试返回指令
TEST(ValueTest, ReturnInstTest) {
  Int32Type i32_type;
  ConstantInt return_value(&i32_type, 42);

  ReturnInst return_inst(&return_value);

  EXPECT_EQ(return_inst.getReturnValue(), &return_value);

  // 测试操作数列表
  const auto &operands = return_inst.getOperands();
  EXPECT_EQ(operands.size(), 1);
  EXPECT_EQ(operands[0], &return_value);

  // 测试用户列表
  const auto &return_value_users = return_value.getUsers();
  EXPECT_EQ(return_value_users.size(), 1);
  EXPECT_EQ(return_value_users[0], &return_inst);

  // 测试打印
  std::string print_result = return_inst.print();
  EXPECT_TRUE(print_result.find("ret i32 42") != std::string::npos);

  // 测试getName和getType方法抛出异常
  EXPECT_THROW(return_inst.getName(), std::runtime_error);
  EXPECT_THROW(return_inst.getType(), std::runtime_error);

  // 测试无返回值的情况
  ReturnInst return_void(nullptr);
  EXPECT_EQ(return_void.getReturnValue(), nullptr);

  std::string print_void_result = return_void.print();
  EXPECT_TRUE(print_void_result.find("ret void") != std::string::npos);
}

// 测试PHI节点
TEST(ValueTest, PHINodeTest) {
  Int32Type i32_type;

  // 创建模拟的基本块和值
  FunctionType func_type(&i32_type, {});
  Function function("test_func", &func_type, nullptr);
  BasicBlock block1("block1", &function);
  BasicBlock block2("block2", &function);
  ConstantInt value1(&i32_type, 10);
  ConstantInt value2(&i32_type, 20);

  PHINode phi("result", &i32_type);

  EXPECT_EQ(phi.getType(), &i32_type);
  EXPECT_EQ(phi.getName(), "%result");

  // 添加输入值
  phi.addIncoming(&value1, &block1);
  phi.addIncoming(&value2, &block2);

  // 测试操作数列表
  const auto &operands = phi.getOperands();
  EXPECT_EQ(operands.size(), 2);
  EXPECT_EQ(operands[0], &value1);
  EXPECT_EQ(operands[1], &value2);

  // 测试用户列表
  const auto &value1_users = value1.getUsers();
  EXPECT_EQ(value1_users.size(), 1);
  EXPECT_EQ(value1_users[0], &phi);

  const auto &value2_users = value2.getUsers();
  EXPECT_EQ(value2_users.size(), 1);
  EXPECT_EQ(value2_users[0], &phi);

  // 测试打印
  std::string print_result = phi.print();
  EXPECT_TRUE(print_result.find("%result = phi i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("[ 10, %block1 ]") != std::string::npos);
  EXPECT_TRUE(print_result.find("[ 20, %block2 ]") != std::string::npos);
}

// 测试PHI节点类型检测
TEST(ValueTest, PHINodeTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;

  // 创建模拟的基本块和值
  FunctionType func_type(&i32_type, {});
  Function function("test_func", &func_type, nullptr);
  BasicBlock block1("block1", &function);
  BasicBlock block2("block2", &function);
  ConstantInt i32_val1(&i32_type, 10);
  ConstantInt i32_val2(&i32_type, 20);
  ConstantInt i8_val1(&i8_type, 5);
  ConstantInt i8_val2(&i8_type, 15);

  PHINode phi("result", &i32_type);

  // 正常情况：输入值类型与PHI节点类型一致
  EXPECT_NO_THROW(phi.addIncoming(&i32_val1, &block1));
  EXPECT_NO_THROW(phi.addIncoming(&i32_val2, &block2));

  // 异常情况：输入值类型与PHI节点类型不一致
  EXPECT_THROW(phi.addIncoming(&i8_val1, &block1), std::runtime_error);
  EXPECT_THROW(phi.addIncoming(&i8_val2, &block2), std::runtime_error);
}

// 测试函数调用指令
TEST(ValueTest, CallInstTest) {
  Int32Type i32_type;
  Int8Type i8_type;

  // 创建模拟的函数和参数
  std::vector<Type *> param_types = {&i32_type, &i8_type};
  FunctionType func_type(&i32_type, param_types);
  Function function("test_func", &func_type, nullptr);
  ConstantInt arg1(&i32_type, 10);
  ConstantInt arg2(&i8_type, 20);
  std::vector<Value *> args = {&arg1, &arg2};

  CallInst call("result", &function, args);

  EXPECT_EQ(call.getType(), &i32_type);
  EXPECT_EQ(call.getName(), "%result");
  EXPECT_EQ(call.getFunction(), &function);

  const auto &call_args = call.getArgs();
  EXPECT_EQ(call_args.size(), 2);
  EXPECT_EQ(call_args[0], &arg1);
  EXPECT_EQ(call_args[1], &arg2);

  // 测试操作数列表
  const auto &operands = call.getOperands();
  EXPECT_EQ(operands.size(), 2);
  EXPECT_EQ(operands[0], &arg1);
  EXPECT_EQ(operands[1], &arg2);

  // 测试用户列表
  const auto &arg1_users = arg1.getUsers();
  EXPECT_EQ(arg1_users.size(), 1);
  EXPECT_EQ(arg1_users[0], &call);

  const auto &arg2_users = arg2.getUsers();
  EXPECT_EQ(arg2_users.size(), 1);
  EXPECT_EQ(arg2_users[0], &call);

  // 测试打印
  std::string print_result = call.print();
  EXPECT_TRUE(print_result.find("%result = call i32 @test_func") !=
              std::string::npos);
  EXPECT_TRUE(print_result.find("i32 10") != std::string::npos);
  EXPECT_TRUE(print_result.find("i8 20") != std::string::npos);

  // 检测括号的存在
  EXPECT_TRUE(print_result.find("(") != std::string::npos);
  EXPECT_TRUE(print_result.find(")") != std::string::npos);
}

// 测试函数调用指令类型检测
TEST(ValueTest, CallInstTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;

  // 创建模拟的函数和参数
  std::vector<Type *> param_types = {&i32_type, &i8_type};
  FunctionType func_type(&i32_type, param_types);
  Function function("test_func", &func_type, nullptr);
  ConstantInt i32_val(&i32_type, 10);
  ConstantInt i8_val(&i8_type, 20);

  // 正常情况：参数数量和类型匹配
  std::vector<Value *> args1 = {&i32_val, &i8_val};
  EXPECT_NO_THROW(CallInst("result1", &function, args1));

  // 异常情况：参数数量不匹配
  std::vector<Value *> args2 = {&i32_val};
  EXPECT_THROW(CallInst("result2", &function, args2), std::runtime_error);

  std::vector<Value *> args3 = {&i32_val, &i8_val, &i32_val};
  EXPECT_THROW(CallInst("result3", &function, args3), std::runtime_error);

  // 异常情况：参数类型不匹配
  std::vector<Value *> args4 = {&i8_val, &i32_val};
  EXPECT_THROW(CallInst("result4", &function, args4), std::runtime_error);
}

// 测试地址计算指令
TEST(ValueTest, GetElementPtrInstTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);
  ConstantInt index1(&i32_type, 0);
  ConstantInt index2(&i8_type, 1);
  std::vector<Value *> indices = {&index1, &index2};

  GetElementPtrInst gep("result", &ptr_type, &i32_type, &alloca_ptr, indices);

  EXPECT_EQ(gep.getType(), &ptr_type);
  EXPECT_EQ(gep.getName(), "%result");
  EXPECT_EQ(gep.getPtr(), &alloca_ptr);

  const auto &gep_indices = gep.getIndices();
  EXPECT_EQ(gep_indices.size(), 2);
  EXPECT_EQ(gep_indices[0], &index1);
  EXPECT_EQ(gep_indices[1], &index2);

  // 测试操作数列表
  const auto &operands = gep.getOperands();
  EXPECT_EQ(operands.size(), 3);
  EXPECT_EQ(operands[0], &alloca_ptr);
  EXPECT_EQ(operands[1], &index1);
  EXPECT_EQ(operands[2], &index2);

  // 测试用户列表
  const auto &ptr_users = alloca_ptr.getUsers();
  EXPECT_EQ(ptr_users.size(), 1);
  EXPECT_EQ(ptr_users[0], &gep);

  const auto &index1_users = index1.getUsers();
  EXPECT_EQ(index1_users.size(), 1);
  EXPECT_EQ(index1_users[0], &gep);

  const auto &index2_users = index2.getUsers();
  EXPECT_EQ(index2_users.size(), 1);
  EXPECT_EQ(index2_users[0], &gep);

  // 测试打印
  std::string print_result = gep.print();
  EXPECT_TRUE(print_result.find("%result = getelementptr i32") !=
              std::string::npos);
  EXPECT_TRUE(print_result.find("%ptr") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32 0") != std::string::npos);
  EXPECT_TRUE(print_result.find("i8 1") != std::string::npos);
}

// 测试地址计算指令 - 结构体类型
TEST(ValueTest, GetElementPtrInstStructTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;
  StructType struct_type("Person");

  // 设置结构体成员
  std::vector<Type *> elements = {&i32_type, &i8_type, &i32_type};
  struct_type.setBody(elements);

  // 创建结构体指针
  AllocaInst alloca_ptr("person_ptr", &struct_type);

  // 创建索引
  ConstantInt index0(&i32_type, 0); // 访问第0个元素（第一个成员）
  ConstantInt index1(&i32_type, 1); // 访问第1个元素（第二个成员）
  std::vector<Value *> indices1 = {&index0};
  std::vector<Value *> indices2 = {&index0, &index1};

  // 测试访问整个结构体
  GetElementPtrInst gep1("struct_ptr", &ptr_type, &struct_type, &alloca_ptr,
                         indices1);

  EXPECT_EQ(gep1.getType(), &ptr_type);
  EXPECT_EQ(gep1.getName(), "%struct_ptr");
  EXPECT_EQ(gep1.getPtr(), &alloca_ptr);

  // 测试打印
  std::string print_result1 = gep1.print();
  EXPECT_TRUE(
      print_result1.find("%struct_ptr = getelementptr %struct.Person") !=
      std::string::npos);
  EXPECT_TRUE(print_result1.find("%person_ptr") != std::string::npos);
  EXPECT_TRUE(print_result1.find("i32 0") != std::string::npos);

  // 测试访问结构体成员
  GetElementPtrInst gep2("member_ptr", &ptr_type, &struct_type, &alloca_ptr,
                         indices2);

  EXPECT_EQ(gep2.getType(), &ptr_type);
  EXPECT_EQ(gep2.getName(), "%member_ptr");
  EXPECT_EQ(gep2.getPtr(), &alloca_ptr);

  // 测试打印
  std::string print_result2 = gep2.print();
  EXPECT_TRUE(
      print_result2.find("%member_ptr = getelementptr %struct.Person") !=
      std::string::npos);
  EXPECT_TRUE(print_result2.find("%person_ptr") != std::string::npos);
  EXPECT_TRUE(print_result2.find("i32 0") != std::string::npos);
  EXPECT_TRUE(print_result2.find("i32 1") != std::string::npos);
}

// 测试地址计算指令类型检测
TEST(ValueTest, GetElementPtrInstTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  // 使用AllocaInst创建指针而不是使用ConstantInt
  AllocaInst alloca_ptr("ptr", &i32_type);
  ConstantInt i32_val(&i32_type, 10);
  ConstantInt i8_val(&i8_type, 20);

  // 正常情况：指针类型的指针和整数类型的索引
  std::vector<Value *> indices1 = {&i32_val, &i8_val};
  EXPECT_NO_THROW(GetElementPtrInst("result1", &ptr_type, &i32_type,
                                    &alloca_ptr, indices1));

  // 异常情况：指针类型不是指针
  std::vector<Value *> indices2 = {&i32_val, &i8_val};
  EXPECT_THROW(
      GetElementPtrInst("result2", &ptr_type, &i32_type, &i32_val, indices2),
      std::runtime_error);

  // 异常情况：索引不是整数类型
  std::vector<Value *> indices3 = {&alloca_ptr, &i8_val};
  EXPECT_THROW(
      GetElementPtrInst("result3", &ptr_type, &i32_type, &alloca_ptr, indices3),
      std::runtime_error);

  std::vector<Value *> indices4 = {&i32_val, &alloca_ptr};
  EXPECT_THROW(
      GetElementPtrInst("result4", &ptr_type, &i32_type, &alloca_ptr, indices4),
      std::runtime_error);
}

// 测试基本块包含多个指令时的打印
TEST(ValueTest, BasicBlockMultipleInstructionsTest) {
  Int32Type i32_type;
  Int1Type i1_type;

  // 创建模拟的函数和基本块
  FunctionType func_type(&i32_type, {});
  Function function("test_func", &func_type, nullptr);
  BasicBlock block("entry", &function);

  // 创建多个指令
  ConstantInt const_42(&i32_type, 42);
  ConstantInt const_10(&i32_type, 10);
  ConstantInt const_1(&i1_type, 1);

  // 创建alloca指令
  auto alloca_inst = std::make_unique<AllocaInst>("var", &i32_type);

  // 创建store指令
  auto store_inst =
      std::make_unique<StoreInst>(&i32_type, alloca_inst.get(), &const_42);

  // 创建load指令
  auto load_inst =
      std::make_unique<LoadInst>("loaded_val", &i32_type, alloca_inst.get());

  // 创建二元运算指令
  auto add_inst = std::make_unique<BinaryOperator>(
      "sum", &i32_type, load_inst.get(), &const_10, "add");

  // 创建比较指令
  auto icmp_inst = std::make_unique<ICmpInst>("cmp", "eq", &i1_type,
                                              add_inst.get(), &const_42);

  // 创建分支指令
  BasicBlock then_block("then", &function);
  BasicBlock else_block("else", &function);
  auto br_inst =
      std::make_unique<ConBrInst>(icmp_inst.get(), &then_block, &else_block);

  // 将指令添加到基本块
  block.addInstruction(std::move(alloca_inst));
  block.addInstruction(std::move(store_inst));
  block.addInstruction(std::move(load_inst));
  block.addInstruction(std::move(add_inst));
  block.addInstruction(std::move(icmp_inst));

  // 保存br_inst的指针，因为移动后br_inst.get()会是nullptr
  auto *br_inst_ptr = br_inst.get();
  block.addInstruction(std::move(br_inst));

  // 测试基本块是否已终止
  EXPECT_TRUE(block.isTerminated());

  // 测试基本块的终止指令
  EXPECT_EQ(block.getTerminator(), br_inst_ptr);

  // 测试打印
  std::string print_result = block.print();

  // 检查基本块名称
  EXPECT_TRUE(print_result.find("entry:") != std::string::npos);

  // 检查各个指令的打印
  EXPECT_TRUE(print_result.find("%var = alloca i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("store i32 42, ptr %var") != std::string::npos);
  EXPECT_TRUE(print_result.find("%loaded_val = load i32, ptr %var") !=
              std::string::npos);
  EXPECT_TRUE(print_result.find("%sum = add i32 %loaded_val, 10") !=
              std::string::npos);
  EXPECT_TRUE(print_result.find("%cmp = icmp eq i32 %sum, 42") !=
              std::string::npos);
  EXPECT_TRUE(print_result.find("br i1 %cmp, label %then, label %else") !=
              std::string::npos);

  // 检查指令的缩进
  size_t pos = 0;
  size_t line_count = 0;
  while ((pos = print_result.find('\n', pos)) != std::string::npos) {
    line_count++;
    pos++;
    if (line_count > 1) { // 跳过第一行（基本块标签）
      // 确保不会超出字符串边界
      if (pos + 2 <= print_result.length()) {
        EXPECT_EQ(print_result.substr(pos, 2),
                  "  "); // 每行指令前应该有两个空格
      }
    }
  }
}

// 测试函数类
TEST(ValueTest, FunctionTest) {
  Int32Type i32_type;
  Int8Type i8_type;

  // 创建函数类型：i32 (i32, i8)
  std::vector<Type *> param_types = {&i32_type, &i8_type};
  FunctionType func_type(&i32_type, param_types);

  // 创建函数
  Function function("test_func", &func_type, nullptr);

  // 测试基本属性
  EXPECT_EQ(function.getName(), "test_func");
  EXPECT_EQ(function.getType(), &func_type);
  EXPECT_EQ(function.getParent(), nullptr);
  EXPECT_FALSE(function.isDefined()); // 初始状态下函数未定义

  // 测试参数列表（初始为空）
  const auto &args = function.getArguments();
  EXPECT_EQ(args.size(), 0);

  // 测试基本块列表（初始为空）
  const auto &bbs = function.getBasicBlocks();
  EXPECT_EQ(bbs.size(), 0);

  // 测试打印
  std::string print_result = function.print();
  EXPECT_TRUE(print_result.find("declare") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("@test_func") != std::string::npos);
  EXPECT_TRUE(print_result.find("(") != std::string::npos);
  EXPECT_TRUE(print_result.find(")") != std::string::npos);
  EXPECT_TRUE(print_result.find("{") != std::string::npos);
  EXPECT_TRUE(print_result.find("}") != std::string::npos);
}

// 测试函数基本块管理
TEST(ValueTest, FunctionBasicBlockTest) {
  Int32Type i32_type;

  // 创建函数类型：i32 ()
  std::vector<Type *> param_types = {};
  FunctionType func_type(&i32_type, param_types);

  // 创建函数
  Function function("test_func", &func_type, nullptr);

  // 测试创建基本块
  BasicBlock *bb1 = function.createBasicBlock("entry");
  EXPECT_NE(bb1, nullptr);
  EXPECT_EQ(bb1->getName(), "entry");
  EXPECT_EQ(bb1->getParent(), &function);

  // 测试创建同名基本块（应该自动添加后缀）
  BasicBlock *bb2 = function.createBasicBlock("entry");
  EXPECT_NE(bb2, nullptr);
  EXPECT_EQ(bb2->getName(), "entry.1");
  EXPECT_EQ(bb2->getParent(), &function);

  // 测试再次创建同名基本块
  BasicBlock *bb3 = function.createBasicBlock("entry");
  EXPECT_NE(bb3, nullptr);
  EXPECT_EQ(bb3->getName(), "entry.2");
  EXPECT_EQ(bb3->getParent(), &function);

  // 测试获取基本块列表
  const auto &bbs = function.getBasicBlocks();
  EXPECT_EQ(bbs.size(), 3);
  EXPECT_EQ(bbs[0], bb1);
  EXPECT_EQ(bbs[1], bb2);
  EXPECT_EQ(bbs[2], bb3);

  // 测试根据索引获取基本块
  EXPECT_EQ(function.getBBbyIndex(0), bb1);
  EXPECT_EQ(function.getBBbyIndex(1), bb2);
  EXPECT_EQ(function.getBBbyIndex(2), bb3);
  EXPECT_EQ(function.getBBbyIndex(3), nullptr);   // 索引超出范围
  EXPECT_EQ(function.getBBbyIndex(100), nullptr); // 索引超出范围

  // 测试添加基本块
  auto bb4 = std::make_unique<BasicBlock>("custom", &function);
  auto bb4_ptr = bb4.get();
  function.addBasicBlock(std::move(bb4));

  const auto &bbs_after = function.getBasicBlocks();
  EXPECT_EQ(bbs_after.size(), 4);
  EXPECT_EQ(bbs_after[3], bb4_ptr);
  EXPECT_EQ(function.getBBbyIndex(3), bb4_ptr);
}

// 测试函数参数管理
TEST(ValueTest, FunctionArgumentTest) {
  Int32Type i32_type;
  Int8Type i8_type;

  // 创建函数类型：i32 (i32, i8)
  std::vector<Type *> param_types = {&i32_type, &i8_type};
  FunctionType func_type(&i32_type, param_types);

  // 创建函数
  Function function("test_func", &func_type, nullptr);

  // 测试添加参数
  auto arg1 = std::make_unique<Argument>("a", &i32_type, &function);
  auto arg1_ptr = arg1.get();
  function.addArgument(std::move(arg1));

  auto arg2 = std::make_unique<Argument>("b", &i8_type, &function);
  auto arg2_ptr = arg2.get();
  function.addArgument(std::move(arg2));

  // 测试获取参数列表
  const auto &args = function.getArguments();
  EXPECT_EQ(args.size(), 2);
  EXPECT_EQ(args[0], arg1_ptr);
  EXPECT_EQ(args[1], arg2_ptr);

  // 测试根据索引获取参数
  EXPECT_EQ(function.getArgByIndex(0), arg1_ptr);
  EXPECT_EQ(function.getArgByIndex(1), arg2_ptr);
  EXPECT_EQ(function.getArgByIndex(2), nullptr);   // 索引超出范围
  EXPECT_EQ(function.getArgByIndex(100), nullptr); // 索引超出范围

  // 测试参数属性
  EXPECT_EQ(arg1_ptr->getName(), "a");
  EXPECT_EQ(arg1_ptr->getType(), &i32_type);
  EXPECT_EQ(arg1_ptr->getParent(), &function);

  EXPECT_EQ(arg2_ptr->getName(), "b");
  EXPECT_EQ(arg2_ptr->getType(), &i8_type);
  EXPECT_EQ(arg2_ptr->getParent(), &function);
}

// 测试函数设置参数列表
TEST(ValueTest, FunctionSetArgumentsTest) {
  Int32Type i32_type;
  Int8Type i8_type;

  // 创建函数类型：i32 (i32, i8)
  std::vector<Type *> param_types = {&i32_type, &i8_type};
  FunctionType func_type(&i32_type, param_types);

  // 创建函数
  Function function("test_func", &func_type, nullptr);

  // 创建参数列表
  std::vector<std::unique_ptr<Argument>> args;
  args.push_back(std::make_unique<Argument>("a", &i32_type, &function));
  args.push_back(std::make_unique<Argument>("b", &i8_type, &function));

  // 保存参数指针以便后续验证
  Argument *arg1_ptr = args[0].get();
  Argument *arg2_ptr = args[1].get();

  // 测试设置参数列表
  function.setArguments(std::move(args));

  // 测试获取参数列表
  const auto &function_args = function.getArguments();
  EXPECT_EQ(function_args.size(), 2);
  EXPECT_EQ(function_args[0], arg1_ptr);
  EXPECT_EQ(function_args[1], arg2_ptr);

  // 测试根据索引获取参数
  EXPECT_EQ(function.getArgByIndex(0), arg1_ptr);
  EXPECT_EQ(function.getArgByIndex(1), arg2_ptr);
  EXPECT_EQ(function.getArgByIndex(2), nullptr); // 索引超出范围

  // 测试参数属性
  EXPECT_EQ(arg1_ptr->getName(), "a");
  EXPECT_EQ(arg1_ptr->getType(), &i32_type);
  EXPECT_EQ(arg1_ptr->getParent(), &function);

  EXPECT_EQ(arg2_ptr->getName(), "b");
  EXPECT_EQ(arg2_ptr->getType(), &i8_type);
  EXPECT_EQ(arg2_ptr->getParent(), &function);

  // 测试替换参数列表
  std::vector<std::unique_ptr<Argument>> new_args;
  new_args.push_back(std::make_unique<Argument>("x", &i32_type, &function));
  new_args.push_back(std::make_unique<Argument>("y", &i8_type, &function));
  new_args.push_back(std::make_unique<Argument>("z", &i32_type, &function));

  // 保存新参数指针
  Argument *new_arg1_ptr = new_args[0].get();
  Argument *new_arg2_ptr = new_args[1].get();
  Argument *new_arg3_ptr = new_args[2].get();

  // 设置新的参数列表
  function.setArguments(std::move(new_args));

  // 验证新参数列表
  const auto &new_function_args = function.getArguments();
  EXPECT_EQ(new_function_args.size(), 3);
  EXPECT_EQ(new_function_args[0], new_arg1_ptr);
  EXPECT_EQ(new_function_args[1], new_arg2_ptr);
  EXPECT_EQ(new_function_args[2], new_arg3_ptr);

  // 测试根据索引获取新参数
  EXPECT_EQ(function.getArgByIndex(0), new_arg1_ptr);
  EXPECT_EQ(function.getArgByIndex(1), new_arg2_ptr);
  EXPECT_EQ(function.getArgByIndex(2), new_arg3_ptr);
  EXPECT_EQ(function.getArgByIndex(3), nullptr); // 索引超出范围
}

// 测试函数打印功能
TEST(ValueTest, FunctionPrintTest) {
  Int32Type i32_type;
  Int8Type i8_type;

  // 创建函数类型：i32 (i32, i8)
  std::vector<Type *> param_types = {&i32_type, &i8_type};
  FunctionType func_type(&i32_type, param_types);

  // 创建函数
  Function function("test_func", &func_type, nullptr);

  // 测试未定义函数的打印
  std::string print_result = function.print();
  EXPECT_TRUE(print_result.find("declare") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("@test_func") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result.find("i8") != std::string::npos);

  // 添加参数
  auto arg1 = std::make_unique<Argument>("a", &i32_type, &function);
  auto arg2 = std::make_unique<Argument>("b", &i8_type, &function);
  function.addArgument(std::move(arg1));
  function.addArgument(std::move(arg2));

  // 测试带参数函数的打印
  std::string print_result_with_args = function.print();
  EXPECT_TRUE(print_result_with_args.find("declare") != std::string::npos);
  EXPECT_TRUE(print_result_with_args.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result_with_args.find("@test_func") != std::string::npos);
  EXPECT_TRUE(print_result_with_args.find("i32 %a") != std::string::npos);
  EXPECT_TRUE(print_result_with_args.find("i8 %b") != std::string::npos);

  // 添加基本块
  BasicBlock *bb = function.createBasicBlock("entry");

  // 创建返回指令
  ConstantInt const_val(&i32_type, 42);
  auto ret_inst = std::make_unique<ReturnInst>(&const_val);
  bb->addInstruction(std::move(ret_inst));

  // 测试完整函数的打印（添加基本块后，函数自动设置为已定义状态）
  std::string print_result_full = function.print();
  EXPECT_TRUE(print_result_full.find("define") !=
              std::string::npos); // 现在是define
  EXPECT_TRUE(print_result_full.find("i32") != std::string::npos);
  EXPECT_TRUE(print_result_full.find("@test_func") != std::string::npos);
  EXPECT_TRUE(print_result_full.find("i32 %a") != std::string::npos);
  EXPECT_TRUE(print_result_full.find("i8 %b") != std::string::npos);
  // 由于is_defined_为true，基本块内容被打印出来
  // 基本块名称可能是"entry"或"entry.1"等，所以我们检查基本块名称是否包含"entry"
  EXPECT_TRUE(print_result_full.find("entry") != std::string::npos);
  EXPECT_TRUE(print_result_full.find("ret i32 42") != std::string::npos);
}

// 测试函数类型获取
TEST(ValueTest, FunctionTypeTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  VoidType void_type;

  // 测试返回i32的函数类型
  std::vector<Type *> param_types1 = {&i32_type, &i8_type};
  FunctionType func_type1(&i32_type, param_types1);
  Function function1("test_func1", &func_type1, nullptr);

  EXPECT_EQ(function1.getType(), &func_type1);
  EXPECT_EQ(function1.getType()->getReturnType(), &i32_type);
  EXPECT_EQ(function1.getType()->getNumParams(), 2);
  EXPECT_EQ(function1.getType()->getParamType(0), &i32_type);
  EXPECT_EQ(function1.getType()->getParamType(1), &i8_type);

  // 测试返回void的函数类型
  std::vector<Type *> param_types2 = {};
  FunctionType func_type2(&void_type, param_types2);
  Function function2("test_func2", &func_type2, nullptr);

  EXPECT_EQ(function2.getType(), &func_type2);
  EXPECT_EQ(function2.getType()->getReturnType(), &void_type);
  EXPECT_EQ(function2.getType()->getNumParams(), 0);
}

} // namespace llvm