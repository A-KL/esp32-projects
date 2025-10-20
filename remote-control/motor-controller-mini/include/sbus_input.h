#pragma once

#include <type_traits>
#include <sbus.h>

#include <driver_config.h>

#define INPUT_SBUS_MIN  175 //200
#define INPUT_SBUS_MAX  1809

bfs::SbusRx sbus_rx(sbus_serial, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusTx sbus_tx(sbus_serial, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusData sbus_data;

inline void sbus_init() 
{
    if (sbus_configured < 2) {
        log_i("SBUS is not configured.");
        return;
    }
    sbus_rx.Begin();
    sbus_tx.Begin();
    log_i("SBUS initialization...OK");
}

// Output

void sbus_write(const bfs::SbusData &data) 
{
    if (sbus_configured < 2) {
        log_i("SBUS is not configured.");
        return;
    }
    sbus_tx.data(data);
    sbus_tx.Write();
} 

template<short TMin, short TMax>
void sbus_write(const int16_t* data, const size_t count) 
{
  static bfs::SbusData sbus_send_data;
  for (auto i=0; i<count; i++) {
      sbus_send_data.ch[i] = map(data[i], TMin, TMax, INPUT_SBUS_MIN, INPUT_SBUS_MAX);     
  }
  sbus_write(sbus_send_data);
} 

// Input

uint8_t sbus_receive(int16_t* outputs)
{
    if (!sbus_rx.Read()) {
        return 0;
    }

    sbus_data = sbus_rx.data();

#ifdef INPUT_SBUS_DEBUG
        log_d("CPU Core: (%d) SBUS: (%d %d %d %d %d %d %d %d)", 
        xPortGetCoreID(),

        sbus_data.ch[0],
        sbus_data.ch[1],
        sbus_data.ch[2],
        sbus_data.ch[3],
        
        sbus_data.ch[4],
        sbus_data.ch[5],
        sbus_data.ch[6],
        sbus_data.ch[7]);
#endif

    for (auto i=0; i<sbus_data.NUM_CH; ++i){
        outputs[i] = sbus_data.ch[i];
    }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    return sbus_data.NUM_CH;
}

// Output

template<int16_t TMin, int16_t TMax>
inline void sbus_write(const int16_t* values, const uint8_t channels) 
{
#ifdef OUTPUT_SBUS_DEBUG 
    log_d_values("[SBUS] ", values, min(channels, sbus_data.NUM_CH));
#endif
    const uint8_t sbus_ch_count = sbus_data.NUM_CH;
    for (auto i = 0; i < min(channels, sbus_ch_count); i++) {
         sbus_data.ch[i] = map(values[i], TMin, TMax, INPUT_SBUS_MIN, INPUT_SBUS_MAX);
    }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();
}