#ifndef alpaca_h
#define alpaca_h

void alpaca_get_account(char* response, size_t response_size);
void alpaca_get_latest_bars(const char* ticker, char* response, size_t response_size);
void alpaca_post_order(const char* order_json, char* response, size_t response_size);
void alpaca_create_order_json(const char* symbol, int qty, const char* side, const char* type, const char* time_in_force, char* order_json, size_t order_json_size);

#endif