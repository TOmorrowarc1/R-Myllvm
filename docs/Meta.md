# 工作组件类及其接口
以下三个类负责创建并管理 Type 与 Value 类，构成 LLVM IR 生成器的核心工作组件。

### 1. LLVMContext
该类型代指 LLVM 上下文，用于管理 LLVM IR 中类型与常量的创建和销毁。（本项目只使用整数常量）

依赖：
- .hpp: class Type;
- .cpp: Type.cpp
  
成员：
- `std::map<int32_t, std::unique_ptr<Type>> integer_types_` - 存储已创建的整数类对象，避免重复创建，通过位数索引。
- `std::map<std::string, std::unique_ptr<Type>> struct_types_` - 存储已创建的Type Struct对象，避免重复创建，通过名称索引。
- `std::map<std::pair<Type*, int32_t>, std::unique_ptr<Type>> array_types_` - 存储已创建的数组类对象，避免重复创建，通过元素类型和长度索引。
- `std::unique_ptr<PointerType> pointer_type_` - 指针类型单例对象。
- `std::map<std::pair<Type*, llvm::ArrayRef<Type*>>, std::unique_ptr<FunctionType>> function_types_` - 存储已创建的函数类对象，避免重复创建，通过返回类型和参数类型列表索引。（注意：需要自定义llvm::ArrayRef的比较函数，使用仿函数与字典序比较）
- `std::map<std::pair<Integertype*, int64_t>, std::unique_ptr<ConstantInt>> int_constants_` - 存储已创建的整数常量对象，避免重复创建，通过类型与整数值索引。
  
接口：
- 构造函数：空的。
- `auto getVoidTy() -> VoidType*` - 获取或创建void类型对象。
- `auto getInt32Ty() -> Int32Type*` - 获取或创建32位整数类型对象。
- `auto getInt8Ty() -> Int8Type*` - 获取或创建8位整数类型对象。
- `auto getInt1Ty() -> Int1Type*` - 获取或创建1位整数类型对象。
- `auto getStructType(const std::string& name)-> StructType*` - 获取或创建结构体类型对象。
- `auto getArrayType(Type* element_type, int32_t length)-> ArrayType*` - 获取或创建数组类型对象。
- `auto getFunctionType(Type* return_type, const vector<Type*>& param_types) -> FunctionType*` - 获取或创建函数类型对象。
- `auto getPointerType() -> PointerType*` - 获取指针类型对象。
- `auto getIntConstant(Integertype* type, int64_t value) -> ConstantInt*` - 获取或创建整数常量对象。


### 2. Module
该类型指代编译单元，作为单个文件的 LLVM IR 代码容器。其内部拥有多个 Function 与 GlobalVariable 的所有权，同时 Module 承担创建 function 与全局变量的职责。

- **成员**:
  - `std::string module_name_` - 模块名称。
  - `LLVMContext* context_` - 所属上下文指针。
  - `std::map<std:::string, std::unique_ptr<Function>> functions_` - 模块内函数列表。
  - `std::map<std::string, std::unique_ptr<GlobalVariable>> global_vars_` - 模块内全局变量列表。
  
- **接口**:
  - `Module(const std::string& name, LLVMContext* context)` - 构造函数，初始化模块名称与上下文指针。
  - `auto getFunction(const std::string& name) -> Function*` - 根据名称获取函数对象指针，若不存在则返回 nullptr。
  - `auto createFunction(const std::string& name, FunctionType* func_type) -> Function*` - 创建函数对象并添加到模块中，返回函数对象指针，若已创建则报错。
  - `void getOrCreateFunction(const std::string& name, FunctionType* func_type)` - 获取或创建函数对象。
  - `void addFunction(const std::string& name, std::unique_ptr<Function>&& function)` - 向模块中添加函数对象。
  - `auto getGlobalVariable(const std::string& name) -> GlobalVariable*` - 根据名称获取全局变量对象指针，若不存在则返回 nullptr。
  - `auto createGlobalVariable(const std::string& name, Type* var_type, bool is_constant, llvm::Constant* init_value) -> GlobalVariable*` - 创建全局变量对象并添加到模块中，返回全局变量对象指针，若已创建则报错。
  - `void getOrCreateGlobalVariable(const std::string& name, Type* var_type, bool is_constant, llvm::Constant* init_value)` - 获取或创建全局变量对象。
  - `void addGlobalVariable(const std::string& name, std::unque_ptr<GlobalVariable>&& global_var)` - 向模块中添加全局变量对象。
  - `auto getContext() -> LLVMContext*` - 获取所属上下文指针。
  - `auto print() -> std::string` - 打印模块内所有函数与全局变量的信息，实现方式如下：
    - 遍历 global_vars_，调用每个 GlobalVariable 的 print 方法，拼接结果字符串。
    - 遍历 functions_，调用每个 Function 的 print 方法，拼接结果字符串。
    - 返回最终拼接的字符串。

