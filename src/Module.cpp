#include "Module.hpp"
#include "Context.hpp"
#include "Value.hpp"

namespace llvm {

Module::Module(const std::string &name, LLVMContext *context)
    : module_name_(name), context_(context) {}

auto Module::getFunction(const std::string &name) -> Function * {
  auto it = functions_.find(name);
  if (it != functions_.end()) {
    return it->second.get();
  }
  return nullptr;
}

void Module::addFunction(const std::string &name,
                         std::unique_ptr<Function> &&function) {
  functions_[name] = std::move(function);
}

auto Module::getOrCreateFunction(const std::string &name,
                                 FunctionType *func_type) -> Function * {
  auto it = functions_.find(name);
  if (it != functions_.end()) {
    return it->second.get();
  }

  auto new_function = std::make_unique<Function>(name, func_type, this);
  Function *result = new_function.get();
  functions_[name] = std::move(new_function);
  return result;
}

auto Module::createFunction(const std::string &name, FunctionType *func_type) -> Function * {
  auto it = functions_.find(name);
  if (it != functions_.end()) {
    // 函数已存在，报错
    throw std::runtime_error("Function '" + name + "' already exists in module");
  }

  auto new_function = std::make_unique<Function>(name, func_type, this);
  Function *result = new_function.get();
  functions_[name] = std::move(new_function);
  return result;
}

auto Module::getGlobalVariable(const std::string &name) -> GlobalVariable * {
  auto it = global_vars_.find(name);
  if (it != global_vars_.end()) {
    return it->second.get();
  }
  return nullptr;
}

void Module::addGlobalVariable(const std::string &name,
                               std::unique_ptr<GlobalVariable> &&global_var) {
  global_vars_[name] = std::move(global_var);
}

auto Module::getOrCreateGlobalVariable(const std::string &name, Type *var_type,
                                       bool is_constant, Constant *init_value)
    -> GlobalVariable * {
  auto it = global_vars_.find(name);
  if (it != global_vars_.end()) {
    return it->second.get();
  }

  auto new_global_var =
      std::make_unique<GlobalVariable>(name, var_type, init_value, is_constant);
  GlobalVariable *result = new_global_var.get();
  global_vars_[name] = std::move(new_global_var);
  return result;
}

auto Module::createGlobalVariable(const std::string &name, Type *var_type,
                                  bool is_constant, Constant *init_value) -> GlobalVariable * {
  auto it = global_vars_.find(name);
  if (it != global_vars_.end()) {
    // 全局变量已存在，报错
    throw std::runtime_error("Global variable '" + name + "' already exists in module");
  }

  auto new_global_var =
      std::make_unique<GlobalVariable>(name, var_type, init_value, is_constant);
  GlobalVariable *result = new_global_var.get();
  global_vars_[name] = std::move(new_global_var);
  return result;
}

auto Module::getContext() -> LLVMContext * { return context_; }

auto Module::print() -> std::string {
  std::string result;

  // 打印全局变量
  for (const auto &[name, global_var] : global_vars_) {
    result += global_var->print() + "\n";
  }

  // 如果有全局变量，添加一个空行分隔
  if (!global_vars_.empty() && !functions_.empty()) {
    result += "\n";
  }

  // 打印函数
  for (const auto &[name, function] : functions_) {
    result += function->print() + "\n\n";
  }

  return result;
}

} // namespace llvm