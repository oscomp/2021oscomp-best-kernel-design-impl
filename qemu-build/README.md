# qemu-build

We recommend to build qemu in this directory.

For the first time:

```bash
../qemu/configure --target-list="riscv64-softmmu"
make -j8
```

later:

```bash
make -j8
```

We use `-j8` to make full use of our computer's CPU and to compile as fast as possible.
