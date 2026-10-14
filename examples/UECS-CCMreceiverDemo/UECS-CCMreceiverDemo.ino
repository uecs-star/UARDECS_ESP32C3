//////////////////////////////////////////////
//UARDECS Sample Program "UECS-CCMreceiver"
//Original By H.kurosaki 2026/6/26
//Modified for ESP32C3 2026/10/10 by T.Hoshi
/////////////////////////////////////////////
/*
This is a sample program that receives sensor 
measurements (every 10 seconds) and remote control 
commands (every 1 second) to perform ON/OFF 
control based on configured setpoints. As this is 
a demonstration that runs on the board alone—
without actual sensors or switches attached—all 
values ​​are configured via the Web (HTTP). 

Notes regarding setting up the compilation 
environment and porting the source code to other 
models are provided at the end.
 
　センサからの計測値(10秒ごと)と遠隔操作(1秒ごと)を受信して、
設定値によるON-OFF制御をするサンプルプログラムです。
センサとスイッチを装着しない基板だけで動作するデモなので
値は全てWeb(HTTP)を使って設定します。
コンパイル環境の構築や他機種からソースコード移植を行う
場合の注意点を一番後ろに記載しています
*/
/*
UARDECS_ESP32C3 Library

A library that provides communication capabilities 
compliant with the "Ubiquitous Environment Control 
System (UECS)" standard, which is widely used in 
protected horticulture in Japan.

This library facilitates UECS sweet application 
development on the ESP32C3 chip board by providing 
UDP packet broadcast communication via Wi-Fi and 
internet web server functionality, all compliant 
with the UECS communication protocol. It supports 
the ESP32C3-OLED, XIAO ESP32C3, and compatible 
boards. Please refer to LICENSE.md for licensing 
information. 

I. Differences between the ESP32C3 version and 
the previous version of the UARDEC library
(1) The connection method has changed from Ethernet 
to Wi-Fi, resulting in associated specification 
changes. Issues have been reported where broadcast 
CCMs fail to reach other nodes due to Wi-Fi-specific 
security settings (such as Wi-Fi client isolation). 
Please review security settings on devices like 
Wi-Fi bridges and verify packet delivery using 
tools such as a network analyzer. 
(2) The behavior of provider responses to the 
"Provider Search CCM" (which uses UDP port 16521) 
has changed regarding the TYPE attribute. Previously, 
only providers with an exact match for the search 
key in the TYPE attribute would respond. In the 
ESP32C3 version, providers with a partial match a
lso respond. This makes it easier to search for 
providers that share the same CCM identifier naming 
convention item names or node type names. However, 
since applications using this protocol will receive 
response CCMs from a larger number of providers than 
before, please be mindful of potential buffer overflows.

II. Points to note when using the ESP32C3 board with UARDECS:
(1) GPIO 3 is designated as the signal pin for entering 
safe mode. For the XIAO ESP32C3 board, this corresponds 
to pin D1. Connecting this pin to GND (0V) activates 
safe mode. The pin state is checked only once during 
the ARDECS setup process; changing the state afterward 
has no effect.
(2) On the ESP32, variables declared with `const` are 
automatically allocated to flash memory, so using the 
`PROGMEM=` assignment operator is no longer necessary.
(3) The ESP32C3 comes with a pre-configured MAC address, 
so manual definition is not required. However, since 
the device connects via Wi-Fi, you must configure the 
SSID and password for safe mode. To conserve memory, 
the character limit is set to 25 characters (values ​
​exceeding 25 characters cannot be configured). Additionally, 
safe mode utilizes DHCP (automatic addressing) to assign 
the network address automatically.

UARDECS_ESP32C3 for ArduinoIDE library software are distributed under the 2 clause BSD license.

Copyright 2026 Takehiko Hoshi, Hideto Kurosaki and Ken-ichiro Yasuba, Wakayama, Japan

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, 
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.

UARDECS_ESP32C3 ライブラリ

　日本の施設園芸分野で広く利用されている「ユビキタス環境制御
システム（UECS）」規格に準拠した通信機能を提供するライブラリ
です。
　本ライブラリは、UECS通信プロトコルに準拠したWi-Fi経由のUDP
パケットブロードキャスト通信やWebサーバー機能を提供すること
で、ESP32C3搭載ボード上でのUECS対応アプリケーション開発を容
易にします。ESP32C3-OLED、XIAO ESP32C3、および互換ボードを
サポートしています。ライセンス情報については、LICENSE.mdを
ご参照ください。

I.従来版のUARDECSとESP32C3版UARDECSとの相違
(1)Ethenet接続からWiFi接続になり、それに伴った仕様変更が実
施されています。WiFi特有のセキュリティー設定(WiFi端末分離
機能など)でブロードキャストCCMが他のノードで受信できない現
象の発生が報告されています。WiFiブリッジ等のセキュリティー
設定を見直し、ネットワークアナライザ等で配送確認してくださ
い。
(2)UDP16521ポートを使う提供者サーチCCMのTYPE属性に対する、
提供者応答の挙動が変わりました。提供者を検索するTYPE属性の
検索キーは、これまでは完全一致した提供者だけが応答してい
ました。ESP32C3版では、部分一致した提供者も応答するように
しました。CCM識別子命名規約の項目名やノード種別名が同一の
提供者を容易にサーチ可能です。ただ、このプロトコルを使用
するアプリケーションでは、従来より多数の提供者からの応答
CCMが送信されますので、バッファオーバーフローに注意して
ください。

II.UARDECSでESP32C3基板を使用するうえでの注意点
(1)セーフモードにする信号ピンはGPIO 3に設定しています。
XIAO ESP32C3ボードの場合はD1ピンになります。これをGND(0V)
にすることでセーフモードになります。ピンの状態チェックは
UARDECSのセットアップ時に一度だけです。以降変えても影響
ありません。
(2)ESP32からconst宣言だけでフラッシュメモリに割り当てら
れるのでPROGMEM=を使う必要はなくなりました。
(3)ESP32C3にはあらかじめmacアドレスが設定されていますの
で、定義不要になりました。また、WiFi接続になりますのでも
セーフモードの時のSSIDとpasswordの設定が必要です。メモリ
容量節約のため文字長は25文字までにしています。(25文字以
上は設定できません)そして、セーフモードではDHCP(自動ア
ドレス)接続になり、自動でアドレス設定されます。

Arduino IDE用ライブラリソフトウェア「UARDECS_ESP32C3」は、2条項BSDライセ
ンスの下で配布されています。

Copyright 2026 Takehiko Hoshi, Hideto Kurosaki and Ken-ichiro Yasuba, 
Wakayama, Japan

ソースコード形式およびバイナリ形式での再配布および使用は、変更の有無に
かかわらず、以下の条件が満たされる場合に許可されます。

1. ソースコードを再配布する場合、上記の著作権表示、この条件のリスト、
および以下の免責事項を保持する必要があります。 

2. バイナリ形式で再配布する場合、上記の著作権表示、この条件のリスト、
および以下の免責事項を、配布物と共に提供されるドキュメントやその他の
資料に記載する必要があります。

本ソフトウェアは、著作権者および貢献者によって「現状のまま」提供され、
商品性および特定目的への適合性に関する黙示の保証を含む（ただしこれらに
限定されない）いかなる明示的または黙示的な保証も否認されます。いかなる
場合においても、著作権者または貢献者は、本ソフトウェアの使用から生じる
直接的、間接的、付随的、特別、懲罰的、または派生的損害（代替品または代
替サービスの調達、使用・データ・利益の損失、または業務の中断を含むがこ
れらに限定されない）について、たとえそのような損害の可能性について知ら
されていたとしても、契約、無過失責任、または不法行為（過失その他を含む）
のいずれの責任理論に基づいても、一切の責任を負いません。
*/

