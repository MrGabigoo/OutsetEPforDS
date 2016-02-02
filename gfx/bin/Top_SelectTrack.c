#include <PA_BgStruct.h>

extern const char Top_SelectTrack_Tiles[];
extern const char Top_SelectTrack_Map[];
extern const char Top_SelectTrack_Pal[];

const PA_BgStruct Top_SelectTrack = {
	PA_BgNormal,
	256, 192,

	Top_SelectTrack_Tiles,
	Top_SelectTrack_Map,
	{Top_SelectTrack_Pal},

	9280,
	{1536}
};
