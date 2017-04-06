#include "fileio.h"

void SaveImage(image_t *img)
{
	FILE *file = fopen(img->filename, "w");
	if (file != NULL)
	{
		// determine file format based on filename
		char *ext = GetFilenameExt(img->filename);
		if (MyStrCmp(ext, "sti"))
		{
			fprintf(file, "%%STI-1.1\nW%dH%d\n", img->imgW, img->imgH);
			for (int i = 0; i < img->imgH; i++)
			{
				for (int j = 0; j < img->imgW; j++)
				{
					fprintf(file, "%d,", img->image[j][i]);
				}
				fprintf(file, "\n");
			}
			notificationText = "Saved image";
			img->edited = false;
		}
		else
		{
			notificationText = "Unknown file extension";
			img->filename = "";
		}

		fclose(file);
	}
	else
	{
		notificationText = "Cannot read file";
		img->filename = "";
	}
}

bool OpenImage(image_t *img, char* filename)
{
	FILE *file = fopen(filename, "r");

	if (file == NULL)
	{
		notificationText = "Cannot read file";
		return false;
	}

	int w, h;
	char version[10];
	if (fscanf(file, "%s\nW%dH%d\n", &version, &w, &h) != 3 || !MyStrCmp(version, "%STI-1.1"))
	{
		notificationText = "Error reading file: invalid file format";
		fclose(file);
		return false;
	}

	img->CleanArray();
	img->imgW = w;
	img->imgH = h;
	img->InitArray();

	bool corrupted = false;
	int pixel;
	for (int i = 0; i < img->imgH; i++)
	{
		for (int j = 0; j < img->imgW; j++)
		{
			if (fscanf(file, "%d,", &pixel) != 1 || pixel > 15 || pixel < 0)
			{
				// invalid pixel -> fill with default color (white)
				img->image[j][i] = WHITE;
				corrupted = true;
			}
			else
				img->image[j][i] = pixel;
		}
	}
	
	notificationText = corrupted ? "Image data appears to be corrupted. Error pixels have been filled with white" : "Loaded image";
	img->edited = false;
	img->filename = filename;
	fclose(file);
	return true;
}