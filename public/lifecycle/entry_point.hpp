#pragma once

namespace rinalang::lifecycle {

    /// @brief スタート時に実行される処理を持つクラスに対して約束するインターフェース
    class startable {
    public:

        startable() = default;

        ~startable() = default;

        bool did_start = false;

        /// @brief スタート時に実行される処理を定義する純粋仮想関数
        virtual void start() const = 0;
    };

    /// @brief 毎フレーム実行される処理を持つクラスに対して約束するインターフェース
    class tickable {
    public:

        tickable() = default;

        ~tickable() = default;

        /// @brief 毎フレーム実行される処理を定義する純粋仮想関数
        /// @param delta_time 前回のフレームからの経過時間（秒）
        virtual void tick(const float delta_time) const = 0;

    };

} // namespace rinalang::lifecycle