#include <Uardecs_esp32c3.h>
// Please set the SSID and password (up to 25 characters) to be 
// used in Safe Mode. It connects via DHCP. Wi-Fi connections in 
// Normal Mode are based on the parameters (SSID, IP address, etc.)
//  configured via the web interface.
// セーフモードで使うSSIDとPASSWORF（25文字以内）を設定してください。 
// DHCP接続します。通常モードでのWi-Fi接続は、Web設定のパラメータ(SSID, 
// IPアドレス等)に基づきます。
#define SAFEMODE_SSID "SSID"
#define SAFEMODE_PASS "PASSWORD"
// Set UECS node description
const char U_name[] =   "UARDECSsample Receive"; //MAX 20 chars
const char U_vender[] = "Kindai Univ. T.Hoshi"; //MAX 20 chars
// What is a UECSID?
// For experimental purposes, leaving it as "000000000000" 
// is acceptable. However, if the device is to be sold as 
// a commercial product, please use the UECSID issued by 
// the UECS Association, as they are responsible for issuing 
// these IDs.
// UECSIDとは？
// 実験用途では000000000000のままで問題ありません。製品として
// 販売する場合は、UECS研究会がUECSIDの発行を行っているため、
// UECS研究会より発行されたUECSIDを記述してください。
const char U_uecsid[] = "000000000000"; //12 chars fixed
const char U_footnote[] = "UARDECS Sample CCM Receiver";
char U_nodename[20] = "CCM Receiver"; //MAX 19chars
// Structre valiables for string the UECS node specification
// Valiavle name, "U_orgAttribute" can not change
UECSOriginalAttribute U_orgAttribute;
// setting CCM description for web page
const int U_HtmlLine = 5; //Total number of HTML table rows.
// No.1 item
const char NAME1_st[] = "Meas. value";
const char UNIT1_st[] = "";
const char NOTE1_st[] = "センサ計測値";
signed long MeasVal;
// No.2 item
const char NAME2_st[] = "Under SP";
const char UNIT2_st[] = "";
const char NOTE2_st[] = "下限設定値";
signed long SetpVal;
// No.3 item
const char NAME3_st[] = "Remote SW";
const char UNIT3_st[] = "";
const char NOTE3_st[] = "遠隔操作値";
signed long RemoteSW;
const char SW_auto[] = "AUTO";
const char SW_off[] = "OFF";
const char SW_on[] = "ON";
const char SW_non[] = "----";
const char *Menu_no3[4] = { SW_auto, SW_off, SW_on,  SW_non } ;
// No.4 item
const char NAME4_st[] = "Manual SW";
const char UNIT4_st[] = "";
const char NOTE4_st[] = "Web操作値";
const char *Menu_no4[3] = { SW_auto, SW_off, SW_on } ;
signed long ManualSW;
// No.5 item
const char NAME5_st[] = "Control out";
const char UNIT5_st[] = "";
const char NOTE5_st[] = "制御出力";
const char *Menu_no5[2] = { SW_off, SW_on } ;
signed long ContOut;

