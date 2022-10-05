# Publisher / Subscriber pattern

## Use case:

One to many communication over UDP.  
Publisher provides a constant stream of data.  
Subscriber registers to the Publisher to receive data.

## Flow description

* Publisher listens on a port known by subscribers
* Subscriber sends a subscription request to the publisher
* If the request is valid, Publisher starts sending data to the subscriber. If
  not, an error is returned
* After a period of time specified in the subscription request, the Subscriber
  sends a keep-alive request for the publisher to keep pushing data
* if the Publisher did not receive the keep-alive request, a message is query is
  sent to the subscriber notifying that the connection will be closed if a
  keep-alive message is not sent back
* When de-registering, the subscriber sends a request to the publisher to stop
  the stream

## Messages description

### Common header

To differentiate messages, the first octet is a message type identifier:
* 0 = DATA. Represents a chunk of data sent from the Publisher to subscribers
* 1 = REQUEST. Sent when the registering a new subscriber
* 2 = KEEP-ALIVE. Sent by the subscriber to keep the connection opened.
* 3 = CLOSE. Sent by the subscriber to stop the data stream

### DATA

* uint8  - message type: 0 (DATA)
* uint8  - TODO: use it to ask the subscriber to send a Keep-alive ?
* uint16 - sequence number: The index used to detect missing datagrams and ask
for re-transmission. The sequence number is incremented by 1 for each datagram.
* Frame content

### REQUEST

* uint8   - message type: 1 (REQUEST)
* uint8   -
* uint16  - port to be used to receive the data stream on the client side

### KEEP-ALIVE

* uint8   - message type: 2 (KEEP-ALIVE)

### KEEP-ALIVE

* uint8   - message type: 3 (CLOSE)

## Discussion

* Why rely on UDP instead of TCP for one to one communication (registration,
error keep-alive and close) ? => Minimize the number of sockets. Only one
required on publisher and subscriber side
