#ifndef QGPU_H
#define QGPU_H

// = = = > COLORS
// ANSI escape code using 8-bit color depth
#define RST     "\033[0m"

#define REGULAR       0
#define BOLD          1

#define BLACK         16
#define WHITE         15
// = > LIGHT
#define LIGHT_GRAY    252
#define LIGHT_RED     211
#define LIGHT_GREEN   120
#define LIGHT_YELLOW  227
#define LIGHT_ORANGE  215
#define LIGHT_BLUE    75
#define LIGHT_MAGENTA 207
#define LIGHT_CYAN    51
// = > NORMAL
#define GRAY          244
#define RED           160
#define GREEN         2
#define YELLOW        220
#define ORANGE        208
#define BLUE          27
#define MAGENTA       200
#define CYAN          81
// = > DARK
#define DARK_GRAY     241
#define DARK_RED      88
#define DARK_GREEN    22
#define DARK_YELLOW   136
#define DARK_ORANGE   130
#define DARK_BLUE     19
#define DARK_MAGENTA  128
#define DARK_CYAN     68

void qgpuInit();

#endif