const char** dummy = NULL;
// How to show any items in Web page.
struct UECSUserHtml U_html[U_HtmlLine]={
// Name,     Form format,    Unit,     Remark,  Select,ItemN, value,      min,   max, digit 
  {NAME1_st, UECSSHOWDATA,  UNIT1_st, NOTE1_st, dummy,    0, &(MeasVal),  -9999, 9999, 1},
  {NAME2_st, UECSINPUTDATA, UNIT2_st, NOTE2_st, dummy,    0, &(SetpVal),  -9999, 9999, 1},
  {NAME3_st, UECSSHOWSTRING,UNIT3_st, NOTE3_st, Menu_no3, 4, &(RemoteSW), 0,     3,    0},
  {NAME4_st, UECSSELECTDATA,UNIT4_st, NOTE4_st, Menu_no4, 3, &(ManualSW), 0,     2,    0},
  {NAME5_st, UECSSHOWSTRING,UNIT5_st, NOTE5_st, Menu_no5, 2, &(ContOut),  0,     1,    0},
};

// UserCCM setting
// To know the end in CCM description
//  CCMID_dummy must put on last.
enum {
  CCMID_MeasVal,
  CCMID_RemoteSW,
  CCMID_ContOut,
  CCMID_cnd,
  CCMID_dummy,
};
const int U_MAX_CCM = CCMID_dummy;
UECSCCM U_ccmList[U_MAX_CCM];
const char ccmName1[] = "Measurement value";
const char ccmType1[] = "MeasVal.mXX";
const char ccmUnit1[] = "";
const char ccmName2[] = "Remote Switch";
const char ccmType2[] = "RemoSW.mXX";
const char ccmUnit2[] = "";
const char ccmName3[] = "Control Output";
const char ccmType3[] = "ContOut.aXX";
const char ccmUnit3[] = "";
const char ccmNameCnd[] = "NodeCondition";
const char ccmTypeCnd[] = "cnd.aXX";
const char ccmUnitCnd[] = "";

