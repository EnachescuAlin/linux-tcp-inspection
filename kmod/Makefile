NAME		:= TcpInspection
obj-m		:= $(NAME).o
KERNEL_DIR	?= /lib/modules/$(shell uname -r)/build

$(NAME)-y	:= main.o syscall_hooks.o hooks.o

all:
	make -C $(KERNEL_DIR) M=$(shell pwd)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
