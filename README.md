UARDECS_ESP32C3 Library

A library that provides communication capabilities compliant with the "Ubiquitous Environment Control System (UECS)" standard, which is widely used in protected horticulture in Japan.

This library facilitates UECS sweet application development on the ESP32C3 chip board by providing UDP packet broadcast communication via Wi-Fi and internet web server functionality, all compliant with the UECS communication protocol. It supports the ESP32C3-OLED, XIAO ESP32C3, and compatible boards. Please refer to LICENSE.md for licensing information. 

I. Differences between the ESP32C3 version and the previous version of the UARDECS library
(1) The connection method has changed from Ethernet to Wi-Fi, resulting in associated specification changes. Issues have been reported where broadcast CCMs fail to reach other nodes due to Wi-Fi-specific security settings (such as Wi-Fi client isolation). Please review security settings on devices like Wi-Fi bridges and verify packet delivery using tools such as a network analyzer. 
(2) The behavior of provider responses to the "Provider Search CCM" (which uses UDP port 16521) has changed regarding the TYPE attribute. Previously, only providers with an exact match for the search key in the TYPE attribute would respond. In the ESP32C3 version, providers with a partial match also respond. This makes it easier to search for providers that share the same CCM identifier naming convention item names or node type names. However, since applications using this protocol will receive response CCMs from a larger number of providers than before, please be mindful of potential buffer overflows.

II. Points to note when using the ESP32C3 board with UARDECS:
(1) GPIO 3 is designated as the signal pin for entering safe mode. For the XIAO ESP32C3 board, this corresponds to pin D1. Connecting this pin to GND (0V) activates safe mode. The pin state is checked only once during the ARDECS setup process; changing the state afterward has no effect.
(2) On the ESP32, variables declared with `const` are automatically allocated to flash memory, so using the `PROGMEM=` assignment operator is no longer necessary.
(3) The ESP32C3 comes with a pre-configured MAC address, so manual definition is not required. However, since the device connects via Wi-Fi, you must configure the SSID and password for safe mode. To conserve memory, the character limit is set to 25 characters (values ​​exceeding 25 characters cannot be configured). Additionally, safe mode utilizes DHCP (automatic addressing) to assign the network address automatically.

UARDECS_ESP32C3 ライブラリ

　日本の施設園芸分野で広く利用されている「ユビキタス環境制御システム（UECS）」規格に準拠した通信機能を提供するライブラリです。
　本ライブラリは、UECS通信プロトコルに準拠したWi-Fi経由のUDPパケットブロードキャスト通信やWebサーバー機能を提供することで、ESP32C3搭載ボード上でのUECS対応アプリケーション開発を容易にします。ESP32C3-OLED、XIAO ESP32C3、および互換ボードをサポートしています。ライセンス情報については、LICENSE.mdをご参照ください。

I.従来版のUARDECSとESP32C3版UARDECSとの相違
(1)Ethenet接続からWiFi接続になり、それに伴った仕様変更が実施されています。WiFi特有のセキュリティー設定(WiFi端末分離機能など)でブロードキャストCCMが他のノードで受信できない現象の発生が報告されています。WiFiブリッジ等のセキュリティー設定を見直し、ネットワークアナライザ等で配送確認してください。
(2)UDP16521ポートを使う提供者サーチCCMのTYPE属性に対する、提供者応答の挙動が変わりました。提供者を検索するTYPE属性の検索キーは、これまでは完全一致した提供者だけが応答していました。ESP32C3版では、部分一致した提供者も応答するようにしました。CCM識別子命名規約の項目名やノード種別名が同一の提供者を容易にサーチ可能です。ただ、このプロトコルを使用するアプリケーションでは、従来より多数の提供者からの応答CCMが送信されますので、バッファオーバーフローに注意してください。

II.UARDECSでESP32C3基板を使用するうえでの注意点
(1)セーフモードにする信号ピンはGPIO 3に設定しています。XIAO ESP32C3ボードの場合はD1ピンになります。これをGND(0V)にすることでセーフモードになります。ピンの状態チェックはUARDECSのセットアップ時に一度だけです。以降変えても影響ありません。
(2)ESP32からconst宣言だけでフラッシュメモリに割り当てられるのでPROGMEM=を使う必要はなくなりました。
(3)ESP32C3にはあらかじめmacアドレスが設定されていますので、定義不要になりました。また、WiFi接続になりますのでもセーフモードの時のSSIDとpasswordの設定が必要です。メモリ容量節約のため文字長は25文字までにしています。(25文字以上は設定できません)そして、セーフモードではDHCP(自動アドレス)接続になり、自動でアドレス設定されます。
