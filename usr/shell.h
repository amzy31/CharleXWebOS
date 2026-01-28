/*
  Emulation Unix-like terminal.
 */
#include "../include/input_init.h"

// help function 
void help(){
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
        print_on_terminal("disk - Disk manager");
        newline_on_terminal();
        print_on_terminal("Press ESC to exit the operating system");
        newline_on_terminal();
 
}

// get and read the keys  and kernel reaction
int input(){
  static char command_buffer[256];
  static int buffer_index = 0;
  static char current_dir[256] = "/";
  static char files[100][256];
  static int file_count = 0;

  char ch = 0;
  char keycode = 0;
  int ret = 0;

  do{
    keycode = get_input_prompt();
    // if user press enter (KEY)
    if(keycode == KEY_ENTER){
      command_buffer[buffer_index] = '\0';
      int handled = 0;
      if (buffer_index == 4 && command_buffer[0] == 'h' && command_buffer[1] == 'e' && command_buffer[2] == 'l' && command_buffer[3] == 'p') {
	      help();
              handled = 1;
      } else if (buffer_index == 2 && command_buffer[0] == 'l' && command_buffer[1] == 's') {
        newline_on_terminal();
        for(int i=0; i<file_count; i++){
          print_on_terminal(files[i]);
          newline_on_terminal();
        }
        newline_on_terminal();
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
        handled = 1;
      } else if (buffer_index == 3 && command_buffer[0] == 'p' && command_buffer[1] == 'w' && command_buffer[2] == 'd') {
        newline_on_terminal();
        print_on_terminal(current_dir);
        newline_on_terminal();
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
        handled = 1;
      } else if (buffer_index >= 3 && command_buffer[0] == 'c' && command_buffer[1] == 'd' && command_buffer[2] == ' ') {
        int i = 3, j = 0;
        char new_dir[256];
        while (command_buffer[i] && j < 255) {
          new_dir[j++] = command_buffer[i++];
        }
        new_dir[j] = '\0';
        if (new_dir[0] == '/' && new_dir[1] == '\0') {
          // only root directory is valid
          int k=0;
          while(new_dir[k]) { current_dir[k] = new_dir[k]; k++; }
          current_dir[k] = '\0';
        } else {
          newline_on_terminal();
          print_on_terminal("Error: not valid directory!");
          newline_on_terminal();
        }
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
        handled = 1;
      } else if (buffer_index >= 6 && command_buffer[0] == 'm' && command_buffer[1] == 'k' && command_buffer[2] == 'd' && command_buffer[3] == 'i' && command_buffer[4] == 'r' && command_buffer[5] == ' ') {
        int i = 6, j = 0;
        char name[256];
        while (command_buffer[i] && j < 255) {
          name[j++] = command_buffer[i++];
        }
        name[j] = '\0';
        if(file_count < 100){
          int k=0;
          while(name[k]) { files[file_count][k] = name[k]; k++; }
          files[file_count][k] = '\0';
          file_count++;
        }
        newline_on_terminal();
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
        handled = 1;
      } else if (buffer_index >= 6 && command_buffer[0] == 't' && command_buffer[1] == 'o' && command_buffer[2] == 'u' && command_buffer[3] == 'c' && command_buffer[4] == 'h' && command_buffer[5] == ' ') {
        int i = 6, j = 0;
        char name[256];
        while (command_buffer[i] && j < 255) {
          name[j++] = command_buffer[i++];
        }
        name[j] = '\0';
        if(file_count < 100){
          int k=0;
          while(name[k]) { files[file_count][k] = name[k]; k++; }
          files[file_count][k] = '\0';
          file_count++;
        }
        newline_on_terminal();
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
        handled = 1;
      } else if (buffer_index == 5 && command_buffer[0] == 'c' && command_buffer[1] == 'l' && command_buffer[2] == 'e' && command_buffer[3] == 'a' && command_buffer[4] == 'r') {
        clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
        sleep(0x10000); // small delay to prevent flickering
        next_line_index = 1;
        vga_index = 0;
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
        handled = 1;
      } else if (buffer_index == 4 && command_buffer[0] == 'd' && command_buffer[1] == 'i' && command_buffer[2] == 's' && command_buffer[3] == 'k') {
        newline_on_terminal();
        uint32 master_size = 0, slave_size = 0;
        uint32 disk_count = detect_disks(&master_size, &slave_size);
        if (disk_count == 0) {
          print_on_terminal("Disk manager: No disks found");
        } else {
          print_on_terminal("Disk manager: ");
          char count_str[4];
          itoa(disk_count, count_str);
          print_on_terminal(count_str);
          print_on_terminal(" disk(s) found");
          newline_on_terminal();
          if (master_size > 0) {
            print_on_terminal("Master drive size: ");
            char size_str[16];
            itoa(master_size, size_str);
            print_on_terminal(size_str);
            print_on_terminal(" MB");
            newline_on_terminal();
          }
          if (slave_size > 0) {
            print_on_terminal("Slave drive size: ");
            char size_str[16];
            itoa(slave_size, size_str);
            print_on_terminal(size_str);
            print_on_terminal(" MB");
            newline_on_terminal();
          }
        }
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
        handled = 1;
      }
      if (!handled) {
        newline_on_terminal();
        print_on_terminal("root@charlex:");
        print_on_terminal(current_dir);
        print_on_terminal("# ");
      }
      buffer_index = 0;
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
        ret = 1;
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

    sleep(0xcffffff); // small delay for emulator input speed

  }while(ch > 0);
  return ret;
}
