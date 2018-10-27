#include "ucs2.h"

int ucs2ToUtf8(unsigned short int ch, char *out, int max)
{
	int ret;

	//Encode UTF-8 from the 16-bit UCS-2 character
	ret = 0; //Default to failed.
	if (ch <= 0x7F) {
		if (max >= 1) {
			out[0] = (char)ch;
			ret = 1;
		}
	}
	else if (ch <= 0x7FF) {
		if (max >= 2) {
			out[0] = 0xC0 | (ch >> 6 & 0x1F);
			out[1] = 0x80 | (ch & 0x3F);
			ret = 2;
		}
	}
	else if (ch < 0xFFFF) {
		if (max >= 3) {
			out[0] = 0xE0 | (ch >> 12 & 0x0F);
			out[1] = 0x80 | (ch >> 6 & 0x3F);
			out[2] = 0x80 | (ch & 0x3F);
			ret = 3;
		}
	}

	return ret;
}

int utf8ToUcs2(const char *ch, unsigned short int *out)
{
	int ret;

	//Encode a 16-bit UCS-2 character from the UTF-8 character
	ret = 0; //Default to failed.
	if ((ch[0] & 0x80) == 0) {
		*out = ch[0];
		ret = 1;
	}
	else if ((ch[0] & 0xE0) == 0xC0) {
		if (ch[1] != '\0') { //Ensure that the 2nd byte is not null.
			*out = (ch[0] & 0x1F) << 6 | (ch[1] & 0x3F);
			ret = 2;
		}
	}
	else if ((ch[0] & 0xF0) == 0xE0) {
		if (ch[1] != '\0' && ch[2] != '\0') { //Ensure that the consecutive bytes are not null.
			*out = ((unsigned short int)ch[0] & 0x0F) << 12 | ((unsigned short int)ch[1] & 0x3F) << 6 | (ch[2] & 0x3F);
			ret = 3;
		}
	}

	return ret;
}

