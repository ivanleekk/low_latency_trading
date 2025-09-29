#ifndef alpaca_h
#define alpaca_h

typedef struct Bars {
    char symbol[16];
    double open;
    double high;
    double low;
    double close;
    int64_t trade_count;
    int64_t bar_volume;
    double volume_weighted_average_price;
    char t[32]; // timestamp
} Bars;

void alpaca_get_account(char* response, size_t response_size);
void alpaca_get_latest_bars(const char* ticker, char* response, size_t response_size);
void alpaca_post_order(const char* order_json, char* response, size_t response_size);
void alpaca_create_order_json(const char* symbol, int qty, const char* side, const char* type, const char* time_in_force, char* order_json, size_t order_json_size);
Bars alpaca_decode_bars_json(char* json);

#endif