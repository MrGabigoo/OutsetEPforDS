#include <PA_BgStruct.h>

extern const char Background_2_Tiles[];
extern const char Background_2_Map[];
extern const char Background_2_Pal[];

const PA_BgStruct Background_2 = {
	PA_BgNormal,
	256, 192,

	Background_2_Tiles,
	Background_2_Map,
	{Background_2_Pal},

	48832,
	{1536}
};
