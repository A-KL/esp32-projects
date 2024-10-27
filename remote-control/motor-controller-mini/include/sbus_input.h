#pragma once

#include <sbus.h>

#include <driver_limits.h>
#include <driver_config.h>

#define INPUT_SBUS_DEBUG

#define INPUT_SBUS_MIN  180 //200
#define INPUT_SBUS_MAX  1800

bfs::SbusRx sbus_rx(sbus_serial, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusTx sbus_tx(sbus_serial, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);

bfs::SbusData sbus_data;

inline void sbus_init() {
    sbus_rx.Begin();
    sbus_tx.Begin();
    log_i("SBUS initialization...OK");
}

// Output

void sbus_write(const bfs::SbusData &data) {
    sbus_tx.data(data);
    sbus_tx.Write();
} 

template<short TMin, short TMax>
void sbus_write(const int16_t* data, const size_t count) {
  for (auto i=0; i<count; i++) {
      sbus_data.ch[i] = map(data[i], TMin, TMax, INPUT_SBUS_MIN, INPUT_SBUS_MAX);     
  }
  sbus_write(sbus_data);
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
        outputs[i] = sbus_data.ch[i]; // MAP?
    }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    return sbus_data.NUM_CH;
}