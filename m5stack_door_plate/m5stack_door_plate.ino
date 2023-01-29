#include <WiFi.h>
#include <WebServer.h>
#include <M5Stack.h>

// 定数定義
#define WIFI_MAX_TRY_COUNT 50

// グローバル変数
String g_html;
String g_c_html;
WebServer g_server(80);
boolean g_setup_flag = true;

// デバイス初期化
void init(unsigned char text_size, unsigned long delay_time)
{
    M5.begin();
    SD.begin();

    M5.Power.begin();

    delay(delay_time);

    M5.Lcd.setTextSize(text_size);
    M5.Lcd.setBrightness(100);
}

// SDカード内の指定のテキストファイルを読み出す
String readTextfile(const char *path)
{
    String text = "";

    File fp = SD.open(path);
    if(fp)
    {
        while(fp.available())
        {
            text = text + fp.readString();
        }
        fp.close();
    }

    return text;
}

// テキスト化したconfig.iniの指定の設定値を読み出す
String getConfigValue(String config_ini, const char *key, String line_feed_code="\r\n")
{
    String key_string = "";
    String substring_text = "";
    String value = "";
    unsigned int start_index = 0;
    unsigned int end_index = 0;

    // 検索文字列に改行コードを連結
    key_string = String(key);
    key_string += line_feed_code;

    // 検索文字列の後ろから切り出し
    start_index = config_ini.indexOf(key_string.c_str()) + key_string.length();
    substring_text = config_ini.substring(start_index);

    // 改行コードまで切り抜き
    end_index = substring_text.indexOf(line_feed_code);
    value = substring_text.substring(0, end_index);

    return value;
}

// Webサーバーイベントハンドラ：on
void handleOn()
{
    // URLパラメータ解析
    if (g_server.hasArg("status"))
    {
        // 仕事中
        if (g_server.arg("status").equals("working"))
        {
            M5.Lcd.drawJpgFile(SD, "/working.jpg");
        }
        // 会議中
        else if (g_server.arg("status").equals("meeting"))
        {
            M5.Lcd.drawJpgFile(SD, "/meeting.jpg");
        }
        // 休憩中
        else if (g_server.arg("status").equals("rest"))
        {
            M5.Lcd.drawJpgFile(SD, "/rest.jpg");
        }
    }

    g_server.send(200, "text/html", g_html);
}

void handleOnChild()
{
    // URLパラメータ解析
    if (g_server.hasArg("status"))
    {
        // 仕事中_子ども用
        if (g_server.arg("status").equals("working_for_child"))
        {
            M5.Lcd.drawJpgFile(SD, "/working_for_child.jpg");
        }
        // 会議中_子ども用
        else if (g_server.arg("status").equals("meeting_for_child"))
        {
            M5.Lcd.drawJpgFile(SD, "/meeting_for_child.jpg");
        }
        // 休憩中_子ども用
        else if (g_server.arg("status").equals("rest_for_child"))
        {
            M5.Lcd.drawJpgFile(SD, "/rest_for_child.jpg");
        }
    }

    g_server.send(200, "text/html", g_c_html);
}


// Webサーバーイベントハンドラ：onNotFound
void handleOnNotFound(void)
{
    g_server.send(404, "text/plain", "Not Found.");
}

void setup(void)
{
    String config_ini = "";
    String ssid = "";
    String password = "";

    unsigned char wifi_try_count = 0;

    // デバイス初期化
    init(2, 500);

    // Configファイル読み込み
    config_ini = readTextfile("/config.ini");
    if(config_ini.length() > 0) {
        M5.Lcd.println("Loaded config.ini");
    } else {
        M5.Lcd.println("Failed laod config.ini");
        g_setup_flag = false;
    }

    // Config：パスワード取得
    password = getConfigValue(config_ini, "#SSID_PASSWORD");
    // Config：SSID取得
    ssid = getConfigValue(config_ini, "#SSID");

    // htmlファイル読み込み
    g_html = readTextfile("/index.html");
    if(g_html.length() > 0) {
        M5.Lcd.println("Loaded index.html");
    } else {
        M5.Lcd.println("Failed laod index.html");
        g_setup_flag = false;
    }

    g_c_html = readTextfile("/child.html");
    if(g_c_html.length() > 0) {
        M5.Lcd.println("Loaded child.html");
    } else {
        M5.Lcd.println("Failed laod child.html");
        g_setup_flag = false;
    }

    // Wi-Fi接続開始
    M5.Lcd.println("Try connecting Wifi");
    WiFi.begin(ssid.c_str(), password.c_str());

    // Wi-Fi接続ステータス確認
    while (WiFi.status() != WL_CONNECTED) {
        if (wifi_try_count >= WIFI_MAX_TRY_COUNT){
            g_setup_flag = false;
            break;
        }

        M5.Lcd.print(".");
        delay(100);

        wifi_try_count++;
    }
    M5.Lcd.println("");

    // Wi-Fi接続成功
    if (WiFi.status() == WL_CONNECTED) {
        M5.Lcd.println("Wifi connection successful");
        M5.Lcd.print("IP: ");
        M5.Lcd.println(WiFi.localIP());
        M5.Lcd.println("");
    }

    // Webサーバー起動
    if (g_setup_flag) {
        g_server.on("/", handleOn);
        g_server.on("/child.html", handleOnChild);
        g_server.onNotFound(handleOnNotFound);
        g_server.begin();

        M5.Lcd.print("Please connect to http://");
        M5.Lcd.print(WiFi.localIP());
        M5.Lcd.print("/ with your browser");
    } else {
        M5.Lcd.println("System boot failure");
    }
}

void loop() {
    if (g_setup_flag) {
        g_server.handleClient();
    }
}
