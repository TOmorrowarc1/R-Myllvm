#include "gtest/gtest.h"
#include "Value.hpp"

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
    EXPECT_TRUE(print_result_no_init.find("@my_global_no_init") != std::string::npos);
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
    EXPECT_EQ(bin_op.getName(), "result");
    EXPECT_EQ(bin_op.getOp(), "add");
    EXPECT_EQ(bin_op.getLHS(), &lhs);
    EXPECT_EQ(bin_op.getRHS(), &rhs);
    
    // 测试操作数列表
    const auto& operands = bin_op.getOperands();
    EXPECT_EQ(operands.size(), 2);
    EXPECT_EQ(operands[0], &lhs);
    EXPECT_EQ(operands[1], &rhs);
    
    // 测试用户列表
    const auto& lhs_users = lhs.getUsers();
    EXPECT_EQ(lhs_users.size(), 1);
    EXPECT_EQ(lhs_users[0], &bin_op);
    
    const auto& rhs_users = rhs.getUsers();
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

// 测试一元运算指令
TEST(ValueTest, UnaryOperatorTest) {
    Int32Type i32_type;
    ConstantInt operand(&i32_type, 10);
    
    UnaryOperator un_op("result", "neg", &i32_type, &operand);
    
    EXPECT_EQ(un_op.getType(), &i32_type);
    EXPECT_EQ(un_op.getName(), "result");
    EXPECT_EQ(un_op.getOp(), "neg");
    EXPECT_EQ(un_op.getOperand(), &operand);
    
    // 测试操作数列表
    const auto& operands = un_op.getOperands();
    EXPECT_EQ(operands.size(), 1);
    EXPECT_EQ(operands[0], &operand);
    
    // 测试用户列表
    const auto& operand_users = operand.getUsers();
    EXPECT_EQ(operand_users.size(), 1);
    EXPECT_EQ(operand_users[0], &un_op);
    
    // 测试打印
    std::string print_result = un_op.print();
    EXPECT_TRUE(print_result.find("%result") != std::string::npos);
    EXPECT_TRUE(print_result.find("neg") != std::string::npos);
    EXPECT_TRUE(print_result.find("i32") != std::string::npos);
    EXPECT_TRUE(print_result.find("10") != std::string::npos);
}

// 测试加载指令
TEST(ValueTest, LoadInstTest) {
    Int32Type i32_type;
    PointerType ptr_type;
    ConstantInt ptr_value(&ptr_type, 1000); // 模拟指针值
    
    LoadInst load_inst("result", &i32_type, &ptr_value);
    
    EXPECT_EQ(load_inst.getType(), &i32_type);
    EXPECT_EQ(load_inst.getName(), "result");
    EXPECT_EQ(load_inst.getPtr(), &ptr_value);
    
    // 测试操作数列表
    const auto& operands = load_inst.getOperands();
    EXPECT_EQ(operands.size(), 1);
    EXPECT_EQ(operands[0], &ptr_value);
    
    // 测试用户列表
    const auto& ptr_users = ptr_value.getUsers();
    EXPECT_EQ(ptr_users.size(), 1);
    EXPECT_EQ(ptr_users[0], &load_inst);
    
    // 测试打印
    std::string print_result = load_inst.print();
    EXPECT_TRUE(print_result.find("%result") != std::string::npos);
    EXPECT_TRUE(print_result.find("load") != std::string::npos);
    EXPECT_TRUE(print_result.find("i32") != std::string::npos);
    EXPECT_TRUE(print_result.find("ptr") != std::string::npos);
    EXPECT_TRUE(print_result.find("1000") != std::string::npos);
}

// 测试存储指令
TEST(ValueTest, StoreInstTest) {
    Int32Type i32_type;
    PointerType ptr_type;
    ConstantInt value(&i32_type, 42);
    ConstantInt ptr(&ptr_type, 1000); // 模拟指针值
    
    StoreInst store_inst(&i32_type, &ptr, &value);
    
    EXPECT_EQ(store_inst.getType(), &i32_type);
    EXPECT_EQ(store_inst.getPtr(), &ptr);
    EXPECT_EQ(store_inst.getValue(), &value);
    
    // 测试操作数列表
    const auto& operands = store_inst.getOperands();
    EXPECT_EQ(operands.size(), 2);
    EXPECT_EQ(operands[0], &ptr);
    EXPECT_EQ(operands[1], &value);
    
    // 测试用户列表
    const auto& ptr_users = ptr.getUsers();
    EXPECT_EQ(ptr_users.size(), 1);
    EXPECT_EQ(ptr_users[0], &store_inst);
    
    const auto& value_users = value.getUsers();
    EXPECT_EQ(value_users.size(), 1);
    EXPECT_EQ(value_users[0], &store_inst);
    
    // 测试打印
    std::string print_result = store_inst.print();
    EXPECT_TRUE(print_result.find("store") != std::string::npos);
    EXPECT_TRUE(print_result.find("i32") != std::string::npos);
    EXPECT_TRUE(print_result.find("42") != std::string::npos);
    EXPECT_TRUE(print_result.find("ptr") != std::string::npos);
    EXPECT_TRUE(print_result.find("1000") != std::string::npos);
}

// 测试内存分配指令
TEST(ValueTest, AllocaInstTest) {
    Int32Type i32_type;
    
    AllocaInst alloca_inst("ptr", &i32_type);
    
    EXPECT_EQ(alloca_inst.getName(), "ptr");
    
    // 测试类型 - 应该是指针类型
    Type* alloca_type = alloca_inst.getType();
    EXPECT_TRUE(alloca_type->isPointerTy());
    EXPECT_FALSE(alloca_type->isAggregateTy());
    
    // 测试打印
    std::string print_result = alloca_inst.print();
    EXPECT_TRUE(print_result.find("%ptr") != std::string::npos);
    EXPECT_TRUE(print_result.find("alloca") != std::string::npos);
    EXPECT_TRUE(print_result.find("i32") != std::string::npos);
}

// 测试整数比较指令
TEST(ValueTest, ICmpInstTest) {
    Int32Type i32_type;
    Int1Type i1_type;
    ConstantInt lhs(&i32_type, 10);
    ConstantInt rhs(&i32_type, 20);
    
    ICmpInst icmp_inst("result", "eq", &i1_type, &lhs, &rhs);
    
    EXPECT_EQ(icmp_inst.getType(), &i1_type);
    EXPECT_EQ(icmp_inst.getName(), "result");
    EXPECT_EQ(icmp_inst.getPredicate(), "eq");
    EXPECT_EQ(icmp_inst.getLHS(), &lhs);
    EXPECT_EQ(icmp_inst.getRHS(), &rhs);
    
    // 测试操作数列表
    const auto& operands = icmp_inst.getOperands();
    EXPECT_EQ(operands.size(), 2);
    EXPECT_EQ(operands[0], &lhs);
    EXPECT_EQ(operands[1], &rhs);
    
    // 测试用户列表
    const auto& lhs_users = lhs.getUsers();
    EXPECT_EQ(lhs_users.size(), 1);
    EXPECT_EQ(lhs_users[0], &icmp_inst);
    
    const auto& rhs_users = rhs.getUsers();
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

} // namespace llvm