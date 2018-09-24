/*
 * =====================================================================================
 *
 *       Filename:  copy.c
 *
 *    Description:  将硬件参数放入变量
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include "include/multiboot2.h"
#include "../include/sys/video.h"
#include "../include/stdio.h"
#include "include/boot.h"

extern boot_params_t  boot_params ;
extern video_t video;


void copy_boot_params (unsigned long magic, unsigned long addr){
	
	struct multiboot_tag *tag;
	printf("boot_params addr: %x\n", addr);
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
	{	
		printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
		__asm__("hlt");
	}
	if (addr & 7)
	{
		printf ("Unaligned mbi: 0x%x\n", addr);
		__asm("hlt");
	}
	int i;

	//unsigned int size = *(unsigned *) addr;
	for (tag = (struct multiboot_tag *) (addr + 8);tag->type != MULTIBOOT_TAG_TYPE_END;
			tag = (struct multiboot_tag *)((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))){
		char *s_str, *d_str;
		switch (tag->type)
		{
			case MULTIBOOT_TAG_TYPE_CMDLINE:
					s_str = (char *)(((struct multiboot_tag_string *) tag)->string);
					d_str = boot_params.cmd_line;
					while( (*d_str++ = *s_str++) != '\0' );
					break;
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
					s_str = (char *)(((struct multiboot_tag_string *) tag)->string);
					d_str = boot_params.loader_name;
					while( (*d_str++ = *s_str++) != '\0' );				
					break;
			case MULTIBOOT_TAG_TYPE_MODULE:
					break;
			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
					boot_params.mem_lower = (unsigned int)(((struct multiboot_tag_basic_meminfo *) tag)->mem_lower);
					boot_params.mem_upper = (unsigned int)(((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
					break;
			case MULTIBOOT_TAG_TYPE_BOOTDEV:
					boot_params.biosdev = (int)(((struct multiboot_tag_bootdev *) tag)->biosdev);
					boot_params.slice = (int)(((struct multiboot_tag_bootdev *) tag)->slice);
					boot_params.part = (int)(((struct multiboot_tag_bootdev *) tag)->part);
					break;
			case MULTIBOOT_TAG_TYPE_MMAP:
					i = 0;
					multiboot_memory_map_t *mmap;
					for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;(multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
							mmap = (multiboot_memory_map_t *)((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size)){
						boot_params.e820_map[i].addr = mmap->addr;
						boot_params.e820_map[i].size = mmap->len;
						boot_params.e820_map[i].type = mmap->type;
						++i;
					}
					boot_params.e820entries = i;		
					break;	
					
					
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
					multiboot_uint32_t color;
					unsigned i;
					struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *) tag;
					
					void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;
					video.fb = fb;
					video.mode = VGA_MODE;	
					printf("fb addr: %x\n", fb);
					switch (tagfb->common.framebuffer_type) {
						case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED: {
							unsigned best_distance, distance;
							struct multiboot_color *palette;
							palette = tagfb->framebuffer_palette;
							color = 0;
							best_distance = 4*256*256;
							
							for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++) {
								
								distance = (0xff - palette[i].blue)
								* (0xff - palette[i].blue)
								+ palette[i].red * palette[i].red
								+ palette[i].green * palette[i].green;
								
								if (distance < best_distance) {
									color = i;
									best_distance = distance;
								}
							}
						}
							break;
						case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
							color = ((1 << tagfb->framebuffer_blue_mask_size) - 1)
							<< tagfb->framebuffer_blue_field_position;
							break;
						case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
							video.mode = TEXT_MODE;
							color = 0;
							return;
							break;
						default:
							color = 0xffffffff;
							break;
					}
					printf("width: %d height: %d\n", tagfb->common.framebuffer_width, tagfb->common.framebuffer_height);
					video.width = tagfb->common.framebuffer_width;
					video.height = tagfb->common.framebuffer_height;
					video.color = tagfb->common.framebuffer_bpp;

					for (i = 0; i < tagfb->common.framebuffer_width
					&& i < tagfb->common.framebuffer_height; i++) {
						switch (tagfb->common.framebuffer_bpp) {
						case 8: {
							multiboot_uint8_t *pixel = fb
							+ tagfb->common.framebuffer_pitch * i + i;
							*pixel = color;
						}
							break;
						case 15:
						case 16: {
							multiboot_uint16_t *pixel = fb + tagfb->common.framebuffer_pitch * i + 2 * i;
							*pixel = color;
						}
							break;
						case 24: {
							multiboot_uint32_t *pixel = fb + tagfb->common.framebuffer_pitch * i + 3 * i;
							*pixel = (color & 0xffffff) | (*pixel & 0xff000000);
						}
							break;
						case 32: {
							multiboot_uint32_t *pixel = fb + tagfb->common.framebuffer_pitch * i + 4 * i;
							*pixel = color;
						}
							break;
					}
				}
				break;
			}
			default:
					break;
		}
	}
}
