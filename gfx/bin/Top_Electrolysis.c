#include <PA_BgStruct.h>

extern const char Top_Electrolysis_Tiles[];
extern const char Top_Electrolysis_Map[];
extern const char Top_Electrolysis_Pal[];

const PA_BgStruct Top_Electrolysis = {
	PA_BgNormal,
	256, 192,

	Top_Electrolysis_Tiles,
	Top_Electrolysis_Map,
	{Top_Electrolysis_Pal},

	8128,
	{1536}
};
