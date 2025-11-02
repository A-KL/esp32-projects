#pragma once

#include "lgfx/v1/panel/Panel_LCD.hpp"

namespace lgfx
{
 inline namespace v1
 {
  //----------------------------------------------------------------------------
  struct Panel_ILI9341_IPS : public Panel_ILI9341_Base
  {
    protected:

      static constexpr uint8_t CMD_FRMCTR1 = 0xB1;
      static constexpr uint8_t CMD_FRMCTR2 = 0xB2;
      static constexpr uint8_t CMD_FRMCTR3 = 0xB3;
      static constexpr uint8_t CMD_INVCTR  = 0xB4;
      static constexpr uint8_t CMD_DFUNCTR = 0xB6;
      static constexpr uint8_t CMD_PWCTR1  = 0xC0;
      static constexpr uint8_t CMD_PWCTR2  = 0xC1;
      static constexpr uint8_t CMD_PWCTR3  = 0xC2;
      static constexpr uint8_t CMD_PWCTR4  = 0xC3;
      static constexpr uint8_t CMD_PWCTR5  = 0xC4;
      static constexpr uint8_t CMD_VMCTR1  = 0xC5;
      static constexpr uint8_t CMD_VMCTR2  = 0xC7;
      static constexpr uint8_t CMD_GMCTRP1 = 0xE0; // Positive Gamma Correction (E0h)
      static constexpr uint8_t CMD_GMCTRN1 = 0xE1; // Negative Gamma Correction (E1h)
      static constexpr uint8_t CMD_RDINDEX = 0xD9; // ili9341
      static constexpr uint8_t CMD_IDXRD   = 0xDD; // ILI9341 only, indexed control register read

      // #define ILI9341_MADCTL 0x36   ///< Memory Access Control
      // #define ILI9341_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
      // #define ILI9341_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set
      // #define ILI9341_DFUNCTR 0xB6 ///< Display Function Control
      // #define ILI9341_SLPOUT 0x11 ///< Sleep Out
      // #define ILI9341_PTLON 0x12  ///< Partial Mode ON
      // #define ILI9341_DISPON 0x29   ///< Display ON

      const uint8_t* getInitCommands(uint8_t listno) const override
      {
        static constexpr uint8_t list0[] =
        {
            0xEF       , 3, 0x03,0x80,0x02,
            0xCF       , 3, 0x00,0xC1,0x30,
            0xED       , 4, 0x64,0x03,0x12,0x81,
            0xE8       , 3, 0x85,0x00,0x78,
            0xCB       , 5, 0x39,0x2C,0x00,0x34,0x02,
            0xF7       , 1, 0x20,
            0xEA       , 2, 0x00,0x00,
            CMD_PWCTR1,  1, 0x23,
            CMD_PWCTR2,  1, 0x10,
            CMD_VMCTR1,  2, 0x3e,0x28,
            CMD_VMCTR2,  1, 0x86,
            // added
            // ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
            // ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
            // ILI9341_PIXFMT  , 1, 0x55,
            // added
            CMD_FRMCTR1, 2, 0x00, 0x18, //0x13,
            // 
            //ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
            // 
            0xF2       , 1, 0x00,

            CMD_GAMMASET,1, 0x01,  // Gamma set, curve 1
            CMD_GMCTRP1,15, 0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00,
            CMD_GMCTRN1,15, 0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F,
            CMD_DFUNCTR, 3, 0x08,0xC2,0x27,
            CMD_SLPOUT , 0+CMD_INIT_DELAY, 120,    // Exit sleep mode
            CMD_IDMOFF , 0,
            CMD_DISPON , 0+CMD_INIT_DELAY, 100,
            0xFF,0xFF, // end
        };
        switch (listno)
        {
        case 0: return list0;
        default: return nullptr;
        }
      }
    };
  }
  //----------------------------------------------------------------------------
}