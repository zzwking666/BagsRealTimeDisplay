# actCry 激活码组件（业务层）

本目录从旧底层库 `RW_UL`（`D:\Projects\MaiLiDingZi\ThirdRep\RW_UL\RW_UL\`）中提炼而来，
供使用**新版底层库 RWUL** 的项目作为业务层激活码组件使用。

## 结构

- `cla/` —— 加密引擎（`rw::cla` 命名空间）。AES-256-CBC 对称加密 + SHA-256，基于 **OpenSSL**。
  生产代码仅用 OpenSSL（`openssl/evp.h`、`rand.h`、`sha.h`）；旧库中的 `cryptopp` 仅用于 `test/`，
  提炼时未带入，故**无 cryptopp 依赖**。
- `actCry/` —— 激活编解码与注册表持久化（`rw::actCry` 命名空间）。依赖 `cla` 与新 `RWUL::hoei`。

## 依赖

- `RWUL::hoei`（新版底层库的硬件指纹组件，提供主板 UUID / CPU 型号）
- `OpenSSL::Crypto`
- Windows 系统 API（`advapi32`、`kernel32`，注册表与 `__cpuid`/WMIC）

> 提炼时已将 `actCry_HWID.cpp`、`actCry_utility.cpp` 中的 `#include "hoei_HardwareInfo.hpp"`
> 改为 `#include "rwul/hoei/hoei_HardwareInfo.hpp"`，以适配新 RWUL 的头文件路径；
> 命名空间 `rw::hoei` 新旧一致，无需改动调用代码。

## CMake 目标

- `BusinessLayer::cla` —— 加密引擎静态库
- `BusinessLayer::actCry` —— 激活组件静态库（链 `cla` + `RWUL::hoei`）

## 主要 API（`actCry_ActivationInfo.hpp`）

```cpp
rw::actCry::ActivationInfo::generateActivationCode(info, key); // 生成激活码（加密）
rw::actCry::ActivationInfo::parseActivationCode(code, key, ok);// 解析激活码（解密）
rw::actCry::ActivationInfo::save(info, cfg);                   // 写入 Windows 注册表
rw::actCry::ActivationInfo::load(cfg, ok);                     // 从注册表读取
```

加密链：`ActivationInfo` → `cla::StrEncryption` → AES-256-CBC，
密钥 = SHA-256(password) 的前 32 个 hex 字符，随机 16 字节 IV 追加到密文，输出 hex 字符串。

## 移植到其他使用新 RWUL 的项目

1. 拷贝整个 `actCry/` 目录到目标项目；
2. 在父层 `CMakeLists.txt` 中 `add_subdirectory(actCry)`；
3. 确保上游已 `find_package(RWUL ... COMPONENTS hoei)` 且 vcpkg 提供 `openssl`；
4. 链接 `BusinessLayer::actCry`（会传递带入 `cla`、`RWUL::hoei`、OpenSSL）即可使用 `rw::actCry::ActivationInfo` 全套 API。
