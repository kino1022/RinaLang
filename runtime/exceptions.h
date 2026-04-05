//
// Created by 藤田優太 on 2026/04/06.
//

#ifndef RINALANG_EXCEPTIONS_H
#define RINALANG_EXCEPTIONS_H
#include <stdexcept>

namespace rinalang {
    
    struct RuntimeError : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
    
    /// @brief コンポーネント関連の例外の基底クラス
    struct ComponentError : RuntimeError{
        using std::runtime_error::runtime_error;
    };

    /// @brief コンポーネントが見つからない際に発生する例外
    struct ComponentNotFound : ComponentError {
        using ComponentError::ComponentError;
    };

    /// @brief コンポーネントが重複アタッチされた際に発生する例外
    struct ComponentNotImplemented : ComponentError {
        using ComponentError::ComponentError;
    };
} // rinalang

#endif //RINALANG_EXCEPTIONS_H