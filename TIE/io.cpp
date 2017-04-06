#include "io.h"

void PrintMenu()
{
	short pos = 1;
	gotoxy(MENUPOSX, pos++);
	cputs("Text Image Editor - Marcin Szycik 165116");
	pos++;
	gotoxy(MENUPOSX, pos++);
	cputs("arrow keys = move cursor");
	gotoxy(MENUPOSX, pos++);
	cputs("ctrl + arrow keys = scroll");
	gotoxy(MENUPOSX, pos++);
	cputs("alt + arrow keys = select area");
	gotoxy(MENUPOSX, pos++);
	cputs("c = copy selected area");
	gotoxy(MENUPOSX, pos++);
	cputs("p = paste");
	gotoxy(MENUPOSX, pos++);
	cputs("0-9, qwerty = select color");
	gotoxy(MENUPOSX, pos++);
	cputs("f = fill single color area");
	gotoxy(MENUPOSX, pos++);
	cputs("l = start/stop drawing line");
	gotoxy(MENUPOSX, pos++);
	cputs("k = start/stop drawing rectangle");
	gotoxy(MENUPOSX, pos++);
	cputs("n = new file");
	gotoxy(MENUPOSX, pos++);
	cputs("o = load file");
	gotoxy(MENUPOSX, pos++);
	cputs("i = load sample image");
	gotoxy(MENUPOSX, pos++);
	cputs("s = save");
	gotoxy(MENUPOSX, pos++);
	cputs("backspace = undo");
	gotoxy(MENUPOSX, pos);
	cputs("esc = cancel/exit program");
}

void PrintInfos(coordinates_t *cursor, image_t *img, image_t *clipboard, imgstack_t *stack, short currentColor)
{
	char outputBufor[4];
	
	// cursor position
	gotoxy(1, BOTTOMMENUPOSY - 1);
	clreol();
	putch('[');
	_itoa_s(cursor->x + 1, outputBufor, 10);
	cputs(outputBufor);
	putch(',');
	_itoa_s(cursor->y + 1, outputBufor, 10);
	cputs(outputBufor);
	putch(']');

	putch(' ');

	// current mode
	switch (mode)
	{
	case COPYAREA:
		cputs("[select area]");
		break;
	case DRAWLINE:
		cputs("[drawing line]");
		break;
	case DRAWRECT:
		cputs("[drawing rectangle]");
		break;
	default:
		cputs("[move]");
	}

	putch(' ');

	// is undo possible?
	textbackground(stack->next == NULL ? RED : GREEN);
	cputs("[UNDO]");

	textbackground(DEFAULTBGCOLOR);
	putch(' ');

	// clipboard
	if (clipboard->imgW > 0 && clipboard->imgH > 0)
	{
		if (clipboard->imgW > img->imgW || clipboard->imgH > img->imgH)
			textbackground(BROWN);
		else
			textbackground(GREEN);
	}
	else
		textbackground(RED);

	cputs("[CLIP:");
	_itoa_s(clipboard->imgW, outputBufor, 10);
	cputs(outputBufor);
	putch('x');
	_itoa_s(clipboard->imgH, outputBufor, 10);
	cputs(outputBufor);
	cputs("px");
	if (clipboard->imgW > img->imgW || clipboard->imgH > img->imgH) cputs(" > IMG");
	putch(']');

	// color palette
	gotoxy(1, BOTTOMMENUPOSY);
	
	textcolor(WHITE);
	textbackground(BLACK);
	cputs(currentColor == BLACK ? "[1]" : " 1 ");
	textbackground(BLUE);
	cputs(currentColor == BLUE ? "[2]" : " 2 ");
	textbackground(GREEN);
	cputs(currentColor == GREEN ? "[3]" : " 3 ");
	textbackground(CYAN);
	cputs(currentColor == CYAN ? "[4]" : " 4 ");
	textbackground(RED);
	cputs(currentColor == RED ? "[5]" : " 5 ");
	textbackground(MAGENTA);
	cputs(currentColor == MAGENTA ? "[6]" : " 6 ");
	textbackground(BROWN);
	cputs(currentColor == BROWN ? "[7]" : " 7 ");
	textbackground(LIGHTGRAY);
	cputs(currentColor == LIGHTGRAY ? "[8]" : " 8 ");
	textbackground(DARKGRAY);
	cputs(currentColor == DARKGRAY ? "[9]" : " 9 ");
	textbackground(LIGHTBLUE);
	cputs(currentColor == LIGHTBLUE ? "[0]" : " 0 ");
	textcolor(BLACK);
	textbackground(LIGHTGREEN);
	cputs(currentColor == LIGHTGREEN ? "[q]" : " q ");
	textbackground(LIGHTCYAN);
	cputs(currentColor == LIGHTCYAN ? "[w]" : " w ");
	textbackground(LIGHTRED);
	cputs(currentColor == LIGHTRED ? "[e]" : " e ");
	textbackground(LIGHTMAGENTA);
	cputs(currentColor == LIGHTMAGENTA ? "[r]" : " r ");
	textbackground(YELLOW);
	cputs(currentColor == YELLOW ? "[t]" : " t ");
	textbackground(WHITE);
	cputs(currentColor == WHITE ? "[y]" : " y ");

	textbackground(DEFAULTBGCOLOR);
	textcolor(DEFAULTTEXTCOLOR);

	putch(' ');
	clreol();

	// image dimensions + filename
	putch('[');
	_itoa_s(img->imgW, outputBufor, 10);
	cputs(outputBufor);
	putch('x');
	_itoa_s(img->imgH, outputBufor, 10);
	cputs(outputBufor);
	cputs("px] ");
	if(img->filename == "")
	{
		if (img->imgW > 0 || img->imgH > 0)
			cputs("[New file]");
		else
			cputs("[No file loaded]");
	}
	else
		cputs(img->filename);
}

