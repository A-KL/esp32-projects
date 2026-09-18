#define LGFX_USE_V1

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device{
  lgfx::Panel_ST7789  _panel_instance;
  lgfx::Bus_SPI       _bus_instance;
  lgfx::Light_PWM     _light_instance;

  public:LGFX(void){
    { // バス制御の設定を行います。
      auto cfg = _bus_instance.config();// バス設定用の構造体を取得します。
      
      // VSPI_HOST
      //cfg.spi_host = VSPI_HOST;          // Select SPI to use ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // * Due to the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST descriptions are deprecated,
      // so if an error occurs, use SPI2_HOST and SPI3_HOST instead.
      cfg.spi_host   = VSPI_HOST;  // 使用するSPIを選択 (VSPI_HOST or HSPI_HOST)
      cfg.spi_mode   = 0;          // SPI通信モードを設定 (0 ~ 3)
      cfg.freq_write = 40000000;   // 送信時のSPIクロック(最大80MHz,80MHzを整数割値に丸め)
      cfg.freq_read  = 16000000;   // 受信時のSPIクロック
      cfg.spi_3wire  = false;      // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock   = true;       // トランザクションロックを使用する場合はtrueを設定
      cfg.dma_channel=  1;         // 使用DMAチャンネル設定(1or2,0=disable)(0=DMA不使用)

      // SDカードと共通のSPIバスを使う場合、MISOは省略せず必ず設定してください。
      _bus_instance.config(cfg);   // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance);// バスをパネルにセットします。
    }
    { // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config();// 表示パネル設定用の構造体を取得します。
      cfg.pin_cs          =     5; // CS  が接続されているピン番号(-1 = disable)
      cfg.pin_rst         =     4; // RST が接続されているピン番号(-1 = disable)
      cfg.pin_busy        =    -1; // BUSYが接続されているピン番号(-1 = disable)
      cfg.memory_width    =   240; // ドライバICがサポートしている最大の幅
      cfg.memory_height   =   320; // ドライバICがサポートしている最大の高さ
      cfg.offset_x        =     0; // パネルのX方向オフセット量
      cfg.offset_y        =     0; // パネルのY方向オフセット量
      cfg.offset_rotation =     0; // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel=     8; // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits =     1; // ピクセル外のデータ読出し前のダミーリードのビット数
      cfg.readable        =  true; // データ読出しが可能な場合 trueに設定
      cfg.invert          =  true; // パネルの明暗が反転場合 trueに設定
      cfg.rgb_order       = false; // パネルの赤と青が入れ替わる場合 trueに設定 ok
      cfg.dlen_16bit      = false; // データ長16bit単位で送信するパネル trueに設定
      cfg.bus_shared      = false; // SDカードとバスを共有 trueに設定
      _panel_instance.config(cfg);
    }
    { // バックライト制御の設定を行います。（必要なければ削除）
      auto cfg = _light_instance.config();    // バックライト設定用の構造体を取得します。

      cfg.pin_bl = 32;              // バックライトが接続されているピン番号
      cfg.invert = false;           // バックライトの輝度を反転させる場合 true
      cfg.freq   = 44100;           // バックライトのPWM周波数
      cfg.pwm_channel = 7;          // 使用するPWMのチャンネル番号

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
    }
    setPanel(&_panel_instance);// 使用するパネルをセットします。
  }
};
LGFX tft; // 準備したクラスのインスタンスを作成します。