# Pressure plates
An iterative prototype in the context of my master thesis "Exploration von interaktiven und unaufdringlichen Interfaces in einer semi-public Meetup Situation" (en. "Exploration of interactive and unobtrusive interfaces in a semi-public meetup situation") WS19/30 got created.
It consists of two plates lying on the floor, that communicate with each other and present a game.
The plates were refined over 3 iterations and consist of multiple metal parts that get assembled like shown in the following graphic:
![plate prototype assembling](/img/pressure_plate_assembling-09-09.png)

Two different ESP8266 got used for the prototypes (solely on the basis of availability).
- [Waveshare e-Paper ESP8266 Driver Board](https://www.waveshare.com/e-paper-esp8266-driver-board.htm)
- [Adafruit Feather HUZZAH with ESP8266](https://www.adafruit.com/product/3046)

The ESPs communicate over UDP. The finalized and used code can be found [here](/websockets/pressure_plate_UDP_e_ink_ESP) and [here](/websockets/pressure_plate_UDP_feather_huzzah_ESP).

The game works in such a way that the plates rotate through different, random colours. If the plates show the same colour, the players have to press their plate. The player who presses first wins.
