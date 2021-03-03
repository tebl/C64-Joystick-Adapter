bool init_mode(byte mode);
void handle_mode();

void set_linked_led(const bool value);
void set_state(const int port_id, const byte key_id, byte cycle, const int period);
void flip_state(const int port_id, const byte key_id, const uint16_t key_code, const int period_on, const int period_off);
void clear_state(const int port_id, const byte key_id);

void debounce_joystick_key(const int port_id, const byte key_id, const bool invert = false);
bool is_waiting_release(const int port_id, const byte key_id);
void clear_key(const int port_id, const byte key_id);
bool is_key_active(const word gamepad_state, const word key_mask);