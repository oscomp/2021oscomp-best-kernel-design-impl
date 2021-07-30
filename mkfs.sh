sudo umount /dev/sdb1
sudo umount /dev/sdb1
sudo mkfs.vfat -F 32 /dev/sdb1
sudo mount /dev/sdb1 fs
sudo cp codes/user/riscv64/* fs/
sudo umount fs
sudo umount /dev/sdb1