# 2021-07-19 ~ 2021-07-25 HoitFS测试安排-Part1

> 已经很就没有记Doc了，因为大家都有很多事情，毕竟开发阶段已经告一段落，当务之急便是做一些优化与测试工作。优化工作将安排在之后的章节讲述，本节给出HoitFS测试计划

## 测量指标

- Read
- Write
- Build / Mount (Recovery)
- Small Write
- *GC
- *Wear Leveling

## 具体细则

- Read

  |                     | HoitFS_OPT | HoitFS_ORIGIN | Spiffs |
  | ------------------- | ---------- | ------------- | ------ |
  | Seq Read（五组）    |            |               |        |
  | Random Read（五组） |            |               |        |

- Write

  |                      | HoitFS_OPT | HoitFS_ORIGIN | Spiffs |
  | -------------------- | ---------- | ------------- | ------ |
  | Seq Write（五组）    |            |               |        |
  | Random Write（五组） |            |               |        |

- Build / Mount

  |                     | HoitFS_OPT | HoitFS_ORIGIN | Spiffs |
  | ------------------- | ---------- | ------------- | ------ |
  | Clean Mount（五组） |            |               |        |
  | Dirty Mount（五组） |            |               |        |

- Small Write

  |                     | HoitFS_OPT | HoitFS_ORIGIN | Spiffs |
  | ------------------- | ---------- | ------------- | ------ |
  | Small Write（五组） |            |               |        |

  

