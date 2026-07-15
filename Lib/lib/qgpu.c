#include "qgpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

// = = = QPrint
static int qclamp(int v, int min, int max) { return v < min ? min : v > max ? max : v; }
static int oldClr = 255, actClr = 255, actStyle = 0; // White , Regular text
void setColor(int color) { oldClr = actClr; actClr = qclamp(color, 0, 255); }
void setStyle(int style) { actStyle = qclamp(style, 0, 1); }
void print(const char* format, ...) { printf("\033[%i;38;5;%im", actStyle, actClr); va_list args; va_start(args, format); vprintf(format, args); va_end(args); printf(RST); }
void printc(int color, const char* format, ...) { printf("\033[%i;38;5;%im", actStyle, color); va_list args; va_start(args, format); vprintf(format, args); va_end(args); printf(RST); }
// = = = QPrint end

// = = = ERROR HANDLING
#define QGPU_ERROR(CODE, FORMAT, ...) { if (CODE) { printf("\033[1;38;5;%imQGPU Error [\033[1;38;5;%im%03i\033[1;38;5;%im] in file '%s' on line \033[1;38;5;%im%i\033[1;38;5;%im:\n —> "FORMAT"\n"RST, \
	RED, LIGHT_RED, CODE, RED, __FILE_NAME__, LIGHT_RED, __LINE__, RED, ##__VA_ARGS__); exit(1); } }

static void c(int v, int n) { printf("\033[0;38;5;%im██%s", v, n==1? "\n" : n==2 ? "\n"RST : ""); }
void qgpuPrintColors() {
	printc(GRAY, "QGPU terminal colors: "); c(BLACK,0); c(WHITE,1);
	c(LIGHT_GRAY,0); c(LIGHT_RED,0); c(LIGHT_GREEN,0); c(LIGHT_YELLOW,0); c(LIGHT_ORANGE,0); c(LIGHT_BLUE,0); c(LIGHT_MAGENTA,0); c(LIGHT_CYAN,1);
	c(GRAY,0);       c(RED,0);       c(GREEN,0);       c(YELLOW,0);       c(ORANGE,0);       c(BLUE,0);       c(MAGENTA,0);       c(CYAN,1);
	c(DARK_GRAY,0);  c(DARK_RED,0);  c(DARK_GREEN,0);  c(DARK_YELLOW,0);  c(DARK_ORANGE,0);  c(DARK_BLUE,0);  c(DARK_MAGENTA,0);  c(DARK_CYAN,2);
}

void qgpuInit() {
	setStyle(BOLD);
	printc(ORANGE, "The application was made with the QGPU library.\n");
	printc(GRAY, "QGPU repo: "); printc(LIGHT_MAGENTA, "https://github.com/Qunerum/QGPU\n");
	qgpuPrintColors();
	setStyle(REGULAR);
	QGPU_ERROR(1, "Test error %02X", 10)

}
