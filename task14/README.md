# Prepare build
- Create overlay
```
overlay
├── etc
│   ├── passwd
│   └── shadow
├── home
│   └── user
├── init
└── system
    └── device_table.txt

4 directories, 4 file
```
- make menuconfig (set overlay, kernel patches folder...)
- make linux-menuconfig
- make source
- make
- make linux-rebuild (won't patch)
- make linux-dirclean linux

# Run
 ```
 qemu-system-x86_64 -kernel output/images/bzImage \
-drive file=output/images/rootfs.ext4,format=raw \
-nographic -append "root=/dev/sda console=ttyS0" -enable-kvm
```
