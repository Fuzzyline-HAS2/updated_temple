#include "temple.h"

/**
 * @brief 디스플레이에 변화를 주거나 변화가 있을시 실행
 */
void DisplayCheck()
{
    while (Serial2.available() > 0)
    {
        String nextion_string = Serial2.readStringUntil(' ');
        NextionReceived(&nextion_string);
    }
}

/**
 * @brief 디스플레이에서 오는 Serial을 확인
 *
 * @param nextion_string Serial 문자열 데이터
 */
void NextionReceived(String *nextion_string)
{
}

/**
 * @brief Nextion Display 구문(코드)를 입력하고 싶을때 사용
 */
void SendCmd(String command)
{
    String cmd = "";
    if (command.startsWith("page") && (String)(const char *)shift_machine["select_language"] != "KO")
    {
        cmd = "page E" + command.substring(5);
    }
    else
    {
        cmd = command;
    }

    sendCommand(cmd.c_str());
}