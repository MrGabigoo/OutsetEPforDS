#include <PA9.h>
#include <locale.h>

s16 score;
s16 lives;
s16 health;
u8 RoomCount;
u8 CurrentRoom;

//////////////////////
/// DSGM Internals ///
//////////////////////

s16 DSGMPL = 0;
s16 DSGMSL = 0;
s16 DSGML = 0;
s32 Frames = 0;
s32 RoomFrames = 0;
s16 Seconds = 0;
s16 RoomSeconds = 0;

bool Top_Screen = true;
bool Bottom_Screen = false;

struct RoomDataStruct {
 s16 TopWidth;
 s16 TopHeight;
 s16 BottomWidth;
 s16 BottomHeight;
 s16 TopX;
 s16 TopY;
 s16 BottomX;
 s16 BottomY;
 bool CollisionLoaded;
} RoomData;

typedef struct {
  u8 EName;
  s16 OriginalX;
  s16 OriginalY;
  s16 X;
  s16 Y;
  s8 VX;
  s8 VY;
  bool Screen;
  bool InUse;
  u8 Width;
  u8 Height;
  s8 SpriteID;
  bool SpriteChanged;
  bool HasSprite;
  u8 Frame;
} Instance;
Instance Instances[128];

typedef struct {
  int Time;
  bool Activated;
  bool Rang;
} Alarm;
Alarm Alarms[32];

extern const unsigned short PA_TiledFont_Map[];
extern const unsigned char  PA_TiledFont_Tiles[];
extern const unsigned short PA_TiledFont_Pal[];

void DSGM_Init_PAlib(void) {
  pa_checklid = 0;
  _PA_LidDown = 0;
  setlocale(LC_ALL, "C");
  PA_VBLCountersReset();
  PA_VBLFunctionReset();
  PA_SetBrightness(0, 0); PA_SetBrightness(1, 0);
  PA_font[0]      = 0;                         PA_font[1] = 0;
  PA_textmap[0]   = (u16*) PA_TiledFont_Map;   PA_textmap[1] = (u16*) PA_TiledFont_Map;
  PA_texttiles[0] = (u8*)  PA_TiledFont_Tiles; PA_texttiles[1] = (u8*) PA_TiledFont_Tiles;
  PA_textpal[0]   = (u16*) PA_TiledFont_Pal;   PA_textpal[1]   = (u16*) PA_TiledFont_Pal;
  PA_UpdateUserInfo();
  PA_SetScreenSpace(48);
  MotionVBL = PA_Nothing;
  PaddleVBL = PA_Nothing;
  GHPadVBL  = PA_Nothing;
  PA_UpdateRTC();
  PA_InitRand();
  irqSet(IRQ_VBLANK, PA_vblFunc);
  irqEnable(IRQ_VBLANK);
}

void DSGM_ObjectsSync (void) {
  u8 i;
  for (i = 0; i <= 127; i++) {
    if (Instances[i].InUse) {
      Instances[i].X += Instances[i].VX;
      Instances[i].Y += Instances[i].VY;
      if (Instances[i].Screen) {
        if (Instances[i].HasSprite) {
          if (Instances[i].X > RoomData.TopX - PA_GetSpriteLx(1, i) && Instances[i].X < RoomData.TopX + 256 && Instances[i].Y > RoomData.TopY - PA_GetSpriteLy(1, i) && Instances[i].Y < RoomData.TopY + 192) {
            PA_SetSpriteXY(1, i, Instances[i].X - RoomData.TopX, Instances[i].Y - RoomData.TopY);
          } else {
            PA_SetSpriteX(1, i, 256);
          }
        }
      } else {
        if (Instances[i].HasSprite) {
          if (Instances[i].X > RoomData.BottomX - PA_GetSpriteLx(0, i) && Instances[i].X < RoomData.BottomX + 256 && Instances[i].Y > RoomData.BottomY - PA_GetSpriteLy(0, i) && Instances[i].Y < RoomData.BottomY + 192) {
            PA_SetSpriteXY(0, i, Instances[i].X - RoomData.BottomX, Instances[i].Y - RoomData.BottomY);
          } else {
            PA_SetSpriteX(0, i, 256);
          }
        }
      }
    }
  }
}

void DSGM_AlarmsSync(void) {
  u8 i;
  for (i = 0; i < 32; i++) {
    if (Alarms[i].Activated) {
      if (Alarms[i].Time == 0) {
        Alarms[i].Activated = false;
        Alarms[i].Rang = true;
      } else {
        Alarms[i].Time -= 1;
      }
    }
  }
}

