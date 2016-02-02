#include <PA9.h>
#include <dirent.h>
#include <filesystem.h>
#include <unistd.h>
#include "dsgm_gfx.h"
#include "GameWorks.h"


int main (void) {
  RoomCount = 3;
  score = 0;
  lives = 3;
  health = 100;
  CurrentRoom = Room_Get_Index("Room_1");
  swiWaitForVBlank();
  PA_InitFifo();
  PA_Init();
  DSGM_Init_PAlib();
  Reset_Alarms();
  nitroFSInit();
  chdir("nitro:/");
  DSGM_Init_Sound();
  Room_1();
  return 0;
}
bool Room_1(void) {
  PA_ResetSpriteSys();
  PA_ResetBgSys();
  PA_LoadBackground(1, 2, &Background_1);
  PA_LoadBackground(0, 2, &Background_3);
  PA_LoadSpritePal(1, 0, (void*)DSGMPal0_Pal); PA_LoadSpritePal(0, 0, (void*)DSGMPal0_Pal);
  DSGM_Setup_Room(256, 192, 256, 192, 0, 0, 0, 0);
  PA_LoadText(1, 0, &Default); PA_LoadText(0, 0, &Default);
  Create_Object(Obj_SplashController, 0, false, 0, 0);
  DSGM_Complete_Room();
  while(true) {
    for (DSGMPL = 0; DSGMPL <= 127; DSGMPL++) {
      if (Instances[DSGMPL].InUse) {
        if (Instances[DSGMPL].EName == Obj_LookingBack && Stylus.Newpress && PA_SpriteTouched(DSGMPL)) Obj_LookingBackTouchNewPress_Event(DSGMPL);
      }
    }
    for (DSGMPL = 0; DSGMPL <= 127; DSGMPL++) {
      if(Instances[DSGMPL].InUse) {
        if (Instances[DSGMPL].EName == Obj_Stop) {
          if(Pad.Newpress.Start) Obj_StopButtonPressStart_Event(DSGMPL);
        }
      }
    }
    Frames += 1;
    RoomFrames += 1;
    if (Frames % 60 == 0) Seconds += 1;
    if (Frames % 60 == 0) RoomSeconds += 1;
    DSGM_ObjectsSync();
    DSGM_AlarmsSync();
    PA_WaitForVBL();
    PA_EasyBgScrollXY(1, 2, RoomData.TopX, RoomData.TopY);
    PA_EasyBgScrollXY(0, 2, RoomData.BottomX, RoomData.BottomY);
  }
  return true;
}
bool Room_2(void) {
  PA_ResetSpriteSys();
  PA_ResetBgSys();
  PA_LoadBackground(1, 2, &Background_2);
  PA_LoadBackground(0, 2, &Background_4);
  PA_LoadSpritePal(1, 0, (void*)DSGMPal0_Pal); PA_LoadSpritePal(0, 0, (void*)DSGMPal0_Pal);
  DSGM_Setup_Room(256, 192, 256, 192, 0, 0, 0, 0);
  PA_LoadText(1, 0, &Default); PA_LoadText(0, 0, &Default);
  Create_Object(Obj_SplashController, 0, false, 0, 0);
  DSGM_Complete_Room();
  while(true) {
    for (DSGMPL = 0; DSGMPL <= 127; DSGMPL++) {
      if (Instances[DSGMPL].InUse) {
        if (Instances[DSGMPL].EName == Obj_LookingBack && Stylus.Newpress && PA_SpriteTouched(DSGMPL)) Obj_LookingBackTouchNewPress_Event(DSGMPL);
      }
    }
    for (DSGMPL = 0; DSGMPL <= 127; DSGMPL++) {
      if(Instances[DSGMPL].InUse) {
        if (Instances[DSGMPL].EName == Obj_Stop) {
          if(Pad.Newpress.Start) Obj_StopButtonPressStart_Event(DSGMPL);
        }
      }
    }
    Frames += 1;
    RoomFrames += 1;
    if (Frames % 60 == 0) Seconds += 1;
    if (Frames % 60 == 0) RoomSeconds += 1;
    DSGM_ObjectsSync();
    DSGM_AlarmsSync();
    PA_WaitForVBL();
    PA_EasyBgScrollXY(1, 2, RoomData.TopX, RoomData.TopY);
    PA_EasyBgScrollXY(0, 2, RoomData.BottomX, RoomData.BottomY);
  }
  return true;
}
bool Room_Menu(void) {
  PA_ResetSpriteSys();
  PA_ResetBgSys();
  PA_LoadBackground(1, 2, &Top_Main);
  PA_LoadBackground(0, 2, &Bottom_SongSelection);
  PA_LoadSpritePal(1, 0, (void*)DSGMPal0_Pal); PA_LoadSpritePal(0, 0, (void*)DSGMPal0_Pal);
  DSGM_Setup_Room(256, 192, 256, 192, 0, 0, 0, 0);
  PA_LoadText(1, 0, &Default); PA_LoadText(0, 0, &Default);
  Create_Object(Obj_Stop, 0, false, 160, 16);
  Create_Object(Obj_LookingBack, 1, false, 0, 48);
  DSGM_Complete_Room();
  while(true) {
    for (DSGMPL = 0; DSGMPL <= 127; DSGMPL++) {
      if (Instances[DSGMPL].InUse) {
        if (Instances[DSGMPL].EName == Obj_LookingBack && Stylus.Newpress && PA_SpriteTouched(DSGMPL)) Obj_LookingBackTouchNewPress_Event(DSGMPL);
      }
    }
    for (DSGMPL = 0; DSGMPL <= 127; DSGMPL++) {
      if(Instances[DSGMPL].InUse) {
        if (Instances[DSGMPL].EName == Obj_Stop) {
          if(Pad.Newpress.Start) Obj_StopButtonPressStart_Event(DSGMPL);
        }
      }
    }
    Frames += 1;
    RoomFrames += 1;
    if (Frames % 60 == 0) Seconds += 1;
    if (Frames % 60 == 0) RoomSeconds += 1;
    DSGM_ObjectsSync();
    DSGM_AlarmsSync();
    PA_WaitForVBL();
    PA_EasyBgScrollXY(1, 2, RoomData.TopX, RoomData.TopY);
    PA_EasyBgScrollXY(0, 2, RoomData.BottomX, RoomData.BottomY);
  }
  return true;
}
