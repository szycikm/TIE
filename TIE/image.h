#pragma once
#include <cstddef>

typedef struct
{
	bool edited;
	bool initialized;
	char *filename;
	short imgW;
	short imgH;
	short **image;

	void InitArray()
	{
		if (!initialized)
		{
			image = new short*[imgW];
			for (short i = 0; i < imgW; i++)
				image[i] = new short[imgH];

			initialized = true;
		}
	}

	void CleanArray()
	{
		if (initialized)
		{
			for (short i = 0; i < imgW; i++)
				delete[] image[i];

			delete[] image;

			initialized = false;
		}
	}
} image_t;

typedef struct list
{
	image_t value;
	struct list *next;
} imgstack_t;

// Initializes image stack
void InitStack(imgstack_t *head);
// Inserts element to stack
void StackPush(imgstack_t *s, image_t value);
// Gets topmost element from stack and removes it
image_t StackPop(imgstack_t *s);
// Removes all elements from stack
void CleanStack(imgstack_t *s);