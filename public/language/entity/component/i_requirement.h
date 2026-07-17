//
// Created by 藤田優太 on 2026/07/17.
//
#pragma once

#ifndef RINALANG_I_REQUIREMENT_H
#define RINALANG_I_REQUIREMENT_H
#include <typeindex>
#include <vector>

#endif //RINALANG_I_REQUIREMENT_H

namespace rinalang::language::entity::component {

    ///@brief 存在するにあたって特定のコンポーネントのアタッチを要求できるクラス
    class i_requirement {

    public:

        ///@brief 要求するコンポーネントの型の配列を返すメソッド
        ///@return 要求するコンポーネントの型の配列
        virtual std::vector<std::type_index> get_requirement ();

    };

}
