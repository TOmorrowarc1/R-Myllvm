# Value 系统概述

Value 系统以 Value 多态类为核心。Value 多态类用于表示LLVM IR中的各种值对象，包括常量、变量、指令等，同时通过内部成员建立 DFG 和 CFG。
Value类别所有对象也由 Context 统一管理和分配内存，确切而言通过LLVM IR 代码从 Module 到 Instructions 的组合结构进行管理（不过在这个模块本身设计中不考虑）

Value类继承结构如下：
```
llvm::Value (抽象基类)
  │
  ├── llvm::GlobalVariable (全局变量)
  │
  ├── llvm::Function (函数)
  │     
  ├── llvm::BasicBlock (基本块)
  │
  ├── llvm::User (中间基类)
  │     │
  │     │
  │     ├── llvm::Instruction (指令)
  │     │     │
  │     │     ├── llvm::BinaryOperator (二元运算指令)
  │     │     ├── llvm::UnaryOperator (一元运算指令)
  │     │     ├── llvm::LoadInst (加载指令)
  │     │     ├── llvm::StoreInst (存储指令)
  │     │     ├── llvm::AllocaInst (内存分配指令)
  │     │     ├── llvm::ICmpInst (整数比较指令)
  │     │     ├── llvm::TerminatorInst (终止指令)
  │     │     │     ├── llvm::BrInst (无条件跳转指令)
  │     │     │     ├── llvm::ConBrInst (条件分支跳转指令)
  │     │     │     └── llvm::ReturnInst (返回指令)
  │     │     ├── llvm::PHINode (PHI节点)
  │     │     ├── llvm::CallInst (函数调用指令)
  │     │     └── llvm::GetElementPtrInst (地址计算指令)
  │     │
  │     └── llvm::Constant (常量值)
  │           │
  │           ├── llvm::ConstantArray (数组常量)
  │           ├── llvm::ConstantStruct (结构体常量)
  │           └── llvm::ConstantInt (整数常量)
  │
  └── llvm::Argument (函数参数)
```

## Value 类型及其接口

### 0. 基类 `llvm::Value`
`llvm::Value` 是所有值类的抽象基类，定义了值系统的基本接口和行为。

**成员**：
- `std::vector<User*> users_` - 使用该值的用户列表

**接口**：
- `auto getType() const -> Type*` - 获取值的类型信息，纯虚函数等待继承。
- `auto getName() const -> std::string` - 获取值的名称，纯虚函数等待继承。
- `auto print() const -> std::string` - 打印值信息，纯虚函数等待继承。
- `void addUser(User* user)` - 添加使用该值的用户
- `const std::vector<User*>& getUsers() const` - 获取使用该值的用户列表
  
### 1. 全局变量 `llvm::GlobalVariable`
该类表示一个全局变量，包含变量名称、类型和（可能存在的）初始值等信息。

