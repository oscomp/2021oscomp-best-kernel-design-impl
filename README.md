# 软硬协同的用户态中断

[题目地址](https://github.com/oscomp/proj6-RV64N-on-NutShell)

## 目标

本项目在现有 [RISC-V 用户态中断扩展规范草案](https://five-embeddev.com/riscv-isa-manual/latest/n.html) 的基础上将其进一步完善，提出一种符合该规范的模拟器和 FPGA 实现，并基于用户态中断在内核中实现优化的信号机制，在用户态实现一个硬件驱动，展示其设计和性能优势。

## 文档

本项目文档的 Markdown 版本位于 docs/src 文件夹中，推荐优先选择下列版本，阅读效果更佳。

- [mdBook on GitHub Pages](https://gallium70.github.io/rv-n-ext-impl/)
- [PDF](docs/Risc-V%20Extension%20N%20Implementation.pdf)

## 代码仓库

本项目由多个代码仓库组成，均托管在 GitHub 上，并以 Git 子模块的形式添加在本仓库中。使用本项目时建议优先选择 GitHub 版本，我们也在比赛使用的 GitLab 平台上进行了镜像。各仓库地址如下，供参考：

| 仓库                           | GitHub 仓库地址                                                   | GitLab 仓库地址                                                             |
| ------------------------------ | ----------------------------------------------------------------- | --------------------------------------------------------------------------- |
| 本项目主仓库                   | https://github.com/Gallium70/rv-n-ext-impl                        | https://gitlab.eduxiji.net/carbon/project325618-89175                       |
| 带有 N 扩展的 QEMU             | https://github.com/duskmoon314/qemu                               | https://gitlab.eduxiji.net/duskmoon314/qemu                                 |
| Labeled RISC-V-N               | https://github.com/Gallium70/labeled-RISC-V-N                     | https://gitlab.eduxiji.net/carbon/labeled-risc-v-n                          |
| Labeled RISC-V Rust Bootloader | https://github.com/Gallium70/lrv-rust-bl                          | https://gitlab.eduxiji.net/carbon/lrv-rust-bl                               |
| rustsbi-qemu 多核扩展          | https://github.com/duskmoon314/rustsbi-qemu/tree/legacy-multicore | https://gitlab.eduxiji.net/duskmoon314/rustsbi-qemu/-/tree/legacy-multicore |
| rv-csr-test                    | https://github.com/duskmoon314/rv-csr-test                        | https://gitlab.eduxiji.net/duskmoon314/rv-csr-test                          |
| rCore-N                        | https://github.com/duskmoon314/rCore-N                            | https://gitlab.eduxiji.net/duskmoon314/rcore-n                              |
