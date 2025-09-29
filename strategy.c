#include <stdio.h>
#include <stdlib.h>
#include "alpaca.h"

int price_strategy(Bars bars) {
    if (bars.volume_weighted_average_price < 257.00) {
        char order_json[256];
        char response[8192] = {0};

        alpaca_create_order_json("AAPL", 1, "buy", "market", "day", order_json, sizeof(order_json));
        alpaca_post_order(order_json, response, sizeof(response));
        printf("Order Response: %s\n", response);
        return 1; // Trade placed
    } else {
        printf("Price condition not met, no order placed.\n");
        return 0; // No trade
    }
}