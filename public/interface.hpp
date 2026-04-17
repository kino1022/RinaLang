#pragma once
#include <concepts>

namespace rinalang {

    class implementable;
    
    class interface_base : public implementable { };
    
    /// @brief 特定のinterface_baseを継承しているかどうかを判定できるクラス
    class implementable {
    public:
        /// @brief 継承したclassが特定のinterface_baseの派生クラスを継承しているかどうかを取得する
        /// @return 継承している場合にtrue
        template<class T>
        requires std::derived_from<T, interface_base>
        bool implement_interface () const {
            return static_cast<T*>(this) != nullptr;
        }
    };
    
}

