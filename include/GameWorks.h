#include "dsgm_gfx.h"
#include "custom_gfx.h"
#include "Defines.h"
#include "ActionWorks.h"





enum ObjectEnums { Obj_SplashController = 1, Obj_Stop = 2, Obj_LookingBack = 3,  };
void Set_Sprite(u8 InstanceID, char *SpriteName, bool DeleteOld);
void Create_Object(u8 ObjectEnum, u8 InstanceID, bool Screen, s16 X, s16 Y);
u8 Sprite_Get_ID(char *SpriteName);
u8 Room_Get_Index(char *RoomName);
void Goto_Room_Backend(u8 RoomIndex);
u8 Count_Instances(u8 ObjectEnum);
void Goto_Next_Room(void);

void Obj_SplashControllerCreate_Event(u8 DAppliesTo) {
  for (DSGML = 0; DSGML <= (1 * 60); DSGML++) {
  PA_WaitForVBL();
  }
  Goto_Next_Room();
}
void Obj_StopButtonPressStart_Event(u8 DAppliesTo) {
  Stop_BG_Sound();
  PA_LoadBackground(1, 2, &Top_SelectTrack);
}
void Obj_LookingBackTouchNewPress_Event(u8 DAppliesTo) {
  Stop_BG_Sound();
  AS_SoundQuickPlay(Sound_1);
  PA_LoadBackground(1, 2, &Top_LookingBack);
}


void Set_Sprite(u8 InstanceID, char *SpriteName, bool DeleteOld) {
  Instances[InstanceID].HasSprite = true;
  if (DeleteOld) PA_DeleteSprite(Instances[InstanceID].Screen, InstanceID);
  switch(Sprite_Get_ID(SpriteName)) {
    case 0:
      Instances[InstanceID].Width = 16; Instances[InstanceID].Height = 16;
      PA_CreateSprite(Instances[InstanceID].Screen, InstanceID, (void*)Sprite_Play_Sprite, OBJ_SIZE_16X16, 1, 0, 256, 192);
      break;
  }
}

void Create_Object(u8 ObjectEnum, u8 InstanceID, bool Screen, s16 X, s16 Y) {
  Instances[InstanceID].EName = ObjectEnum;  Instances[InstanceID].InUse = true; Instances[InstanceID].Screen = Screen;
  Instances[InstanceID].OriginalX = X; Instances[InstanceID].OriginalY = Y;
  Instances[InstanceID].X = X; Instances[InstanceID].Y = Y;
  Instances[InstanceID].VX = 0; Instances[InstanceID].VY = 0;
  if (ObjectEnum == Obj_SplashController) {
     Instances[InstanceID].HasSprite = false;
     Obj_SplashControllerCreate_Event(InstanceID);
  } else if (ObjectEnum == Obj_Stop) {
     Instances[InstanceID].HasSprite = false;
  } else if (ObjectEnum == Obj_LookingBack) {
     Instances[InstanceID].HasSprite = true;
     Set_Sprite(InstanceID, "Sprite_Play", false);
  }

}
u8 Sprite_Get_ID(char *SpriteName) {
 if (strcmp(SpriteName, "Sprite_Play") == 0) return 0;
 return 0;
}

u8 Room_Get_Index(char *RoomName) {
 if (strcmp(RoomName, "Room_1") == 0) return 0;
 if (strcmp(RoomName, "Room_2") == 0) return 1;
 if (strcmp(RoomName, "Room_Menu") == 0) return 2;
 return 0;
}

void Goto_Room_Backend(u8 RoomIndex) {
  RoomFrames = 0;
  RoomSeconds = 0;
  if (RoomIndex == 0) Room_1();
  if (RoomIndex == 1) Room_2();
  if (RoomIndex == 2) Room_Menu();
}

void Goto_Next_Room(void) {
 if (CurrentRoom < RoomCount) {
  CurrentRoom += 1;
  Goto_Room_Backend(CurrentRoom);
 }
}

