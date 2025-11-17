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

auto Module::createFunction(const std::string &name, FunctionType *func_type)
    -> Function * {
  auto it = functions_.find(name);
  if (it != functions_.end()) {
    // 函数已存在，报错
    throw std::runtime_error("Function '" + name +
                             "' already exists in module");
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
                                  bool is_constant, Constant *init_value)
    -> GlobalVariable * {
  auto it = global_vars_.find(name);
  if (it != global_vars_.end()) {
    // 全局变量已存在，报错
    throw std::runtime_error("Global variable '" + name +
                             "' already exists in module");
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

  // 调用 context_ 的 getAllStructTypes 方法，获取所有结构体类型列表
  auto struct_types = context_->getAllStructTypes();
  // 遍历调用每个 StructType 的 printDef 方法，拼接结果字符串
  for (auto *struct_type : struct_types) {
    result += struct_type->printDef() + "\n";
  }

  // 如果有结构体类型，添加一个空行分隔
  if (!struct_types.empty() && (!global_vars_.empty() || !functions_.empty())) {
    result += "\n";
  }

  // 遍历 global_vars_，调用每个 GlobalVariable 的 print 方法，拼接结果字符串
  for (const auto &[name, global_var] : global_vars_) {
    result += global_var->print() + "\n";
  }

  // 如果有全局变量，添加一个空行分隔
  if (!global_vars_.empty() && !functions_.empty()) {
    result += "\n";
  }

  // 遍历 functions_，调用每个 Function 的 print 方法，拼接结果字符串
  for (const auto &[name, function] : functions_) {
    result += function->print() + "\n";
  }

  return result;
}

} // namespace llvm