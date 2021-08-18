## _Memory Space Saved by Lazy Allocation_

### 对于lazy策略在内存空间上的优化效果的测试

### Test on the number of free pages

#### 	_使用 lazy策略和不使用 lazy策略下的比较_



|                                                              |  without lazy alloc   |       |                   | with lazy alloc            |       |                   | Compare     |
| ------------------------------------------------------------ | :-------------------: | ----- | ----------------- | -------------------------- | ----- | ----------------- | ----------- |
| __test on program__                                          | before runing program | after | __pages consume__ | before runing programafter | after | __pages consume__ | __improve__ |
| lmbench_all lat_syscall -P 1 null  lmbench_all lat_select -n 100 -P 1 file  lmbench_all lat_sig -P 1 install  lmbench_all lat_sig -P 1 catch |         1107          | 662   | 445               | 1232                       | 912   | 320               | 28.02%      |
| lmbench_all lat_proc -P 1 fork  lmbench_all lat_proc -P 1 exec  lmbench_all lat_proc -P 1 shell |         1107          | 566   | 541               | 1232                       | 880   | 352               | 34.94%      |
| busybox_all                                                  |         1103          | 368   | 735               | 1279                       | 624   | 655               | 10.88%      |