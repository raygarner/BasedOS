#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//to check if the compiler thinks we are targeting athe wrong operating system
#if defined(__linux__)
#error "you are not using a cross compiler so it's likely that you will run into trouble"
#endif

//this will only work with 32-bit targets
#if !defined(__i386__)
#error "this must be compiled to 32-bit"
#endif

//colour constants set by hardware
enum vga_colour{
	VGA_COLOUR_BLACK = 0,
	VGA_COLOUR_BLUE = 1,
	VGA_COLOUR_GREEN = 2,
	VGA_COLOUR_CYAN = 3,
	VGA_COLOUR_RED = 4,
	VGA_COLOUR_MAGENTA = 5,
	VGA_COLOUR_BROWN = 6,
	VGA_COLOUR_LIGHT_GREY = 7,
	VGA_COLOUR_DARK_GREY = 8,
	VGA_COLOUR_LIGHT_BLUE = 9,
	VGA_COLOUR_LIGHT_GREEN = 10,
	VGA_COLOUR_LIGHT_CYAN = 11,
	VGA_COLOUR_LIGHT_RED = 12,
	VGA_COLOUR_LIGHT_MAGENTA = 13,
	VGA_COLOUR_LIGHT_BROWN = 14,
	VGA_COLOUR_WHITE = 15,
};

/*calculates the terminal colour from light grey and black by applying a bitwise OR
and then a bitwise left shift*/
static inline uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg){
	return fg | bg << 4;
}

//calculates the terminal index
static inline uint16_t vga_entry(unsigned char uc, uint8_t colour){
	return (uint16_t) uc | (uint16_t) colour << 8;
}

//calculates the length of a string
size_t strlen(const char* str){
	size_t len = 0;

	while (str[len])
		len++;
	
	return len;
}

static const size_t VGA_WIDTH = 80; //80
static const size_t VGA_HEIGHT = 25; //25

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_colour;

//'volatile' is used here to prevent the compiler from eliminating dead stores
volatile uint16_t* terminal_buffer;

void terminal_initialize(void){
	terminal_row = 0;
	terminal_column = 0;
	terminal_colour = vga_entry_colour(VGA_COLOUR_LIGHT_BROWN, VGA_COLOUR_BROWN);
	terminal_buffer = (uint16_t*) 0xB8000;
	
	for (size_t y = 0; y < VGA_HEIGHT; y++){
		for (size_t x = 0; x < VGA_WIDTH; x++){
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_colour);
		}
	}
}

void terminal_setcolour(uint8_t colour){
	terminal_colour = colour;
}

void terminal_putentryat(char c, uint8_t colour, size_t x, size_t y){
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, colour);
}

void terminal_putchar(char c){
	terminal_putentryat(c, terminal_colour, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH){
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT){
			terminal_row = 0;
	
			terminal_scroll();
		}
			//add terminal scroll down code here
	}
}

