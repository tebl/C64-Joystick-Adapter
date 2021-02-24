bool init_mode(byte mode);
void handle_mode();
void debounce_joystick_key(const int port_id, const byte key_id, const bool invert = false);
bool is_waiting_release(const int port_id, const byte key_id);
void clear_key(const int port_id, const byte key_id);
bool is_key_active(const word gamepad_state, const word key_mask);