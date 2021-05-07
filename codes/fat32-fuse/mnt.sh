sudo umount sd_mnt
sudo mkfs.vfat -F 32 /dev/sdb1
sudo chmod 777 /dev/sdb1
sudo mount /dev/sdb1 sd_mnt
sudo chmod 777 sd_mnt