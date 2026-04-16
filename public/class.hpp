#pragma once
#include <vector>
#include <memory>
#include <functional>

#include "interface.hpp"

namespace rinalang {
    
    class implementable;
    
    /// @brief Rinalangのclass型の既定クラス
    class class_base : public implementable {
    public:
    
        class_base () = default;
        virtual ~class_base() = default;

        /// @brief スタート時に実行されるメソッドのポインタを格納するvector
        std::vector<void *> start_events;

        /// @brief アップデート時に実行されるメソッドのポインタを格納するvector
        std::vector<void *> update_events;

        virtual std::unique_ptr<class_base> clone () = 0;
    
        virtual bool operator== (const class_base& other) const;
        virtual bool operator!= (const class_base& other) const;

        virtual void on_start () const;

        virtual void on_update (const float delta_time) const;
        
    };
}
