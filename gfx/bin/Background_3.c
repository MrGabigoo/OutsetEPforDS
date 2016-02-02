#include <PA_BgStruct.h>

extern const char Background_3_Tiles[];
extern const char Background_3_Map[];
extern const char Background_3_Pal[];

const PA_BgStruct Background_3 = {
	PA_BgNormal,
	256, 192,

	Background_3_Tiles,
	Background_3_Map,
	{Background_3_Pal},

	26240,
	{1536}
};
