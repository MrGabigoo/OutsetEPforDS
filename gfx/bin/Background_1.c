#include <PA_BgStruct.h>

extern const char Background_1_Tiles[];
extern const char Background_1_Map[];
extern const char Background_1_Pal[];

const PA_BgStruct Background_1 = {
	PA_BgNormal,
	256, 192,

	Background_1_Tiles,
	Background_1_Map,
	{Background_1_Pal},

	18176,
	{1536}
};
