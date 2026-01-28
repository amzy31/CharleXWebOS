#include "include/utils.h"
#include "include/input_init.h"

// Variable definitions
uint32 vga_index;
uint32 next_line_index = 1;
uint8 g_fore_color = WHITE;
uint8 g_back_color = BLACK;
uint16* vga_buffer;

// ATA I/O ports
#define ATA_DATA_PORT 0x1F0
#define ATA_ERROR_PORT 0x1F1
#define ATA_SECTOR_COUNT_PORT 0x1F2
#define ATA_LBA_LOW_PORT 0x1F3
#define ATA_LBA_MID_PORT 0x1F4
#define ATA_LBA_HIGH_PORT 0x1F5
#define ATA_DRIVE_HEAD_PORT 0x1F6
#define ATA_STATUS_PORT 0x1F7
#define ATA_COMMAND_PORT 0x1F7

// ATA commands
#define ATA_CMD_IDENTIFY 0xEC
#define ATA_CMD_READ_SECTORS 0x20

// Drive select values
#define ATA_MASTER_DRIVE 0xA0
#define ATA_SLAVE_DRIVE 0xB0

// Status register bits
#define ATA_STATUS_BUSY 0x80
#define ATA_STATUS_READY 0x40
#define ATA_STATUS_DRQ 0x08
#define ATA_STATUS_ERROR 0x01

uint32 strlen(const char* str)
{
  uint32 length = 0;
  while(str[length])
    length++;
  return length;
}

uint32 digit_count(int num)
{
  uint32 count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}

void itoa(int num, char *number)
{
  int dgcount = digit_count(num);
  int index = dgcount - 1;
  char x;
  if(num == 0 && dgcount == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[dgcount] = '\0';
  }
}

// Wait for ATA drive to be ready (not busy)
void ata_wait_ready() {
  uint8 status;
  do {
    status = inb(ATA_STATUS_PORT);
  } while (status & ATA_STATUS_BUSY);
}

// Select ATA drive (master or slave)
void ata_select_drive(uint8 drive) {
  outb(ATA_DRIVE_HEAD_PORT, drive);
  sleep(0x1000); // Small delay
}

// Check if ATA drive exists and get its size
uint32 ata_detect_drive(uint8 drive, uint32* size_mb) {
  uint16 identify_data[256];
  uint8 status;

  // Select drive
  ata_select_drive(drive);

  // Send IDENTIFY command
  outb(ATA_SECTOR_COUNT_PORT, 0);
  outb(ATA_LBA_LOW_PORT, 0);
  outb(ATA_LBA_MID_PORT, 0);
  outb(ATA_LBA_HIGH_PORT, 0);
  outb(ATA_COMMAND_PORT, ATA_CMD_IDENTIFY);

  // Check if drive exists
  status = inb(ATA_STATUS_PORT);
  if (status == 0) {
    return 0; // No drive
  }

  // Wait for drive to be ready
  ata_wait_ready();

  // Check for errors
  status = inb(ATA_STATUS_PORT);
  if (status & ATA_STATUS_ERROR) {
    return 0; // Drive error
  }

  // Check if data is ready
  if (!(status & ATA_STATUS_DRQ)) {
    return 0; // No data ready
  }

  // Read IDENTIFY data
  for (int i = 0; i < 256; i++) {
    identify_data[i] = inb(ATA_DATA_PORT) | (inb(ATA_DATA_PORT) << 8);
  }

  // Extract drive size from IDENTIFY data
  // Word 60-61 contain total number of user addressable sectors
  uint32 total_sectors = identify_data[60] | (identify_data[61] << 16);
  *size_mb = total_sectors / 2048; // Convert sectors to MB (512 bytes per sector)

  return 1; // Drive found
}

// Detect all ATA drives and return count
uint32 detect_disks(uint32* master_size, uint32* slave_size) {
  uint32 disk_count = 0;

  // Check master drive
  if (ata_detect_drive(ATA_MASTER_DRIVE, master_size)) {
    disk_count++;
  } else {
    *master_size = 0;
  }

  // Check slave drive
  if (ata_detect_drive(ATA_SLAVE_DRIVE, slave_size)) {
    disk_count++;
  } else {
    *slave_size = 0;
  }

  return disk_count;
}

uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color){
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color){
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}

void init_vga(uint8 fore_color, uint8 back_color){
  vga_buffer = (uint16*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
}

void set_vga_color(uint8 fore_color, uint8 back_color){
  g_fore_color = fore_color;
  g_back_color = back_color;
}

void scroll_screen(){
  for(uint32 i = 0; i < 24 * 80; i++){
    vga_buffer[i] = vga_buffer[i + 80];
  }
  for(uint32 i = 24 * 80; i < 25 * 80; i++){
    vga_buffer[i] = vga_entry(' ', g_fore_color, g_back_color);
  }
  next_line_index = 24;
  vga_index = 24 * 80;
  sleep(0x10000); // small delay to prevent flickering
}

void newline_on_terminal(){
  if(next_line_index >= 25){
    next_line_index = 1;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
    vga_index = 80 * 1;
  } else {
    vga_index = 80 * next_line_index;
    next_line_index++;
  }
}

void print_char(char ch){
  vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
  vga_index++;
}

void print_on_terminal(char *str){
  uint32 index =0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

void print_int(int num){
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_on_terminal(str_num);
}

uint8 inb(uint16 port){
  uint8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
  return ret;
}

void outb(uint16 port, uint8 data){
  asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

char get_input_prompt(){
  char ch = 0;
  while((ch = inb(KEYBOARD_PORT)) != 0){
    if(ch > 0)
      return ch;
  }
  return ch;
}

void wait_for_io(uint32 timer_count){
  while(1){
    asm volatile("nop" /*do noting*/  );
    timer_count--;
    if(timer_count <= 0)
      break;
    }
}

void sleep(uint32 timer_count){
  wait_for_io(timer_count);
}
