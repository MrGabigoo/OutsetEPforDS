#include <PA_BgStruct.h>

extern const char Top_Volcano_Tiles[];
extern const char Top_Volcano_Map[];
extern const char Top_Volcano_Pal[];

const PA_BgStruct Top_Volcano = {
	PA_BgNormal,
	256, 192,

	Top_Volcano_Tiles,
	Top_Volcano_Map,
	{Top_Volcano_Pal},

	7296,
	{1536}
};
