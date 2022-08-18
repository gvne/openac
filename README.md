# OpenAC : Open source implementation of Audio Cast

## DNTP : dumb Network time protocol ([NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol)) implementation

To handle the synchronization of multiple clients, a reference clock needs to be used. This library is designed to determine with enough precision the delay between an emitter and a receiver.

This tools can be seen as a simplified version of the NTP:
- The client sends a packet over UDP to a server filled with the originate timestamp
- The server receives it and add the receive timestamp.
- when possible, the server adds the transmit timestamp and sends it back to the client
At reception, the client computes the time offset and round trip delay. Using those value a synchronization is obtained (the greater the round trip delay, the less accurate the time offset should be considered)

This library is defined as dumb as it does not rely on any stratum mechanism and omits much of the NTP complexity
