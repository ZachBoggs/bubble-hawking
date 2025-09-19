#include <iostream>
#include <vector>
#include <espeak-ng/speak_lib.h>

#include "raylib.h"
#include "raymath.h"

using std::string;
using std::pair;
using std::vector;

struct circleDetails
{
  circleDetails()
    : size(0),x(0),y(0),maxSize(GetRandomValue(10,70))
  {
  }

  circleDetails(const int& nX,const int& nY)
    : x(nX),y(nY),size(0),maxSize(GetRandomValue(10,70))
  {
  }

  int maxSize;
  int size;
  int x;
  int y;
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "cmake project templated");

    SetTargetFPS(60);

    string exampleText("Hold down space");
    int fontSize(42);

    Color colors[5]{RED,ORANGE,GREEN,BLUE,VIOLET};
    Color colorsFaded[5];

    for(int i=0;i<5;++i)
    {
      colorsFaded[i] = Color{colors[i].r,colors[i].g,colors[i].b,110};
    }

    int colorIndex(0);

    //  circleDetails,color index
    vector<pair<circleDetails,int>> circleManager;

    string textToSpeechPrompt("Bubble");

    espeak_Initialize(AUDIO_OUTPUT_PLAYBACK,0,nullptr,0);
    espeak_SetVoiceByName("en-us-nyc");

    espeak_SetParameter(espeakRATE,150,0);
    espeak_SetParameter(espeakPITCH,10,0);
    espeak_SetParameter(espeakRANGE,10,0);
    espeak_SetParameter(espeakVOLUME,90,0);

    int framesDown(0);

    vector<Vector2> speechCaptionsLoc;

    while (!WindowShouldClose())
    {
      if(IsKeyDown(KEY_SPACE))
      {
        colorIndex = GetRandomValue(0,4);
        Vector2 circleLocation{(float)GetRandomValue(0,screenWidth),(float)GetRandomValue(0,screenHeight)};
        speechCaptionsLoc.push_back(circleLocation);

        circleDetails newCircle((int)circleLocation.x,(int)circleLocation.y);

        pair<circleDetails,int> newPair(newCircle,colorIndex);

        circleManager.push_back(newPair);
        framesDown++;

        if(framesDown % 5 == 0)
        {
          espeak_Cancel();
          espeak_Synth(textToSpeechPrompt.c_str(),textToSpeechPrompt.length() + 1,0,POS_CHARACTER,0,espeakCHARS_AUTO,nullptr,nullptr);
          espeak_SetParameter(espeakRATE,150 + (framesDown/10),0);
          espeak_SetParameter(espeakPITCH,10 + (framesDown/5),0);
          espeak_SetParameter(espeakVOLUME,50 + (framesDown/5),0);
        }
      }else
      {
        framesDown = 0;
        espeak_SetParameter(espeakRATE,150,0);
        espeak_SetParameter(espeakPITCH,10,0);
        espeak_SetParameter(espeakVOLUME,50,0);
      }

      for(int i=0;i<circleManager.size();++i)
      {
        circleManager[i].first.size += 1;

        if(circleManager[i].first.size >= circleManager[i].first.maxSize)
        {
          speechCaptionsLoc.erase(speechCaptionsLoc.begin() + i);
          circleManager.erase(circleManager.begin() + i);
        }
      }


      BeginDrawing();
      ClearBackground(RAYWHITE);

      for(int i=0;i<circleManager.size();++i)
      {
        circleDetails& currentCircle = circleManager[i].first;
        DrawCircle(currentCircle.x,currentCircle.y,currentCircle.size,colorsFaded[circleManager[i].second]);
      }

      for(int i=0;i<speechCaptionsLoc.size();++i)
      {
        DrawText(textToSpeechPrompt.c_str(),speechCaptionsLoc[i].x - (MeasureText(textToSpeechPrompt.c_str(),20) / 2),speechCaptionsLoc[i].y + 20,20,BLACK);
      }

      DrawText(exampleText.c_str(),(screenWidth/2) - (MeasureText(exampleText.c_str(),fontSize+1) / 2),(screenHeight/2) - ((fontSize+1)/2),fontSize+1,RAYWHITE);
      DrawText(exampleText.c_str(),(screenWidth/2) - (MeasureText(exampleText.c_str(),fontSize) / 2),(screenHeight/2) - (fontSize/2),fontSize,BLACK);
      //DrawText(exampleText.c_str(),(screenWidth/2) - (MeasureText(exampleText.c_str(),fontSize) / 2),(screenHeight/2) - (fontSize/2),fontSize,colors[colorIndex]);

      if(circleManager.size() > 0)
      {
        string circleCount("Circles: ");
        circleCount += std::to_string(circleManager.size());

        DrawText(circleCount.c_str(),20,50,20,BLACK);
      }
      DrawFPS(10, 10);

      EndDrawing();
    }

    CloseWindow();

    return 0;
}
