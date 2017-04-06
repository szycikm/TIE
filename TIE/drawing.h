#pragma once
#include "defines.h"
#include "conio2.h"
#include "io.h"
#include "image.h"

// Fills canvas with colors from **image array
void UpdateImg(image_t *img, canvas_t *can);
// Draws a rectangle in **image
void DrawRect(image_t *img, const coordinates_t *c1, const coordinates_t *c2, short color);
// Draws a line in image (width = 1)
void DrawLine(image_t *img, const coordinates_t *c1, const coordinates_t *c2, short color);
// Fills image from starting point defined by cursor position
void Fill(image_t *img, const coordinates_t *cursor, short color);
// Displays feedback rectangle composed of c for drawing rectangles/copying
void DrawFeedbackRect(image_t *img, const coordinates_t *c1, const coordinates_t *c2, canvas_t *canvas, char c);
// Displays line composed of # for drawing lines
void DrawFeedbackLine(const image_t *img, const coordinates_t *c1, const coordinates_t *c2, const canvas_t *canvas);
// Copies fragment of source to target (src1 and src2 specify copy fragment, tgt specifies cursor position in target image)
void Copy(const image_t *source, image_t *target, const coordinates_t *src1, const coordinates_t *src2, const coordinates_t *tgt);