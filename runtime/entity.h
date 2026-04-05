#pragma once
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include "exceptions.h"

namespace myrt {

struct IComponentHolder {
  virtual ~IComponentHolder() = default;
  virtual void* ptr() = 0;
  virtual const void* ptr() const = 0;
};

template <class T>
struct ComponentHolder final : IComponentHolder {
  template <class... Args>
  explicit ComponentHolder(Args&&... args) : value(std::forward<Args>(args)...) {}

  void* ptr() override { return &value; }
  const void* ptr() const override { return &value; }

  T value;
};

class EntityBase {
public:
  EntityBase() = default;
  virtual ~EntityBase() = default;

  EntityBase(const EntityBase&) = delete;
  EntityBase& operator=(const EntityBase&) = delete;
  EntityBase(EntityBase&&) = default;
  EntityBase& operator=(EntityBase&&) = default;

  template <class T, class... Args>
  T& add(Args&&... args) {
    const std::type_index key(typeid(T));
    if (_components.contains(key)) {
      throw ComponentAlreadyAttached(typeid(T).name());
    }
    auto holder = std::make_unique<ComponentHolder<T>>(std::forward<Args>(args)...);
    T& ref = holder->value;
    _components.emplace(key, std::move(holder));
    return ref;
  }

  template <class T>
  bool has() const {
    const std::type_index key(typeid(T));
    return _components.find(key) != _components.end();
  }

  template <class T>
  T& get() {
    const std::type_index key(typeid(T));aa
    auto it = _components.find(key);
    if (it == _components.end()) {
      throw ComponentNotFound(typeid(T).name());
    }
    return *static_cast<T*>(it->second->ptr());
  }

  template <class T>
  const T& get() const {
    const std::type_index key(typeid(T));
    auto it = _components.find(key);
    if (it == _components.end()) {
      throw ComponentNotFound(typeid(T).name());
    }
    return *static_cast<const T*>(it->second->ptr());
  }

  template <class T>
  void remove() {
    const std::type_index key(typeid(T));
    _components.erase(key);
  }

private:
  std::unordered_map<std::type_index, std::unique_ptr<IComponentHolder>> _components;
};

// new T() を統一するためのヘルパ（生成C++で使うと楽）
template <class T, class... Args>
std::shared_ptr<T> make(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace myrt