void DSGM_Init_Sound(void) {
  PA_VBLFunctionInit(AS_SoundVBL);
  AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
  AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
}

void DSGM_Setup_Room(s32 TopWidth, s32 TopHeight, s32 BottomWidth, s32 BottomHeight, s32 TopX, s32 TopY, s32 BottomX, s32 BottomY) {
  RoomData.TopWidth = TopWidth;
  RoomData.TopHeight = TopHeight;
  RoomData.BottomWidth = BottomWidth;
  RoomData.BottomHeight = BottomHeight;
  RoomData.TopX = TopX;
  RoomData.TopY = TopY;
  RoomData.BottomX = BottomX;
  RoomData.BottomY = BottomY;
  u8 i = 0;
  for (i = 0; i <= 127; i++) {
    Instances[i].InUse = false;
    Instances[i].EName = 0;
    Instances[i].HasSprite = false;
    Instances[i].SpriteID = -1;
    Instances[i].Frame = 0;
  }
  PA_LoadDefaultText(1, 0);
  PA_LoadDefaultText(0, 0);
}

void Reset_Alarms(void) {
  u8 i;
  for (i = 0; i < 32; i++) {
    Alarms[i].Time = 60;
    Alarms[i].Activated = false;
    Alarms[i].Rang = false;
  }
}

void DSGM_Complete_Room(void) {
  u8 i = 0;
  for (i = 0; i <= 127; i++) {
    PA_SetSpritePrio(true, i, 1);
    PA_SetSpritePrio(false, i, 1);
  }
  DSGM_ObjectsSync();
  PA_UpdateStylus();
  PA_UpdatePad();
}

bool IsBgCollision(bool screen, s16 x, s16 y, u8 layer){
  return PA_EasyBgGetPixel(screen, layer, x, y);
}

///////////////
/// Objects ///
///////////////

s8 Get_Single_ID(u8 ObjectEnum) {
  u8 i;
  for (i = 0; i < 128; i++) {
    if (Instances[i].InUse) {
      if (Instances[i].EName == ObjectEnum) return i;
    }
  }
  return -1;
}

u8 NextInstance(void) {
  u8 i;
  for (i = 0; i <= 127; i++) {
    if(!Instances[i].InUse) {
      return i;
    }
  }
  return 127;
}

bool Sprite_Collision(u8 AppliesTo1, u8 AppliesTo2) {
 if (Instances[AppliesTo1].Screen != Instances[AppliesTo2].Screen) return false;
 s16 Point1X1 = Instances[AppliesTo1].X;
 s16 Point1Y1 = Instances[AppliesTo1].Y;
 s16 Point1X2 = Point1X1 + Instances[AppliesTo1].Width;
 s16 Point1Y2 = Point1Y1 + Instances[AppliesTo1].Height;
 s16 Point2X1 = Instances[AppliesTo2].X;
 s16 Point2Y1 = Instances[AppliesTo2].Y;
 s16 Point2X2 = Point2X1 + Instances[AppliesTo2].Width;
 s16 Point2Y2 = Point2Y1 + Instances[AppliesTo2].Height;
 if (((Point1X1 > Point2X1 && Point1X1 < Point2X2) &&
    (Point1Y1 > Point2Y1 && Point1Y1 < Point2Y2)) ||
    ((Point1X2 > Point2X1 && Point1X2 < Point2X2) &&
    (Point1Y1 > Point2Y1 && Point1Y1 < Point2Y2)) ||
    ((Point1X1 > Point2X1 && Point1X1 < Point2X2) &&
    (Point1Y2 > Point2Y1 && Point1Y2 < Point2Y2)) ||
    ((Point1X2 > Point2X1 && Point1X1 < Point2X2) &&
    (Point1Y2 > Point2Y1 && Point1Y2 < Point2Y2)) ||
    ((Point2X1 > Point1X1 && Point2X1 < Point1X2) &&
    (Point2Y1 > Point1Y1 && Point2Y1 < Point1Y2)) ||
    ((Point2X2 > Point1X1 && Point2X2 < Point1X2) &&
    (Point2Y1 > Point1Y1 && Point2Y1 < Point1Y2)) ||
    ((Point2X1 > Point1X1 && Point2X1 < Point1X2) &&
    (Point2Y2 > Point1Y1 && Point2Y2 < Point1Y2)) ||
    ((Point2X2 > Point1X1 && Point2X1 < Point1X2) &&
    (Point2Y2 > Point1Y1 && Point2Y2 < Point1Y2))) return true;
 return false;
}

