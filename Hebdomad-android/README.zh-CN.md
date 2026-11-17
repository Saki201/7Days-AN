# Hebdomad 安卓 ARMv7 实验移植

这是可独立安装、内含用户提供资源包的 Android APK。无需 Vita3K 或另一款应用。

## 实现范围

此项目通过逆向分析，为发布版 ARM Linux 引擎重写安卓平台接口。原游戏逻辑仍以 `assets/engine.arm` 中的机器码执行，不进行 CPU 模拟。新增 Java/C++ 源码可编译；`reverse-engineering/recovered.c` 是分析用伪代码，不能直接编译，也不是原作者源码。项目没有完成整个游戏引擎的源码重建。

适配代码负责加载原 ELF、重定位导入、部分 GLIBC/Bionic ABI 转换、SDL3 函数表替换、安卓 GLES 上下文和音频输出。资源包首次启动会校验 SHA-256 并复制到应用私有目录，后续启动保留存档。

## 安装和操作

安装提供的 APK，打开 Hebdomad，等待首次准备数据。资源包已经内置。初次默认选项可能是 Load Game，使用上键选 New Game，再确认。

- 方向区：W/A/S/D，原游戏移动及菜单选择。
- 确认/动作：J，选择、交互和推进对话。
- 物品/返回：K，打开物品界面或返回。
- 地图：M。
- 返回：Escape。

触屏支持多个手指同时按键；切到后台会释放所有按键。开场和对话期间部分操作由原游戏暂时禁用。若帧率很低，按住按钮稍久，避免一次短触在两帧之间结束。

## 兼容边界

APK 仅包含 `armeabi-v7a`，需要系统支持 32 位 ARM 应用。仅支持 arm64 的设备不能安装或运行此版。声明最低 Android 5.0，实际测试环境为 Android 7.0/API24 ARMv7 软件模拟器。其他 Android 版本、实体手机、后续全部关卡和通关流程未经验证。不能承诺所有设备正常运行。

测试截图、日志和逐项结果见 `validation/`。当前版本使用开发签名并开启调试，便于取得错误日志。加载器要求原地址空间可用；若系统拒绝内存执行或地址被占用，会明确报错。部分未调用的 Linux 平台导入仍保留诊断中止处理。

内置游戏是用户数据包顶层的英文 `res.spk`；没有将另一个本地化包替换进去，也没有验证中文版本。

## 从源码构建

Windows PowerShell，安装 Python 3、JDK 21、Android SDK platform android-35 / build-tools 35.0.1、NDK r27d，然后执行：

```powershell
$env:ANDROID_SDK_ROOT='你的 Android SDK 目录'
$env:ANDROID_NDK_HOME='你的 NDK r27d 目录'
$env:JAVA_HOME='你的 JDK 21 目录'
python build.py
```

输出为 `build/Hebdomad-experimental.apk`。构建脚本创建自己的开发签名私钥；交付源码不包含本次签名私钥，所以自行构建的 APK 更新安装时可能需要卸载原包（卸载会删除应用存档）。

`third_party/SDL3-3.4.20.aar`、头文件及 dynapi 表已经附带。其他构建工具未打包。`assets/engine.arm` 与 `assets/res.spk` 也已包含，便于重现本次构建。

## 来源与许可

原发布仓库：https://github.com/0x4d696e68/Hebdomad ，原 ELF 从 v1.1.0 DEB 提取；数据来自用户提供的七夜数据包。原游戏及移植发布物的权利仍归相应权利人，此交付不授予其再分发许可。

SDL3 使用 zlib 许可，见 `third_party/SDL-LICENSE.txt`。原程序静态链接了 C++ 运行库；新增适配库通过 Android NDK 的 libc++ 构建。反编译文件仅用于分析，并保留来源说明。
