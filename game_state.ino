#include "updated_temple.h"

/**
 * @brief DB gamestate가 setting 일 때 한번동작하는 코드
 */
void SettingFunc()
{
    bool activate_bool = false;
    applyBrightness();
    SendCmd("page ready");
    NeoFunc = NeoNo;
    lightColor(pixels_round, white);
    lightColor(pixels_side, white);
    lightColor(pixels_square, white);
}

/**
 * @brief DB gamestate가 ready 일 때 한번동작하는 코드
 */

void ReadyFunc()
{
    bool activate_bool = false;
    applyBrightness();
    SendCmd("page ready");
    NeoFunc = NeoBeforeTagger;
}

/**
 * @brief DB gamestate가 activate 일 때 반복동작하는 코드
 */
void ActivateFunc()
{
    RfidLoop();
}

/**
 * @brief DB gamestate가 activate 일 때 한번동작하는 코드
 */
void ActivateRunOnce()
{
    applyBrightness();
    activate_bool = true;
}

void DataChange()
{
    static StaticJsonDocument<1000> cur;

    String cmd;

    if ((int)my["brightness"] != (int)cur["brightness"])
    {
        applyBrightness();
    }

    if ((String)(const char *)my["game_state"] != (String)(const char *)cur["game_state"])
    {
        if ((String)(const char *)my["game_state"] == "setting")
        {
            SettingFunc();
        }
        else if ((String)(const char *)my["game_state"] == "ready")
        {
            ReadyFunc();
        }
        else if ((String)(const char *)my["game_state"] == "activate")
        {
            ActivateRunOnce();
        }
    }

    if ((String)(const char *)my["device_state"] != (String)(const char *)cur["device_state"])
    {
        if ((String)(const char *)my["device_state"] == "activate")
        {
            SendCmd("page basic");
            NeoFunc = NeoGaming;
        }
        else if ((String)(const char *)my["device_state"] == "player_win")
        {
            SendCmd("page lose");
            NeoFunc = NeoLose;
        }
        else if ((String)(const char *)my["device_state"] == "player_lose")
        {
            SendCmd("page win");
            NeoFunc = NeoWin;
        }
        else if ((String)(const char *)my["device_state"] == "blink")
        {
            NeoFunc = NeoTagger;
            activate_bool = true;
        }
    }

    if ((int)my["taken_chip"] != (int)cur["taken_chip"])
    {
        cmd = "taken_chip.pic=4+" + (String)(const char *)my["taken_chip"];
        sendCommand(cmd.c_str());
    }

    Serial.println("Data Change");
    cur = my;
}