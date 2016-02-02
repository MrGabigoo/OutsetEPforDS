#include <PA_BgStruct.h>

extern const char Top_CometInbound_Tiles[];
extern const char Top_CometInbound_Map[];
extern const char Top_CometInbound_Pal[];

const PA_BgStruct Top_CometInbound = {
	PA_BgNormal,
	256, 192,

	Top_CometInbound_Tiles,
	Top_CometInbound_Map,
	{Top_CometInbound_Pal},

	8448,
	{1536}
};
