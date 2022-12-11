# OpenAC : Open source implementation of Audio Cast

## DNTP : dumb Network time protocol ([NTP](https://en.wikipedia.org/wiki/Network_Time_Protocol)) implementation

To handle the synchronization of multiple clients, a reference clock needs to be used. This library is designed to determine with enough precision the delay between an emitter and a receiver.

This tools can be seen as a simplified version of the NTP:
- The client sends a packet over UDP to a server filled with the originate timestamp
- The server receives it and add the receive timestamp.
- when possible, the server adds the transmit timestamp and sends it back to the client
At reception, the client computes the time offset and round trip delay. Using those value a synchronization is obtained (the greater the round trip delay, the less accurate the time offset should be considered)

This library is defined as dumb as it does not rely on any stratum mechanism and omits much of the NTP complexity

## Wire / Cable : Publisher & Listener interface to cast audio

A wire lets you cast a single channel of audio when the cable gives the possibility
to send multiple channels.

These libraries rely on the Real-time transport protocol ([RTP](https://en.wikipedia.org/wiki/Real-time_Transport_Protocol)).
Packets sent by publishers conform to the standard.   
To deal with the synchronization, we add an extension to the basic RTP format.
The extension ID is x0001001100110111. Its 128bits are divided into:
- 8 bits unsigned int for the version.
- 8 bits of padding to match the required 32bits alignment
- 16 bits unsigned int for the DNTP server port
- 4 times 8 bits unsigned int to encode the DNTP server ip v4 address. An address of 0.0.0.0 meaning that the DNTP server has the same address as the source of that packet
- 64 bits unsigned int representing the NTP timestamp at which the stream started (RTP sequence number = 0 and timestamp = 0)

When receiving data from a new source, the listener connects to the DNTP server to
determine the time offset between client and server. That offset is added to the
desired stream latency.

## Use case: cast Spotify to multiple devices

On a raspberry pi zero equiped with a [DAC](https://shop.pimoroni.com/products/audio-dac-shim-line-out), I installed and configured [spotifyd](https://github.com/Spotifyd/spotifyd). Using [spotify connect](https://support.spotify.com/ca-en/article/spotify-connect/) I can now cast spotify to by raspberry pi.  
Now, the idea is to send the pi output to a loopback device (supported by [alsa](https://www.alsa-project.org/wiki/Matrix:Module-aloop)) and to use our tool to send the audio stream of that loopback device on the network.
```bash
# Setup the loopback device and force it so that it appears even after a reboot
sudo modprobe snd-aloop
echo snd-aloop | sudo tee -a /etc/modules
sudo reboot
# Update the default output (see https://www.alsa-project.org/wiki/Setting_the_default_device)
...
# you can now check if your device records data using arecord (on my case loopback is device 0. Subdevice 1 was the one that recorded something)
arecord -D hw:0,1 -V stereo -r 16000 -f S16_LE -c 2 /dev/null
```

Now that the device is ready, we can start publishing stuff !
First we need to compile our casting tool:
```bash
sudo apt install git

sudo apt install clang \
libasound2-dev \
libjack-jackd2-dev \
ladspa-sdk \
libcurl4-openssl-dev \
libfreetype6-dev \
libx11-dev \
libxcursor-dev \
libxext-dev \
libxinerama-dev \
libxrandr-dev \
libxrender-dev \
libxcomposite-dev


sudo apt install libwebkit2gtk-4.0-dev \
mesa-common-dev \
libglu1-mesa-dev

sudo apt install cmake
```