成员：
- `std::string name_` - 变量名称`
- `Type* type_` - 变量类型
- `Constant* initial_value_` - 变量初始值，可以为空表示未初始化
  
接口：
- `GlobalVariable(const std::string& name, Type* type, Constant* initial_value)` - 构造函数
- `auto getType() const -> Type*` - 获取变量类型
- `auto getName() const -> std::string` - 获取变量名称
- `auto getInitialValue() const -> Constant*` - 获取变量初始值
- `auto print() const -> std::string` - 打印全局变量信息

### 2. 函数 `llvm::Function`
该类表示一个函数，包含函数签名和函数体（基本块列表）。

成员：
- `std::string name_` - 函数名称
- `FunctionType* func_type_` - 函数类型
- `std::vector<std::unique_ptr<BasicBlock>> basic_blocks_` - 函数体内的基本块列表
- `std::vector<std::unique_ptr<Argument>> arguments_` - 函数参数列表
- `bool is_defined_` - 函数是否已定义（有函数体）
- `Module* parent_` - 所属模块

接口：
- `Function(const std::string& name, FunctionType* func_type, Module* parent)` - 构造函数
- `void addBasicBlock(std::unique_ptr<BasicBlock>&& bb)` - 向函数添加基本块
- `const std::vector<BasicBlock*>& getBasicBlocks() const` - 获取函数体内的基本块列表
- `auto getBBbyIndex(int64_t index) const -> BasicBlock*` - 根据索引获取基本块
- `void addArgument(std::unique_ptr<Argument>&& arg)` - 向函数添加参数
- `const std::vector<Argument*>& getArguments() const` - 获取函数参数列表
- `auto getArgByIndex(int64_t index) const -> Argument*` - 根据索引获取函数参数
- `auto isDefined() const -> bool` - 检查函数是否已定义
- `auto getType() const -> Type*` - 获取函数类型
- `auto getName() const -> std::string` - 获取函数名称
- `auto getParent() const -> Module*` - 获取所属模块
- `auto print() const -> std::string` - 打印函数信息

### 3. 基本块 `llvm::BasicBlock`
该类表示一个含有多条线型指令，由终止指令结束，从属于一个函数的基本块。（我们暂时不考虑前驱与后继，这些会在优化时添加）

成员：
- `std::string name_` - 基本块名称
- `Function* parent_` - 所属函数
- `std::vector<std::unique_ptr<Instruction>> instructions_` - 基本块内的指令

接口：
- `BasicBlock(const std::string& name, Function* parent)` - 构造函数
- `void addInstruction(std::unique_ptr<Instruction>&& inst)` - 向基本块添加指令
- `const std::vector<Instruction*>& getInstructions() const` - 获取基本块内的指令列表
- `auto isTerminated() const -> bool` - 检查基本块是否以终止指令结尾
- `auto getTerminator() const -> Instruction*` - 获取基本块的终止指令
- `auto getType() const -> Type*` - 获取基本块类型，实现为直接报错 —— 不会被用作值。
- `auto getName() const -> std::string` - 获取基本块名称
- `auto getParent() const -> Function*` - 获取所属函数
- `auto print() const -> std::string` - 打印基本块信息

### 中间基类 `llvm::User`
成员：
- `std::vector<Value*> operands_` - 操作数列表，用于生成 DFG

方法：
- `void addOperand(Value* operand)` - 添加操作数
- `const std::vector<Value*>& getOperands() const` - 获取操作数列表
  
### 4. 指令 `llvm::Instruction`
该类对应LLVM中的每一条指令。

#### 4.1 二元运算指令 `llvm::BinaryOperator`
成员：
- `std::string name_` - 指令对应Value（结果寄存器）的名称
- `std::string op_` - 操作码（如 "add", "sub" 等）
- `Type* type_` - 结果类型
- `Value* lhs_` - 左操作数
- `Value* rhs_` - 右操作数

接口：
- `BinaryOperator(const std::string& name, Type* type, Value* lhs, Value* rhs, const std::string& op)` - 构造函数
- `auto getLHS() const -> Value*` - 获取左操作数
- `auto getRHS() const -> Value*` - 获取右操作数
- `auto getOp() const -> std::string` - 获取操作码
- `auto getType() const -> Type*` - 获取结果类型
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

#### 4.2 一元运算指令
成员：
- `std::string name_` - 指令对应Value（结果寄存器）的名称
- `std::string op_` - 操作码（如 "neg", "not" 等）
- `Type* type_` - 结果类型
- `Value* operand_` - 操作数

接口:
- `UnaryOperator(const std::string& name, const std::string& op, Type* type, Value* operand)` - 构造函数
- `auto getOperand() const -> Value*` - 获取操作数
- `auto getOp() const -> std::string` - 获取操作码
- `auto getType() const -> Type*` - 获取结果类型
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

#### 4.3 加载指令
成员：
- `std::string name_` - 指令对应 Value（结果寄存器）的名称
- `Type* type_` - 结果类型
- `Value* ptr_` - 指向加载地址的指针

接口：
- `LoadInst(const std::string& name, Type* type, Value* ptr)` - 构造函数
- `auto getPtr() const -> Value*` - 获取指针操作数
- `auto getType() const -> Type*` - 获取结果类型
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

#### 4.4 存储指令
成员：
- `Type* type_` - 存储值的类型
- `Value* ptr_` - 指向存储地址的指针
- `Value* value_` - 要存储的值
  
接口：
- `StoreInst(Type* type, Value* ptr, Value* value)` - 构造函数
- `auto getPtr() const -> Value*` - 获取指针操作数
- `auto getValue() const -> Value*` - 获取存储值操作数
- `auto getType() const -> Type*` - 获取存储值类型
- `auto getName() const -> std::string` - 获取指令名称，实现为直接报错 —— 不会被用作值。
- `auto print() const -> std::string` - 打印指令信息

#### 4.5 内存分配指令
成员：
- `std::string name_` - 指令对应 Value（结果寄存器）的名称，对应一段栈上空间
- `Type* type_` - 分配空间的类型

接口：
- `AllocaInst(const std::string& name, Type* type)` - 构造函数
- `auto getType() const -> Type*` - 返回 PointerType（产生值为指针）
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

#### 4.6 整数比较指令
成员：
- `std::string name_` - 指令对应 Value（结果寄存器）的名称
- `std::string predicate_` - 比较谓词（如 "eq", "ne", "slt" 等）
- `Type* type_` - 结果类型（通常为 Int1Type）
- `Value* lhs_` - 左操作数
- `Value* rhs_` - 右操作数

接口：
- `ICmpInst(const std::string& name, const std::string& predicate, Type* type, Value* lhs, Value* rhs)` - 构造函数
- `auto getLHS() const -> Value*` - 获取左操作数
- `auto getRHS() const -> Value*` - 获取右操作数
- `auto getPredicate() const -> std::string` - 获取比较谓词
- `auto getType() const -> Type*` - 获取结果类型
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

#### 4.8 终止指令
中间基类，表示与控制流相关负责收束基本块的指令。

##### 4.8.1 无条件跳转指令
成员：
- `BasicBlock* target_` - 跳转目标基本块

接口：
- `BrInst(BasicBlock* target)` - 构造函数
- `auto getTarget() const -> BasicBlock*` - 获取跳转目标基本块
- `auto getType() const -> Type*` - 获取指令类型，实现为直接报错 —— 不会被用作值。
- `auto getName() const -> std::string` - 获取指令名称，实现为直接报错 —— 不会被用作值。
- `auto print() const -> std::string` - 打印指令信息

##### 4.8.2 分支跳转指令
成员：
- `Value* condition_` - 条件值
- `BasicBlock* then_block_` - 条件为真时跳转的基本块
- `BasicBlock* else_block_` - 条件为假时跳转的基本块

接口：
- `ConBrInst(Value* condition, BasicBlock* then_block, BasicBlock* else_block)` - 构造函数，要求检验条件值是否为i1，否则报错。
- `auto getCondition() const -> Value*` - 获取条件值
- `auto getThenBlock() const -> BasicBlock*` - 获取条件为真时跳转的基本块
- `auto getElseBlock() const -> BasicBlock*` - 获取条件为假时跳转的基本块
- `auto getType() const -> Type*` - 获取指令类型，实现为直接报错 —— 不会被用作值。
- `auto getName() const -> std::string` - 获取指令名称，实现为直接报错 —— 不会被用作值。
- `auto print() const -> std::string` - 打印指令信息

##### 4.8.3 返回指令
成员：
- `Value* return_value_` - 返回值，可以为空表示无返回值

接口：
- `ReturnInst(Value* return_value)` - 构造函数
- `auto getReturnValue() const -> Value*` - 获取返回值
- `auto getType() const -> Type*` - 获取指令类型，实现为直接报错 —— 不会被用作值。
- `auto getName() const -> std::string` - 获取指令名称，实现为直接报错 —— 不会被用作值。
- `auto print() const -> std::string` - 打印指令信息

#### 4.9 PHI节点
成员：
- `std::string name_` - 指令对应 Value（结果寄存器）的名称
- `std::vector<std::pair<Value*, BasicBlock*>> incomings_` - 输入值及其对应的前驱基本块列表

接口：
- `PHINode(const std::string& name, Type* type)` - 构造函数
- `void addIncoming(Value* value, BasicBlock* block)` - 添加输入值及其对应的前驱基本块
- `auto getType() const -> Type*` - 获取结果类型
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

#### 4.10 函数调用指令
成员：
- `std::string name_` - 指令对应 Value（结果寄存器）的名称
- `Function* function_` - 被调用的函数
- `std::vector<Value*> args_` - 函数调用参数列表

接口：
- `CallInst(const std::string& name, Function* function, const std::vector<Value*>& args)` - 构造函数，注意检查参数类型是否匹配。
- `auto getFunction() const -> Function*` - 获取被调用的函数
- `auto getArgs() const -> const std::vector<Value*>&` - 获取函数调用参数列表
- `auto getType() const -> Type*` - 获取结果类型，从函数类型获取
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

#### 4.11 地址计算指令
成员：
- `std::string name_` - 指令对应 Value（结果寄存器）的名称
- `Type* type_` - 结果类型（指针类型）
- `Type* base_type_` - 第一个索引指向元素类型
- `Value* ptr_` - 基础指针操作数
- `std::vector<Value*> indices_` - 索引操作数列表

接口：
- `GetElementPtrInst(const std::string& name, Type* type, Type* base_type, Value* ptr, const std::vector<Value*>& indices)` - 构造函数
- `auto getPtr() const -> Value*` - 获取基础指针操作数
- `auto getIndices() const -> const std::vector<Value*>&` - 获取索引操作数列表
- `auto getType() const -> Type*` - 获取结果类型
- `auto getName() const -> std::string` - 获取结果名称
- `auto print() const -> std::string` - 打印指令信息

   
### 5. 常量值 `llvm::Constant`
该类型用于表示编译期常量。

#### 5.1 整数类型常量 `llvm::ConstantInt`
成员：
- `Type* type_` - 整数类型，可能为32或8或1位。
- `int64_t value_` - 整数值。
  
接口：
- `ConstantInt(Type* type, int64_t value)` - 构造函数，此处存在数值溢出的问题，但是我们选择信任使用者（我自己）。
- `auto getType() const -> Type*` - 获取整数类型
- `auto getValue() const -> int64_t` - 获取整数值
- `auto getName() const -> std::string` - 获取整数常量，实现为输出整数对应字符串，print亦然。
- `auto print() const -> std::string` - 打印整数常量信息。

#### 5.2 结构体常量 `llvm::ConstantStruct`
暂时不进行实现，涉及到多层复合的问题。
#### 5.3 数组常量 `llvm::ConstantArray`
暂时不必实现，同上。

### 6. 函数参数 `llvm::Argument`
该类型用于表示函数参数，在函数中，参数是数据流的源头。

成员：
- `std::string name_` - 参数名称
- `Type* type_` - 参数类型
- `Function* parent_` - 所属函数
  
接口：
- `Argument(const std::string& name, Type* type, Function* parent)` - 构造函数
- `auto getType() const -> Type*` - 获取参数类型
- `auto getName() const -> std::string` - 获取参数名称
- `auto getParent() const -> Function*` - 获取所属函数
- `auto print() const -> std::string` - 打印参数信息