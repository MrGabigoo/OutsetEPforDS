#include <PA_BgStruct.h>

extern const char Background_4_Tiles[];
extern const char Background_4_Map[];
extern const char Background_4_Pal[];

const PA_BgStruct Background_4 = {
	PA_BgNormal,
	256, 192,

	Background_4_Tiles,
	Background_4_Map,
	{Background_4_Pal},

	15232,
	{1536}
};
