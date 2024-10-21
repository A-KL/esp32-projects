/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <cstdlib> // rnd
#include "lvgl.h"
#include "app_hal.h"

#include "ui/ui.h"

static int ui_update_thread(void * data)
{
    (void)data;

    while(1) {
        delay(1000);
        ui_set_altitude(rand()%1000 + 2000);
    }

    return 0;
}

int main(void)
{
	lv_init();

	hal_setup();

  ui_init();

  //updates(ui_update_thread);

  hal_loop();

  // /* Change the active screen's background color */
  // lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

  // auto width = LV_HOR_RES;// lv_display_get_horizontal_resolution(NULL);
  // auto height = LV_VER_RES; // lv_display_get_vertical_resolution(NULL);
  // auto bottom_h = 50;

  // /* Draw title bar */
  // // static lv_style_t style_title;
  // // lv_style_init(&style_title);
  // // lv_style_set_radius(&style_title, 0);
  // // lv_style_set_border_width(&style_title, 0);

  // /* Make a gradient */
  // // lv_style_set_bg_opa(&style_title, LV_OPA_COVER);
  // // static lv_grad_dsc_t grad;
  // // grad.dir = LV_GRAD_DIR_VER;
  // // grad.stops_count = 2;
  // // grad.stops[0].color = lv_color_hex(0xffffff);
  // // grad.stops[1].color = lv_palette_main(LV_PALETTE_GREY);
  // // grad.stops[0].frac  = 128;
  // // grad.stops[1].frac  = 250;
  // // lv_style_set_bg_grad(&style_title, &grad);

  // // lv_obj_t * title = lv_obj_create(lv_scr_act());

  // // lv_obj_add_style(title, &style_title, 0);
  // // lv_obj_set_content_width(title, lv_obj_get_width(lv_scr_act()));
  // // lv_obj_set_content_height(title, 8);
  // // lv_obj_set_align(title, LV_ALIGN_TOP_LEFT);

  // /*Create a white label, set its text and align it to the center*/
  // static lv_style_t style;
  // lv_style_init(&style);
  // lv_style_set_text_font(&style, &lv_font_montserrat_44);

  // // Current background
  // lv_obj_t * altitude_bgn = lv_obj_create(lv_scr_act());
  // lv_obj_set_size(altitude_bgn , width / 2, height-bottom_h);
  // lv_obj_set_pos(altitude_bgn , 0, 0);
  // lv_obj_set_style_bg_color(altitude_bgn , lv_palette_lighten(LV_PALETTE_GREEN, 3), LV_PART_MAIN);

  // // max background
  // lv_obj_t * altitude_max_bgn = lv_obj_create(lv_scr_act());
  // lv_obj_set_size(altitude_max_bgn , width / 2, bottom_h);
  // lv_obj_set_pos(altitude_max_bgn, 0, height - bottom_h);
  // lv_obj_set_style_bg_color(altitude_max_bgn , lv_palette_lighten(LV_PALETTE_CYAN, 3), LV_PART_MAIN);

  // // Current
  // lv_obj_t * altitude_label = lv_label_create(altitude_bgn);
  // lv_obj_add_style(altitude_label, &style, 0);
  // lv_label_set_text(altitude_label, "3000m");

  // lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);
  // lv_obj_align(altitude_label, LV_ALIGN_CENTER, 0, 0);

  // // Max
  // lv_obj_t * altitude_max_label = lv_label_create(altitude_max_bgn);
  // lv_obj_add_style(altitude_max_label, &style, 0);
  // lv_label_set_text(altitude_max_label, "3200m");

  // lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);
  // lv_obj_align(altitude_max_label, LV_ALIGN_CENTER, 0, 0);

  /* Progress Bar */
  // static lv_style_t style_bg;
  // static lv_style_t style_indic;

  // lv_style_init(&style_bg);
  // lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_BLUE));
  // lv_style_set_border_width(&style_bg, 2);
  // lv_style_set_pad_all(&style_bg, 6); /*To make the indicator smaller*/
  // lv_style_set_radius(&style_bg, 6);
  // lv_style_set_anim_time(&style_bg, 1000);

  // lv_style_init(&style_indic);
  // lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
  // lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
  // lv_style_set_radius(&style_indic, 3);

  // lv_obj_t * bar = lv_bar_create(lv_scr_act());
  // lv_obj_remove_style_all(bar);  /*To have a clean start*/
  // lv_obj_add_style(bar, &style_bg, 0);
  // lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);

  // lv_obj_set_size(bar, 200, 20);
  // lv_obj_center(bar);
  // lv_bar_set_value(bar, 100, LV_ANIM_ON);

	//hal_loop();
}
