# 项目简介

该项目为R-Compiler编译器的LLVM IR生成器模块，负责生成LLVM IR代码。该模块模仿LLVM IR的设计理念，包括类型系统，CFG、DFG模块等，最终向外暴露一系列接口供编译器调用。

# MyLLVM's API

## Type 类及其接口

Type 多态类用于代指一个LLVM Value对象的类型信息，映射到汇编中的内存布局，数据表示。
具体说明见 [Type 系统](./docs/Type.md)。

## Value 类及其接口

Value 多态类代指一切可以被使用的数据，是形成 LLVM IR 中 DFG 的基础。
具体说明见 [Value 系统](./docs/Value.md)。

## 工作组件类及其接口
具体说明见 [LLVM 工作组件](./docs/Meta.md)。

### 1. LLVMContext
该类型代指 LLVM 上下文，用于管理 LLVM IR 中类型的创建和销毁（理论上常量也由Context管理，但本项目未使用此功能）。

### 2. Module
该类型指代编译单元，最大的 LLVM IR 代码容器。内部拥有多个 Function 与 GlobalVariable 的所有权。

### 3. IRBuilder
该类型为 LLVM IR 指令构建器，使用 Builder 模式生成各种对象。

// TODO: 结构体常量，数组常量以及要命的结构体长度计算。