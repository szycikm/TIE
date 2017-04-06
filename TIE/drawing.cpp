#include "drawing.h"

void UpdateImg(image_t *img, canvas_t *canvas)
{
	for (short i = 0; i < (img->imgH > CANVASH ? CANVASH : img->imgH); i++)
	{
		for (short j = 0; j < (img->imgW > CANVASW ? CANVASW : img->imgW); j++)
		{
			gotoxy(CANVASX + j + 1, CANVASY + i + 1);
			textbackground(img->image[j + canvas->marginLeft][i + canvas->marginTop]);
			putch(' ');
		}
	}
}

void DrawRect(image_t *img, const coordinates_t *c1, const coordinates_t *c2, short color)
{
	if (c1->x >= 0 && c1->y >= 0 && c2->x >= 0 && c2->y >= 0 && c1->x <= img->imgW && c1->y <= img->imgH && c2->x <= img->imgW && c2->y <= img->imgH)
	{
		for (short i = (c1->x < c2->x ? c1->x : c2->x); i <= (c1->x > c2->x ? c1->x : c2->x); i++)
		{
			for (short j = (c1->y < c2->y ? c1->y : c2->y); j <= (c1->y > c2->y ? c1->y : c2->y); j++)
			{
				img->image[i][j] = color;
			}
		}
		img->edited = true;
	}
}

void DrawLine(image_t *img, const coordinates_t *c1, const coordinates_t *c2, short color)
{
	short shorterx = c1->x < c2->x ? c1->x : c2->x;
	short longerx = c1->x > c2->x ? c1->x : c2->x;
	short shortery = c1->y < c2->y ? c1->y : c2->y;
	short longery = c1->y > c2->y ? c1->y : c2->y;
	if (longerx - shorterx > longery - shortery)
	{
		// more horizontal line
		short y;
		for (short i = shorterx; i <= longerx; i++)
		{
			if ((c1->x > c2->x && c1->y > c2->y) || (c1->x < c2->x && c1->y < c2->y))
			{
				// Q2/Q4
				y = shortery + (longery - shortery + 1) * (i - shorterx) / (longerx - shorterx + 1);
			}
			else
			{
				// Q1/Q3
				y = longery - (longery - shortery + 1) * (i - shorterx) / (longerx - shorterx + 1);
			}
			img->image[i][y] = color;
		}
	}
	else
	{
		// more vertical line
		short x;
		for (short i = shortery; i <= longery; i++)
		{
			if ((c1->x > c2->x && c1->y > c2->y) || (c1->x < c2->x && c1->y < c2->y))
			{
				// Q2/Q4
				x = shorterx + (longerx - shorterx + 1) * (i - shortery) / (longery - shortery + 1);
			}
			else
			{
				// Q1/Q3
				x = longerx - (longerx - shorterx + 1) * (i - shortery) / (longery - shortery + 1);
			}
			img->image[x][i] = color;
		}
	}
}

// Not to be called directly! Fill() must be called before in order to set prevcolor
void FillHelper(image_t *img, short x, short y, short color, short prevcolor)
{
	img->image[x][y] = color;
	img->edited = true;
	if (x > 0 && img->image[x - 1][y] == prevcolor) FillHelper(img, x - 1, y, color, prevcolor);
	if (y > 0 && img->image[x][y - 1] == prevcolor) FillHelper(img, x, y - 1, color, prevcolor);
	if (x < img->imgW - 1 && img->image[x + 1][y] == prevcolor) FillHelper(img, x + 1, y, color, prevcolor);
	if (y < img->imgH - 1 && img->image[x][y + 1] == prevcolor) FillHelper(img, x, y + 1, color, prevcolor);
}

void Fill(image_t *img, const coordinates_t *cursor, short color)
{
	if (color == img->image[cursor->x][cursor->y]) return; // no need to fill - desired color is already set
	FillHelper(img, cursor->x, cursor->y, color, img->image[cursor->x][cursor->y]);
}

