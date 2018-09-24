#!/bin/bash

DISK=c.img
M_BOOT_DIR=/mnt/boot
M_ROOT_DIR=/mnt/rootfs
KERNEL=vmlinux

function mount_disk(){
	sudo kpartx -av ${DISK}
	if [[ $(is_dir ${M_BOOT_DIR}) -ne 0 ]];then
		sudo mkdir -p ${M_BOOT_DIR}
	fi
	sudo mount /dev/mapper/loop0p1 ${M_BOOT_DIR}
	if [[ $(is_dir ${M_ROOT_DIR}) -ne 0 ]];then
		sudo mkdir -p ${M_ROOT_DIR}
	fi
       	sudo mount /dev/mapper/loop0p2 ${M_ROOT_DIR}
}
function umount_disk(){
	sudo umount ${M_ROOT_DIR} -l	
	sudo umount ${M_BOOT_DIR} -l	
}

function is_dir(){
	if [[ -d $1 ]];then
		return 0
	else
		return 1
	fi
}

function is_file(){
	if [[ -f $1 ]];then
		return 0
	else
		return 1
	fi
}
if [[ $(is_file ${KERNEL}) -eq 0 ]];then
	mount_disk
	sudo cp -rf ${KERNEL} ${M_ROOT_DIR}/boot
	umount_disk
	sudo kpartx -d ${DISK}
	bochs
else
	echo "kernel not exist"
fi
