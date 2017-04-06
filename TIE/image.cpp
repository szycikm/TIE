#include "image.h"

void InitStack(imgstack_t *head)
{
	head->next = NULL;
}

void StackPush(imgstack_t *s, image_t value)
{
	imgstack_t *nexxt = new imgstack_t;

	// initialize stack element
	image_t temp = { false, false, "", value.imgW, value.imgH, 0 };
	nexxt->value = temp;
	nexxt->value.InitArray();

	// insert values into newly created element
	for (short i = 0; i < nexxt->value.imgW; i++)
	{
		for (short j = 0; j < nexxt->value.imgH; j++)
		{
			nexxt->value.image[i][j] = value.image[i][j];
		}
	}
	// add new element to list
	nexxt->next = s->next;
	s->next = nexxt;
}

image_t StackPop(imgstack_t *s)
{
	if (s->next == NULL)
	{
		// no next element exists -> return empty image
		image_t temp = { false, false, "", 0, 0, 0 };
		return temp;
	}

	// create new image to return
	image_t el = { false, false, "", s->next->value.imgW, s->next->value.imgH, 0 };
	el.InitArray();

	// copy stack values to new image
	for (short i = 0; i < el.imgW; i++)
	{
		for (short j = 0; j < el.imgH; j++)
		{
			el.image[i][j] = s->next->value.image[i][j];
		}
	}

	// destroy stack element
	s->next->value.CleanArray();
	void* nexxt = s->next;
	s->next = s->next->next;
	delete nexxt;

	return el;
}

void CleanStack(imgstack_t *s)
{
	while (s->next != NULL)
	{
		StackPop(s);
	}
}