bool Sprite_Collision_Mid(u8 AppliesTo1, u8 AppliesTo2) {
 if (Instances[AppliesTo1].Screen != Instances[AppliesTo2].Screen) return false;
  s16 MidPoint1X = Instances[AppliesTo1].X + (Instances[AppliesTo1].Width / 2);
  s16 MidPoint1Y = Instances[AppliesTo1].Y + (Instances[AppliesTo1].Height / 2);
  if (MidPoint1X > Instances[AppliesTo2].X && MidPoint1X < Instances[AppliesTo2].X + Instances[AppliesTo2].Width &&
      MidPoint1Y > Instances[AppliesTo2].Y && MidPoint1Y < Instances[AppliesTo2].Y + Instances[AppliesTo2].Height) {
      return true;
  }
  return false;
}

bool Object_Under_Point(bool Screen, s16 X, s16 Y, u8 ObjectEnum) {
  u8 i;
  for (i = 0; i < 128; i++) {
    if (Instances[i].InUse) {
      u16 IX = Instances[i].X;
      u16 IY = Instances[i].Y;
      u8 IW = Instances[i].Width;
      u8 IH = Instances[i].Height;
      if (Instances[i].Screen == Screen && Instances[i].EName == ObjectEnum && X >= IX && X < (IX + IW) && Y >= IY && Y < (IY + IH)) {
        return true;
      }
    }
  }
  return false;
}

bool Object_Under_H_Line(bool Screen, int X1, int X2, int Y, u8 ObjectEnum) {
 int i;
 for (i = 0; i < 128; i++) {
  if (Instances[i].InUse) {
   if (Instances[i].Screen == Screen && Instances[i].EName == ObjectEnum && Instances[i].X > (X1 - Instances[i].Width) && Instances[i].X < X2 && Instances[i].Y <= Y && (Instances[i].Y + Instances[i].Height) >= Y) {
    return true;
   }
  }
 }
 return false;
}

bool Object_Under_V_Line(bool Screen, int Y1, int Y2, int X, u8 ObjectEnum) {
 int i;
 for (i = 0; i < 128; i++) {
  if (Instances[i].InUse) {
   if (Instances[i].Screen == Screen && Instances[i].EName == ObjectEnum && Instances[i].Y > (Y1 - Instances[i].Height) && Instances[i].Y < Y2 && Instances[i].X <= X && (Instances[i].X + Instances[i].Width) >= X) {
    return true;
   }
  }
 }
 return false;
}

bool Move_Object_Stylus(u8 AppliesTo) {
  if (Instances[AppliesTo].Screen) return false;
  s16 X = Instances[AppliesTo].X;
  s16 Y = Instances[AppliesTo].Y;
  s8 LX = Instances[AppliesTo].Width;
  s8 LY = Instances[AppliesTo].Height;
  if (Stylus.Released) {
    PA_MovedSprite.Moving = false;
  } else if (Stylus.Held & !PA_MovedSprite.Moving) {
    if ((PA_MoveSpriteType == 0) && (Stylus.X > X) && (Stylus.X < X + LX) && (Stylus.Y > Y) && (Stylus.Y < Y + LY)) {
      PA_MovedSprite.Moving = true;
      PA_MovedSprite.Sprite = AppliesTo;
    }
    if (PA_MoveSpriteType && (PA_Distance(Stylus.X, Stylus.Y, X, Y) < LX * LY)) {		
      PA_MovedSprite.Moving = true;
      PA_MovedSprite.Sprite = AppliesTo;
    }
	}
  if (PA_MovedSprite.Moving && (PA_MovedSprite.Sprite == AppliesTo)) {
    Instances[AppliesTo].X = Stylus.X - (LX / 2);
    Instances[AppliesTo].Y = Stylus.Y - (LY / 2);
   return true;
  }
  return false;
}

void Animate(u8 AppliesTo, u8 FirstFrame, u8 LastFrame, u8 FPS) {
  PA_StartSpriteAnim(Instances[AppliesTo].Screen, AppliesTo, FirstFrame, LastFrame, FPS);
}

void Set_H_Flip(u8 AppliesTo, bool Flip) {
  PA_SetSpriteHflip(Instances[AppliesTo].Screen, AppliesTo, Flip);
}

