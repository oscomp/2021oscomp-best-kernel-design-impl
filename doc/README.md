# UltraOS项目文档

该目录下含有UltraOS的设计、开发以及调试文档。


#### 文件包阅读方法

UltraOS文档将从一个宏观的角度，尽可能抛开代码的具体实现，描述UltraOS的架构设计。而其他的各大Markdown文件，将从细节的角度触发，描述具体的实现，会将开发过程、代码设计、数据结构、一些迭代过程的思考、学习的心得等等包括在内。

因此，如果想要一览UltraOS的设计，建议阅读UltraOS文档.pdf，如果只是想要借鉴或者找到具体的实现，则需要阅读Markdown文件以及对应的具体代码。代码也提供了必要的注释，以供参考。

#### 文件包结构

基本上，我们以不同的模块来进行文件的组织，在重要文件夹下我们通常会加入各部分的细致文档对该模块进行细致介绍。

- 测试.md：UltraOS为支持测试程序所作出的努力和思考。
- 调试.md + 问题.md：UltraOS所遇到的非常严重或者非常隐蔽的问题。
- 设备管理.md：UltraOS的I/O框架设计以及rCore-Tutorial-v3的学习笔记以及相关概念和资料的整合。
- 系统调用支持.md：部分值得记录的系统调用简单实现说明。
- ConsistencyModel.md：RISC-V的内存弱一致性相关的问题和UltraOS的相关处理。
- FAT32.md：UltraOS为构建FAT32文件系统所作出的努力和思考以及设计和优化方法。
- fs_kernel.md：UltraOS文件系统对内核提供的服务以及系统调用支持。
- Lazy_Alloc.md：UltraOS关于Lazy的性能表现测试。
- Mointor.md：UltraOS混合调试工具Monitor的说明。
- MultiCore.md：UltraOS为支持多核运行所作出的努力和思考，以及遇到的问题和解决方案。
- Optimization.md：UltraOS为提升性能性能所作出的努力和思考，以及遇到的问题和解决方案。
- Shell.md：UltraOS用户程序Shell支持。
- Signal.md：UltraOS信号机制。
- **UltraOS文档.pdf（重要）**：UltraOS的设计需求、理念、架构、特点、展望等全方位详细文档。
- UltraOS性能测试表.xlsx：UltraOS开发过程中的性能测试。
- UserSupportThinking.md：UltraOS对于用户态支持所作的努力和思考。
