#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define WINDOWTITLE "TIE"
#define MENUPOSX 1
#define BOTTOMMENUPOSY 30
#define CANVASX 42
#define CANVASY 1
#define CANVASW 75
#define CANVASH 24
#define DEFAULTBGCOLOR BLACK
#define DEFAULTTEXTCOLOR LIGHTGRAY
#define BORDERCHAR 'X'
#define CURSORCHAR '*'
#define DRAWINGCHAR '#'
#define COPYCHAR '@'

// keycodes
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_CTRL_UP 141
#define KEY_CTRL_DOWN 145
#define KEY_CTRL_LEFT 115
#define KEY_CTRL_RIGHT 116
#define KEY_ALT_UP 152
#define KEY_ALT_DOWN 160
#define KEY_ALT_LEFT 155
#define KEY_ALT_RIGHT 157
#define KEY_BACKSPACE 8
#define KEY_ENTER 13
#define KEY_ESC 27

enum mode_t
{
	MOVE,
	COPYAREA,
	DRAWLINE,
	DRAWRECT
};

typedef struct
{
	short x;
	short y;
} coordinates_t;

typedef struct
{
	short marginTop;
	short marginLeft;
} canvas_t;