void DisplayNotification(char **text)
{
	gotoxy(1, BOTTOMMENUPOSY - 2);
	clreol();
	textcolor(LIGHTRED);
	textbackground(WHITE);
	cputs(*text);
	*text = "";
}

void CleanNotification()
{
	textbackground(DEFAULTBGCOLOR);
	gotoxy(1, BOTTOMMENUPOSY - 2);
	clreol();
}

char* GetUserInput(const char *desc)
{
	textbackground(DEFAULTBGCOLOR);
	textcolor(DEFAULTTEXTCOLOR);
	unsigned char input = 0;
	char *userValue = "";

	_setcursortype(_NORMALCURSOR);
	gotoxy(1, BOTTOMMENUPOSY - 2);
	clreol();
	cputs(desc);
	cputs(": ");
	short descLength = MyStrLen(desc) + 3;

	// until user presses enter
	while (input != KEY_ENTER)
	{
		input = getche();
		if (input == KEY_BACKSPACE)
		{
			short length = MyStrLen(userValue);
			if (length > 0)
			{
				userValue[length - 1] = '\0';
				gotoxy(descLength + length - 1, BOTTOMMENUPOSY - 2);
				clreol();
			}
			else
				gotoxy(descLength, BOTTOMMENUPOSY - 2); // length of user input = 0 -> cursor must stay in place
		}
		else if (input == KEY_ESC)
		{
			_setcursortype(_NOCURSOR);
			return "";
		}
		else if(input != KEY_ENTER)
			userValue = AppendCharPtr(userValue, input);
	}

	_setcursortype(_NOCURSOR);
	gotoxy(1, BOTTOMMENUPOSY - 2);
	clreol();
	return userValue;
}

void DrawBorder(image_t *img)
{
	textbackground(WHITE);
	textcolor(BLACK);
	// horizontal borders cover corners and are 2 wider, and vertical are moved down by 1
	
	for (int i = 0; i < (img->imgW < CANVASW ? img->imgW : CANVASW) + 2; i++)
	{
		gotoxy(CANVASX + i, CANVASY);
		putch(BORDERCHAR);
		gotoxy(CANVASX + i, CANVASY + (img->imgH < CANVASH ? img->imgH : CANVASH) + 1);
		putch(BORDERCHAR);
	}

	for (int i = 0; i < (img->imgH < CANVASH ? img->imgH : CANVASH); i++)
	{
		gotoxy(CANVASX, CANVASY + 1 + i);
		putch(BORDERCHAR);
		gotoxy(CANVASX + (img->imgW < CANVASW ? img->imgW : CANVASW) + 1, CANVASY + 1 + i);
		putch(BORDERCHAR);
	}
}

void DrawScrollers(image_t *img, canvas_t *canvas)
{
	textbackground(DEFAULTBGCOLOR);
	textcolor(DEFAULTTEXTCOLOR);
	if(img->imgW > CANVASW)
	{
		short scrollerl = CANVASW * CANVASW / img->imgW;
		if (scrollerl < 1) scrollerl = 1; // zero-width scroller prevention
		short scrollerpos = canvas->marginLeft * CANVASW / img->imgW;
		gotoxy(CANVASX, CANVASY + CANVASH + 2);
		putch('<');
		for (int i = 0; i < scrollerpos; i++) putch('=');
		for (int i = 0; i < scrollerl; i++) putch('|');
		for (int i = 0; i < CANVASW - scrollerpos - scrollerl; i++) putch('=');
		putch('>');
	}

	if (img->imgH > CANVASH)
	{
		short scrollerl = CANVASH * CANVASH / img->imgH;
		if (scrollerl < 1) scrollerl = 1; // zero-width scroller prevention
		short scrollerpos = canvas->marginTop * CANVASH / img->imgH;
		gotoxy(CANVASX + CANVASW + 2, CANVASY);
		cputs("/\\");
		for (int i = 0; i < scrollerpos; i++)
		{
			gotoxy(CANVASX + CANVASW + 2, CANVASY + i + 1);
			cputs("||");
		}
		for (int i = scrollerpos; i < scrollerpos + 1 + scrollerl; i++)
		{
			gotoxy(CANVASX + CANVASW + 2, CANVASY + i + 1);
			cputs("==");
		}
		for (int i = scrollerpos + scrollerl + 1; i < CANVASH + 1; i++)
		{
			gotoxy(CANVASX + CANVASW + 2, CANVASY + i + 1);
			cputs("||");
		}
		cputs("\\/");
	}
}

short ChooseTextColor(short bgColor)
{
	return bgColor < 10 ? WHITE : BLACK;
}