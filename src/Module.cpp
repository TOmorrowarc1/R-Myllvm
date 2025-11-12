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

auto Module::getContext() -> LLVMContext * { return context_; }

auto Module::print() -> std::string {
  std::string result = "Module: " + module_name_ + "\n\n";

  // 打印全局变量
  result += "Global Variables:\n";
  for (const auto &[name, global_var] : global_vars_) {
    result += "  " + global_var->print() + "\n";
  }
  result += "\n";

  // 打印函数
  result += "Functions:\n";
  for (const auto &[name, function] : functions_) {
    result += "  " + function->print() + "\n";
  }

  return result;
}

} // namespace llvm