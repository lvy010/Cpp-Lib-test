#include <iostream>
#include <string>
#include <Alembic/AbcCoreHDF5/All.h> // HDF5写入支持
#include <Alembic/Abc/All.h>         // OArchive, OObject

int main() {
    try {
        std::cout << "=== Alembic OObject 测试 ===" << std::endl;
        
        // 创建Alembic归档文件
        Alembic::Abc::OArchive outArchive = Alembic::Abc::OArchive(
            Alembic::AbcCoreHDF5::WriteArchive(),
            "my_scene.abc",
            Alembic::Abc::MetaData(),
            Alembic::Abc::ErrorHandler::kThrowPolicy
        );
        
        std::cout << "成功创建归档文件: my_scene.abc" << std::endl;
        
        // 获取归档顶层对象
        Alembic::Abc::OObject topObject = outArchive.getTop();
        std::cout << "顶层对象名称: " << topObject.getName().c_str() << std::endl; // 输出: 顶层对象名称: AbcExport
        
        // 创建"my_scene_group"作为顶层对象的子级
        Alembic::Abc::OObject mySceneGroup(topObject, "my_scene_group");
        std::cout << "创建对象: " << mySceneGroup.getFullName().c_str() << std::endl; // 输出: 创建对象: /my_scene_group
        
        // 创建"Desk_Group"作为my_scene_group的子级
        Alembic::Abc::OObject deskGroup(mySceneGroup, "Desk_Group");
        std::cout << "创建对象: " << deskGroup.getFullName().c_str() << std::endl; // 输出: 创建对象: /my_scene_group/Desk_Group
        
        // 创建"Chair_Group"作为my_scene_group的子级
        Alembic::Abc::OObject chairGroup(mySceneGroup, "Chair_Group");
        std::cout << "创建对象: " << chairGroup.getFullName().c_str() << std::endl; // 输出: 创建对象: /my_scene_group/Chair_Group
        
        // 在Desk_Group下创建更多子对象
        Alembic::Abc::OObject deskSurface(deskGroup, "Desk_Surface");
        std::cout << "创建对象: " << deskSurface.getFullName().c_str() << std::endl;
        
        Alembic::Abc::OObject deskLegs(deskGroup, "Desk_Legs");
        std::cout << "创建对象: " << deskLegs.getFullName().c_str() << std::endl;
        
        // 在Chair_Group下创建子对象
        Alembic::Abc::OObject chairSeat(chairGroup, "Chair_Seat");
        std::cout << "创建对象: " << chairSeat.getFullName().c_str() << std::endl;
        
        Alembic::Abc::OObject chairBack(chairGroup, "Chair_Back");
        std::cout << "创建对象: " << chairBack.getFullName().c_str() << std::endl;
        
        // 显示完整的对象层级结构
        std::cout << "\n=== 完整的对象层级结构 ===" << std::endl;
        std::cout << "/" << std::endl;
        std::cout << "└── " << topObject.getName().c_str() << std::endl;
        std::cout << "    └── my_scene_group" << std::endl;
        std::cout << "        ├── Desk_Group" << std::endl;
        std::cout << "        │   ├── Desk_Surface" << std::endl;
        std::cout << "        │   └── Desk_Legs" << std::endl;
        std::cout << "        └── Chair_Group" << std::endl;
        std::cout << "            ├── Chair_Seat" << std::endl;
        std::cout << "            └── Chair_Back" << std::endl;
        
        std::cout << "\n测试完成！归档文件已保存为: my_scene.abc" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 