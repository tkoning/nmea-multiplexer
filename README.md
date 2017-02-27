# nmea-multiplexer

this is a simple NMEA multiplexer project
The code runs on a arduino nano. 
NMEA information input is from 2 sources: AIS at 38400 baud on hardware serial in, and GPS at 4800 baud on the software serial input.
Output is at 38400 baud on the hardware serial out / usb out 

Some information will get lost in the multiplexing especially at high AIS input rates, sinde the AIS input rate is equal to the output baudrate of the multiplexer
If you need different baudrates or get to many signals lost use a arduino mega which has 3 hardware serial ports

Since NMEA information is rs 232 or rs 485 conversion of the signal is necessary. 
I used a max 232 to TTL converter
they can be found on ebay (http://www.ebay.com/itm/5Pcs-Mini-RS232-To-TTL-MAX3232-Converter-Adaptor-Module-Serial-Port-Board-/282131187736?hash=item41b0544818:g:GBEAAOSwHoFXqZit)

