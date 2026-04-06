# 変数宣言
```
var v : int32;
let l : int32 = 1;
ref r : int32 = 1;
```
## var
varは値と参照の両方を扱えるフィールドです。
参照を扱う際はハンドルの参照、shared_ptr<T>として扱います。
代入の際には値の代入、もしくは参照の付け替えとして機能します。

## let
constに相当する宣言です。
参照型はcosnt std::shared_ptr<T>として扱います。
またletで宣言した際には初期化の際の代入が必須です。代入がない場合はコンパイルエラーとして扱われます。

## ref
C++における参照を束縛する宣言です。
```ref x: T = expr;```とした際のexprは参照として扱える式でないとなりません。
値型を参照した際にはC++レイヤーでT&として扱われますが、参照型を参照した際にはshared_ptr<T>として扱われます。
また初期化の際には代入が必須となります。代入がない場合はコンパイルエラーとして扱われます。

# 可視性宣言
```
pri var private : int32;

pro var protected : int32;

pub var public : int32;

```
- 可視性の宣言は以上のキーワードで行われます。
- 可視性はすべての場合でデフォルトでpubです。pubとして宣言したい場合は明示的に書く必要がありません。


# 関数宣言
```

fn mymethods (x: int32, y: int32) : int32 {
    return 1;
}

//引数のデフォルト定義
fn mymethods (x: int32 = 10) : int 32 {
    return x;
}

fn mymethods (ref x: int32) : void {
    
}

```

# 可変関数
インスタンスのフィールドを書き換える事の出来る関数はmutキーワードを付記する必要があります。
```
mut fn move (ref x: int32, ref y: int32, move_value: int32) : void {
    x += move_value;
    y += move_value;
}
```

# 
