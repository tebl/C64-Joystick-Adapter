void init_led();

bool is_pwr_on();
void set_pwr(int value);
void set_pwr(bool value);
void fade_pwr();
void fade_pwr(const int minimum);
void boost_pwr();
void flash_pwr(int num_flashes);