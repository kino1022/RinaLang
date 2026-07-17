#pragma once
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <language/entity/entity_id.hpp>
#include "component/component_base.h"
#include "component/component_holder.h"

namespace rinalang::language::entity {

  struct entity_component_init;

  class i_implementable {};

  class class_base : public i_implementable {};
  
  class entity_base : public class_base, public component::i_requirement {

    entity_id _id;

    std::unordered_map<std::type_index, std::unique_ptr<component::i_component_holder>> _components;

  public:

    entity_base(std::vector<entity_component_init> inits);

    bool operator==(const entity_base &other) const;

    ///@brief 特定のコンポーネントがアタッチされているかどうかを取得するメソッド
    template<class T>
    requires std::is_base_of_v<component::component_base, T>
    bool has () const {
      const std::type_index key (typeid(T));
      return _components.contains(key);
    }

    bool has (const std::type_index& key) const {
      return _components.contains(key);
    }

    ///@brief 特定のコンポーネントの参照を取得するメソッド
    template <class T>
      requires std::is_base_of_v<component::component_base, T>
    T* get_component_ref () const {
      const std::type_index key (typeid(T));
      if (_components.contains(key)) {
        return _components.at(key);
      }
      return nullptr;
    }

    void attach(const std::type_index key, std::unique_ptr<component::i_component_holder> holder){
      _components.insert_or_assign(key, std::move(holder));
    }

  };
}