void Set_V_Flip(u8 AppliesTo, bool Flip) {
  PA_SetSpriteVflip(Instances[AppliesTo].Screen, AppliesTo, Flip);
}

void Delete_Instance(u8 AppliesTo) {
  Instances[AppliesTo].InUse = false;
  PA_DeleteSprite(Instances[AppliesTo].Screen, AppliesTo);
}

s16 Get_X(u8 AppliesTo) {
  return Instances[AppliesTo].X;
}

s16 Get_Y(u8 AppliesTo) {
  return Instances[AppliesTo].Y;
}

void Set_X(u8 AppliesTo, s16 X) {
  Instances[AppliesTo].X = X;
}

void Set_Y(u8 AppliesTo, s16 Y) {
  Instances[AppliesTo].Y = Y;
}

void Set_XY(u8 AppliesTo, s16 X, s16 Y) {
  Set_X(AppliesTo, X);
  Set_Y(AppliesTo, Y);
}

void Set_Frame(u8 AppliesTo, u8 Frame) {
  PA_SetSpriteAnim(Instances[AppliesTo].Screen, AppliesTo, Frame);
  Instances[AppliesTo].Frame = Frame;
}

u8 Get_Frame(u8 AppliesTo) {
  return Instances[AppliesTo].Frame;
}

u8 Count_Instances(u8 ObjectEnum) {
  u8 Returnable = 0;
  u8 i = 0;
  for (i = 0; i <= 127; i++) {
    if (Instances[i].InUse) {
      if (Instances[i].EName == ObjectEnum) Returnable += 1;
    }
  }
  return Returnable;
}

////////////////
/// Graphics ///
////////////////

void Draw_Health(bool Screen, u8 XTiles, u8 YTiles) {
  PA_OutputText(Screen, XTiles, YTiles, "   ");
  PA_OutputText(Screen, XTiles, YTiles, "%d", health);
}

void Draw_Lives(bool Screen, u8 XTiles, u8 YTiles) {
  PA_OutputText(Screen, XTiles, YTiles, "   ");
  PA_OutputText(Screen, XTiles, YTiles, "%d", lives);
}

void Draw_Score(bool Screen, u8 XTiles, u8 YTiles) {
  PA_OutputText(Screen, XTiles, YTiles, "   ");
  PA_OutputText(Screen, XTiles, YTiles, "%d", score);
}

void Draw_String(bool Screen, u8 XTiles, u8 YTiles, char *Variable) {
  PA_OutputText(Screen, XTiles, YTiles, "%s", Variable);
}

void Draw_Variable(bool Screen, u8 XTiles, u8 YTiles, int Variable, bool IsFloat) {
  if (IsFloat) {
    PA_OutputText(Screen, XTiles, YTiles, "%f", Variable);
  } else {
    PA_OutputText(Screen, XTiles, YTiles, "%d", Variable);
  }
}

void Set_BG_Scroll(bool Screen, u8 Layer, s16 X, s16 Y) {
  //Default Layer is 2
  PA_EasyBgScrollXY(Screen, Layer, X, Y);
}

void Draw_Text(bool Screen, u8 XTiles, u8 YTiles, char *Text) {
  PA_OutputText(Screen, XTiles, YTiles, Text);
}

void Set_Camera_XY(bool Screen, u16 X, u16 Y) {
  if (X >= 0 && Y >= 0) {
    if (Screen) {
      if ((X <= RoomData.TopWidth - 256) && (Y <= RoomData.TopHeight - 192)) {
        RoomData.TopX = X; RoomData.TopY = Y;
      }
    } else {
      if ((X <= RoomData.BottomWidth - 256) && (Y <= RoomData.BottomHeight - 192)) {
        RoomData.BottomX = X; RoomData.BottomY = Y;
      }
    }
  }
}

u16 Get_Camera_X(bool Screen) {
  if (Screen) return RoomData.TopX;
  return RoomData.BottomX;
}

u16 Get_Camera_Y(bool Screen) {
  if (Screen) return RoomData.TopY;
  return RoomData.BottomY;
}

///////////////
//// Logic ////
///////////////

bool Test_Chance(int16 Sides) {
  if (PA_RandMinMax(1, Sides) == 1) return true;
  return false;
}

bool Is_Divisible(int16 Number, int16 Divisor) {
  if (Number % Divisor == 0) return true;
  return false;
}

