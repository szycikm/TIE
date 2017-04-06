#include "conio2.h"
#include "defines.h"
#include "drawing.h"
#include "io.h"
#include "fileio.h"
#include "image.h"

char *notificationText = "";
mode_t mode = MOVE;

int main(int argc, char *argv[])
{
	unsigned char input;
	short currentColor = BLACK;
	bool refresh = true; // determines if screen must be cleaned before next update
	coordinates_t cursor = { 0, 0 };
	coordinates_t prevCursor = { 0, 0 }; // for drawing rectangles/lines
	coordinates_t copyCursor = { 0, 0 }; // for storing copy position
	image_t img = { false, false, "", 0, 0, 0 }, clipboard = { false, false, "", 0, 0, 0 }; // clipboard.initialized determines wheather selection started
	canvas_t canvas = { 0, 0 };
	imgstack_t stack;
	InitStack(&stack);
	settitle(WINDOWTITLE);
	_setcursortype(_NOCURSOR);
	textbackground(DEFAULTBGCOLOR);
	textcolor(DEFAULTTEXTCOLOR);

	if(argc == 2)
	{
		// load image specified in fommand line
		OpenImage(&img, argv[1]);
	}

	while (true)
	{
		textbackground(DEFAULTBGCOLOR);
		textcolor(DEFAULTTEXTCOLOR);
		if (refresh)
		{
			clrscr();
			PrintMenu();
			refresh = false;
		}
		PrintInfos(&cursor, &img, &clipboard, &stack, currentColor);
		CleanNotification();
		if(notificationText != "") DisplayNotification(&notificationText);
		if (img.initialized)
		{
			DrawBorder(&img);
			DrawScrollers(&img, &canvas);
			UpdateImg(&img, &canvas);
			
			if (mode == DRAWRECT)
				DrawFeedbackRect(&img, &cursor, &prevCursor, &canvas, DRAWINGCHAR);
			else if (mode == COPYAREA)
				DrawFeedbackRect(&img, &cursor, &copyCursor, &canvas, COPYCHAR);
			else if (mode == DRAWLINE)
				DrawFeedbackLine(&img, &cursor, &prevCursor, &canvas);

			gotoxy(CANVASX + cursor.x + 1 - canvas.marginLeft, CANVASY + cursor.y + 1 - canvas.marginTop);
			textcolor(ChooseTextColor(img.image[cursor.x][cursor.y]));
			textbackground(img.image[cursor.x][cursor.y]);
			putch(CURSORCHAR);
		}

		input = getch();

		if (input == 0)
		{
			// arrow was entered
			// sidenote: ctrl+arrows will scroll console by default. For it to work disable crtl key shortcuts

			input = getch();
			
			if (img.initialized)
			{
				switch (input)
				{
				case KEY_UP:
					if (cursor.y - canvas.marginTop > 0)
						cursor.y--;
					break;
				case KEY_DOWN:
					if (cursor.y - canvas.marginTop + 1 < (img.imgH < CANVASH ? img.imgH : CANVASH))
						cursor.y++;
					break;
				case KEY_LEFT:
					if (cursor.x - canvas.marginLeft > 0)
						cursor.x--;
					break;
				case KEY_RIGHT:
					if (cursor.x - canvas.marginLeft + 1 < (img.imgW < CANVASW ? img.imgW : CANVASW))
						cursor.x++;
					break;
				case KEY_CTRL_UP:
					if (img.imgH > CANVASH && canvas.marginTop > 0)
					{
						canvas.marginTop--;
						cursor.y--;
					}
					break;
				case KEY_CTRL_DOWN:
					if (img.imgH > CANVASH && img.imgH - canvas.marginTop > CANVASH)
					{
						canvas.marginTop++;
						cursor.y++;
					}
					break;
				case KEY_CTRL_LEFT:
					if (img.imgW > CANVASW && canvas.marginLeft > 0)
					{
						canvas.marginLeft--;
						cursor.x--;
					}
					break;
				case KEY_CTRL_RIGHT:
					if (img.imgW > CANVASW && img.imgW - canvas.marginLeft > CANVASW)
					{
						canvas.marginLeft++;
						cursor.x++;
					}
					break;
				case KEY_ALT_UP:
					mode = COPYAREA;
					if (copyCursor.y - canvas.marginTop > 0)
						copyCursor.y--;
					break;
				case KEY_ALT_DOWN:
					mode = COPYAREA;
					if (copyCursor.y - canvas.marginTop + 1 < (img.imgH < CANVASH ? img.imgH : CANVASH))
						copyCursor.y++;
					break;
				case KEY_ALT_LEFT:
					mode = COPYAREA;
					if (copyCursor.x - canvas.marginLeft > 0)
						copyCursor.x--;
					break;
				case KEY_ALT_RIGHT:
					mode = COPYAREA;
					if (copyCursor.x - canvas.marginLeft + 1 < (img.imgW < CANVASW ? img.imgW : CANVASW))
						copyCursor.x++;
					break;
				default:
					break;
				}

				if(mode != COPYAREA && (input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT || input == KEY_CTRL_UP || input == KEY_CTRL_DOWN || input == KEY_CTRL_LEFT || input == KEY_CTRL_RIGHT))
					copyCursor = cursor;
			}
		}
		else
		{
			// normal char was entered

			// changing colors
			if (input == '1')
				currentColor = BLACK;
			else if (input == '2')
				currentColor = BLUE;
			else if (input == '3')
				currentColor = GREEN;
			else if (input == '4')
				currentColor = CYAN;
			else if (input == '5')
				currentColor = RED;
			else if (input == '6')
				currentColor = MAGENTA;
			else if (input == '7')
				currentColor = BROWN;
			else if (input == '8')
				currentColor = LIGHTGRAY;
			else if (input == '9')
				currentColor = DARKGRAY;
			else if (input == '0')
				currentColor = LIGHTBLUE;
			else if (input == 'Q' || input == 'q')
				currentColor = LIGHTGREEN;
			else if (input == 'W' || input == 'w')
				currentColor = LIGHTCYAN;
			else if (input == 'E' || input == 'e')
				currentColor = LIGHTRED;
			else if (input == 'R' || input == 'r')
				currentColor = LIGHTMAGENTA;
			else if (input == 'T' || input == 't')
				currentColor = YELLOW;
			else if (input == 'Y' || input == 'y')
				currentColor = WHITE;
			else if (input == 'c' || input == 'C')
			{
				// copy selected area
				if (mode == COPYAREA)
				{
					clipboard.CleanArray();
					clipboard.imgH = (cursor.y > copyCursor.y ? cursor.y - copyCursor.y : copyCursor.y - cursor.y) + 1;
					clipboard.imgW = (cursor.x > copyCursor.x ? cursor.x - copyCursor.x : copyCursor.x - cursor.x) + 1;
					clipboard.InitArray();
					coordinates_t tempCursor = { 0, 0 };
					Copy(&img, &clipboard, &cursor, &copyCursor, &tempCursor);
					mode = MOVE;
				}
			}
			else if (input == 'p' || input == 'P')
			{
				// paste
				if (mode == MOVE && clipboard.initialized)
				{
					coordinates_t tempCursorTopLeft = { 0, 0 };
					coordinates_t tempCursorBottomRight = { clipboard.imgW - 1, clipboard.imgH - 1 };
					StackPush(&stack, img);
					Copy(&clipboard, &img, &tempCursorTopLeft, &tempCursorBottomRight, &cursor);
				}
			}
			else if (input == 'f' || input == 'F')
			{
				// fill area from cursor position
				if (mode == MOVE)
				{
					StackPush(&stack, img);
					Fill(&img, &cursor, currentColor);
				}
			}
			else if (input == 'l' || input == 'L')
			{
				// draw line
				if (img.initialized)
				{
					if (mode == MOVE)
					{
						mode = DRAWLINE;
						prevCursor.x = cursor.x;
						prevCursor.y = cursor.y;
					}
					else if (mode == DRAWLINE)
					{
						StackPush(&stack, img);
						DrawLine(&img, &cursor, &prevCursor, currentColor);
						mode = MOVE;
					}
				}
			}
			else if (input == 'k' || input == 'K')
			{
				// draw rect
				if (img.initialized)
				{
					if (mode == MOVE)
					{
						mode = DRAWRECT;
						prevCursor.x = cursor.x;
						prevCursor.y = cursor.y;
					}
					else if (mode == DRAWRECT)
					{
						StackPush(&stack, img);
						DrawRect(&img, &cursor, &prevCursor, currentColor);
						mode = MOVE;
					}
				}
			}
			else if (input == 'o' || input == 'O')
			{
				if (mode == MOVE)
				{
					// load image
					if (img.edited && !MyStrCmp(GetUserInput("Unsaved image detected. Continue anyway? (y/n)"), "y")) continue;

					if (OpenImage(&img, GetUserInput("Input file name")))
					{
						cursor.x = 0;
						cursor.y = 0;
						canvas.marginTop = 0;
						canvas.marginLeft = 0;
						CleanStack(&stack);
						refresh = true;
					}
				}
			}
			else if (input == 'n' || input == 'N')
			{
				if (mode == MOVE)
				{
					// new image
					if (img.edited && !MyStrCmp(GetUserInput("Unsaved image detected. Continue anyway? (y/n)"), "y")) continue;

					short w = atoi(GetUserInput("New image width [px]"));
					short h = atoi(GetUserInput("New image height [px]"));

					if (w > 0 && h > 0)
					{
						img.CleanArray();
						img.imgW = w;
						img.imgH = h;
						img.InitArray();

						cursor.x = 0;
						cursor.y = 0;
						img.filename = "";
						canvas.marginTop = 0;
						canvas.marginLeft = 0;

						// fill whole canvas with a single color (white)						
						prevCursor.x = img.imgW - 1;
						prevCursor.y = img.imgH - 1;
						DrawRect(&img, &cursor, &prevCursor, WHITE);

						img.edited = false;
						CleanStack(&stack);
						refresh = true;
					}
					else
						notificationText = "New image dimensions not valid";				
				}
			}
			else if (input == 's' || input == 'S')
			{
				if (mode == MOVE)
				{
					// save image
					if (img.initialized)
					{
						if (img.filename == "")
							img.filename = GetUserInput("Input file name");

						if (img.filename != "")
							SaveImage(&img);
						else
							notificationText = "Invalid filename";
					}
					else
						notificationText = "Nothing to save";
				}
			}
			else if (input == 'i' || input == 'I')
			{
				if (mode == MOVE)
				{
					// load sample image
					if (img.edited && !MyStrCmp(GetUserInput("Unsaved image detected. Continue anyway? (y/n)"), "y")) continue;

					if (OpenImage(&img, "sample.sti"))
					{
						cursor.x = 0;
						cursor.y = 0;
						canvas.marginTop = 0;
						canvas.marginLeft = 0;
						CleanStack(&stack);
						refresh = true;
					}
				}
			}
			else if (input == KEY_BACKSPACE)
			{
				// undo
				if (mode == MOVE)
				{
					image_t temp = StackPop(&stack);

					// if there is a move to undo
					if(temp.initialized)
					{
						coordinates_t tempCursorTopLeft = { 0, 0 };
						coordinates_t tempCursorBottomRight = { temp.imgW - 1, temp.imgH - 1 };
						Copy(&temp, &img, &tempCursorTopLeft, &tempCursorBottomRight, &tempCursorTopLeft);
					}
				}
			}
			else if (input == KEY_ESC)
			{
				// cancel current action/exit program if no action
				if (mode != MOVE)
				{
					mode = MOVE;
				}
				else
				{
					if (img.edited && !MyStrCmp(GetUserInput("Unsaved image detected. Continue anyway? (y/n)"), "y")) continue;
					clipboard.CleanArray(); 
					img.CleanArray();
					CleanStack(&stack);
					break;
				}
			}
		}
	}
	return 0;
}