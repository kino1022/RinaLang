# 宣言

```c++
interface interface_name {
    //約束する関数の宣言
    fn method_name (arg1: type1, arg2: type2) : return_type;
    
    //約束する関数のデフォルト実装
    fn method_name2 (arg1: type1) : return_type {
    }
    
    //フィールドのデフォルト実装
    var field_name : type { get; mut set; }
}
```

# 説明
interfaceはclassやentity,componentなどに対して実装すると以下の記述を強制します
  - 指定した名前と引数、返り値を持った関数の記述
  - 指定した型と名前を持ったフィールドの記述

これらが満たされていない場合はコンパイルエラーとなります

実装する際には以下のようにimplementsキーワードを利用して実装します

```c++
class class_name implements interface_name {}
```

また以下のように宣言することで複数のinterfaceを実装することも可能です

```c++
class class_name implements interface_name1, interface_name2 {}
```