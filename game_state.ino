#include "updated_temple.h"

/**
 * @brief DB gamestate가 setting 일 때 한번동작하는 코드
 */
void SettingFunc()
{
    bool activate_bool = false;
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
    activate_bool = true;
}

void DataChange()
{
    if (!(const char *)my["device_name"])
    {
        Serial.println("[DataChange] 서버 데이터 없음, 스킵");
        return;
    }

    static StaticJsonDocument<1000> cur;

    String cmd;

    bool brightness_changed = ((int)my["brightness"] != (int)cur["brightness"]);

    if ((String)(const char *)my["game_state"] != (String)(const char *)cur["game_state"])
    {
        if ((String)(const char *)my["game_state"] == "setting")
        {
            if (brightness_changed) applyBrightness();
            SettingFunc();
        }
        else if ((String)(const char *)my["game_state"] == "ready")
        {
            if (brightness_changed) applyBrightness();
            ReadyFunc();
        }
        else if ((String)(const char *)my["game_state"] == "activate")
        {
            if (brightness_changed) applyBrightness();
            ActivateRunOnce();
        }
    }
    else if (brightness_changed)
    {
        applyBrightness();
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
        else if ((String)(const char *)my["device_state"] == "github")
        {
            ota.check();
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