#ifndef __SCOPE_H__
#define __SCOPE_H__
void init_osc();
void scope_keyboard();
void scopeSet(int A, int B, int C);
void scope_draw(int current_pid);
int scopeApid;
int scopeBpid;
int scopeCpid;
unsigned char *VRAM; 
int XSIZE; 
int x_p;
int y_A;
int y_B;
int y_C;
int y_Int;
int start_row;
int end_row;
int start_col;
int end_col;
#endif
