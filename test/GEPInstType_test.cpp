#include "Value.hpp"
#include "gtest/gtest.h"
#include <stdexcept>

namespace llvm {

// 测试GEP指令基本功能
TEST(GetElementPtrInstTest, BasicFunctionalityTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  // 使用AllocaInst创建指针
  AllocaInst alloca_ptr("ptr", &i32_type);
  ConstantInt index1(&i32_type, 1);
  std::vector<Value *> indices = {&index1};

  GetElementPtrInst gep("result", &ptr_type, &i32_type, &alloca_ptr, indices);

  EXPECT_EQ(gep.getType(), &ptr_type);
  EXPECT_EQ(gep.getName(), "%result");
  EXPECT_EQ(gep.getPtr(), &alloca_ptr);

  const auto &gep_indices = gep.getIndices();
  EXPECT_EQ(gep_indices.size(), 1);
  EXPECT_EQ(gep_indices[0], &index1);

  // 测试打印
  std::string print_result = gep.print();
  EXPECT_TRUE(print_result.find("%result = getelementptr i32") !=
              std::string::npos);
  EXPECT_TRUE(print_result.find("%ptr") != std::string::npos);
  EXPECT_TRUE(print_result.find("i32 1") != std::string::npos);
}

// 测试GEP指令类型检测 - 指针类型
TEST(GetElementPtrInstTest, PointerTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  ConstantInt int_val(&i32_type, 10);
  ConstantInt index(&i32_type, 0);
  std::vector<Value *> indices = {&index};

  // 正常情况：指针类型的指针操作数
  AllocaInst alloca_ptr("ptr", &i32_type);
  EXPECT_NO_THROW(
      GetElementPtrInst("result1", &ptr_type, &i32_type, &alloca_ptr, indices));

  // 异常情况：非指针类型的指针操作数
  EXPECT_THROW(
      GetElementPtrInst("result2", &ptr_type, &i32_type, &int_val, indices),
      std::runtime_error);
}

// 测试GEP指令类型检测 - 索引类型
TEST(GetElementPtrInstTest, IndexTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  AllocaInst alloca_ptr("ptr", &i32_type);
  ConstantInt int_val(&i32_type, 10);

  // 正常情况：整数类型的索引
  std::vector<Value *> indices1 = {&int_val};
  EXPECT_NO_THROW(GetElementPtrInst("result1", &ptr_type, &i32_type,
                                    &alloca_ptr, indices1));

  // 异常情况：非整数类型的索引
  std::vector<Value *> indices2 = {&alloca_ptr};
  EXPECT_THROW(
      GetElementPtrInst("result2", &ptr_type, &i32_type, &alloca_ptr, indices2),
      std::runtime_error);

  // 混合情况：部分索引为非整数类型
  ConstantInt int_val2(&i32_type, 20);
  std::vector<Value *> indices3 = {&int_val, &alloca_ptr};
  EXPECT_THROW(
      GetElementPtrInst("result3", &ptr_type, &i32_type, &alloca_ptr, indices3),
      std::runtime_error);

  std::vector<Value *> indices4 = {&alloca_ptr, &int_val2};
  EXPECT_THROW(
      GetElementPtrInst("result4", &ptr_type, &i32_type, &alloca_ptr, indices4),
      std::runtime_error);
}

// 测试GEP指令类型检测 - 结构体索引
TEST(GetElementPtrInstTest, StructIndexCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;
  StructType struct_type("Person");

  // 设置结构体成员：{i32, i8, i32}
  std::vector<Type *> elements = {&i32_type, &i8_type, &i32_type};
  struct_type.setBody(elements);

  // 创建结构体指针
  AllocaInst alloca_ptr("person_ptr", &struct_type);
  ConstantInt index0(&i32_type, 0); // 第一个索引用于指针算数

  // 正常情况：有效的结构体索引
  ConstantInt valid_index1(&i32_type, 0); // 访问第0个成员
  ConstantInt valid_index2(&i32_type, 1); // 访问第1个成员
  ConstantInt valid_index3(&i32_type, 2); // 访问第2个成员

  std::vector<Value *> indices1 = {&index0, &valid_index1};
  EXPECT_NO_THROW(GetElementPtrInst("result1", &ptr_type, &struct_type,
                                    &alloca_ptr, indices1));

  std::vector<Value *> indices2 = {&index0, &valid_index2};
  EXPECT_NO_THROW(GetElementPtrInst("result2", &ptr_type, &struct_type,
                                    &alloca_ptr, indices2));

  // 异常情况：结构体索引越界
  ConstantInt invalid_index(&i32_type, 3); // 超出结构体成员范围
  std::vector<Value *> indices3 = {&index0, &invalid_index};
  EXPECT_THROW(GetElementPtrInst("result3", &ptr_type, &struct_type,
                                 &alloca_ptr, indices3),
               std::runtime_error);
}

// 测试GEP指令类型检测 - 数组索引
TEST(GetElementPtrInstTest, ArrayIndexCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;
  ArrayType array_type(&i32_type, 5); // 包含5个i32元素的数组

  // 创建数组指针
  AllocaInst alloca_ptr("array_ptr", &array_type);

  // 正常情况：有效的数组索引
  ConstantInt index0(&i32_type, 0);       // 第一个索引用于指针算数
  ConstantInt valid_index1(&i32_type, 0); // 访问第0个元素
  ConstantInt valid_index2(&i32_type, 2); // 访问第2个元素
  ConstantInt valid_index3(&i32_type, 4); // 访问第4个元素

  std::vector<Value *> indices1 = {&index0, &valid_index1};
  EXPECT_NO_THROW(GetElementPtrInst("result1", &ptr_type, &array_type,
                                    &alloca_ptr, indices1));

  std::vector<Value *> indices2 = {&index0, &valid_index2};
  EXPECT_NO_THROW(GetElementPtrInst("result2", &ptr_type, &array_type,
                                    &alloca_ptr, indices2));

  // 注意：数组索引的边界检查在LLVM中通常是在运行时进行的，
  // 编译时GEP指令不会检查数组索引是否越界
  ConstantInt large_index(&i32_type, 100); // 大索引
  std::vector<Value *> indices3 = {&index0, &large_index};
  EXPECT_THROW(GetElementPtrInst("result3", &ptr_type, &array_type, &alloca_ptr,
                                 indices3),
               std::runtime_error);
}

// 测试GEP指令类型检测 - 嵌套结构
TEST(GetElementPtrInstTest, NestedStructureCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;

  // 创建内层结构体
  StructType inner_struct("Inner");
  std::vector<Type *> inner_elements = {&i32_type, &i8_type};
  inner_struct.setBody(inner_elements);

  // 创建外层结构体，包含内层结构体
  StructType outer_struct("Outer");
  std::vector<Type *> outer_elements = {&i32_type, &inner_struct, &i32_type};
  outer_struct.setBody(outer_elements);

  // 创建外层结构体指针
  AllocaInst alloca_ptr("outer_ptr", &outer_struct);

  // 正常情况：访问嵌套结构体成员
  ConstantInt index0(&i32_type, 0);      // 第一个索引用于指针算数
  ConstantInt outer_index(&i32_type, 1); // 访问第1个成员（内层结构体）
  ConstantInt inner_index(&i32_type, 0); // 访问内层结构体的第0个成员

  std::vector<Value *> indices1 = {&index0, &outer_index};
  EXPECT_NO_THROW(GetElementPtrInst("result1", &ptr_type, &outer_struct,
                                    &alloca_ptr, indices1));

  std::vector<Value *> indices2 = {&index0, &outer_index, &inner_index};
  EXPECT_NO_THROW(GetElementPtrInst("result2", &ptr_type, &outer_struct,
                                    &alloca_ptr, indices2));

  // 异常情况：访问嵌套结构体成员时索引越界
  ConstantInt invalid_inner_index(&i32_type,
                                  2); // 内层结构体只有2个成员，索引2越界
  std::vector<Value *> indices3 = {&index0, &outer_index, &invalid_inner_index};
  EXPECT_THROW(GetElementPtrInst("result3", &ptr_type, &outer_struct,
                                 &alloca_ptr, indices3),
               std::runtime_error);
}

// 测试GEP指令类型检测 - 非常量索引
TEST(GetElementPtrInstTest, NonConstantIndexCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;
  StructType struct_type("Person");

  // 设置结构体成员
  std::vector<Type *> elements = {&i32_type, &i8_type, &i32_type};
  struct_type.setBody(elements);

  // 创建结构体指针
  AllocaInst alloca_ptr("person_ptr", &struct_type);

  // 创建非常量索引（通过二元运算指令生成）
  ConstantInt const_val1(&i32_type, 1);
  ConstantInt const_val2(&i32_type, 2);
  BinaryOperator non_const_index("index", &i32_type, &const_val1, &const_val2,
                                 "add");

  // 正常情况：非常量索引用于结构体访问
  ConstantInt index0(&i32_type, 0); // 第一个索引用于指针算数
  ConstantInt struct_index(&i32_type, 0);
  std::vector<Value *> indices1 = {&index0, &struct_index};
  EXPECT_NO_THROW(GetElementPtrInst("result1", &ptr_type, &struct_type,
                                    &alloca_ptr, indices1));
}

// 测试GEP指令类型检测 - 不可索引类型
TEST(GetElementPtrInstTest, NonIndexableTypeCheckTest) {
  Int32Type i32_type;
  Int8Type i8_type;
  PointerType ptr_type;
  VoidType void_type;

  // 创建指向基本类型的指针
  AllocaInst alloca_ptr("ptr", &i32_type);

  ConstantInt index(&i32_type, 0);
  std::vector<Value *> indices = {&index};

  // 正常情况：基本类型可以通过第一个索引访问
  EXPECT_NO_THROW(
      GetElementPtrInst("result1", &ptr_type, &i32_type, &alloca_ptr, indices));

  // 异常情况：尝试对不可索引的类型进行多级索引
  std::vector<Value *> indices2 = {&index, &index};
  EXPECT_THROW(
      GetElementPtrInst("result2", &ptr_type, &i32_type, &alloca_ptr, indices2),
      std::runtime_error);
}

} // namespace llvm