// Description of CCM communication
void UserInit() {
//           S/R,  Index name,     Description,Type name,  unit,priority,digit,CCM Lv.    
  UECSsetCCM(false, CCMID_MeasVal, ccmName1,  ccmType1,  ccmUnit1,  29, 1, A_1M_1);
  UECSsetCCM(false, CCMID_RemoteSW,ccmName2,  ccmType2,  ccmUnit2,  29, 0, A_1S_0);
  UECSsetCCM(true,  CCMID_ContOut, ccmName3,  ccmType3,  ccmUnit3,  29, 0, A_1M_1);
  UECSsetCCM(true,  CCMID_cnd,     ccmNameCnd,ccmTypeCnd,ccmUnitCnd,29, 0, A_1S_0);
}
void OnWebFormRecieved() {
	// set control out
	ContOut = controlOut();
}
void UserEverySecond() {
	// refresh variables to receiverd data
  if(U_ccmList[CCMID_RemoteSW].validity) { // If the CCM is alive
		RemoteSW = U_ccmList[CCMID_RemoteSW].value + 1; // set CCM value
	}
	else {
	  RemoteSW = 3; // Set invalidation value
	}
	// set control out
	ContOut = controlOut();
	U_ccmList[CCMID_ContOut].value = ContOut;
	// Set cond flag
	U_ccmList[CCMID_cnd].value = (signed long)U_orgAttribute.status;

}
void UserEveryMinute() {
}
void UserEveryLoop() {
}
void loop() { 
	UECSloop();
}
// Decision ContOut value
signed long controlOut() {
	if(ManualSW == 1) return 0; // manual OFF
	else if(ManualSW == 2) return 1; // manual ON
	if(RemoteSW == 1) return 0; // remote OFF
	else if(RemoteSW == 2) return 1; // remote ON
	if(U_ccmList[CCMID_MeasVal].validity) { // If measurement is alive
	  MeasVal = U_ccmList[CCMID_MeasVal].value;
  	if(MeasVal < SetpVal) return 1; // automaic control by measurement
		else return 0;
	}
	else { // Measurement data is not available
		MeasVal = -9999;
	}
	return 0; // default is Off for safety
}

