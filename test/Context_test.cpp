#include "Context.hpp"
#include "Type.hpp"
#include "Value.hpp"
#include "gtest/gtest.h"

namespace llvm {

// 测试LLVMContext的基本功能
TEST(ContextTest, BasicFunctionalityTest) {
  LLVMContext context;

  // 测试获取void类型
  VoidType *void_type = context.getVoidTy();
  ASSERT_NE(void_type, nullptr);
  EXPECT_FALSE(void_type->isPointerTy());
  EXPECT_FALSE(void_type->isAggregateTy());
  EXPECT_EQ(void_type->print(), "void");

  // 测试获取整数类型
  Int32Type *i32_type = context.getInt32Ty();
  ASSERT_NE(i32_type, nullptr);
  EXPECT_EQ(i32_type->print(), "i32");

  Int8Type *i8_type = context.getInt8Ty();
  ASSERT_NE(i8_type, nullptr);
  EXPECT_EQ(i8_type->print(), "i8");

  Int1Type *i1_type = context.getInt1Ty();
  ASSERT_NE(i1_type, nullptr);
  EXPECT_EQ(i1_type->print(), "i1");

  // 测试获取指针类型
  PointerType *ptr_type = context.getPointerType();
  ASSERT_NE(ptr_type, nullptr);
  EXPECT_TRUE(ptr_type->isPointerTy());
  EXPECT_EQ(ptr_type->print(), "ptr");
}

// 测试类型缓存机制
TEST(ContextTest, TypeCachingTest) {
  LLVMContext context;

  // 多次获取相同类型应该返回同一个对象
  VoidType *void_type1 = context.getVoidTy();
  VoidType *void_type2 = context.getVoidTy();
  EXPECT_EQ(void_type1, void_type2);

  Int32Type *i32_type1 = context.getInt32Ty();
  Int32Type *i32_type2 = context.getInt32Ty();
  EXPECT_EQ(i32_type1, i32_type2);

  Int8Type *i8_type1 = context.getInt8Ty();
  Int8Type *i8_type2 = context.getInt8Ty();
  EXPECT_EQ(i8_type1, i8_type2);

  Int1Type *i1_type1 = context.getInt1Ty();
  Int1Type *i1_type2 = context.getInt1Ty();
  EXPECT_EQ(i1_type1, i1_type2);

  // 指针类型也应该是单例
  PointerType *ptr_type1 = context.getPointerType();
  PointerType *ptr_type2 = context.getPointerType();
  EXPECT_EQ(ptr_type1, ptr_type2);
}

// 测试结构体类型创建
TEST(ContextTest, StructTypeTest) {
  LLVMContext context;

  // 创建结构体类型
  StructType *struct_type1 = context.getStructType("MyStruct");
  ASSERT_NE(struct_type1, nullptr);
  EXPECT_EQ(struct_type1->getName(), "%struct.MyStruct");
  EXPECT_TRUE(struct_type1->isAggregateTy());

  // 相同名称的结构体类型应该返回同一个对象
  StructType *struct_type2 = context.getStructType("MyStruct");
  EXPECT_EQ(struct_type1, struct_type2);

  // 不同名称的结构体类型应该返回不同对象
  StructType *struct_type3 = context.getStructType("DifferentStruct");
  ASSERT_NE(struct_type3, nullptr);
  EXPECT_NE(struct_type1, struct_type3);
  EXPECT_EQ(struct_type3->getName(), "%struct.DifferentStruct");
}

// 测试数组类型创建
TEST(ContextTest, ArrayTypeTest) {
  LLVMContext context;

  // 获取元素类型
  Type *i32_type = context.getInt32Ty();
  Type *i8_type = context.getInt8Ty();

  // 创建数组类型
  ArrayType *array_type1 = context.getArrayType(i32_type, 5);
  ASSERT_NE(array_type1, nullptr);
  EXPECT_EQ(array_type1->getElementType(), i32_type);
  EXPECT_EQ(array_type1->getNumElements(), 5);
  EXPECT_TRUE(array_type1->isAggregateTy());

  // 相同参数的数组类型应该返回同一个对象
  ArrayType *array_type2 = context.getArrayType(i32_type, 5);
  EXPECT_EQ(array_type1, array_type2);

  // 不同参数的数组类型应该返回不同对象
  ArrayType *array_type3 = context.getArrayType(i32_type, 3);
  ASSERT_NE(array_type3, nullptr);
  EXPECT_NE(array_type1, array_type3);
  EXPECT_EQ(array_type3->getNumElements(), 3);

  // 不同元素类型的数组类型应该返回不同对象
  ArrayType *array_type4 = context.getArrayType(i8_type, 5);
  ASSERT_NE(array_type4, nullptr);
  EXPECT_NE(array_type1, array_type4);
  EXPECT_EQ(array_type4->getElementType(), i8_type);
}

// 测试函数类型创建
TEST(ContextTest, FunctionTypeTest) {
  LLVMContext context;

  // 获取类型
  VoidType *void_type = context.getVoidTy();
  Int32Type *i32_type = context.getInt32Ty();
  Int8Type *i8_type = context.getInt8Ty();

  // 创建函数类型
  std::vector<Type *> param_types = {i8_type, i8_type};
  FunctionType *func_type1 = context.getFunctionType(i32_type, param_types);
  ASSERT_NE(func_type1, nullptr);
  EXPECT_EQ(func_type1->getReturnType(), i32_type);
  EXPECT_EQ(func_type1->getNumParams(), 2);
  EXPECT_EQ(func_type1->getParamType(0), i8_type);
  EXPECT_EQ(func_type1->getParamType(1), i8_type);

  // 测试void返回类型的函数类型
  FunctionType *void_func_type =
      context.getFunctionType(void_type, param_types);
  ASSERT_NE(void_func_type, nullptr);
  EXPECT_EQ(void_func_type->getReturnType(), void_type);
  EXPECT_EQ(void_func_type->getNumParams(), 2);
  EXPECT_EQ(void_func_type->getParamType(0), i8_type);
  EXPECT_EQ(void_func_type->getParamType(1), i8_type);

  // 测试无参数的void函数类型
  std::vector<Type *> empty_params;
  FunctionType *empty_func_type =
      context.getFunctionType(void_type, empty_params);
  ASSERT_NE(empty_func_type, nullptr);
  EXPECT_EQ(empty_func_type->getReturnType(), void_type);
  EXPECT_EQ(empty_func_type->getNumParams(), 0);

  // 相同参数的函数类型应该返回同一个对象
  FunctionType *func_type2 = context.getFunctionType(i32_type, param_types);
  EXPECT_EQ(func_type1, func_type2);

  // 不同返回类型的函数类型应该返回不同对象
  Type *i1_type = context.getInt1Ty();
  FunctionType *func_type3 = context.getFunctionType(i1_type, param_types);
  ASSERT_NE(func_type3, nullptr);
  EXPECT_NE(func_type1, func_type3);
  EXPECT_EQ(func_type3->getReturnType(), i1_type);

  // 不同参数类型的函数类型应该返回不同对象
  std::vector<Type *> different_param_types = {i8_type};
  FunctionType *func_type4 =
      context.getFunctionType(i32_type, different_param_types);
  ASSERT_NE(func_type4, nullptr);
  EXPECT_NE(func_type1, func_type4);
  EXPECT_EQ(func_type4->getNumParams(), 1);
}

// 测试整数常量创建
TEST(ContextTest, ConstantIntTest) {
  LLVMContext context;

  // 获取整数类型
  Int32Type *i32_type = context.getInt32Ty();
  Int8Type *i8_type = context.getInt8Ty();
  Int1Type *i1_type = context.getInt1Ty();

  // 创建整数常量
  ConstantInt *const_int1 = context.getIntConstant(i32_type, 42);
  ASSERT_NE(const_int1, nullptr);
  EXPECT_EQ(const_int1->getType(), i32_type);
  EXPECT_EQ(const_int1->getValue(), 42);

  // 相同类型和值的常量应该返回同一个对象
  ConstantInt *const_int2 = context.getIntConstant(i32_type, 42);
  EXPECT_EQ(const_int1, const_int2);

  // 不同值的常量应该返回不同对象
  ConstantInt *const_int3 = context.getIntConstant(i32_type, 100);
  ASSERT_NE(const_int3, nullptr);
  EXPECT_NE(const_int1, const_int3);
  EXPECT_EQ(const_int3->getValue(), 100);

  // 不同类型的常量应该返回不同对象
  ConstantInt *const_int4 = context.getIntConstant(i8_type, 42);
  ASSERT_NE(const_int4, nullptr);
  EXPECT_NE(const_int1, const_int4);
  EXPECT_EQ(const_int4->getType(), i8_type);
  EXPECT_EQ(const_int4->getValue(), 42);

  // 测试1位整数常量
  ConstantInt *const_int5 = context.getIntConstant(i1_type, 1);
  ASSERT_NE(const_int5, nullptr);
  EXPECT_EQ(const_int5->getType(), i1_type);
  EXPECT_EQ(const_int5->getValue(), 1);
}

// 测试复杂场景
TEST(ContextTest, ComplexScenarioTest) {
  LLVMContext context;

  // 创建一个复杂的结构体类型
  StructType *struct_type = context.getStructType("ComplexStruct");
  ASSERT_NE(struct_type, nullptr);

  // 设置结构体字段
  Int32Type *i32_type = context.getInt32Ty();
  Int8Type *i8_type = context.getInt8Ty();
  std::vector<Type *> elements = {i32_type, i8_type, i32_type};
  struct_type->setBody(elements);

  EXPECT_EQ(struct_type->getNumElements(), 3);
  EXPECT_EQ(struct_type->getElementType(0), i32_type);
  EXPECT_EQ(struct_type->getElementType(1), i8_type);
  EXPECT_EQ(struct_type->getElementType(2), i32_type);

  // 创建数组类型
  ArrayType *array_type = context.getArrayType(struct_type, 10);
  ASSERT_NE(array_type, nullptr);
  EXPECT_EQ(array_type->getElementType(), struct_type);
  EXPECT_EQ(array_type->getNumElements(), 10);

  // 创建函数类型，使用结构体作为参数
  std::vector<Type *> func_params = {struct_type, array_type};
  FunctionType *func_type = context.getFunctionType(i32_type, func_params);
  ASSERT_NE(func_type, nullptr);
  EXPECT_EQ(func_type->getReturnType(), i32_type);
  EXPECT_EQ(func_type->getNumParams(), 2);
  EXPECT_EQ(func_type->getParamType(0), struct_type);
  EXPECT_EQ(func_type->getParamType(1), array_type);

  // 创建整数常量
  ConstantInt *const_int =
      context.getIntConstant(static_cast<IntegerType *>(i32_type), 12345);
  ASSERT_NE(const_int, nullptr);
  EXPECT_EQ(const_int->getType(), i32_type);
  EXPECT_EQ(const_int->getValue(), 12345);
}

// 测试复合类型（嵌套结构体）
TEST(ContextTest, NestedStructTypeTest) {
  LLVMContext context;

  // 创建基础结构体
  StructType *inner_struct = context.getStructType("InnerStruct");
  ASSERT_NE(inner_struct, nullptr);

  Int32Type *i32_type = context.getInt32Ty();
  Int8Type *i8_type = context.getInt8Ty();
  std::vector<Type *> inner_elements = {i32_type, i8_type};
  inner_struct->setBody(inner_elements);

  // 创建包含嵌套结构体的外层结构体
  StructType *outer_struct = context.getStructType("OuterStruct");
  ASSERT_NE(outer_struct, nullptr);

  Type *i1_type = context.getInt1Ty();
  std::vector<Type *> outer_elements = {i1_type, inner_struct, i32_type};
  outer_struct->setBody(outer_elements);

  // 验证嵌套结构体的属性
  EXPECT_EQ(inner_struct->getNumElements(), 2);
  EXPECT_EQ(inner_struct->getElementType(0), i32_type);
  EXPECT_EQ(inner_struct->getElementType(1), i8_type);

  // 验证外层结构体的属性
  EXPECT_EQ(outer_struct->getNumElements(), 3);
  EXPECT_EQ(outer_struct->getElementType(0), i1_type);
  EXPECT_EQ(outer_struct->getElementType(1), inner_struct);
  EXPECT_EQ(outer_struct->getElementType(2), i32_type);

  // 验证结构体名称
  EXPECT_EQ(inner_struct->getName(), "%struct.InnerStruct");
  EXPECT_EQ(outer_struct->getName(), "%struct.OuterStruct");

  // 验证结构体打印
  EXPECT_EQ(inner_struct->print(), "%struct.InnerStruct");
  EXPECT_EQ(outer_struct->print(), "%struct.OuterStruct");
}

// 测试多维数组和结构体数组
TEST(ContextTest, ComplexArrayTypeTest) {
  LLVMContext context;

  // 获取基础类型
  Int32Type *i32_type = context.getInt32Ty();
  Int8Type *i8_type = context.getInt8Ty();

  // 创建一维数组
  ArrayType *array1d = context.getArrayType(i32_type, 5);
  ASSERT_NE(array1d, nullptr);
  EXPECT_EQ(array1d->getElementType(), i32_type);
  EXPECT_EQ(array1d->getNumElements(), 5);
  EXPECT_EQ(array1d->print(), "[i32 x 5]");

  // 创建二维数组（数组 of 数组）
  ArrayType *array2d = context.getArrayType(array1d, 3);
  ASSERT_NE(array2d, nullptr);
  EXPECT_EQ(array2d->getElementType(), array1d);
  EXPECT_EQ(array2d->getNumElements(), 3);
  EXPECT_EQ(array2d->print(), "[[i32 x 5] x 3]");

  // 创建三维数组
  ArrayType *array3d = context.getArrayType(array2d, 2);
  ASSERT_NE(array3d, nullptr);
  EXPECT_EQ(array3d->getElementType(), array2d);
  EXPECT_EQ(array3d->getNumElements(), 2);
  EXPECT_EQ(array3d->print(), "[[[i32 x 5] x 3] x 2]");

  // 验证数组缓存机制
  ArrayType *array1d_again = context.getArrayType(i32_type, 5);
  EXPECT_EQ(array1d, array1d_again);

  ArrayType *array2d_again = context.getArrayType(array1d, 3);
  EXPECT_EQ(array2d, array2d_again);

  // 创建不同类型的数组
  ArrayType *i8_array = context.getArrayType(i8_type, 10);
  ASSERT_NE(i8_array, nullptr);
  EXPECT_EQ(i8_array->getElementType(), i8_type);
  EXPECT_EQ(i8_array->getNumElements(), 10);
  EXPECT_EQ(i8_array->print(), "[i8 x 10]");

  // 验证不同类型的数组不相同
  EXPECT_NE(array1d, i8_array);
}

// 测试结构体数组
TEST(ContextTest, StructArrayTypeTest) {
  LLVMContext context;

  // 创建结构体类型
  StructType *point_struct = context.getStructType("Point");
  ASSERT_NE(point_struct, nullptr);

  Int32Type *i32_type = context.getInt32Ty();
  std::vector<Type *> point_elements = {i32_type, i32_type}; // x, y coordinates
  point_struct->setBody(point_elements);

  // 创建结构体数组
  ArrayType *point_array = context.getArrayType(point_struct, 10);
  ASSERT_NE(point_array, nullptr);
  EXPECT_EQ(point_array->getElementType(), point_struct);
  EXPECT_EQ(point_array->getNumElements(), 10);
  EXPECT_EQ(point_array->print(), "[%struct.Point x 10]");

  // 创建更复杂的结构体数组
  StructType *line_struct = context.getStructType("Line");
  ASSERT_NE(line_struct, nullptr);

  std::vector<Type *> line_elements = {point_struct,
                                       point_struct}; // start, end points
  line_struct->setBody(line_elements);

  ArrayType *line_array = context.getArrayType(line_struct, 5);
  ASSERT_NE(line_array, nullptr);
  EXPECT_EQ(line_array->getElementType(), line_struct);
  EXPECT_EQ(line_array->getNumElements(), 5);
  EXPECT_EQ(line_array->print(), "[%struct.Line x 5]");

  // 验证结构体定义打印
  EXPECT_EQ(point_struct->printDef(), "%struct.Point = type { i32, i32 }");
  EXPECT_EQ(line_struct->printDef(),
            "%struct.Line = type { %struct.Point, %struct.Point }");
}

// 测试复合类型与数组的复杂组合
TEST(ContextTest, ComplexCompositeTypeTest) {
  LLVMContext context;

  // 创建基础类型
  Int32Type *i32_type = context.getInt32Ty();
  Int8Type *i8_type = context.getInt8Ty();
  Int1Type *i1_type = context.getInt1Ty();

  // 创建简单结构体
  StructType *simple_struct = context.getStructType("Simple");
  std::vector<Type *> simple_elements = {i32_type, i8_type};
  simple_struct->setBody(simple_elements);

  // 创建包含数组的结构体
  StructType *array_struct = context.getStructType("ArrayContainer");
  ArrayType *i32_array = context.getArrayType(i32_type, 4);
  std::vector<Type *> array_elements = {i1_type, i32_array, i8_type};
  array_struct->setBody(array_elements);

  // 创建结构体数组
  ArrayType *struct_array = context.getArrayType(simple_struct, 3);

  // 创建包含结构体数组的复杂结构体
  StructType *complex_struct = context.getStructType("Complex");
  std::vector<Type *> complex_elements = {i32_type, struct_array, array_struct};
  complex_struct->setBody(complex_elements);

  // 验证所有类型都正确创建
  ASSERT_NE(simple_struct, nullptr);
  ASSERT_NE(array_struct, nullptr);
  ASSERT_NE(i32_array, nullptr);
  ASSERT_NE(struct_array, nullptr);
  ASSERT_NE(complex_struct, nullptr);

  // 验证类型属性
  EXPECT_EQ(simple_struct->getNumElements(), 2);
  EXPECT_EQ(array_struct->getNumElements(), 3);
  EXPECT_EQ(complex_struct->getNumElements(), 3);

  EXPECT_EQ(array_struct->getElementType(1), i32_array);
  EXPECT_EQ(complex_struct->getElementType(1), struct_array);
  EXPECT_EQ(complex_struct->getElementType(2), array_struct);

  // 验证数组属性
  EXPECT_EQ(i32_array->getElementType(), i32_type);
  EXPECT_EQ(i32_array->getNumElements(), 4);
  EXPECT_EQ(struct_array->getElementType(), simple_struct);
  EXPECT_EQ(struct_array->getNumElements(), 3);

  // 验证打印输出
  EXPECT_EQ(simple_struct->printDef(), "%struct.Simple = type { i32, i8 }");
  EXPECT_EQ(array_struct->printDef(),
            "%struct.ArrayContainer = type { i1, [i32 x 4], i8 }");
  EXPECT_EQ(i32_array->print(), "[i32 x 4]");
  EXPECT_EQ(struct_array->print(), "[%struct.Simple x 3]");
  EXPECT_EQ(complex_struct->printDef(),
            "%struct.Complex = type { i32, [%struct.Simple x 3], "
            "%struct.ArrayContainer }");

  // 创建使用这些复杂类型的函数类型
  std::vector<Type *> func_params = {complex_struct, struct_array, i32_type};
  FunctionType *complex_func_type =
      context.getFunctionType(complex_struct, func_params);
  ASSERT_NE(complex_func_type, nullptr);
  EXPECT_EQ(complex_func_type->getReturnType(), complex_struct);
  EXPECT_EQ(complex_func_type->getNumParams(), 3);
  EXPECT_EQ(complex_func_type->getParamType(0), complex_struct);
  EXPECT_EQ(complex_func_type->getParamType(1), struct_array);
  EXPECT_EQ(complex_func_type->getParamType(2), i32_type);
}

} // namespace llvm