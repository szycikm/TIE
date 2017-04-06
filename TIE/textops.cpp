#include "textops.h"

char *AppendCharPtr(const char *str, const char c)
{
	int length = MyStrLen(str);
	char *appended = new char[length + 2];

	for (int i = 0; i < length; i++)
		appended[i] = str[i];

	appended[length] = c;
	appended[length + 1] = '\0';
	return appended;
}

char *GetFilenameExt(char *filename)
{
	char *bck = filename, *retval = "";
	while(*bck != '\0')
	{
		if (*bck == '.')
		{
			bck++;
			while(*bck != '\0')
			{
				retval = AppendCharPtr(retval, *bck);
				bck++;
			}
			return retval;
		}

		bck++;
	}
	return "";
}

bool MyStrCmp(char *str1, char *str2)
{
	char *bck1 = str1, *bck2 = str2;
	while(*bck1 != '\0')
	{
		if (*bck1 != *bck2) return false;
		if (*bck2 == '\0') return true;
		bck1++;
		bck2++;
	}
	return *bck2 == '\0';
}

int MyStrLen(const char *str)
{
	const char *bck = str;
	int cnt = 0;
	while (*bck != '\0')
	{
		cnt++;
		bck++;
	}
	return cnt;
}