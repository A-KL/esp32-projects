#pragma once

#include <Arduino.h>

String parse_input(const String& input, String& id, String& property, String& value)
{
  bool id_read = false;
  bool property_read = false;
  bool value_read = false;

  id = "";
  property = "";
  value = "";

  for (auto &c : input) {
    if (c == '|') {
      value_read = true;
      continue;
    } else if (c == '.') {
      id_read = true;
      continue;
    } else if (c == ':') {
      property_read = true;
      continue;
    }

    if (!id_read) {
      id += c;
    } 
    else if (!property_read) {
      property += c;
    }
    else if (!value_read) {
      value += c;
    } else {
      auto d = input.indexOf('|');
      if (d != -1) {
        return input.substring(d + 1);
      }
      break;
    }
  }
  return "";
}