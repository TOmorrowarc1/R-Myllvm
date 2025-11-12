#include "gtest/gtest.h"
#include "Context.hpp"
#include "Type.hpp"
#include "Value.hpp"

namespace llvm {

// 测试LLVMContext的基本功能
TEST(ContextTest, BasicFunctionalityTest) {
    LLVMContext context;
    
    // 测试获取整数类型
    Type* i32_type = context.getInt32Ty();
    ASSERT_NE(i32_type, nullptr);
    EXPECT_EQ(i32_type->print(), "i32");
    
    Type* i8_type = context.getInt8Ty();
    ASSERT_NE(i8_type, nullptr);
    EXPECT_EQ(i8_type->print(), "i8");
    
    Type* i1_type = context.getInt1Ty();
    ASSERT_NE(i1_type, nullptr);
    EXPECT_EQ(i1_type->print(), "i1");
    
    // 测试获取指针类型
    PointerType* ptr_type = context.getPointerType();
    ASSERT_NE(ptr_type, nullptr);
    EXPECT_TRUE(ptr_type->isPointerTy());
    EXPECT_EQ(ptr_type->print(), "ptr");
}

// 测试类型缓存机制
TEST(ContextTest, TypeCachingTest) {
    LLVMContext context;
    
    // 多次获取相同类型应该返回同一个对象
    Type* i32_type1 = context.getInt32Ty();
    Type* i32_type2 = context.getInt32Ty();
    EXPECT_EQ(i32_type1, i32_type2);
    
    Type* i8_type1 = context.getInt8Ty();
    Type* i8_type2 = context.getInt8Ty();
    EXPECT_EQ(i8_type1, i8_type2);
    
    Type* i1_type1 = context.getInt1Ty();
    Type* i1_type2 = context.getInt1Ty();
    EXPECT_EQ(i1_type1, i1_type2);
    
    // 指针类型也应该是单例
    PointerType* ptr_type1 = context.getPointerType();
    PointerType* ptr_type2 = context.getPointerType();
    EXPECT_EQ(ptr_type1, ptr_type2);
}

// 测试结构体类型创建
TEST(ContextTest, StructTypeTest) {
    LLVMContext context;
    
    // 创建结构体类型
    StructType* struct_type1 = context.getStructType("MyStruct");
    ASSERT_NE(struct_type1, nullptr);
    EXPECT_EQ(struct_type1->getName(), "MyStruct");
    EXPECT_TRUE(struct_type1->isAggregateTy());
    
    // 相同名称的结构体类型应该返回同一个对象
    StructType* struct_type2 = context.getStructType("MyStruct");
    EXPECT_EQ(struct_type1, struct_type2);
    
    // 不同名称的结构体类型应该返回不同对象
    StructType* struct_type3 = context.getStructType("DifferentStruct");
    ASSERT_NE(struct_type3, nullptr);
    EXPECT_NE(struct_type1, struct_type3);
    EXPECT_EQ(struct_type3->getName(), "DifferentStruct");
}

// 测试数组类型创建
TEST(ContextTest, ArrayTypeTest) {
    LLVMContext context;
    
    // 获取元素类型
    Type* i32_type = context.getInt32Ty();
    Type* i8_type = context.getInt8Ty();
    
    // 创建数组类型
    ArrayType* array_type1 = context.getArrayType(i32_type, 5);
    ASSERT_NE(array_type1, nullptr);
    EXPECT_EQ(array_type1->getElementType(), i32_type);
    EXPECT_EQ(array_type1->getNumElements(), 5);
    EXPECT_TRUE(array_type1->isAggregateTy());
    
    // 相同参数的数组类型应该返回同一个对象
    ArrayType* array_type2 = context.getArrayType(i32_type, 5);
    EXPECT_EQ(array_type1, array_type2);
    
    // 不同参数的数组类型应该返回不同对象
    ArrayType* array_type3 = context.getArrayType(i32_type, 3);
    ASSERT_NE(array_type3, nullptr);
    EXPECT_NE(array_type1, array_type3);
    EXPECT_EQ(array_type3->getNumElements(), 3);
    
    // 不同元素类型的数组类型应该返回不同对象
    ArrayType* array_type4 = context.getArrayType(i8_type, 5);
    ASSERT_NE(array_type4, nullptr);
    EXPECT_NE(array_type1, array_type4);
    EXPECT_EQ(array_type4->getElementType(), i8_type);
}

// 测试函数类型创建
TEST(ContextTest, FunctionTypeTest) {
    LLVMContext context;
    
    // 获取类型
    Type* i32_type = context.getInt32Ty();
    Type* i8_type = context.getInt8Ty();
    
    // 创建函数类型
    std::vector<Type*> param_types = {i8_type, i8_type};
    FunctionType* func_type1 = context.getFunctionType(i32_type, param_types);
    ASSERT_NE(func_type1, nullptr);
    EXPECT_EQ(func_type1->getReturnType(), i32_type);
    EXPECT_EQ(func_type1->getNumParams(), 2);
    EXPECT_EQ(func_type1->getParamType(0), i8_type);
    EXPECT_EQ(func_type1->getParamType(1), i8_type);
    
    // 相同参数的函数类型应该返回同一个对象
    FunctionType* func_type2 = context.getFunctionType(i32_type, param_types);
    EXPECT_EQ(func_type1, func_type2);
    
    // 不同返回类型的函数类型应该返回不同对象
    Type* i1_type = context.getInt1Ty();
    FunctionType* func_type3 = context.getFunctionType(i1_type, param_types);
    ASSERT_NE(func_type3, nullptr);
    EXPECT_NE(func_type1, func_type3);
    EXPECT_EQ(func_type3->getReturnType(), i1_type);
    
    // 不同参数类型的函数类型应该返回不同对象
    std::vector<Type*> different_param_types = {i8_type};
    FunctionType* func_type4 = context.getFunctionType(i32_type, different_param_types);
    ASSERT_NE(func_type4, nullptr);
    EXPECT_NE(func_type1, func_type4);
    EXPECT_EQ(func_type4->getNumParams(), 1);
}

// 测试整数常量创建
TEST(ContextTest, ConstantIntTest) {
    LLVMContext context;
    
    // 获取整数类型
    Type* i32_type = context.getInt32Ty();
    Type* i8_type = context.getInt8Ty();
    Type* i1_type = context.getInt1Ty();
    
    // 创建整数常量
    ConstantInt* const_int1 = context.getIntConstant(static_cast<IntegerType*>(i32_type), 42);
    ASSERT_NE(const_int1, nullptr);
    EXPECT_EQ(const_int1->getType(), i32_type);
    EXPECT_EQ(const_int1->getValue(), 42);
    
    // 相同类型和值的常量应该返回同一个对象
    ConstantInt* const_int2 = context.getIntConstant(static_cast<IntegerType*>(i32_type), 42);
    EXPECT_EQ(const_int1, const_int2);
    
    // 不同值的常量应该返回不同对象
    ConstantInt* const_int3 = context.getIntConstant(static_cast<IntegerType*>(i32_type), 100);
    ASSERT_NE(const_int3, nullptr);
    EXPECT_NE(const_int1, const_int3);
    EXPECT_EQ(const_int3->getValue(), 100);
    
    // 不同类型的常量应该返回不同对象
    ConstantInt* const_int4 = context.getIntConstant(static_cast<IntegerType*>(i8_type), 42);
    ASSERT_NE(const_int4, nullptr);
    EXPECT_NE(const_int1, const_int4);
    EXPECT_EQ(const_int4->getType(), i8_type);
    EXPECT_EQ(const_int4->getValue(), 42);
    
    // 测试1位整数常量
    ConstantInt* const_int5 = context.getIntConstant(static_cast<IntegerType*>(i1_type), 1);
    ASSERT_NE(const_int5, nullptr);
    EXPECT_EQ(const_int5->getType(), i1_type);
    EXPECT_EQ(const_int5->getValue(), 1);
}

// 测试复杂场景
TEST(ContextTest, ComplexScenarioTest) {
    LLVMContext context;
    
    // 创建一个复杂的结构体类型
    StructType* struct_type = context.getStructType("ComplexStruct");
    ASSERT_NE(struct_type, nullptr);
    
    // 设置结构体字段
    Type* i32_type = context.getInt32Ty();
    Type* i8_type = context.getInt8Ty();
    std::vector<Type*> elements = {i32_type, i8_type, i32_type};
    struct_type->setBody(elements);
    
    EXPECT_EQ(struct_type->getNumElements(), 3);
    EXPECT_EQ(struct_type->getElementType(0), i32_type);
    EXPECT_EQ(struct_type->getElementType(1), i8_type);
    EXPECT_EQ(struct_type->getElementType(2), i32_type);
    
    // 创建数组类型
    ArrayType* array_type = context.getArrayType(struct_type, 10);
    ASSERT_NE(array_type, nullptr);
    EXPECT_EQ(array_type->getElementType(), struct_type);
    EXPECT_EQ(array_type->getNumElements(), 10);
    
    // 创建函数类型，使用结构体作为参数
    std::vector<Type*> func_params = {struct_type, array_type};
    FunctionType* func_type = context.getFunctionType(i32_type, func_params);
    ASSERT_NE(func_type, nullptr);
    EXPECT_EQ(func_type->getReturnType(), i32_type);
    EXPECT_EQ(func_type->getNumParams(), 2);
    EXPECT_EQ(func_type->getParamType(0), struct_type);
    EXPECT_EQ(func_type->getParamType(1), array_type);
    
    // 创建整数常量
    ConstantInt* const_int = context.getIntConstant(static_cast<IntegerType*>(i32_type), 12345);
    ASSERT_NE(const_int, nullptr);
    EXPECT_EQ(const_int->getType(), i32_type);
    EXPECT_EQ(const_int->getValue(), 12345);
}

} // namespace llvm