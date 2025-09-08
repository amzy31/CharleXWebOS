/*
  Emulation for key board terminal-like bindings.
 */
#include "../include/input_init.h"
#include "screen_ptrns.h"
// get and read the keys  and kernel reaction
void input(){
  static char command_buffer[256];
  static int buffer_index = 0;
  static char current_dir[256] = "/";
  char ch = 0;
  char keycode = 0;
  int enter_cuonter = 1;

  do{
    keycode = get_input_prompt();
    // if user press enter (KEY)
    if(keycode == KEY_ENTER){
      command_buffer[buffer_index] = '\0';
      int handled = 0;
      if (buffer_index == 4 && command_buffer[0] == 'h' && command_buffer[1] == 'e' && command_buffer[2] == 'l' && command_buffer[3] == 'p') {
        newline_on_terminal();
        print_on_terminal("CharleX OS Help:");
        newline_on_terminal();
        print_on_terminal("Available commands:");
        newline_on_terminal();
        print_on_terminal("help - Display this help message");
        newline_on_terminal();
        print_on_terminal("ls - List files in current directory");
        newline_on_terminal();
        print_on_terminal("pwd - Print current directory");
        newline_on_terminal();
        print_on_terminal("cd <dir> - Change directory");
        newline_on_terminal();
        print_on_terminal("mkdir <dir> - Create directory");
        newline_on_terminal();
        print_on_terminal("touch <file> - Create file");
        newline_on_terminal();
        print_on_terminal("clear - Clear the terminal screen");
        newline_on_terminal();
        print_on_terminal("Press ESC to exit the shell");
        newline_on_terminal();
        handled = 1;
      } else if (buffer_index == 2 && command_buffer[0] == 'l' && command_buffer[1] == 's') {
        newline_on_terminal();
        // No files to list in this simple OS
        newline_on_terminal();
        handled = 1;
      } else if (buffer_index == 3 && command_buffer[0] == 'p' && command_buffer[1] == 'w' && command_buffer[2] == 'd') {
        newline_on_terminal();
        print_on_terminal(current_dir);
        newline_on_terminal();
        handled = 1;
      } else if (buffer_index >= 3 && command_buffer[0] == 'c' && command_buffer[1] == 'd' && command_buffer[2] == ' ') {
        // copy arg to current_dir
        int i = 3, j = 0;
        while (command_buffer[i] && j < 255) {
          current_dir[j++] = command_buffer[i++];
        }
        current_dir[j] = '\0';
        newline_on_terminal();
        handled = 1;
      } else if (buffer_index >= 6 && command_buffer[0] == 'm' && command_buffer[1] == 'k' && command_buffer[2] == 'd' && command_buffer[3] == 'i' && command_buffer[4] == 'r' && command_buffer[5] == ' ') {
        newline_on_terminal();
        print_on_terminal("Directory created");
        newline_on_terminal();
        handled = 1;
      } else if (buffer_index >= 6 && command_buffer[0] == 't' && command_buffer[1] == 'o' && command_buffer[2] == 'u' && command_buffer[3] == 'c' && command_buffer[4] == 'h' && command_buffer[5] == ' ') {
        newline_on_terminal();
        print_on_terminal("File created");
        newline_on_terminal();
        handled = 1;
      } else if (buffer_index == 5 && command_buffer[0] == 'c' && command_buffer[1] == 'l' && command_buffer[2] == 'e' && command_buffer[3] == 'a' && command_buffer[4] == 'r') {
        clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
        next_line_index = 1;
        vga_index = 0;
        print_on_terminal("# ");
        handled = 1;
      }
      if (!handled) {
        newline_on_terminal();
        print_on_terminal("# ");
      }
      buffer_index = 0;

      enter_cuonter--;
      ++enter_cuonter;

    }
    /*
     * if user press backspace for delete a single
     */
    else if(keycode == KEY_BACKSPACE && vga_index>82){
        if (vga_index >= 2){

          vga_index -= 1;
          print_on_terminal(" ");
          vga_index -= 1;
          if (buffer_index > 0) {
            buffer_index--;
          }
          
        }

        else{

          get_input_prompt();

        }        
    }


    //if user press TAB (KEY)
    else if(keycode == KEY_TAB){

    }
    //if user press ESC (KEY)
    else if(keycode == KEY_ESC){
        print_on_terminal("EXIT ! : ");
        break;
    }

    else{
      ch = get_ascii_char(keycode);
      if (ch != 0) {
        print_char(ch);
        if (buffer_index < 255) {
          command_buffer[buffer_index++] = ch;
        }
      }
    }
    /*
     * NO SCROLL TERMINAL (static prompt )
     */
    if(enter_cuonter<5){
      enter_cuonter = enter_cuonter *2;
      next_line_index = next_line_index - enter_cuonter;
    }
    else{
      next_line_index = next_line_index - enter_cuonter;
    }

    sleep(0xdffffff); // small delay for emulator input speed

  }while(ch > 0);
}