bool Is_Position_Free(bool Screen, s16 X, s16 Y) {
 return !PA_EasyBgGetPixel(Screen, 3, X, Y);
}

bool Is_Position_Occupied(bool Screen, s16 X, s16 Y) {
 return PA_EasyBgGetPixel(Screen, 3, X, Y);
}

s32 Random(s32 Minimum, s32 Maximum) {
 return PA_RandMinMax(Minimum, Maximum);
}

void Set_String(char *Variable, const char *Text) {
  strcpy(Variable, Text);
  int n = strlen(Text);
  Variable[n] = '\0';
}

bool Strings_Same(char *String1, char *String2) {
  return (strcmp(String1, String2) == 0);
}

void Pause_Alarm(u8 AlarmID, bool Pause) {
  Alarms[AlarmID].Activated = Pause;
}

void Set_Alarm(u8 AlarmID, int Time) {
  Alarms[AlarmID].Time = Time;
  Alarms[AlarmID].Activated = true;
  Alarms[AlarmID].Rang = false;
}

///////////////
//// Sound ////
///////////////

void Stream_BG_Sound(char *Sound) {
  AS_MP3StreamPlay(strcat(Sound, ".mp3"));
}

void Stop_BG_Sound(void) {
  AS_MP3Stop();
}

void Pause_BG_Sound(void) {
  AS_MP3Unpause();
}

void Unpause_BG_Sound(void) {
  AS_MP3Unpause();
}

void Set_BG_Looping(bool Loop) {
  AS_SetMP3Loop(Loop);
}

void Stop_Sound_Effects(void) {
  u8 i;
  for (i = 0; i < 16; i++) {
   AS_SoundStop(i);
  }
}

bool Is_Sound_Playing(void) {
  u8 i;
  for (i = 0; i < 16; i++) {
    if(IPC_Sound->chan[i].busy) return true;
  }
  return false;
}

void Set_Volume(u8 Volume) {
  AS_SetMasterVolume(Volume);
}

void Set_BG_Panning(s8 Panning) {
  //-64 to +64!
  AS_SetMP3Pan(Panning + 64);
}

bool Read_File(char* Buffer, char* Path) {
 FILE *FH;
 FH = fopen(Path, "r");
 if (FH == NULL) return false;
 fread(Buffer, 1, 255, FH);
 fseek(FH, 0, SEEK_END);
 Buffer[ftell(FH)] = 0;
 fclose(FH);
 return true;
}

bool Write_File(char* Content, char* Path) {
 FILE *FH;
 FH = fopen(Path, "w");
 if (FH == NULL) {
  return false;
 }
 fprintf(FH, "%s", Content);
 fclose(FH);
 return true; 
}

bool List_Files(bool Screen, u8 X, u8 Y, char* Path) {
 DIR *FD;
 struct dirent *FE;
 FD = opendir(Path);
 if (FD == NULL) {
  return false;
 }
 while ((FE = readdir (FD)) != NULL) {
  if (!Strings_Same(FE->d_name, "..") && !Strings_Same(FE->d_name, ".")) {
   Draw_Text(Screen, X, Y, FE->d_name);
   Y += 1;
  }
 }
 return true;
}

void Switch_To_NitroFS() {
  chdir("nitro:/");
}

void Switch_To_FAT() {
  chdir("fat:/");
}

void Enable_Rotation(u8 AppliesTo) {
  PA_SetSpriteRotEnable(Instances[AppliesTo].Screen, AppliesTo, AppliesTo);
}

void Disable_Rotation(u8 AppliesTo) {
  PA_SetSpriteRotDisable(Instances[AppliesTo].Screen, AppliesTo);
}

void Set_Angle(u8 AppliesTo, s16 Angle) {
  PA_SetRotsetNoZoom(Instances[AppliesTo].Screen, AppliesTo, Angle);
}

float Angle_To_Radians(float Angle) {
  //Original by Prob_Caboose
  return ((Angle * 142) / 100);
}
	
float Radians_To_Angle(float Radians) {
  //Original by Prob_Caboose
  return ((Radians * 100) / 142);
}

int Angle_Between_Points(s16 X1, s16 Y1, s16 X2, s16 Y2) {
  return PA_GetAngle(X1, Y1, X2, Y2);
}

int Distance_Between_Points(s16 X1, s16 Y1, s16 X2, s16 Y2) {
  return PA_TrueDistance(X1, Y1, X2, Y2);
}