/*
 * OTA: 클라우드 OTA (Over-The-Air) 업데이트
 *
 * GitHub에 올린 펌웨어를 자동으로 다운로드하여 업데이트합니다.
 */

#include "public_key.h"
#include <HTTPClient.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "UserConfig.h"

// 서버의 버전 정보를 확인하는 함수
int checkServerVersion() {
  Serial.println("[OTA 모듈] 서버 버전 확인 중...");

  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  client.setHandshakeTimeout(30000);

  http.begin(client, String(version_url));
  http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
  http.setTimeout(30000);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String versionStr = http.getString();
    versionStr.trim();
    int serverVersion = versionStr.toInt();

    Serial.printf("[OTA 모듈] 서버 버전: %d, 현재 버전: %d\n", serverVersion,
                  CURRENT_FIRMWARE_VERSION);

    http.end();
    client.stop();
    delay(500);
    return serverVersion;
  } else {
    Serial.printf("[OTA 모듈] ⚠️ 버전 확인 실패 (HTTP 코드: %d)\n", httpCode);
    http.end();
    client.stop();
    delay(500);
    return -1;
  }
}

// URL에서 펌웨어를 다운로드하고 OTA 업데이트를 실행하는 함수
void execOTA() {
  if (String(firmware_url).indexOf("http") < 0 ||
      String(firmware_url).indexOf("REPLACE") >= 0) {
    Serial.println("❌ 오류: 유효한 firmware_url을 설정해주세요!");
    return;
  }

  Serial.println("클라우드 OTA 업데이트를 시작합니다...");
  Serial.println("대상 URL: " + String(firmware_url));

  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  client.setHandshakeTimeout(30000);

  http.begin(client, String(firmware_url));
  http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
  http.setTimeout(30000);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("❌ 펌웨어 다운로드 실패 (HTTP 코드: %d)\n", httpCode);
    if (httpCode > 0) {
      Serial.printf("❌ 에러: %s\n", http.errorToString(httpCode).c_str());
    } else {
      Serial.println("❌ 연결 실패. 네트워크를 확인하세요.");
    }
    http.end();
    client.stop();
    return;
  }

  int contentLength = http.getSize();
  Serial.printf("다운로드 크기: %d bytes\n", contentLength);

  if (contentLength <= 0 || contentLength > 2000000) {
    Serial.println("❌ 오류: 잘못된 파일 크기");
    http.end();
    client.stop();
    return;
  }

  if (!Update.begin(contentLength)) {
    Serial.println("❌ OTA를 시작할 공간이 부족합니다.");
    http.end();
    client.stop();
    return;
  }

  Serial.println("OTA 업데이트를 시작합니다. 잠시만 기다려주세요...");

  size_t written = Update.writeStream(http.getStream());

  if (written != contentLength) {
    Serial.printf("❌ 다운로드 불완전: %d / %d bytes\n", written, contentLength);
    Update.abort();
    http.end();
    client.stop();
    return;
  }

  Serial.printf("✅ %d bytes 다운로드 완료\n", written);

  if (!Update.end(true)) {
    Serial.printf("❌ 업데이트 실패: %d\n", Update.getError());
    Update.abort();
    http.end();
    client.stop();
    return;
  }

  if (!Update.isFinished()) {
    Serial.println("❌ 업데이트가 완전히 종료되지 않았습니다.");
    http.end();
    client.stop();
    return;
  }

  Serial.println("✅ OTA 완료!");
  Serial.println("업데이트가 성공적으로 완료되었습니다. 3초 후 재부팅합니다...");

  http.end();
  client.stop();
  delay(3000);

  ESP.restart();
}

void initOTA() {
  Serial.println("\n[OTA 모듈] 초기화 시작...");

  Serial.print("[OTA 모듈] 와이파이 연결 중: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 20) {
    delay(500);
    Serial.print(".");
    tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[OTA 모듈] 와이파이 연결 성공!");
    Serial.print("[OTA 모듈] 할당된 IP 주소: ");
    Serial.println(WiFi.localIP());

    int serverVersion = checkServerVersion();

    if (serverVersion == -1) {
      Serial.println("[OTA 모듈] ⚠️ 버전 확인 실패. OTA 스킵");
    } else if (serverVersion != CURRENT_FIRMWARE_VERSION) {
      Serial.printf("[OTA 모듈] 🔄 버전 불일치 감지! (현재: v%d → 서버: v%d)\n",
                    CURRENT_FIRMWARE_VERSION, serverVersion);
      Serial.println("[OTA 모듈] 5초 후 펌웨어 다운로드를 시작합니다...");
      delay(5000);
      checkOTA();
    } else {
      Serial.printf("[OTA 모듈] ✅ 서버와 동일한 버전 (v%d)\n",
                    CURRENT_FIRMWARE_VERSION);
      Serial.println("[OTA 모듈] OTA 스킵");
    }
  } else {
    Serial.println("\n[OTA 모듈] ❌ 와이파이 연결 실패!");
    Serial.println("[OTA 모듈] 3초 후 자동 재부팅합니다...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("[OTA 모듈] 초기화 완료\n");
}

// OTA 업데이트를 확인하고 실행하는 함수 (언제든지 호출 가능)
void checkOTA() { execOTA(); }
