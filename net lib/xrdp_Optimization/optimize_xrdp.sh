#!/bin/bash

echo "=== XRDP 性能优化脚本 ==="

# 1. 优化系统参数
echo "1. 优化系统参数..."
echo 'net.core.rmem_max = 16777216' >> /etc/sysctl.conf
echo 'net.core.wmem_max = 16777216' >> /etc/sysctl.conf
echo 'net.ipv4.tcp_rmem = 4096 87380 16777216' >> /etc/sysctl.conf
echo 'net.ipv4.tcp_wmem = 4096 65536 16777216' >> /etc/sysctl.conf
echo 'net.ipv4.tcp_congestion_control = bbr' >> /etc/sysctl.conf

# 2. 应用系统参数
sysctl -p

# 3. 重启xrdp服务
echo "2. 重启XRDP服务..."
systemctl restart xrdp

# 4. 检查服务状态
echo "3. 检查XRDP服务状态..."
systemctl status xrdp --no-pager -l

echo "=== 优化完成 ==="
echo "建议："
echo "- 重启客户端连接"
echo "- 如果仍有卡顿，检查网络延迟"
echo "- 考虑降低客户端分辨率"
