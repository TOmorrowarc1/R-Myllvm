#ifndef LLVM_MODULE_HPP
#define LLVM_MODULE_HPP

#include <map>
#include <memory>
#include <string>

namespace llvm {

// 前向声明
class LLVMContext;
class Function;
class FunctionType;
class GlobalVariable;
class Constant;

// Module类，指代编译单元，最大的LLVM IR代码容器
class Module {
private:
  std::string module_name_; // 模块名称
  LLVMContext *context_;    // 所属上下文指针
  std::map<std::string, std::unique_ptr<Function>> functions_; // 模块内函数列表
  std::map<std::string, std::unique_ptr<GlobalVariable>>
      global_vars_; // 模块内全局变量列表

public:
  // 构造函数，初始化模块名称与上下文指针
  Module(const std::string &name, LLVMContext *context);

  // 根据名称获取函数对象指针，若不存在则返回nullptr
  auto getFunction(const std::string &name) -> Function *;

  // 向模块中添加函数对象
  void addFunction(const std::string &name,
                   std::unique_ptr<Function> &&function);

  // 获取或创建函数对象
  auto getOrCreateFunction(const std::string &name, FunctionType *func_type) -> Function *;

  // 根据名称获取全局变量对象指针，若不存在则返回nullptr
  auto getGlobalVariable(const std::string &name) -> GlobalVariable *;

  // 向模块中添加全局变量对象
  void addGlobalVariable(const std::string &name,
                         std::unique_ptr<GlobalVariable> &&global_var);

  // 获取或创建全局变量对象
  auto getOrCreateGlobalVariable(const std::string &name, Type *var_type, bool is_constant, Constant *init_value) -> GlobalVariable *;

  // 获取所属上下文指针
  auto getContext() -> LLVMContext *;

  // 打印模块内所有函数与全局变量的信息
  auto print() -> std::string;
};

} // namespace llvm

#endif // LLVM_MODULE_HPP