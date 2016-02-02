#include <PA_BgStruct.h>

extern const char Top_LookingBack_Tiles[];
extern const char Top_LookingBack_Map[];
extern const char Top_LookingBack_Pal[];

const PA_BgStruct Top_LookingBack = {
	PA_BgNormal,
	256, 192,

	Top_LookingBack_Tiles,
	Top_LookingBack_Map,
	{Top_LookingBack_Pal},

	8320,
	{1536}
};
