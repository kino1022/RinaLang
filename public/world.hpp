#pragma once

#include "class.hpp"
#include <concepts>
#include <vector>

namespace rinalang {

    /// @brief instanceを管理してライフタイムを実行するクラス
    class world : public class_base {

    public:

        

        template<class T, class... Args>
        requires std::derived_from<T, class_base>
        void *create_instance() {
            auto instance = std::make_unique<T>(std::forward<Args>(args)...);
            auto ptr = instance.get();
            instances_.push_back(std::move(instance));
        }

        void execute_start() const;

        void execute_update(const float delta_time) const;
    
    private:

        std::vector<std::unique_ptr<class_base>> instances_;

        float frame_rate_ = 60.0f;

    };
}