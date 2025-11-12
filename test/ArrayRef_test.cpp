
#include "Utils/ArrayRef.hpp"
#include "gtest/gtest.h"

namespace llvm {

// 测试ArrayRef的基本功能
TEST(ArrayRefTest, BasicFunctionalityTest) {
  // 测试默认构造函数
  ArrayRef<int> empty_ref;
  EXPECT_TRUE(empty_ref.empty());
  EXPECT_EQ(empty_ref.size(), 0);

  // 测试使用指针和长度构造
  int data[] = {1, 2, 3, 4, 5};
  ArrayRef<int> ref(data, 5);
  EXPECT_FALSE(ref.empty());
  EXPECT_EQ(ref.size(), 5);
  EXPECT_EQ(ref.data(), data);

  // 测试使用vector构造
  std::vector<int> vec = {10, 20, 30};
  ArrayRef<int> vec_ref(vec);
  EXPECT_FALSE(vec_ref.empty());
  EXPECT_EQ(vec_ref.size(), 3);
  EXPECT_EQ(vec_ref[0], 10);
  EXPECT_EQ(vec_ref[1], 20);
  EXPECT_EQ(vec_ref[2], 30);

  // 测试使用initializer_list构造
  std::vector<int> init_list = {100, 200, 300, 400};
  ArrayRef<int> init_list_ref = init_list;
  EXPECT_FALSE(init_list_ref.empty());
  EXPECT_EQ(init_list_ref.size(), 4);
  EXPECT_EQ(init_list_ref[0], 100);
  EXPECT_EQ(init_list_ref[1], 200);
  EXPECT_EQ(init_list_ref[2], 300);
  EXPECT_EQ(init_list_ref[3], 400);
}

// 测试ArrayRef的访问器
TEST(ArrayRefTest, AccessorTest) {
  int data[] = {1, 2, 3, 4, 5};
  ArrayRef<int> ref(data, 5);

  // 测试front和back
  EXPECT_EQ(ref.front(), 1);
  EXPECT_EQ(ref.back(), 5);

  // 测试operator[]
  EXPECT_EQ(ref[0], 1);
  EXPECT_EQ(ref[1], 2);
  EXPECT_EQ(ref[2], 3);
  EXPECT_EQ(ref[3], 4);
  EXPECT_EQ(ref[4], 5);

  // 测试data
  EXPECT_EQ(ref.data(), data);
}

// 测试ArrayRef的迭代器
TEST(ArrayRefTest, IteratorTest) {
  int data[] = {1, 2, 3, 4, 5};
  ArrayRef<int> ref(data, 5);

  // 测试begin和end
  auto it = ref.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 4);
  ++it;
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(it, ref.end());

  // 测试使用范围for循环
  int i = 1;
  for (int value : ref) {
    EXPECT_EQ(value, i);
    ++i;
  }
}

// 测试ArrayRefComparator
TEST(ArrayRefTest, ComparatorTest) {
  ArrayRefComparator<int> comparator;

  // 测试不同大小的ArrayRef
  int data1[] = {1, 2, 3};
  int data2[] = {1, 2, 3, 4};
  ArrayRef<int> ref1(data1, 3);
  ArrayRef<int> ref2(data2, 4);
  EXPECT_TRUE(comparator(ref1, ref2));
  EXPECT_FALSE(comparator(ref2, ref1));

  // 测试相同大小但内容不同的ArrayRef
  int data3[] = {1, 2, 4};
  ArrayRef<int> ref3(data3, 3);
  EXPECT_TRUE(comparator(ref1, ref3));
  EXPECT_FALSE(comparator(ref3, ref1));

  // 测试相同的ArrayRef
  EXPECT_FALSE(comparator(ref1, ref1));

  // 测试相同内容的ArrayRef
  int data4[] = {1, 2, 3};
  ArrayRef<int> ref4(data4, 3);
  EXPECT_FALSE(comparator(ref1, ref4));
  EXPECT_FALSE(comparator(ref4, ref1));
}

// 测试ArrayRef的复杂场景
TEST(ArrayRefTest, ComplexScenarioTest) {
  // 创建多个ArrayRef
  int data1[] = {1, 2, 3};
  int data2[] = {4, 5, 6, 7};
  int data3[] = {8, 9};

  ArrayRef<int> ref1(data1, 3);
  ArrayRef<int> ref2(data2, 4);
  ArrayRef<int> ref3(data3, 2);

  // 测试比较
  ArrayRefComparator<int> comparator;
  EXPECT_TRUE(comparator(ref3, ref1));
  EXPECT_TRUE(comparator(ref1, ref2));
  EXPECT_TRUE(comparator(ref3, ref2));

  // 测试使用vector创建ArrayRef
  std::vector<int> vec = {10, 20, 30, 40, 50};
  ArrayRef<int> vec_ref(vec);
  EXPECT_EQ(vec_ref.size(), 5);
  EXPECT_EQ(vec_ref[0], 10);
  EXPECT_EQ(vec_ref[4], 50);

  // 测试使用initializer_list创建ArrayRef
  std::vector<int> init_list = {100, 200, 300};
  ArrayRef<int> init_ref = init_list;
  EXPECT_EQ(init_ref.size(), 3);
  EXPECT_EQ(init_ref[0], 100);
  EXPECT_EQ(init_ref[2], 300);

  // 测试迭代器
  int sum = 0;
  for (int value : ref1) {
    sum += value;
  }
  EXPECT_EQ(sum, 6);

  sum = 0;
  for (int value : ref2) {
    sum += value;
  }
  EXPECT_EQ(sum, 22);

  sum = 0;
  for (int value : ref3) {
    sum += value;
  }
  EXPECT_EQ(sum, 17);
}

} // namespace llvm