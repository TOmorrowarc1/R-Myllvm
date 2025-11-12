#include "gtest/gtest.h"
#include "Type.hpp"

namespace llvm {

// 测试Void类型
TEST(TypeTest, VoidTypeTest) {
    VoidType void_type;
    
    EXPECT_FALSE(void_type.isPointerTy());
    EXPECT_FALSE(void_type.isAggregateTy());
    EXPECT_EQ(void_type.print(), "void");
    
    // 测试类型相等性
    VoidType another_void_type;
    EXPECT_TRUE(void_type.isEqual(&another_void_type));
    
    Int32Type i32_type;
    EXPECT_FALSE(void_type.isEqual(&i32_type));
}

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
    
    // 测试print()方法 - 只返回结构体名称
    std::string print_result = struct_type.print();
    EXPECT_EQ(print_result, "%struct.MyStruct");
    
    // 测试printDef()方法 - 返回完整的类型定义
    std::string print_def_result = struct_type.printDef();
    EXPECT_TRUE(print_def_result.find("%struct.MyStruct = type {") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("i32") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("i8") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("}") != std::string::npos);
    
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

// 测试含有数组类型字段的结构类型
TEST(TypeTest, StructWithArrayFieldTest) {
    // 创建一个包含数组字段的结构体
    StructType struct_with_array("StructWithArray");
    
    // 创建一个整数数组类型
    Int32Type i32_type;
    ArrayType array_type(&i32_type, 5);
    
    // 设置结构体字段：包含一个整数和一个数组
    Int8Type i8_type;
    std::vector<Type*> elements = {&i8_type, &array_type};
    struct_with_array.setBody(elements);
    
    // 验证结构体属性
    EXPECT_FALSE(struct_with_array.isPointerTy());
    EXPECT_TRUE(struct_with_array.isAggregateTy());
    EXPECT_EQ(struct_with_array.getName(), "StructWithArray");
    EXPECT_EQ(struct_with_array.getNumElements(), 2);
    EXPECT_EQ(struct_with_array.getElementType(0), &i8_type);
    EXPECT_EQ(struct_with_array.getElementType(1), &array_type);
    
    // 测试print()方法 - 只返回结构体名称
    std::string print_result = struct_with_array.print();
    EXPECT_EQ(print_result, "%struct.StructWithArray");
    
    // 测试printDef()方法 - 返回完整的类型定义
    std::string print_def_result = struct_with_array.printDef();
    EXPECT_TRUE(print_def_result.find("%struct.StructWithArray = type {") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("i8") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("[i32 x 5]") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("}") != std::string::npos);
    
    // 测试类型相等性
    StructType another_struct_with_array("StructWithArray");
    another_struct_with_array.setBody(elements);
    EXPECT_TRUE(struct_with_array.isEqual(&another_struct_with_array));
    
    // 测试不同名称的结构体
    StructType different_name_struct("DifferentName");
    different_name_struct.setBody(elements);
    EXPECT_FALSE(struct_with_array.isEqual(&different_name_struct));
    
    // 测试不同字段的结构体
    Int1Type i1_type;
    std::vector<Type*> different_elements = {&i8_type, &i1_type};
    StructType different_elements_struct("StructWithArray");
    different_elements_struct.setBody(different_elements);
    EXPECT_FALSE(struct_with_array.isEqual(&different_elements_struct));
}

// 测试含有其他复合类型字段的结构类型
TEST(TypeTest, StructWithNestedStructTest) {
    // 创建一个内部结构体
    StructType inner_struct("InnerStruct");
    Int32Type i32_type;
    Int8Type i8_type;
    std::vector<Type*> inner_elements = {&i32_type, &i8_type};
    inner_struct.setBody(inner_elements);
    
    // 创建一个包含内部结构体的外部结构体
    StructType outer_struct("OuterStruct");
    Int1Type i1_type;
    std::vector<Type*> outer_elements = {&i1_type, &inner_struct};
    outer_struct.setBody(outer_elements);
    
    // 验证外部结构体属性
    EXPECT_FALSE(outer_struct.isPointerTy());
    EXPECT_TRUE(outer_struct.isAggregateTy());
    EXPECT_EQ(outer_struct.getName(), "OuterStruct");
    EXPECT_EQ(outer_struct.getNumElements(), 2);
    EXPECT_EQ(outer_struct.getElementType(0), &i1_type);
    EXPECT_EQ(outer_struct.getElementType(1), &inner_struct);
    
    // 测试print()方法 - 只返回结构体名称
    std::string print_result = outer_struct.print();
    EXPECT_EQ(print_result, "%struct.OuterStruct");
    
    // 测试printDef()方法 - 返回完整的类型定义
    std::string print_def_result = outer_struct.printDef();
    EXPECT_TRUE(print_def_result.find("%struct.OuterStruct = type {") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("i1") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("%struct.InnerStruct") != std::string::npos);
    EXPECT_TRUE(print_def_result.find("}") != std::string::npos);
    
    // 测试类型相等性
    StructType another_outer_struct("OuterStruct");
    another_outer_struct.setBody(outer_elements);
    EXPECT_TRUE(outer_struct.isEqual(&another_outer_struct));
    
    // 测试不同的内部结构体
    StructType different_inner_struct("InnerStruct");
    std::vector<Type*> different_inner_elements = {&i32_type}; // 只有一个字段
    different_inner_struct.setBody(different_inner_elements);
    
    StructType struct_with_different_inner("OuterStruct");
    std::vector<Type*> elements_with_different_inner = {&i1_type, &different_inner_struct};
    struct_with_different_inner.setBody(elements_with_different_inner);
    EXPECT_FALSE(outer_struct.isEqual(&struct_with_different_inner));
}

// 测试元素为结构类型的数组类型
TEST(TypeTest, ArrayOfStructsTest) {
    // 创建一个结构体类型
    StructType element_struct("ElementStruct");
    Int32Type i32_type;
    Int8Type i8_type;
    std::vector<Type*> struct_elements = {&i32_type, &i8_type};
    element_struct.setBody(struct_elements);
    
    // 创建一个结构体数组
    ArrayType struct_array(&element_struct, 3);
    
    // 验证数组属性
    EXPECT_FALSE(struct_array.isPointerTy());
    EXPECT_TRUE(struct_array.isAggregateTy());
    EXPECT_EQ(struct_array.getElementType(), &element_struct);
    EXPECT_EQ(struct_array.getNumElements(), 3);
    
    // 测试打印
    std::string print_result = struct_array.print();
    EXPECT_TRUE(print_result.find("ElementStruct") != std::string::npos);
    EXPECT_TRUE(print_result.find("3") != std::string::npos);
    
    // 测试类型相等性
    ArrayType another_struct_array(&element_struct, 3);
    EXPECT_TRUE(struct_array.isEqual(&another_struct_array));
    
    // 测试不同大小的数组
    ArrayType different_size_array(&element_struct, 5);
    EXPECT_FALSE(struct_array.isEqual(&different_size_array));
    
    // 测试不同元素类型的数组
    StructType different_element_struct("DifferentElementStruct");
    std::vector<Type*> different_elements = {&i32_type}; // 只有一个字段
    different_element_struct.setBody(different_elements);
    
    ArrayType array_with_different_elements(&different_element_struct, 3);
    EXPECT_FALSE(struct_array.isEqual(&array_with_different_elements));
    
    // 测试嵌套的复合类型：结构体数组作为另一个结构体的字段
    StructType container_struct("ContainerStruct");
    Int1Type i1_type;
    std::vector<Type*> container_elements = {&i1_type, &struct_array};
    container_struct.setBody(container_elements);
    
    // 验证容器结构体属性
    EXPECT_FALSE(container_struct.isPointerTy());
    EXPECT_TRUE(container_struct.isAggregateTy());
    EXPECT_EQ(container_struct.getName(), "ContainerStruct");
    EXPECT_EQ(container_struct.getNumElements(), 2);
    EXPECT_EQ(container_struct.getElementType(0), &i1_type);
    EXPECT_EQ(container_struct.getElementType(1), &struct_array);
    
    // 测试容器结构体的print()方法 - 只返回结构体名称
    std::string container_print_result = container_struct.print();
    EXPECT_EQ(container_print_result, "%struct.ContainerStruct");
    
    // 测试容器结构体的printDef()方法 - 返回完整的类型定义
    std::string container_print_def_result = container_struct.printDef();
    EXPECT_TRUE(container_print_def_result.find("%struct.ContainerStruct = type {") != std::string::npos);
    EXPECT_TRUE(container_print_def_result.find("i1") != std::string::npos);
    EXPECT_TRUE(container_print_def_result.find("[%struct.ElementStruct x 3]") != std::string::npos);
    EXPECT_TRUE(container_print_def_result.find("}") != std::string::npos);
}

} // namespace llvm