void DrawFeedbackRect(image_t *img, const coordinates_t *c1, const coordinates_t *c2, canvas_t *canvas, char c)
{
	for (int i = (c1->x < c2->x ? c1->x : c2->x); i <= (c1->x > c2->x ? c1->x : c2->x); i++)
	{
		for (int j = (c1->y < c2->y ? c1->y : c2->y); j <= (c1->y > c2->y ? c1->y : c2->y); j++)
		{
			// check if drawing point is outside canvas
			if (i - canvas->marginLeft < CANVASW && j - canvas->marginTop < CANVASH && i - canvas->marginLeft >= 0 && j - canvas->marginTop >= 0)
			{
				gotoxy(CANVASX + i - canvas->marginLeft + 1, CANVASY + j - canvas->marginTop + 1);
				textcolor(ChooseTextColor(img->image[i][j]));
				textbackground(img->image[i][j]);
				putch(c);
			}
		}
	}
}

void DrawFeedbackLine(const image_t *img, const coordinates_t *c1, const coordinates_t *c2, const canvas_t *canvas)
{
	short shorterx = c1->x < c2->x ? c1->x : c2->x;
	short longerx = c1->x > c2->x ? c1->x : c2->x;
	short shortery = c1->y < c2->y ? c1->y : c2->y;
	short longery = c1->y > c2->y ? c1->y : c2->y;
	if(longerx - shorterx > longery - shortery)
	{
		// more horizontal line
		short y;
		for (short i = shorterx; i <= longerx; i++)
		{
			if ((c1->x > c2->x && c1->y > c2->y) || (c1->x < c2->x && c1->y < c2->y))
			{
				// Q2/Q4
				y = shortery + (longery - shortery + 1) * (i - shorterx) / (longerx - shorterx + 1);
			}
			else
			{
				// Q1/Q3
				y = longery - (longery - shortery + 1) * (i - shorterx) / (longerx - shorterx + 1);
			}
			// check if drawing point is outside canvas
			if (i - canvas->marginLeft < CANVASW && y - canvas->marginTop < CANVASH && i - canvas->marginLeft >= 0 && y - canvas->marginTop >= 0)
			{
				gotoxy(CANVASX + i - canvas->marginLeft + 1, CANVASY + y - canvas->marginTop + 1);
				textcolor(ChooseTextColor(img->image[i][y]));
				textbackground(img->image[i][y]);
				putch(DRAWINGCHAR);
			}
		}
	}
	else
	{
		// more vertical line
		short x;
		for (short i = shortery; i <= longery; i++)
		{
			if ((c1->x > c2->x && c1->y > c2->y) || (c1->x < c2->x && c1->y < c2->y))
			{
				// Q2/Q4
				x = shorterx + (longerx - shorterx + 1) * (i - shortery) / (longery - shortery + 1);
			}
			else
			{
				// Q1/Q3
				x = longerx - (longerx - shorterx + 1) * (i - shortery) / (longery - shortery + 1);
			}
			// check if drawing point is outside canvas
			if (x - canvas->marginLeft < CANVASW && i - canvas->marginTop < CANVASH && x - canvas->marginLeft >= 0 && i - canvas->marginTop >= 0)
			{
				gotoxy(CANVASX + x - canvas->marginLeft + 1, CANVASY + i - canvas->marginTop + 1);
				textcolor(ChooseTextColor(img->image[x][i]));
				textbackground(img->image[x][i]);
				putch(DRAWINGCHAR);
			}
		}
	}
}

void Copy(const image_t *source, image_t *target, const coordinates_t *src1, const coordinates_t *src2, const coordinates_t *tgt)
{
	if (src1->x >= 0 && src1->x <= source->imgW && src1->y >= 0 && src1->y <= source->imgH && src2->x >= 0 && src2->x <= source->imgW && src2->y >= 0 && src2->y <= source->imgH && tgt->x >= 0 && tgt->x <= target->imgW && tgt->y >= 0 && tgt->y <= target->imgH)
	{
		short longery = src1->y > src2->y ? src1->y : src2->y;
		short longerx = src1->x > src2->x ? src1->x : src2->x;
		short shortery = src1->y < src2->y ? src1->y : src2->y;
		short shorterx = src1->x < src2->x ? src1->x : src2->x;
		for(int i = 0; i < (longery - shortery < target->imgH - tgt->y - 1 ? longery - shortery + 1 : target->imgH - tgt->y); i++)
		{
			for(int j = 0; j < (longerx - shorterx < target->imgW - tgt->x - 1 ? longerx - shorterx + 1 : target->imgW - tgt->x); j++)
			{
				target->image[j + tgt->x][i + tgt->y] = source->image[j + shorterx][i + shortery];
			}
		}
		target->edited = true;
	}
}