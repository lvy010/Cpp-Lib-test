# Alembic OObject 测试

这个测试演示了如何使用Alembic的`OObject`类创建对象层级结构。

## 功能说明

测试程序会创建一个Alembic归档文件（`my_scene.abc`），并在其中构建以下对象层级结构：

```
/
└── AbcExport
    └── my_scene_group
        ├── Desk_Group
        │   ├── Desk_Surface
        │   └── Desk_Legs
        └── Chair_Group
            ├── Chair_Seat
            └── Chair_Back
```

## 依赖要求

- C++14 或更高版本
- Alembic 库
- HDF5 库

## 安装依赖

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y libalembic-dev libhdf5-dev
```

### CentOS/RHEL
```bash
sudo yum install -y alembic-devel hdf5-devel
```

### macOS (使用Homebrew)
```bash
brew install alembic hdf5
```

## 编译和运行

### 使用Makefile
```bash
# 编译
make

# 运行测试
make run

# 清理
make clean
```

### 使用CMake
```bash
# 创建构建目录
mkdir build && cd build

# 配置
cmake ..

# 编译
make

# 运行
./bin/test_oobject
```

## 预期输出

运行程序后，您应该看到类似以下的输出：

```
=== Alembic OObject 测试 ===
成功创建归档文件: my_scene.abc
顶层对象名称: AbcExport
创建对象: /my_scene_group
创建对象: /my_scene_group/Desk_Group
创建对象: /my_scene_group/Chair_Group
创建对象: /my_scene_group/Desk_Group/Desk_Surface
创建对象: /my_scene_group/Desk_Group/Desk_Legs
创建对象: /my_scene_group/Chair_Group/Chair_Seat
创建对象: /my_scene_group/Chair_Group/Chair_Back

=== 完整的对象层级结构 ===
/
└── AbcExport
    └── my_scene_group
        ├── Desk_Group
        │   ├── Desk_Surface
        │   └── Desk_Legs
        └── Chair_Group
            ├── Chair_Seat
            └── Chair_Back

测试完成！归档文件已保存为: my_scene.abc
```

## 文件说明

- `test_oobject.cpp` - 主要的测试代码
- `CMakeLists.txt` - CMake构建配置
- `Makefile` - 传统Makefile构建配置
- `README.md` - 本说明文件

## 代码要点

1. **创建归档**: 使用`OArchive`创建Alembic文件
2. **获取顶层对象**: 通过`getTop()`获取归档的根对象
3. **创建子对象**: 使用`OObject(parent, name)`构造函数创建层级结构
4. **获取对象信息**: 使用`getName()`和`getFullName()`获取对象名称

## 故障排除

如果遇到编译错误，请检查：

1. Alembic库是否正确安装
2. 头文件路径是否正确
3. 库文件链接是否正确

如果遇到运行时错误，请检查：

1. 是否有写入权限
2. HDF5库是否正确安装
3. 磁盘空间是否充足 