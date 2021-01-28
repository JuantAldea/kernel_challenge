#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYSCALL_EUDYPTULA 439

int main(void)
{
	unsigned int high_id, low_id;

	high_id = 0xc4;
	low_id = 0x9bc9fdc5;

	long ret_ok = syscall(SYSCALL_EUDYPTULA, high_id, low_id, 0);

	printf("OK for %x %x: %ld\n", high_id, low_id, ret_ok);

	/* 0xc49bc9fdc5 */

	long ret_no_ok = syscall(SYSCALL_EUDYPTULA, high_id, low_id >> 1, 0);

	printf("Error for %x %x: %ld\n", high_id, low_id >> 1, ret_no_ok);

	return 0;
}

