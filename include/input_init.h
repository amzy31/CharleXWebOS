#include "kernel.h"
#include "utils.h"
#include "char.h"

extern uint32 vga_index;
extern uint32 next_line_index;
extern uint8 g_fore_color;
extern uint8 g_back_color;
extern uint16* vga_buffer;

extern uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color);
extern void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color);
extern void init_vga(uint8 fore_color, uint8 back_color);
extern void set_vga_color(uint8 fore_color, uint8 back_color);
extern void scroll_screen();
extern void newline_on_terminal();
extern void print_char(char ch);
extern void print_on_terminal(char *str);
extern void print_int(int num);
extern uint8 inb(uint16 port);
extern void outb(uint16 port, uint8 data);
extern char get_input_prompt();
extern void wait_for_io(uint32 timer_count);
extern void sleep(uint32 timer_count);
extern int input();
extern void kernel_up();
