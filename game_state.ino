#include "updated_temple.h"

/**
 * @brief DB gamestate가 setting 일 때 한번동작하는 코드
 */
void SettingFunc()
{
    activate_bool = false;
    sendCommand("page pgSetting");
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
    activate_bool = false;
    sendCommand("page pgInfo");
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
    const char *device_name = (const char *)my["device_name"];
    BleAdvertiserUpdateFromDeviceName(device_name);

    if (!device_name)
    {
        Serial.println("[DataChange] 서버 데이터 없음, 스킵");
        return;
    }

    static StaticJsonDocument<1000> cur;

    String cmd;
    bool any_change = false;

    bool brightness_changed = ((int)my["brightness"] != (int)cur["brightness"]);

    if ((String)(const char *)my["game_state"] != (String)(const char *)cur["game_state"])
    {
        Serial.println("[DataChange] game_state: " +
            (String)(const char *)cur["game_state"] + " -> " +
            (String)(const char *)my["game_state"]);
        any_change = true;
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

    if (brightness_changed)
    {
        Serial.println("[DataChange] brightness: " +
            String((int)cur["brightness"]) + " -> " + String((int)my["brightness"]));
        any_change = true;
    }

    if ((String)(const char *)my["device_state"] != (String)(const char *)cur["device_state"])
    {
        Serial.println("[DataChange] device_state: " +
            (String)(const char *)cur["device_state"] + " -> " +
            (String)(const char *)my["device_state"]);
        any_change = true;
        if ((String)(const char *)my["device_state"] == "activate")
        {
            sendCommand("page pgChipCount");
            NeoFunc = NeoGaming;
        }
        else if ((String)(const char *)my["device_state"] == "player_win")
        {
            sendCommand("page pgTaggerLose");
            //NeoFunc = NeoLose;
        }
        else if ((String)(const char *)my["device_state"] == "player_lose")
        {
            sendCommand("page pgTaggerWin");
            //NeoFunc = NeoWin;
        }
        else if ((String)(const char *)my["device_state"] == "blink")
        {
            sendCommand("page pgAltarTag");
            NeoFunc = NeoTagger;
            activate_bool = true;
        }
        else if ((String)(const char *)my["device_state"] == "github")
        {
            ota.check();
        }
    }

    if((int)my["taken_chip"] != (int)cur["taken_chip"])
    {
        Serial.println("[DataChange] taken_chip: " +
            String((int)cur["taken_chip"]) + " -> " + String((int)my["taken_chip"]));
        any_change = true;
        cmd = "pgChipCount.vSacrificeChip.val=" + (String)(int)my["taken_chip"];
        sendCommand(cmd.c_str());
    }
    if((int)my["max_chip"] != (int)cur["max_chip"])
    {
        Serial.println("[DataChange] max_chip: " +
            String((int)cur["max_chip"]) + " -> " + String((int)my["max_chip"]));
        any_change = true;
        cmd = "pgChipCount.vMaxChip.val=" + (String)(int)my["max_chip"];
        sendCommand(cmd.c_str());
    }
    SyncLanguage();

    if (any_change)
        Serial.println("[DataChange] applied");

    cur = my;
}