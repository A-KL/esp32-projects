// #pragma once

// #include <LovyanGFX.hpp>

// class LGFX_TTGO_HMI_ST7789 : public lgfx::LGFX_Device
// {
//     lgfx::Panel_ST7789   _panel_instance;
//     lgfx::Bus_Parallel8  _bus_instance;

//     lgfx::Light_PWM      _light_instance; //??

//     lgfx::Touch_XPT2046  _touch_instance;

// public:
//     LGFX_TTGO_HMI_ST7789()
//     {
//         {
//             auto cfg = _bus_instance.config();

//             cfg.pin_d0 = 48;
//             cfg.pin_d1 = 47;
//             cfg.pin_d2 = 39;
//             cfg.pin_d3 = 40;
//             cfg.pin_d4 = 41;
//             cfg.pin_d5 = 42;
//             cfg.pin_d6 = 45;
//             cfg.pin_d7 = 46;

//             cfg.pin_rd = -1;
//             cfg.pin_wr = 8;
//             cfg.pin_rs = 7;

// 	// -D TFT_CS=6
// 	// -D TFT_DC=7

//             _bus_instance.config(cfg);
//             _panel_instance.setBus(&_bus_instance);
        
//         }
//         {
//             auto cfg = _panel_instance.config();

//             cfg.pin_cs           =     6;
//             cfg.pin_rst          =    -1;
//             cfg.pin_busy         =    -1;

//             cfg.panel_width      =   240;  // 実際に表示可能な幅
//             cfg.panel_height     =   320;  // 実際に表示可能な高さ
//             cfg.offset_x         =     0;  // パネルのX方向オフセット量
//             cfg.offset_y         =     0;  // パネルのY方向オフセット量
//             cfg.offset_rotation  =     0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
//             cfg.dummy_read_pixel =     8;  // ピクセル読出し前のダミーリードのビット数
//             cfg.dummy_read_bits  =     1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
//             cfg.readable         =  true;  // データ読出しが可能な場合 trueに設定
//             cfg.invert           = false;  // パネルの明暗が反転してしまう場合 trueに設定
//             cfg.rgb_order        = false;  // パネルの赤と青が入れ替わってしまう場合 trueに設定
//             cfg.dlen_16bit       = false;  // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
//             cfg.bus_shared       =  true;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

//              _panel_instance.config(cfg);
//         }
//         {
//             auto cfg = _light_instance.config();    // バックライト設定用の構造体を取得します。

//             cfg.pin_bl = 38;              // バックライトが接続されているピン番号
//             cfg.invert = false;           // バックライトの輝度を反転させる場合 true
//             cfg.freq   = 44100;           // バックライトのPWM周波数
//             cfg.pwm_channel = 7;          // 使用するPWMのチャンネル番号

//             _light_instance.config(cfg);
//             _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
//         }
//         {
// 	        // -D TOUCHSCREEN_IRQ_PIN=9

//             auto cfg = _touch_instance.config();

//             cfg.x_min      = 0;    // タッチスクリーンから得られる最小のX値(生の値)
//             cfg.x_max      = 239;  // タッチスクリーンから得られる最大のX値(生の値)
//             cfg.y_min      = 0;    // タッチスクリーンから得られる最小のY値(生の値)
//             cfg.y_max      = 319;  // タッチスクリーンから得られる最大のY値(生の値)
//             cfg.pin_int    = 38;   // INTが接続されているピン番号
//             cfg.bus_shared = true; // 画面と共通のバスを使用している場合 trueを設定
//             cfg.offset_rotation = 0;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

//             //cfg.spi_host = VSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
//             cfg.freq = 1000000;     // SPIクロックを設定
//             cfg.pin_sclk = 1;     // SCLKが接続されているピン番号
//             cfg.pin_mosi = 3;     // MOSIが接続されているピン番号
//             cfg.pin_miso = 4;     // MISOが接続されているピン番号
//             cfg.pin_cs   = 2;     //   CSが接続されているピン番号

//             _touch_instance.config(cfg);
//             _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
//         }

//         setPanel(&_panel_instance);
//     }
// };