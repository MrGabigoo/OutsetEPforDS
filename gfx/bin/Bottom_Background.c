#include <PA_BgStruct.h>

extern const char Bottom_Background_Tiles[];
extern const char Bottom_Background_Map[];
extern const char Bottom_Background_Pal[];

const PA_BgStruct Bottom_Background = {
	PA_BgNormal,
	256, 192,

	Bottom_Background_Tiles,
	Bottom_Background_Map,
	{Bottom_Background_Pal},

	64,
	{1536}
};
