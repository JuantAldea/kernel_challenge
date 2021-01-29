#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>


#define FAT_IOCTL_SET_VOLUME_LABEL _IOW('r', 0x14, char *)

int set_label(char *mnt_point, char *arg)
{
	int fd = open(mnt_point, 0);

	if (fd < 0) {
		printf("Open %s mount point failed.\n", mnt_point);
		exit(-1);
	}

	return (ioctl(fd, FAT_IOCTL_SET_VOLUME_LABEL, arg));
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s <mount point> <volume label>\n", argv[0]);
		return -1;
	}

	if (set_label(argv[1], argv[2])) {
		printf("Failed.\n");
		return -1;
	}

	printf("Label changed.\n");
	return 0;
}