void setup(){
  // For boards where the USB port is directly connected 
  // to the ESP32-C3's GPIO18/19 pins (such as the XIAO and 
  // many 0.42-inch OLED boards), set "USB CDC On Boot" to 
  // "Enabled." For boards equipped with a USB-UART bridge 
  // IC (such as the CP210x or CH340), set it to "Disabled."
  // USB端子がESP32-C3のGPIO18/19へ直結された基板（XIAOや多くの
  // 0.42インチOLED基板）は USB CDC On Boot を Enabled にする。
  // CP210x/CH340等のUSB-UART変換ICを搭載した基板は Disabled にする。
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("[UARDECS] Booting...");
#if ARDUINO_USB_CDC_ON_BOOT
  Serial.println("[UARDECS] Serial transport: USB CDC");
#else
  Serial.println("[UARDECS] Serial transport: UART0");
#endif
	// Inhibit WiFi sleep
	WiFi.setSleep(false);
  // Wifi Credentials for Safe mode
  setWiFiCredentials(SAFEMODE_SSID, SAFEMODE_PASS);
  Serial.println("[UARDECS] Starting library (WiFi may take up to 20 seconds)...");
  UECSsetup();
	Serial.print("[UARDECS] Working: ");
  Serial.print(getUARDECSfor());
	Serial.print(" ");
  Serial.println(getUARDECSon());
	Serial.print("uecsWiFiSsid = ");
  Serial.println(getUECSWiFiSSID());
  Serial.print("uecsWiFiPassword = ");
  Serial.println(getUECSWiFiPASSWORD());
  if(UECSWiFiConnected()) 
    Serial.println("Success WiFi connection.");
  else Serial.println("Faild WiFi connection.");
  Serial.print("MAC Address = ");
  Serial.println(WiFi.macAddress());
  Serial.print("MY IP ADDRESS = ");
  Serial.println(WiFi.localIP());
  if(UECSSafeMode())
    Serial.println("[UARDECS] Safe mode (GPIO3 / XIAO D1 = LOW)");
  else Serial.println("[UARDECS] Normal mode (GPIO3 / XIAO D1 = HIGH)");
  // reset ContOut
	U_ccmList[CCMID_ContOut].value = ContOut = 0;
}

