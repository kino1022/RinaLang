#pragma once



namespace rinalang::runtime {

    class world {

    private:

        /// <summary>
        /// @updateの付いた関数を実行する関数
        /// </summary>
        /// <param name="delta_time"></param>
        virtual void update (float delta_time) = 0;

    };
};