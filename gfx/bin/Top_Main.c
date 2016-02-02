#include <PA_BgStruct.h>

extern const char Top_Main_Tiles[];
extern const char Top_Main_Map[];
extern const char Top_Main_Pal[];

const PA_BgStruct Top_Main = {
	PA_BgNormal,
	256, 192,

	Top_Main_Tiles,
	Top_Main_Map,
	{Top_Main_Pal},

	13120,
	{1536}
};
