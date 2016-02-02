#include <PA_BgStruct.h>

extern const char Top_Positive_Tiles[];
extern const char Top_Positive_Map[];
extern const char Top_Positive_Pal[];

const PA_BgStruct Top_Positive = {
	PA_BgNormal,
	256, 192,

	Top_Positive_Tiles,
	Top_Positive_Map,
	{Top_Positive_Pal},

	7296,
	{1536}
};
