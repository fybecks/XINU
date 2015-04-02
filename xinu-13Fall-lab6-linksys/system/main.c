#include <xinu.h>


int main(int argc, char **argv)
{
	kprintf("%s\r\n", "main with no syscall");

	return OK;
}
