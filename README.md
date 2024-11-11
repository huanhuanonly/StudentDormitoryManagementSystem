# The StudentDormitoryManagementSystem

_December 28, 2023 by [`Yang Huanhuan`](https://github.com/huanhuanonly) for `Yu Feixia`._


---

## 程序流程图如下：

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/flow-chart.png)

## 主页面截图，共设计了八条不同功能的指令：

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/home.png)

以下演示了它的各个指令的使用方法以及对应的运行截图：

1)	使用 `insert` 插入指令插入两条学生宿舍信息，并使用 `display` 指令打印当前数据库；

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/insert.png)

2)	使用 `remove` 指令删除一条学号为 $230102010202$ 的信息，并使用 `display` 指令打印当前数据库；

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/remove.png)

3)	使用 `change` 指令更改学号为 $230102010201$ 的学生的寝室号为 $301$，名称为 `YuFeixia2`，并使用 `display` 指令打印当前数据库；

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/change.png)

4)	使用 `insert` 指令新增三条指令，并使用 `find` 指令查找出现 $304$ 的数据项；

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/find.png)

5)	使用 `sort` 指令对学号进行排序，并使用 `display` 打印当前数据库，将 `-s` 替换成 `-r` 则是对房间号排序，`-n` 是对名称进行排序；

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/sort.png)

6)	使用 `save` 指令将数据库保存成文件并打开文件进行查看；

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/save.png)

7)	使用 `exit` 指令退出系统；

![](https://raw.githubusercontent.com/huanhuanonly/StudentDormitoryManagementSystem/refs/heads/main/exit.png)

## 设计思想
继命令行语法，使用 `指令 + 参数` 的形式与学生宿舍信息数据库进行交互，为了代码模板化，这里使用面向对象 _**OOP**_ 编程思想实现这个 C 语言程序结构，使用宏定义的技巧，引入 C++ 中的 `class` 编程，支持类构造、析构、类中方法，并编写了一个简单版的 C++ STL 库中的 `std::vector` 容器，内部使用 `void*` 类型的指针使它支持任何类型，并简单封装 C 语言中的内存管理函数后，给予 `vector` 内部调用，它内部使用动态扩容。在这个程序中，它将作为字符串类型 `std::string`，数据库也将使用它进行存储。

对于指令字符串的解析，这里使用字典树（`Trie`）进行存储对应指令的函数指针，`Trie` 的插入和查找的时间复杂度都是 $O(\left|S\right|)$，这个程序中定义了 `trie` 类和它的类函数来维护它的数据。

对于系统中的每个功能都封装成了一个函数，主体代码在 `main` 函数后，它们的函数指针在 `main` 开始时交由 `Trie` 树管理。