/*
(1) Setting up the compilation environment (verified with Arduino IDE 2.3.10)
(a) Installing the ESP32 board library: Go to the menu and select "File" > 
"Preferences". Enter the following URL into the "Additional Boards Manager URLs" 
field and click "OK": https://githubusercontent.com
(b) Installing the ESP32 board: Go to the menu and select "Tools" > "Board" > 
"Boards Manager". Type "esp32" into the search bar. Install "esp32" by Espressif 
Systems.
(c) Connect the ESP32-C3 board to your computer using a USB cable. From the menu, 
select "Tools" > "Board" > "ESP32 Arduino" and choose the board you are using 
(e.g., ESP32C3 Dev Module, XIAO_ESP32C3, etc.). Select the connected "Serial Port". 
These steps establish the minimum environment required to compile this file. 
Please repeat step (c) whenever you swap the board, even if it is the same model.
(d) Click the "->" (arrow) icon in the top-left corner of the IDE screen to start 
compilation and program upload.
(e) Once the upload is complete, the board will automatically reset and the 
program will begin execution.

(2) Regarding Safe Mode and Wi-Fi Network Usage
(a) Safe Mode (the mode used for the initial DHCP connection) is determined by 
the connection status of the GPIO 3 pin. On the XIAO ESP32C3, pin D1 corresponds 
to GPIO 3 (note that pin D3 corresponds to GPIO 5). Connecting D1 to GND (0V) 
activates Safe Mode. Leaving the pin open activates Normal Mode (connecting using 
the IP address configured via the web interface).
(b) The ESP32C3 comes with built-in Wi-Fi capabilities, which are used for 
networking. Therefore, please enter the SSID and password for your available Wi-Fi 
network into the `setWiFiCredentials("SSID", "Password");` function within `setup()`. 
In Safe Mode, the device connects using these credentials via DHCP (automatic 
address assignment).
(c) Connections in Normal Mode utilize the SSID, IP address, and other settings 
configured in the "Network Config" section of the web interface. Please configure 
these settings correctly while in Safe Mode, then disconnect the GPIO 3 (D1) pin 
from GND (leave it open).
(d) Since a MAC (physical) address is pre-configured in the built-in Wi-Fi chip, 
the manual configuration previously required for UARDECS is no longer necessary. 
If you need to retrieve the MAC address, it can be obtained using `WiFi.macAddress()`.

It is source-code compatible with the Arduino versions of UARDECS, UARDECS_MEGA, 
and UARDECS_PICO. In most cases, the code will compile successfully with only a 
change to the header file (library). You only need to include `#include <Uardecs_esp32c3.h>`; 
references to Ethernet-related libraries are not required.

*Note: Modifications will be necessary for any hardware-specific sections of the 
code. For the ESP32C3 targeted by this library, the following pins are reserved 
from the start, in addition to GPIO3 (D1) which is used by UARDECS for safe mode d
etection:
***ESP32C3 + 0.42-inch OLED
・GPIO2: One of the strapping pins that determine the boot mode. If a circuit that 
alters the pin's voltage level at power-on (such as a strong pull-down) is connected, 
the device may fail to boot correctly. It can be used as a standard GPIO after 
booting.
・GPIO5: Connected to the OLED SDA (data line).
・GPIO6: Connected to the OLED SCL (clock line).
・GPIO8: Connected to the onboard blue LED.
・GPIO9: Connected to the BOOT button. *As this is a critical pin for determining 
the boot mode, connecting external circuitry that constantly pulls it to GND will 
prevent program uploading and booting.
・GPIO12 – GPIO17: Dedicated to the internal SPI flash memory. Since they constantly 
engage in high-speed communication to read the program, they cannot be used as 
general-purpose GPIOs at all.
・GPIO18, GPIO19: Used for USB-JTAG/USB-Serial conversion functions (program uploading 
and serial monitor communication via the Type-C port). While it is technically 
possible to use them as GPIOs by disabling these settings in the program, it is 
best to avoid doing so due to the risk of losing communication with the PC.
***XIAO ESP32C3
・D2 (GPIO2): Reason: This is also a strapping pin related to the boot mode. 
Since there is no internal pull-up or pull-down, connected devices may cause boot 
failures.
・D3 (GPIO5 / A3, etc.): In some environments (such as older Arduino IDE definitions), 
this pin may not be correctly recognized or defined as an analog input (A3), or analog 
readings may be affected during Wi-Fi/BLE communication. These pins are also shared 
with JTAG signal lines (such as MTDI).
・D4 and D5 (GPIO6, 7) are used for I2C (Wire) SDA and SCL; it is best to avoid using 
them for other purposes.
・D6 is configured for UART output at startup; using it as an input carries a risk 
of inadvertently allowing a large current to flow. It should be used as an output.
・D8 (GPIO8): This is a strapping pin that determines the boot mode. If both D8 and 
GPIO9 are LOW at startup, the device may exhibit unexpected behavior or enter a 
special mode (UART_BOOT); therefore, care must be taken regarding the voltage level 
at startup (e.g., maintaining a HIGH state) when connecting external circuitry.
・D9 (GPIO9): This pin is connected to the onboard BOOT button and a pull-up resistor. 
While ideal for switch input, connecting a circuit that pulls the pin to GND at startup 
or attaching a heavy external load will prevent the device from booting correctly 
(or entering download mode).

Regarding voltage and current: the operating voltage is 3.3V. According to the 
official datasheet, when the drive capability is set to the maximum (PAD_DRIVER = 3), 
the maximum current per GPIO pin is 40mA for source current (output/sourcing) and 
28mA for sink current (input/sinking).

(3) About EEPROM
This library utilizes 4,096 bytes of unused program flash memory on the ESP32-C3 
as a storage area, serving as a substitute for the EEPROM found on Arduino boards. 
Additionally, it uses 4,096 bytes of SRAM as a cache area (note that this memory 
is allocated upon startup), thereby reducing the amount of available memory by 
that same amount. Regarding durability, while the maximum number of write cycles 
is equivalent to that of an Arduino (100,000 cycles), the memory can only be 
rewritten in 4,096-byte blocks; consequently, modifying even a small portion 
consumes the lifespan of the entire 4,096-byte block. This may not pose a serious 
issue for storing data that is rarely updated, such as IP addresses or 
configuration settings. However, it is significantly disadvantageous for 
applications requiring periodic writes, such as data logging. The memory 
will reach its limit far sooner than on an Arduino, which (presumably) 
allows for byte-level rewriting. If your program requires frequent EEPROM 
writes, it is recommended to design the system to use an external device 
capable of non-volatile data storage.

(1)コンパイル環境の構築(Arduino IDE2.3.10で確認済み)
(a)ESP32用ボードライブラリのインストール
メニューの [ファイル] ＞ [環境設定] を開きます。「追加のボードマネージャのURL」
に次のURLを入力して [OK] を押します。https://githubusercontent.com
(b)ESP32ボードのインストール
メニューの [ツール] ＞ [ボード] ＞ [ボードマネージャー] を開きます。検索窓に 
esp32 と入力します。Espressif Systems の 「esp32」 をインストールします。
(c)パソコンとESP32-C3ボードをUSBケーブルで接続します。
メニューの [ツール] ＞ [ボード] ＞ [ESP32 Arduino] から使用しているボード
（例：ESP32C3 Dev Module や XIAO_ESP32C3 など）を選択します。接続した 
[シリアルポート] を選択します

以上の処理で最低限このファイルがコンパイル可能な環境が整います。(c)の操作は同
じ型番でもボードを交換するたびに実行してください。

(d)IDE画面左上の→マークをクリックするとコンパイルとプログラムの転送が始まります。
(e)転送が終わると自動的にボードがリセットされプログラムが実行されます。

(2)セーフモードとWiFiネットワークの利用について
(a)セーフモード(最初にDHCP接続するときのモード)はGPIO 3の端子の接続で判定します。
XIAO ESP32C3では基板上のD1がGPIO 3です（基板上のD3はGPIO 5なので注意）。
D1をGND(0V)に接続するとセーフモードになります。開放するとノーマルモード
(Web画面で設定したIPアドレスで接続する)になります。
(b)ESP32C3のネットワークはWiFiが標準装備されていて、それを使います。
このため、setup()内のsetWiFiCredentials("SSID", "Password");に、
接続可能なWiFiネットワークのSSIDとPasswordを記載してください。
セーフモードでは、この接続情報でDHCP(自動アドレス設定)で接続されます。
(c)ノーマルモードでの接続は、Web画面のNetwork Configで設定した、SSID、
IPアドレス等の情報になります。セーフモードでこれらの情報を正しく設定して
から、GPIO 3(XIAOではD1)端子を開放してください。
(d)内蔵されているWiFi通信チップにあらかじめMAC(物理)アドレスが設定されて
いますので、以前のUARDECSで必須だった設定は不要になりました。MAC3アドレス
を知りたい場合は、WiFi.macAddress()で返されます。

Arduino用のUARDECS、UARDECS_MEGA、UARDECS_PICOとソースコードレベルで互換
性があります。ヘッダファイル(ライブラリ)の変更のみで大抵はコンパイルをそ
のまま通ります。ヘッダファイルには"#include <Uardecs_esp32c3.h>"の記述の
みで必要で、Ethernet関連のライブラリの記述も不要です。

※ただし、ハードウェアに依存する部分は変更が必要です。
本ライブラリが動作対象として想定しているESP32C3では、セーフモード判定に
UARDECSが使用するGPIO3(D1)以外にも、以下のピンが最初から専有されています。
***ESP32C3+0.42inchOLED
・GPIO2：起動モードを設定するストラッピングピンの1つです。電源が入る瞬間
にこのピンの電位が変化するような回路（強力なプルダウンなど）を接続してい
ると、正常に起動しなくなる場合があります。起動後は通常のGPIOとして使用可
能です。
・GPIO5：OLEDのSDA（データ線）に接続
・GPIO6：OLEDのSCL（クロック線）に接続
・GPIO8：オンボードの青色LEDに接続
・GPIO9：BOO0（ブート）ボタンに接続※起動モードを決定する重要なピンのため、
外部回路で常にGNDへ落とすような使い方をするとプログラムの書き込みや起動が
できなくなります。
・GPIO12 ～ GPIO17：内蔵SPIフラッシュメモリ専用プログラムを読み出すため
に常に高速通信を行っているため、汎用GPIOとしては一切使用できません。
・GPIO18, GPIO19：USB-JTAG / USBシリアル変換機能Type-Cポートからのプロ
グラム書き込みやシリアルモニター通信で使用されています。プログラム上で設定
を無効化すればGPIOとして使えないこともないですが、PCとの通信ができなくな
るリスクがあるため避けるのが無難です。
***XIAO ESP32C3
・D2（GPIO2）理由: こちらも起動モードに関係するストラッピングピンです。プル
アップやプルダウンが内部でされていないため、接続する機器によっては起動失敗の
原因になります。
・D3（GPIO5 / A3など）: 一部の環境（Arduino IDEの古い定義など）でアナログ入
力（A3）として正常に認識・定義されていないケースや、Wi-Fi/BLE通信時にアナログ
値の読み取りに影響を受ける場合があります。またJTAG信号線（MTDIなど）と兼用です。
・D4,D5（GPIO6,7）はI2C(Wire)のSDAとSCLに使用するので、別用途には使用しない方が
無難です。
・D6は起動時にUART出力に設定されるので、もしD6を入力として使うと不用意に大電流が
流れてしまう危険性があります。出力で使うようにしましょう。
・D8（GPIO8）: 起動モード（Boot Mode）を決定するストラッピングピン（Strapping 
Pin）の一つです。起動時にLOWかつGPIO9もLOWになると予期せぬ挙動や特殊モード
（UART_BOOT）に入るため、外部回路を接続する際は起動時の電位（HIGHに維持する等）
に注意が必要です。
・D9（GPIO9）: オンボードの BOOTボタン とプルアップ抵抗に接続されています。
スイッチ入力としては最適ですが、起動時にGNDに落ちるような回路や、強い外部負
荷を接続すると正常に起動（ダウンロードモード）できなくなります。

また、電圧が3.3Vであること、ピンに流せる電流は、GPIOピン1本あたりに流せる電流は、
公式データシートによると、ドライブ能力を最大（PAD_DRIVER = 3）に設定した場合、
ソース電流（出力・吐き出し）が最大40mA、シンク電流（入力・吸い込み）が最大28mAです。

(3)EEPROMについて
本ライブラリはArduinoに搭載されていたEEPROMの代用としてESP32C3のプログラムフラッシュの
未使用領域4096byteを保存領域に使用します。また、SRAMを4096byte分キャッシュ領域として使
用する(起動後に確保されるので注意)ので、この分だけ利用可能なメモリが減少します。

EEPROMの耐久性は書き換え回数のみはArduinoと同等（100,000回)ですが、4096byte単位でしか
書き換えができないため、一部書き換えただけでも4096byteの領域全体の耐久性を損耗します。
これらは書き換え頻度が少ないIPアドレスの保存やその他設定データの保存には深刻な影響はな
いかもしれません。しかし、データのロギングなど一定時間間隔で書き込むような用途には著し
く不利になります。(おそらく)byte単位で書き換えが可能なArduinoより遥かに少ない回数で限界
に達します。プログラム中でEEPROMを高頻度に書き換える場合、別途不揮発性のデータが保存可
能なデバイスを接続する設計にすることを推奨します。
*/