void terminal_write(const char* data, size_t size){
	for (size_t i =0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data){
	terminal_write(data, strlen(data));
}

static inline uint8_t inb(uint16_t port){
	uint8_t ret;
	asm volatile("inb %1, %0"
								:"=a"(ret)
								:"Nd"(port));
	
	return ret;
}

void terminal_linebreak(void){
	terminal_column = 0;
	terminal_row = ++terminal_row;

}

void terminal_writeline(const char* data){
	terminal_writestring(data);
	terminal_linebreak();

}

void clear_array(char* str){
	int i;
	for(i = 0; i<=(sizeof(str) / sizeof(str[0])); ++i)
		str[i] = 0;

}


void terminal_scroll(){
	/*
	for(int i = 0; i < VGA_HEIGHT; i++){
		for(int n = 0; n<VGA_WIDTH; n++){
			terminal_buffer[i * VGA_WIDTH+ n] = terminal_buffer[(i + 1) * VGA_WIDTH+ n];
		}
	}
	*/	
	//memmove(terminal_buffer, terminal_buffer + 80*2, 80*25*2);
}
void kernel_main(void){
	char buffer[20]; //stores the input of the user so that it can be evaluated
	
	int n;

	//for(n=0; n<=20; n++)
		//terminal_writestring(buffer[n]);
	
	int i = 0;
	

	

	//initialize terminal interface
	terminal_initialize();
	
	terminal_writestring("BasedOS:");
	

	
//	while(1){
	//		terminal_writestring(getchar());
	
	//}


	while(1){
		while(!(inb(0x64) &1));

	
		switch(inb(0x60)){
			case 0x1C: //if enter pressed
				i = 0;
				clear_array(buffer);
				//terminal_column = 0;
				//terminal_row = terminal_row + 1;
				terminal_linebreak();
				terminal_writestring("BasedOS:");
				break;
			case 0x0E:
				//terminal_writestring("backspace pressed");
				if(terminal_column > 8){
					terminal_column = terminal_column - 1;
					terminal_writestring(" ");
					terminal_column = terminal_column - 1;
					
					i  = --i;
					buffer[i] = 0;
	
				}
				break;
			case 0x1E:terminal_writestring("a"); buffer[i] = 'a'; i = ++i; break;
			case 0x30:terminal_writestring("b"); buffer[i] = 'b'; i = ++i; break;
			case 0x2E:terminal_writestring("c"); buffer[i] = 'c'; i = ++i; break;
			case 0x20:terminal_writestring("d"); buffer[i] = 'd'; i = ++i; break;
			case 0x12:terminal_writestring("e"); buffer[i] = 'e'; i = ++i; break;
			case 0x21:terminal_writestring("f"); buffer[i] = 'f'; i = ++i; break;
			case 0x22:terminal_writestring("g"); buffer[i] = 'g'; i = ++i; break;
			case 0x23:terminal_writestring("h"); buffer[i] = 'h'; i = ++i; break;
			case 0x17:terminal_writestring("i"); buffer[i] = 'i'; i = ++i; break;
			case 0x24:terminal_writestring("j"); buffer[i] = 'j'; i = ++i; break;
			case 0x25:terminal_writestring("k"); buffer[i] = 'k'; i = ++i; break;
			case 0x26:terminal_writestring("l"); buffer[i] = 'l'; i = ++i; break;
			case 0x32:terminal_writestring("m"); buffer[i] = 'm'; i = ++i; break;
			case 0x31:terminal_writestring("n"); buffer[i] = 'n'; i = ++i; break;
			case 0x18:terminal_writestring("o"); buffer[i] = 'o'; i = ++i; break;
			case 0x19:terminal_writestring("p"); buffer[i] = 'p'; i = ++i; break;
			case 0x10:terminal_writestring("q"); buffer[i] = 'q'; i = ++i; break;
			case 0x13:terminal_writestring("r"); buffer[i] = 'r'; i = ++i; break;
			case 0x1F:terminal_writestring("s"); buffer[i] = 's'; i = ++i; break;
			case 0x14:terminal_writestring("t"); buffer[i] = 't'; i = ++i; break;
			case 0x3C:terminal_writestring("u"); buffer[i] = 'u'; i = ++i; break;
			case 0x2F:terminal_writestring("v"); buffer[i] = 'v'; i = ++i; break;
			case 0x11:terminal_writestring("w"); buffer[i] = 'w'; i = ++i; break;
			case 0x2D:terminal_writestring("x"); buffer[i] = 'x'; i = ++i; break;
			case 0x15:terminal_writestring("y"); buffer[i] = 'y'; i = ++i; break;
			case 0x2C:terminal_writestring("z"); buffer[i] = 'z'; i = ++i; break;
			case 0x39:terminal_writestring(" "); buffer[i] = ' '; i = ++i; break;

		terminal_writeline(buffer);
		}
	}
	
	
	//while(1){
		//if(getScancode() == 0x5A)
			//terminal_writestring("Keyboard input worked!\n");
	//}
}
