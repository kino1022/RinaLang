# 宣言
```c++
component component_name {
    component_name () {
    }
    ~component_name () {
    }
}
```

- コンストラクタには引数を宣言することができますが、デストラクタには引数を宣言することができません

# ライフサイクル宣言

関数に対して以下のようにすることで関数をライフサイクル処理として宣言することができます

```c++

var pos : vector2;
var move_value : float;

@update
fn execute_move (delta_time: float) : void {
    move(ref pos.x, ref pos.y, move_value);
}

fn move (mut ref x: float, mut ref y: float, move_value: float) : void {
    x += move_value;
    y += move_value;
}
```

上記の例では毎フレームexecute_move関数が呼び出されることになります。

## @start
- インスタンスされたそのフレームで一度だけ呼び出される関数として宣言されます。
- 引数は存在してはなりません。
- 返り値はvoidでなければなりません
- 上記の二つが満たされない場合はコンパイルエラーとなります。

```c++
var health : int32;
var init_health : int32 = 100;

@start
fn initialize () : void {
    health = init_health;
}
```

## @update
- 毎フレーム呼び出される関数として宣言します。
- 引数はfloat型のdelta_timeのみを受け取ることができます。
- 返り値はvoidでなければなりません
- 上記の二つが満たされない場合はコンパイルエラーとなります。

## @destroy
- インスタンスが破棄されるフレームで一度だけ呼び出される関数として宣言されます。
- 引数は存在してはなりません。
- 返り値はvoidでなければなりません
- 上記の二つが満たされない場合はコンパイルエラーとなります。