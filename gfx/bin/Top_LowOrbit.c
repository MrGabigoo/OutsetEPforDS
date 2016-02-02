#include <PA_BgStruct.h>

extern const char Top_LowOrbit_Tiles[];
extern const char Top_LowOrbit_Map[];
extern const char Top_LowOrbit_Pal[];

const PA_BgStruct Top_LowOrbit = {
	PA_BgNormal,
	256, 192,

	Top_LowOrbit_Tiles,
	Top_LowOrbit_Map,
	{Top_LowOrbit_Pal},

	7616,
	{1536}
};
