#pragma once
#include "defines.h"
#include <stdlib.h>
#include "conio2.h"
#include "textops.h"
#include "image.h"

extern mode_t mode;

// Prints left-side menu
void PrintMenu();
// Prints bottom menu with all infos
void PrintInfos(coordinates_t *cursor, image_t *img, image_t *clipboard, imgstack_t *stack, short currentColor);
// Displays notification TEXT at the bottom of the screen in specified COLOR, then clears notification string so after next update it will dissapear
void DisplayNotification(char **text);
// Cleans screen where notification used to be
void CleanNotification();
// Allows user to input text, gives brief DESCription of what user must input. Returns user input
char* GetUserInput(const char *desc);
// Draws canvas border made from single char
void DrawBorder(image_t *img);
// Draws horizontal and vertical scrollers (if necessary)
void DrawScrollers(image_t *img, canvas_t *canvas);
// Chooses color of text based on background color
short ChooseTextColor(short bgColor);