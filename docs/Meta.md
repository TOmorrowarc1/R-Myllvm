# 工作组件类及其接口
以下三个类专注于对 Type 与 Value 类的管理与生成，构成 LLVM IR 生成器的核心工作组件。

### 1. LLVMContext
该类型代指 LLVM 上下文，用于管理 LLVM IR 中类型的创建和销毁（理论上常量也由Context管理，但本项目未使用此功能）。

依赖：
- .hpp: class Type;
- .cpp: Type.cpp
  
成员：
- `std::map<int32_t, std::unique_ptr<Type>> integer_types_` - 存储已创建的整数类对象，避免重复创建，通过位数索引。
- `std::map<std::string, std::unique_ptr<Type>> struct_types_` - 存储已创建的Type Struct对象，避免重复创建，通过名称索引。
- `std::map<std::pair<Type*, int32_t>, std::unique_ptr<Type>> array_types_` - 存储已创建的数组类对象，避免重复创建，通过元素类型和长度索引。
- `std::unique_ptr<PointerType> pointer_type_` - 指针类型单例对象。
- `std::map<std::pair<Type*, llvm::ArrayRef<Type*>>, std::unique_ptr<FunctionType>> function_types_` - 存储已创建的函数类对象，避免重复创建，通过返回类型和参数类型列表索引。（注意：需要自定义llvm::ArrayRef的比较函数，使用仿函数与字典序比较）
  
接口：
- 构造函数：空的。
- `auto getInt32Ty() -> Type*` - 获取或创建32位整数类型对象。
- `auto getInt8Ty() -> Type*` - 获取或创建8位整数类型对象。
- `auto getInt1Ty() -> Type*` - 获取或创建1位整数类型对象。
- `auto getStructType(const std::string& name)-> StructType*` - 获取或创建结构体类型对象。
- `auto getArrayType(Type* element_type, int32_t length)-> ArrayType*` - 获取或创建数组类型对象。
- `auto getFunctionType(Type* return_type, const vector<Type*>& param_types) -> FunctionType*` - 获取或创建函数类型对象。
- `auto getPointerType() -> PointerType*` - 获取指针类型对象。


### 2. Module
该类型指代编译单元，最大的 LLVM IR 代码容器。内部拥有多个 Function 与 GlobalVariable 的所有权。

- **成员**:
  - `std::string module_name_` - 模块名称。
  - `LLVMContext* context_` - 所属上下文指针。
  - `std::map<std:::string, std::unique_ptr<Function>> functions_` - 模块内函数列表。
  - `std::map<std::string, std::unique_ptr<GlobalVariable>> global_vars_` - 模块内全局变量列表。
  
- **接口**:
  - `Module(const std::string& name, LLVMContext* context)` - 构造函数，初始化模块名称与上下文指针。
  - `auto getFunction(const std::string& name) -> Function*` - 根据名称获取函数对象指针，若不存在则返回 nullptr。
  - `void addFunction(const std::string& name, std::unique_ptr<Function>&& function)` - 向模块中添加函数对象。
  - `auto getGlobalVariable(const std::string& name) -> GlobalVariable*` - 根据名称获取全局变量对象指针，若不存在则返回 nullptr。
  - `void addGlobalVariable(const std::string& name, std::unque_ptr<GlobalVariable>&& global_var)` - 向模块中添加全局变量对象。
  - `auto getContext() -> LLVMContext*` - 获取所属上下文指针。
  - `auto print() -> std::string` - 打印模块内所有函数与全局变量的信息。

### 3. IRBuilder
该类型为 LLVM IR 指令构建器，使用 Builder 模式生成各种对象。

- **接口**:
  - `std::make_unique<llvm::IRBuilder<>>(*context_)` - 创建 IR 构建器
  - `builder_->SetInsertPoint(block)` - 设置插入点
  - `builder_->GetInsertBlock()` - 获取当前插入块
  - `builder_->GetInsertBlock()->getParent()` - 获取当前函数