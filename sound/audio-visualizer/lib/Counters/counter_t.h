#pragma once

class counter_t {
  public:
    counter_t(int from, int to, int step=1) 
      : _from(from), _to(to), _step(step), _state(to) {}

    int next() {
      auto d = _state + _step;
      if (d <_to) {
        _state = d;
      }
      return _state;
    }

    int prev() {
      auto d = _state - _step;
      if (d > _from) {
        _state = d;
      }
      return _state;
    }

    inline int value() const {
      return _state;
    }

  private:
    const int _from;
    const int _to;
    const int _step;
          int _state;
};

template<typename T>
class state_counter_t : public counter_t {
  public:
    state_counter_t(const T[] &states) : counter_t(0, sizeof(states)/sizeof(T), 1), _states(states)
    { }

    const T* state() const {
      return _states[value()];
    }
    
  private:
    const T* _states;
};