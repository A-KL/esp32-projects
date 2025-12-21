// ------------------------------------------
// #include "esp32_pcnt.h"

// volatile bool pc0_int_flag = false; // counter 0 interrupt flag
// volatile bool pc1_int_flag = false; // counter 1 interrupt flag
// volatile bool clock_int_flag = false; // clock signal interrupt flag

// //create counter object
// PulseCounter pc0,pc1;
// //mutex for hardware pulse counters
// portMUX_TYPE pcntMux0 = portMUX_INITIALIZER_UNLOCKED;
// portMUX_TYPE pcntMux1 = portMUX_INITIALIZER_UNLOCKED;

// int16_t threshold_0 = 1000;
// int16_t threshold_1 = 2000;
// int16_t high_limit = 3000;

// #define clock_duration 500 // one half the signal output duration in millisecs
// ------------------------------------------
#include "FreqCountESP.h"
// #include "freq_counter_t.h"

// #include "AutoAudioInfo.h"
// using namespace audio_tools;
// AudioInfo info(44100, 2, 16);
// AutoAudioInfo auto_info(info);

#define LCK 6
#define BCK 5

static int inputPin = 6;
static int timerMs = 200;
static int mul = 1000 / timerMs;

// _FreqCountESP FreqCountESP1;
// _FreqCountESP FreqCountESP2;

  // //pulse counter ISR
  // IRAM_ATTR void pc0_isr(void *) {
  //   // Prevent context switching during the interrupt service routine with an ISR spinlock
  //   portENTER_CRITICAL_ISR(&pcntMux0); 
  //     // set interrupt flag
  //     pc0_int_flag = true;
  //     // get events
  //   portEXIT_CRITICAL_ISR(&pcntMux0);
  // }
  //pulse counter ISR
  // IRAM_ATTR void pc1_isr(void *) {
  // // Prevent context switching during the interrupt service routine with an ISR spinlock
  // portENTER_CRITICAL_ISR(&pcntMux1);
  //   // set interrupt flag
  //   pc1_int_flag = true;
  //   // get events
  // portEXIT_CRITICAL_ISR(&pcntMux1);
//}

void setup()
{
  Serial.begin(115200);

  pinMode(LCK, INPUT);
  pinMode(BCK, INPUT);

  // pcnt_isr_service_install(0);
  // // setup hardware pulse counters
  // // initialise counter unit 0, channel 0 with signal input GPIO pin and control signal input pin (0 = no control signal input)
  // pc0.initialise(LCK, PCNT_PIN_NOT_USED);
  // pc1.initialise(BCK, PCNT_PIN_NOT_USED);
  // // count up on negative edges, don't count on positive edges 
  // pc0.set_mode(PCNT_COUNT_DIS,PCNT_COUNT_INC,PCNT_MODE_KEEP,PCNT_MODE_KEEP);
  // pc1.set_mode(PCNT_COUNT_DIS,PCNT_COUNT_INC,PCNT_MODE_KEEP,PCNT_MODE_KEEP);
  // // set glich filter to ignore pulses less than 1000 x 2.5ns
  // pc0.set_filter_value(1000);
  // pc1.set_filter_value(1000); 
  // // set and enable threshold 0 and 1 watch points - these will trigger an interrupt.
  // // the event can be disabled and enabled using pc0.event_disable(PCNT_EVT_THRES_0) or pc0.event_enable(PCNT_EVT_THRES_0)
  // // thereshold 0 & 1 events do not stop the counter
  // // pc0.set_event_value(PCNT_EVT_THRES_0,threshold_0);
  // // pc1.set_event_value(PCNT_EVT_THRES_0,threshold_0);

  // // pc0.set_event_value(PCNT_EVT_THRES_1,threshold_1);
  // // pc1.set_event_value(PCNT_EVT_THRES_1,threshold_1);

  // pc0.set_event_value(PCNT_EVT_H_LIM, high_limit);
  // pc1.set_event_value(PCNT_EVT_H_LIM, high_limit);

  // // register interrupt service routine for all counter units, and pass an optional argument (NULL in this case)
  // // pc0.isr_register(pcnt_intr_handler, NULL);
  // pc0.attach_interrupt(pc0_isr);
  // pc0.interrupt_enable(); //

  // pc1.attach_interrupt(pc1_isr);
  // pc1.interrupt_enable(); //

  // // clear and restart the counter
  // pc0.clear();
  // pc0.resume();
  // pc1.clear();
  // pc1.resume();

  FreqCountESP.addPin(LCK);
  FreqCountESP.addPin(BCK);

  FreqCountESP.begin(timerMs);

  Serial.println("Start");
}

uint32_t last_freq = 1;
uint32_t last_bits = 1;

void loop()
{
  // if (pc0_int_flag) {
  //   pc0_int_flag = false; //reset flag
  //   Serial.print("pc0 : ");
  //   switch (pc0.event_status()) // test event type
  //   {
  //     case evt_high_lim:
  //       Serial.println("High Limit");;
  //       break;
  //   }
  // }

  // if (pc1_int_flag) {
  //   pc1_int_flag = false;
  //   Serial.print("pc1 : ");
  //   switch (pc1.event_status())
  //   {
  //     case evt_high_lim:
  //       Serial.println("High Limit");;
  //       break;
  //   }
  // }

  if (FreqCountESP.available())
  {
    last_freq = FreqCountESP.read();
   // Serial.print("Pin: ");
   // Serial.print(FreqCountESP.pin());
   // Serial.print(" Freq: ");
   // Serial.println(last_freq * mul);
  }

  if (FreqCountESP.available(1))
  {
    uint32_t frequency = FreqCountESP.read(1);
    // Serial.print("Pin: ");
    // Serial.print(FreqCountESP.pin(1));
    // Serial.print(" Freq: ");
    // Serial.println(frequency * mul);

    Serial.printf("Freq: %u Hz. Sample size: %u\r\n", last_freq * mul, (frequency / last_freq / 2));
  }
}