#include "gtest/gtest.h"
#include "Type.hpp"

namespace llvm {

// 测试整数类型
TEST(TypeTest, Int32TypeTest) {
    Int32Type i32_type;
    
    EXPECT_FALSE(i32_type.isPointerTy());
    EXPECT_FALSE(i32_type.isAggregateTy());
    EXPECT_EQ(i32_type.print(), "i32");
    
    // 测试类型相等性
    Int32Type another_i32_type;
    EXPECT_TRUE(i32_type.isEqual(&another_i32_type));
    
    Int8Type i8_type;
    EXPECT_FALSE(i32_type.isEqual(&i8_type));
}

TEST(TypeTest, Int8TypeTest) {
    Int8Type i8_type;
    
    EXPECT_FALSE(i8_type.isPointerTy());
    EXPECT_FALSE(i8_type.isAggregateTy());
    EXPECT_EQ(i8_type.print(), "i8");
    
    // 测试类型相等性
    Int8Type another_i8_type;
    EXPECT_TRUE(i8_type.isEqual(&another_i8_type));
    
    Int32Type i32_type;
    EXPECT_FALSE(i8_type.isEqual(&i32_type));
}

TEST(TypeTest, Int1TypeTest) {
    Int1Type i1_type;
    
    EXPECT_FALSE(i1_type.isPointerTy());
    EXPECT_FALSE(i1_type.isAggregateTy());
    EXPECT_EQ(i1_type.print(), "i1");
    
    // 测试类型相等性
    Int1Type another_i1_type;
    EXPECT_TRUE(i1_type.isEqual(&another_i1_type));
    
    Int32Type i32_type;
    EXPECT_FALSE(i1_type.isEqual(&i32_type));
}

// 测试结构体类型
TEST(TypeTest, StructTypeTest) {
    StructType struct_type("MyStruct");
    
    EXPECT_FALSE(struct_type.isPointerTy());
    EXPECT_TRUE(struct_type.isAggregateTy());
    EXPECT_EQ(struct_type.getName(), "MyStruct");
    EXPECT_EQ(struct_type.getNumElements(), 0);
    
    // 设置结构体字段
    Int32Type i32_type;
    Int8Type i8_type;
    std::vector<Type*> elements = {&i32_type, &i8_type};
    struct_type.setBody(elements);
    
    EXPECT_EQ(struct_type.getNumElements(), 2);
    EXPECT_EQ(struct_type.getElementType(0), &i32_type);
    EXPECT_EQ(struct_type.getElementType(1), &i8_type);
    
    // 测试打印
    std::string print_result = struct_type.print();
    EXPECT_TRUE(print_result.find("MyStruct") != std::string::npos);
    EXPECT_TRUE(print_result.find("i32") != std::string::npos);
    EXPECT_TRUE(print_result.find("i8") != std::string::npos);
    
    // 测试类型相等性
    StructType another_struct_type("MyStruct");
    another_struct_type.setBody(elements);
    EXPECT_TRUE(struct_type.isEqual(&another_struct_type));
    
    StructType different_struct_type("DifferentStruct");
    different_struct_type.setBody(elements);
    EXPECT_FALSE(struct_type.isEqual(&different_struct_type));
}

// 测试数组类型
TEST(TypeTest, ArrayTypeTest) {
    Int32Type i32_type;
    ArrayType array_type(&i32_type, 5);
    
    EXPECT_FALSE(array_type.isPointerTy());
    EXPECT_TRUE(array_type.isAggregateTy());
    EXPECT_EQ(array_type.getElementType(), &i32_type);
    EXPECT_EQ(array_type.getNumElements(), 5);
    
    // 测试打印
    std::string print_result = array_type.print();
    EXPECT_TRUE(print_result.find("i32") != std::string::npos);
    EXPECT_TRUE(print_result.find("5") != std::string::npos);
    
    // 测试类型相等性
    ArrayType another_array_type(&i32_type, 5);
    EXPECT_TRUE(array_type.isEqual(&another_array_type));
    
    ArrayType different_array_type(&i32_type, 3);
    EXPECT_FALSE(array_type.isEqual(&different_array_type));
    
    Int8Type i8_type;
    ArrayType different_element_array_type(&i8_type, 5);
    EXPECT_FALSE(array_type.isEqual(&different_element_array_type));
}

// 测试函数类型
TEST(TypeTest, FunctionTypeTest) {
    Int32Type i32_type;
    Int8Type i8_type;
    std::vector<Type*> param_types = {&i8_type, &i8_type};
    
    FunctionType func_type(&i32_type, param_types);
    
    EXPECT_FALSE(func_type.isPointerTy());
    EXPECT_FALSE(func_type.isAggregateTy());
    EXPECT_EQ(func_type.getReturnType(), &i32_type);
    EXPECT_EQ(func_type.getNumParams(), 2);
    EXPECT_EQ(func_type.getParamType(0), &i8_type);
    EXPECT_EQ(func_type.getParamType(1), &i8_type);
    
    // 测试打印
    std::string print_result = func_type.print();
    EXPECT_TRUE(print_result.find("i32") != std::string::npos);
    EXPECT_TRUE(print_result.find("i8") != std::string::npos);
    
    // 测试类型相等性
    FunctionType another_func_type(&i32_type, param_types);
    EXPECT_TRUE(func_type.isEqual(&another_func_type));
    
    Int1Type i1_type;
    FunctionType different_return_func_type(&i1_type, param_types);
    EXPECT_FALSE(func_type.isEqual(&different_return_func_type));
    
    std::vector<Type*> different_param_types = {&i8_type};
    FunctionType different_param_func_type(&i32_type, different_param_types);
    EXPECT_FALSE(func_type.isEqual(&different_param_func_type));
    
    // 测试修改参数类型和返回类型
    func_type.setParamType(0, &i1_type);
    EXPECT_EQ(func_type.getParamType(0), &i1_type);
    
    func_type.setReturnType(&i8_type);
    EXPECT_EQ(func_type.getReturnType(), &i8_type);
    
    func_type.addParamType(&i1_type);
    EXPECT_EQ(func_type.getNumParams(), 3);
    EXPECT_EQ(func_type.getParamType(2), &i1_type);
}

// 测试指针类型
TEST(TypeTest, PointerTypeTest) {
    PointerType ptr_type;
    
    EXPECT_TRUE(ptr_type.isPointerTy());
    EXPECT_FALSE(ptr_type.isAggregateTy());
    EXPECT_EQ(ptr_type.print(), "ptr");
    
    // 测试类型相等性
    PointerType another_ptr_type;
    EXPECT_TRUE(ptr_type.isEqual(&another_ptr_type));
    
    Int32Type i32_type;
    EXPECT_FALSE(ptr_type.isEqual(&i32_type));
}

} // namespace llvm