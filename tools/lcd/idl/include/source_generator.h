#ifndef LCD_SOURCE_GENERATOR_H_
#define LCD_SOURCE_GENERATOR_H_

#include "lcd_ast.h"
#include <stdio.h>

int GenerateSource(Scope* tree, FILE* out_file);

#endif // LCD_SOURCE_GENERATOR_H_
