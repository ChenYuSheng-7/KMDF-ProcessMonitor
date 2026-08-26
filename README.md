## 简介
基于KMDF框架开发的Windows进程监控驱动.

## 注意事项
当前驱动未进行正式代码签名，主要用于学习与测试，需在允许测试驱动加载的开发/测试环境中运行；项目目前主要在 Windows 虚拟机中完成测试.

使用管理员权限运行cmd:

注册/创建驱动:sc create ProcessMonitoring binPath= C:\你的驱动路径\ProcessMonitoring.sys type=kernel

启动驱动:sc start ProcessMonitoring

停止驱动:sc stop ProcessMonitoring

删除驱动:sc delete ProcessMonitoring

## 功能
使用 PsSetCreateProcessNotifyRoutineEx 监控进程创建/退出事件并记录PID,

使用RingBuffer,内核侧缓存128条事件,用户侧支持动态读取,单次至多读取32条事件,

当缓冲区已满时,丢弃(覆盖)最旧事件,并记录丢弃事件数量,

使用WDFWAITLOCK保证数据写入与读取时的并发安全,

支持空环读取,只读标头,跨环读取等边界情况.

## 代码架构
KMDF-ProcessMonitor

├─ Driver.c

├─ Device.c

├─ QueueCreate.c

├─ IOCTL.c

├─ ProcessNotify.c

├─ UserRead.c

├─ Common.h

├─ ProcessMonitoring.inf

└─UserProcessMonitoring

UserProcessMonitoring为用户态测试程序,运行用户态测试程序需获取管理员权限/以管理员身份运行.
