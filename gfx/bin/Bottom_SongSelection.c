#include <PA_BgStruct.h>

extern const char Bottom_SongSelection_Tiles[];
extern const char Bottom_SongSelection_Map[];
extern const char Bottom_SongSelection_Pal[];

const PA_BgStruct Bottom_SongSelection = {
	PA_BgNormal,
	256, 192,

	Bottom_SongSelection_Tiles,
	Bottom_SongSelection_Map,
	{Bottom_SongSelection_Pal},

	16640,
	{1536}
};
