#include "include/kernel.h"
#include "include/utils.h"
#include "include/char.h"
#include "include/reqs.h"
#include "usr/shell.h"



// kernel entery point !!
void kernel_up(){

  // color of terminal(you can change it if you want :D )
  init_vga(WHITE, BLACK);
  logo();


  for (int i = 0; i < 40; i++){
    sleep(0x2FAFFFE); // sleep for logo to load ;;
  }
  //color set to green and black
  set_vga_color(GREEN, BLACK);
  newline_on_terminal();

  // input() for   infinity loop (terminal loop infinity)
  print_on_terminal("Please press ENTER, twice ");

while(1){
  if(input()) break;
}

}
