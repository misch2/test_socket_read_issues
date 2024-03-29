# A PoC to verify issues with read() in ESP32 Arduino libraries

Here's a simple webserver in Python which sends a short response but explicitly adds a 1 second delay between each part.
This allows us to check how ESP32 client works with regards to `WiFiClient.read()` call. 

I'm here trying to show that the `read()` call in `WiFiClient` is implemented using nonblocking reads and therefore if there's any delay in server providing the data then the read() returns -1 instead of waiting for the data. AFAIK this behaviour can't be switched off and has to be handled manually in the client code.

The issue is there for multibyte `read(buffer, size)` variant too. In this case the read(...) sometimes returns less data then required, i.e. for `a=read(buf, 100)` it might return a==7 even when the server hasn't finished sending everything and even when subsequent read() calls would continue returning the response data.

## Steps

### Start the webserver

```
$ python3 webserver.py
Server started http://0.0.0.0:7999
```
### Verify it's working

In another terminal:

```
$ time curl http://localhost:7999/
666666333221
real    0m4.031s
user    0m0.011s
sys     0m0.015s
```

As you can see it takes 4 seconds to finish this request because there's a 1 second pause after each sequence of digits.

### Try the client on ESP32:

```
pio run -t upload -t monitor
```

One would expect that there would be 10 successfully read characters in the output (ASCII code 54 for the char "6", 49 for the char "1" etc.), but due to the delay()s on the server side the read() in client returns -1 immediately even though the socket is still open and additional data are going to be sent by the server.
But if the client waits a while then next read()s will continue returning data.

```
Starting
connecting to wifi
connected to wifi
Testing GET request
HTTP status code: 200
Reading 10 bytes sequentially
sequence 1:
[0] read: 54 ('6')
[1] read: 54 ('6')
[2] read: 54 ('6')
[3] read: 54 ('6')
[4] read: 54 ('6')
[5] read: 54 ('6')
[6] read: -1 ('�')
[7] read: -1 ('�')
[8] read: -1 ('�')
[9] read: -1 ('�')
[10] read: -1 ('�')
waiting 2 seconds
sequence 2:
[0] read: 51 ('3')
[1] read: 51 ('3')
[2] read: 51 ('3')
[3] read: 50 ('2')
[4] read: 50 ('2')
(no data read but there is data available, waiting)
[5] read: 49 ('1')
(no data read but there is data available, waiting)
(no data read but there is data available, waiting)
(no data read but there is data available, waiting)
got real error, errno=128, connected=0
Done
```

## Disclaimer

This is a PoC to demonstrate an issue with WiFiClient / HTTPClient in Arduino framework for ESP32, not an example how to handle the reads correctly! I'm pretty sure there exists a better way how to distinguish between a real error and EINPROGRESS state than checking the isconnected status etc.
