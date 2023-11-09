#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){

	char ch ;

	__asm__ volatile(

		"mov $0, %%rax\n" //syscall no.0
		"mov $0, %%rdi\n" // fd
		"mov %0, %%rsi\n" // buf
		"mov $1, %%rdx\n"
		"syscall\n"
		"mov %%rax, %0"
		:
		: "g"(&ch)
		: "rax", "rdi", "rsi", "rdx");

	printf("讀入的字元為'%c'\n", ch);
	return 0 ;

}
