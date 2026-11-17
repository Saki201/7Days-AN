# Hebdomad 安卓逆向分析记录

状态：完成 N900 二进制的自动分析和伪代码导出；未完成安卓移植，没有可玩的 APK。

## 输入与校验

分析对象来自 0x4d696e68/Hebdomad 的 v1.1.0 发布包 hebdomad_1.1.0_armel.deb。
DEB SHA-256：3379ac6405d479f607bb061ef0cec4006121b725f2fa570bf715c4ff5b52ed60。
提取目标：/opt/hebdomad/hebdomad；32 位、小端 ARM ELF，ET_EXEC。
公开仓库主分支与 v1.1.0 标签只有 README 和截图，未包含引擎源码。

## 本次实际结果

使用 Ghidra 11.4.3、JDK 21，自动分析后导出 5,079 个非外部、非跳板函数，5,075 个得到伪代码。
这不是完整函数覆盖率，也不代表语义正确率。导出成功只表示反编译器返回了文本。
四个失败函数记录在 decompile-errors.tsv；其中一个出现变量合并错误，三个超时。
recovered.c 保留工具生成的警告，包含未知类型、未解析全局变量、间接跳转等，不能直接编译。
main-pseudocode.c 是从 ELF 启动入口传给 __libc_start_main 的地址 0x0001afe8 对应的函数。
6 个 shader 文件是二进制内保留的着色器源码片段，不是完整着色器管线。
resolved-string-expressions.json 为部分 PC 相对字符串表达式的静态解析，未经运行验证。

## 已定位的关键地址

- 0x0001afe8：主入口。
- 0x0002ed90：SPK 索引读取。
- 0x0002fccc：SPK 资源内容读取。
- 0x000326e8：数据目录初始化相关函数。
- 0x0004a544：基础字体读取相关函数。
- 0x0002cd34：PulseAudio／ALSA 音频初始化相关函数。
- 0x000e3cf0：X11 窗口相关函数。

这些是依据伪代码和字符串引用作出的分析命名，仍需要动态验证。

## SPK 格式与工具

根据 0x0002ed90：文件头是小端有符号 16 位条目数，要求大于零。
紧跟 count 个 68 字节条目，每个条目为 64 字节名称和小端 32 位偏移。
根据 0x0002fccc：内容长度由下一个条目的偏移减去当前偏移得到；末项结束于文件末尾。
spk_tool.py 实现上述索引解析和安全解包。真实名称编码尚未确定。

使用 Python 3：

    python spk_tool.py res.spk
    python spk_tool.py res.spk --extract extracted

已通过构造样本的索引读取、内容提取、截断、无效数量、无效偏移和目录穿越检查。
尚未用真实 res.spk 验证；提供的 Google Drive 链接及多个下载入口连接超时。
工具不包含游戏数据。

## 安卓仍缺少的工作

该程序依赖 libdl.so.2、libpthread.so.0、librt.so.1、libm.so.6、libc.so.6、ld-linux.so.3。
字符串和引用还表明它使用 X11、PulseAudio／ALSA 和 EGL／GLES。
本次没有尝试把这些 Linux 二进制当安卓原生库加载，也没有验证兼容运行方案。

原生重建需要恢复游戏对象结构、全局数据、资源格式、函数签名与调用语义，
替换窗口与输入、音频、存储路径及程序生命周期，接入安卓构建，再做场景和存档验证。
不能将 recovered.c 改名为 .cpp 并直接交给 NDK 编译。
若采用 Linux 兼容环境或模拟器运行原包，属于另一种运行方案，仍需验证图形、音频和控制。

当前缺少实际游戏数据，也没有连接安卓设备；未执行游戏启动、画面、音频或操作测试。
没有产出 APK，不应将本压缩包称为安卓移植源码或可安装游戏。

## 包内文件

- recovered.c：自动反编译伪代码。
- functions.tsv、summary.txt、decompile-errors.tsv：导出统计与失败记录。
- elf-report.json：ELF 布局、依赖和动态符号。
- strings.json、string-xrefs.tsv、resolved-string-expressions.json：字符串与引用资料。
- unwind-function-starts.json：异常展开表提取的函数起点，可能与自动函数划分不同。
- shaders/：恢复的源码字符串片段。
- ExportRecovered.java：本次 Ghidra 导出脚本。
- spk_tool.py：据上述分析编写的资源包工具。

本包不包含原始 VPK、DEB、游戏数据或 Ghidra/JDK 工具本体。
