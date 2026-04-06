# 宣言
```c++
entity entity_name {
    entity_name () {
    }
    ~entity_name () {
    }
}
```
# インスタンス化
```c++
var instance = new entity_name()[
    //初期化時にアタッチされるコンポーネントがあれば型をここに記述
    component_name,
];
```

# requireキーワード
```c++
entity entity_name {
    require component_name;
}
//複数のアタッチ要求宣言
entity entity_name2 {
    require component_name1, component_name2;
}
```

requireキーワードを用いることで、インスタンス時に指定した型のコンポーネントがアタッチされていなければコンパイルエラーになります。
