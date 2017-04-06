#pragma once
#include "defines.h"
#include <cstdio>
#include <stdlib.h>
#include "conio2.h"
#include "textops.h"
#include "image.h"

extern char *notificationText;

// Reads IMG contents and saves it on disk
void SaveImage(image_t *img);
// Opens file and reads its contents to IMG
bool OpenImage(image_t *img, char* filename);