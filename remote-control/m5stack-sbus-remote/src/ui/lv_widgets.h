#pragma once

lv_obj_t* lv_ui_create_panel(lv_obj_t *parent, uint32_t color, uint32_t bg_color = 0x000000, int16_t w = 100, int16_t h = 155, int16_t border = 3);
lv_obj_t *lv_ui_create_panel_title(lv_obj_t *parent, const char* title, uint32_t color);

lv_obj_t* lv_ui_create_panel_label_value(lv_obj_t *parent, const char *text, int w_per = 65);
lv_obj_t* lv_ui_create_panel_label(lv_obj_t *parent, const char *text, int w_per = 35);

static const char* ch_labels[] = { "ch0:", "ch1:", "ch2:", "ch3:", "ch4:", "ch5:", "ch6:", "ch7:", "ch8:" };
static const char* ch_no_value = "---";

template <std::size_t TSize>
class WidgetPanel
{
    public:
        void init(lv_obj_t* parent, const char* title, uint32_t color, uint32_t bg_color, uint16_t height = 155)
        {
            _panel = lv_ui_create_panel(parent, color, bg_color, 100, height);
            _panel_title = lv_ui_create_panel_title(_panel, title, color);

            for (auto i=0; i<TSize; i++) {
                _labels[i] = lv_ui_create_panel_label(_panel, ch_labels[i]);
                _values[i] = lv_ui_create_panel_label_value(_panel, ch_no_value);
            }
        }

        void setLabels(const char** labels) {
            for (auto i=0; i<TSize; i++) {
                lv_label_set_text(_labels[i], labels[i]);
            }
        }

         template<typename... Args>
         void setText(int index, const char* format, Args... args) {
            // char buff[15];
            // snprintf(buff, sizeof(buff), format, args...);
            // _list[index] = buff;
         }

         void setValue(int index, int32_t value) {
            if (_values[index]) {
                auto str = std::to_string(value);
                //std::to_chars(str.data(), str.data() + str.size(), value);
                lv_label_set_text(_values[index], str.c_str());
            }
         }

        void clearValue(int index) {
            if (_values[index]) {
                lv_label_set_text(_values[index], ch_no_value);
            }
         }

        void clearValues() {
            for (auto value :_values)
            if (value) {
                lv_label_set_text(value, ch_no_value);
            }
         }

        ~WidgetPanel()
         { }

    private:
         //std::array<char, 10> str;
        //  const char* _title;
        //  const uint32_t _color;
        //  const uint32_t _bg_color

         //lv_obj_t* _parent;
         lv_obj_t* _panel;
         lv_obj_t* _panel_title;

         lv_obj_t* _labels[TSize];
         lv_obj_t* _values[TSize];
};
