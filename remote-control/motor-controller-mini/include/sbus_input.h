#pragma once

#include <type_traits>
#include <sbus.h>

#include <driver_limits.h>
#include <driver_config.h>

#define INPUT_SBUS_MIN  175 //200
#define INPUT_SBUS_MAX  1809

// extern "C++"
// {
//     template <typename _CountofType, size_t _SizeOfArray>
//     char (*__countof_helper(UNALIGNED _CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
//     #define _countof(_Array) sizeof(*__countof_helper(_Array))
// }

// template< typename T, size_t N >
// constexpr size_t countof( const T (&)[N] ) { return N; }

// #define COUNT(a) (__builtin_choose_expr( \
//                   __builtin_types_compatible_p(typeof(a), typeof(&(a)[0])), \
//                   (void)0, \
//                   (sizeof(a)/sizeof((a)[0]))))

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
        outputs[i] = sbus_data.ch[i]; // MAP?
    }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    return sbus_data.NUM_CH;
}