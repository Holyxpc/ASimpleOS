/*
 * =====================================================================================
 *
 *       Filename:  asmfunc.S
 *
 *    Description:  底层汇编函数
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
.text

.global io_hlt, io_sti, io_cli, get_eip

io_hlt: //void io_hlt(void)
    hlt
    ret

io_sti: //void io_sti(void)
    sti
    ret

io_cli: //void io_cli(void)
    cli
    ret

get_eip: //void get_eip(void)
    movl (%esp), %eax
    ret