### 3. IRBuilder
该类型为 LLVM IR 指令构建器，使用 Builder 模式生成并在对应位置插入指令。换言之，在 Module-Function-BB-Instruction 的层级结构中，只有 Instruction的创建与插入由 IRBuilder 负责，余下都交给对应父类负责。（考虑到 BasicBlock 设计问题，IRBuilder 暂时只能将指令插入到 BasicBlock 末尾）

成员：
- `LLVMContext* context_` - 关联上下文指针。
- `BasicBlock* insert_block_` - 当前插入点基本块指针。

位置设计接口:
  - `llvm::IRBuilder(*context_)` - 构造函数，创建 IR 构建器
  - `void SetInsertPoint(BasicBlock* point)` - 设置插入点
  - `void GetInsertBlock()` - 获取当前插入块
  - `void GetInsertBlock()->getParent()` - 获取当前函数
  
指令创建接口：
  - `auto CreateAdd(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建加法指令
  - `auto CreateSub(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建减法指令
  - `auto CreateMul(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建乘法指令
  - `auto CreateSDiv(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建有符号除法指令
  - `auto CreateUDiv(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建无符号除法指令
  - `auto CreateSRem(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建有符号取模指令
  - `auto CreateURem(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建无符号取模指令
  - `auto CreateShl(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建左移指令
  - `auto CreateAShr(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建算术右移指令
  - `auto CreateLShr(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建逻辑右移指令
  - `auto CreateAnd(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建按位与指令
  - `auto CreateOr(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建按位或指令
  - `auto CreateXor(Value* LHS, Value* RHS, const std::string& name = "") -> BinaryOperator*` - 创建按位异或指令
  - `auto CreateNeg(Value* operand, const std::string& name = "") -> UnaryOperator*` - 创建取负指令
  - `auto CreateNot(Value* operand, const std::string& name = "") -> UnaryOperator*` - 创建按位取反指令
  - `auto CreateAlloca(Type* type, const std::string& name = "") -> AllocaInst*` - 创建栈上分配指令（考虑到没有动态数组，我们不需要）动态长度
  - `auto CreateLoad(Type* type, Value* ptr, const std::string& name = "") -> LoadInst*` - 创建加载指令
  - `auto CreateStore(Value* value, Value* ptr) -> StoreInst*` - 创建存储指令
  - `auto CreateRet(Value* value) -> ReturnInst*` - 创建返回指令
  - `auto CreateBr(BasicBlock* dest) -> BrInst*` - 创建无条件分支指令
  - `auto CreateCondBr(Value* cond, BasicBlock* then_bb, BasicBlock* else_bb) -> ConBrInst*` - 创建条件分支指令
  - `auto CreateICmpEQ(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建等于比较指令
  - `auto CreateICmpNE(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建不等于比较指令
  - `auto CreateICmpSLT(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建有符号小于比较指令
  - `auto CreateICmpSLE(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建有符号小于等于比较指令
  - `auto CreateICmpSGT(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建有符号大于比较指令
  - `auto CreateICmpSGE(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建有符号大于等于比较指令
  - `auto CreateICmpULT(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建无符号小于比较指令
  - `auto CreateICmpULE(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建无符号小于等于比较指令
  - `auto CreateICmpUGT(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建无符号大于比较指令
  - `auto CreateICmpUGE(Value* LHS, Value* RHS, const std::string& name = "") -> ICmpInst*` - 创建无符号大于等于比较指令
  - `auto CreatePHI(Type* type, const std::string& name = "") -> PHINode*` - 创建PHI节点指令
  - `auto CreateCall(Function* func, const std::vector<Value*>& args, const std::string& name = "") -> CallInst*` - 创建函数调用指令
  - `auto CreateGEP(Type* type, Value* ptr, const std::vector<Value*>& indices, const std::string& name = "") -> GetElementPtrInst*` - 创建 GetElementPtr 指令
  - `auto CreateMemCpy(Value* dest, Value* src, uint64_t size, bool is_violatile) -> CallInst*` - 创建内存拷贝指令。该函数需要调用LLVM内置的memcpy函数实现内存拷贝操作，其中memcpy函数的声明会在第一次调用时自动创建并添加到Module中；由于不考虑优化，该函数参数只有四个：目的、源、大小、是否易失；声明时需要编辑函数名为memcpy.p0.p0.i64以符合LLVM命名规范。

内部隐藏逻辑：
- `auto genLLVMReg()->std::string` - 生成唯一LLVM寄存器名称的辅助函数，在内部static的存储一个计数器，每次调用自增并返回类似于"%1"、"%2"的名称字符串。