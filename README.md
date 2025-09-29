# Compile

```bash
gcc *.c -lcurl -o main.exe
```

# Run

```bash
./main.exe
```

# Description
This project is a low-latency trading application written in C that interacts with the Alpaca API to fetch market data and execute trades based on a simple price strategy. It uses the libcurl library for HTTP requests and JSON parsing for handling API responses.

# Approach
1. **HTTP Requests**: The application uses libcurl to make HTTP GET and POST requests to the Alpaca API.
2. **JSON Parsing**: It employs a JSON parsing library to decode the responses from the API.
3. **Trading Strategy**: A simple price-based strategy is implemented to decide when to place trades.
4. **Modular Design**: The code is organized into multiple files for better maintainability, including separate files for HTTP handling, Alpaca API interactions, and trading strategies.

# TODO
- [ ] Implement more complex trading strategies.
- [ ] Use WebSocket for real-time data instead of polling.

