#ifndef alpaca_h
#define alpaca_h

void alpaca_get_account(char* response, size_t response_size);
void alpaca_get_latest_bars(const char* ticker, char* response, size_t response_size);
void alpaca_post_order(const char* order_json, char* response, size_t response_size);

#endif