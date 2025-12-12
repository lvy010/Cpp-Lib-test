CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
INCLUDES = -I/usr/local/include -I/usr/include
LIBS = -lAlembicAbc -lAlembicAbcCoreHDF5 -lhdf5

# 目标文件
TARGET = test_oobject
SOURCE = test_oobject.cpp

# 默认目标
all: $(TARGET)

# 编译规则
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(LIBS)

# 清理
clean:
	rm -f $(TARGET) *.abc

# 运行测试
run: $(TARGET)
	./$(TARGET)

# 安装依赖（Ubuntu/Debian）
install-deps:
	sudo apt-get update
	sudo apt-get install -y libalembic-dev libhdf5-dev

# 安装依赖（CentOS/RHEL）
install-deps-centos:
	sudo yum install -y alembic-devel hdf5-devel

.PHONY: all clean run install-deps install-deps-centos 