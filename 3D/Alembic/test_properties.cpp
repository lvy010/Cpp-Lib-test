#include <iostream>
#include <string>
#include <vector>
#include <Alembic/AbcCoreHDF5/All.h> // HDF5写入支持
#include <Alembic/Abc/All.h>         // OArchive、OObject和属性
#include <Alembic/AbcCoreFactory/All.h> // IFactory

int main() {
    try {
        std::cout << "=== Alembic 属性测试 ===" << std::endl;
        
        // ==================== 写入属性 ====================
        std::cout << "\n--- 写入属性 ---" << std::endl;
        
        // 创建Alembic归档文件
        Alembic::Abc::OArchive outArchive = Alembic::Abc::OArchive(
            Alembic::AbcCoreHDF5::WriteArchive(),
            "my_scene_with_props.abc",
            Alembic::Abc::MetaData(),
            Alembic::Abc::ErrorHandler::kThrowPolicy
        );
        
        std::cout << "成功创建归档文件: my_scene_with_props.abc" << std::endl;
        
        Alembic::Abc::OObject topObject = outArchive.getTop();
        
        // 从第二章重建"Desk_Group"
        Alembic::Abc::OObject deskGroup(topObject, "Desk_Group");
        
        // 获取deskGroup的顶层复合属性
        Alembic::Abc::OCompoundProperty deskProps = deskGroup.getProperties();
        
        std::cout << "准备向/Desk_Group添加属性" << std::endl;
        
        // ==================== 写入标量属性 ====================
        std::cout << "\n1. 写入标量属性:" << std::endl;
        
        // 创建名为"desk_height"的双精度标量属性
        Alembic::Abc::ODoubleProperty deskHeightProp(
            deskProps,      // 父属性容器
            "desk_height"   // 属性名称
        );
        
        // 设置高度值
        double height_value = 0.75; // 0.75米
        deskHeightProp.set(height_value);
        std::cout << "  设置desk_height为: " << height_value << std::endl;
        
        // 添加桌子宽度属性
        Alembic::Abc::ODoubleProperty deskWidthProp(deskProps, "desk_width");
        double width_value = 1.2; // 1.2米
        deskWidthProp.set(width_value);
        std::cout << "  设置desk_width为: " << width_value << std::endl;
        
        // 添加桌子深度属性
        Alembic::Abc::ODoubleProperty deskDepthProp(deskProps, "desk_depth");
        double depth_value = 0.6; // 0.6米
        deskDepthProp.set(depth_value);
        std::cout << "  设置desk_depth为: " << depth_value << std::endl;
        
        // 添加桌子材质名称
        Alembic::Abc::OStringProperty deskMaterialProp(deskProps, "material_name");
        std::string material_name = "Oak Wood";
        deskMaterialProp.set(material_name);
        std::cout << "  设置material_name为: " << material_name << std::endl;
        
        // 添加是否可移动标志
        Alembic::Abc::OBoolProperty deskMovableProp(deskProps, "is_movable");
        bool is_movable = true;
        deskMovableProp.set(is_movable);
        std::cout << "  设置is_movable为: " << (is_movable ? "true" : "false") << std::endl;
        
        // ==================== 写入数组属性 ====================
        std::cout << "\n2. 写入数组属性:" << std::endl;
        
        // 创建存储3D浮点向量(V3f)的数组属性
        Alembic::Abc::OV3fArrayProperty deskVerticesProp(
            deskProps,          // 父属性容器
            "desk_vertices"     // 属性名称
        );
        
        // 准备示例数据：桌子的四个顶点
        std::vector<Alembic::Abc::V3f> points = {
            {0.0f, 0.0f, 0.0f},     // 左下角
            {1.2f, 0.0f, 0.0f},     // 右下角
            {1.2f, 0.0f, 0.6f},     // 右上角
            {0.0f, 0.0f, 0.6f}      // 左上角
        };
        
        // 创建数组样本
        Alembic::Abc::V3fArraySample pointsSample(points.data(), points.size());
        
        // 将样本写入属性
        deskVerticesProp.set(pointsSample);
        std::cout << "  写入" << points.size() << "个顶点位置" << std::endl;
        
        // 添加UV坐标数组
        Alembic::Abc::OV2fArrayProperty deskUVsProp(deskProps, "desk_uvs");
        std::vector<Alembic::Abc::V2f> uvs = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
        };
        Alembic::Abc::V2fArraySample uvsSample(uvs.data(), uvs.size());
        deskUVsProp.set(uvsSample);
        std::cout << "  写入" << uvs.size() << "个UV坐标" << std::endl;
        
        // ==================== 写入复合属性 ====================
        std::cout << "\n3. 写入复合属性:" << std::endl;
        
        // 在deskProps中创建"material"复合属性
        Alembic::Abc::OCompoundProperty materialProps(deskProps, "material");
        
        // 在material中添加颜色标量属性
        Alembic::Abc::OV3fProperty materialColor(
            materialProps, // 父属性现在是materialProps
            "color"        
        );
        Alembic::Abc::V3f color_value = {0.8f, 0.6f, 0.4f}; // RGB颜色
        materialColor.set(color_value);
        std::cout << "  设置材质颜色为(" << color_value.x << ", " << color_value.y << ", " << color_value.z << ")" << std::endl;
        
        // 添加粗糙度标量属性
        Alembic::Abc::OFloatProperty materialRoughness(
            materialProps, 
            "roughness"    
        );
        float roughness_value = 0.5f;
        materialRoughness.set(roughness_value);
        std::cout << "  设置材质粗糙度为: " << roughness_value << std::endl;
        
        // 添加金属度属性
        Alembic::Abc::OFloatProperty materialMetallic(
            materialProps,
            "metallic"
        );
        float metallic_value = 0.0f;
        materialMetallic.set(metallic_value);
        std::cout << "  设置材质金属度为: " << metallic_value << std::endl;
        
        // 添加透明度属性
        Alembic::Abc::OFloatProperty materialOpacity(
            materialProps,
            "opacity"
        );
        float opacity_value = 1.0f;
        materialOpacity.set(opacity_value);
        std::cout << "  设置材质透明度为: " << opacity_value << std::endl;
        
        // ==================== 读取属性 ====================
        std::cout << "\n--- 读取属性 ---" << std::endl;
        
        // 设置读取环境
        Alembic::AbcCoreFactory::IFactory factory;
        Alembic::AbcCoreFactory::IFactory::CoreType coreType;
        
        Alembic::Abc::IArchive inArchive = factory.getArchive("my_scene_with_props.abc", coreType);
        
        // 验证并获取顶层对象
        Alembic::Abc::IObject deskGroupRead = inArchive.getTop().getChild("Desk_Group");
        Alembic::Abc::ICompoundProperty deskPropsRead = deskGroupRead.getProperties();
        
        std::cout << "成功读取归档文件: my_scene_with_props.abc" << std::endl;
        
        // ==================== 读取标量属性 ====================
        std::cout << "\n1. 读取标量属性:" << std::endl;
        
        Alembic::Abc::IDoubleProperty deskHeightPropRead(deskPropsRead, "desk_height");
        double height_value_read;
        deskHeightPropRead.get(&height_value_read);
        std::cout << "  读取desk_height: " << height_value_read << std::endl;
        
        Alembic::Abc::IDoubleProperty deskWidthPropRead(deskPropsRead, "desk_width");
        double width_value_read;
        deskWidthPropRead.get(&width_value_read);
        std::cout << "  读取desk_width: " << width_value_read << std::endl;
        
        Alembic::Abc::IDoubleProperty deskDepthPropRead(deskPropsRead, "desk_depth");
        double depth_value_read;
        deskDepthPropRead.get(&depth_value_read);
        std::cout << "  读取desk_depth: " << depth_value_read << std::endl;
        
        Alembic::Abc::IStringProperty deskMaterialPropRead(deskPropsRead, "material_name");
        std::string material_name_read;
        deskMaterialPropRead.get(&material_name_read);
        std::cout << "  读取material_name: " << material_name_read << std::endl;
        
        Alembic::Abc::IBoolProperty deskMovablePropRead(deskPropsRead, "is_movable");
        bool is_movable_read;
        deskMovablePropRead.get(&is_movable_read);
        std::cout << "  读取is_movable: " << (is_movable_read ? "true" : "false") << std::endl;
        
        // ==================== 读取数组属性 ====================
        std::cout << "\n2. 读取数组属性:" << std::endl;
        
        Alembic::Abc::IV3fArrayProperty deskVerticesPropRead(deskPropsRead, "desk_vertices");
        Alembic::Abc::V3fArraySample verticesSample;
        deskVerticesPropRead.get(verticesSample);
        std::cout << "  读取" << verticesSample.size() << "个顶点位置:" << std::endl;
        for (size_t i = 0; i < verticesSample.size(); ++i) {
            const Alembic::Abc::V3f& vertex = verticesSample[i];
            std::cout << "    顶点" << i << ": (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << std::endl;
        }
        
        Alembic::Abc::IV2fArrayProperty deskUVsPropRead(deskPropsRead, "desk_uvs");
        Alembic::Abc::V2fArraySample uvsSample;
        deskUVsPropRead.get(uvsSample);
        std::cout << "  读取" << uvsSample.size() << "个UV坐标:" << std::endl;
        for (size_t i = 0; i < uvsSample.size(); ++i) {
            const Alembic::Abc::V2f& uv = uvsSample[i];
            std::cout << "    UV" << i << ": (" << uv.x << ", " << uv.y << ")" << std::endl;
        }
        
        // ==================== 读取复合属性 ====================
        std::cout << "\n3. 读取复合属性:" << std::endl;
        
        Alembic::Abc::ICompoundProperty materialPropsRead(deskPropsRead, "material");
        
        Alembic::Abc::IV3fProperty materialColorRead(materialPropsRead, "color");
        Alembic::Abc::V3f color_value_read;
        materialColorRead.get(&color_value_read);
        std::cout << "  读取材质颜色: (" << color_value_read.x << ", " << color_value_read.y << ", " << color_value_read.z << ")" << std::endl;
        
        Alembic::Abc::IFloatProperty materialRoughnessRead(materialPropsRead, "roughness");
        float roughness_value_read;
        materialRoughnessRead.get(&roughness_value_read);
        std::cout << "  读取材质粗糙度: " << roughness_value_read << std::endl;
        
        Alembic::Abc::IFloatProperty materialMetallicRead(materialPropsRead, "metallic");
        float metallic_value_read;
        materialMetallicRead.get(&metallic_value_read);
        std::cout << "  读取材质金属度: " << metallic_value_read << std::endl;
        
        Alembic::Abc::IFloatProperty materialOpacityRead(materialPropsRead, "opacity");
        float opacity_value_read;
        materialOpacityRead.get(&opacity_value_read);
        std::cout << "  读取材质透明度: " << opacity_value_read << std::endl;
        
        // ==================== 总结 ====================
        std::cout << "\n=== 测试总结 ===" << std::endl;
        std::cout << "✓ 成功写入和读取标量属性 (double, string, bool)" << std::endl;
        std::cout << "✓ 成功写入和读取数组属性 (V3f, V2f)" << std::endl;
        std::cout << "✓ 成功写入和读取复合属性 (material)" << std::endl;
        std::cout << "✓ 归档文件已保存为: my_scene_with_props.abc" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 