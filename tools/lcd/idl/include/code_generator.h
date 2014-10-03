#ifndef LCD_HEADER_GENERATOR_H_
#define LCD_HEADER_GENERATOR_H_

#include "lcd_ast.h"
#include <stdio.h>

int GenerateHeader(Scope* tree, FILE* out_file);

#endif // LCD_HEADER_GENERATOR_H_
