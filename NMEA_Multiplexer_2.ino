 #include <AltSoftSerial.h>

/* arduino gebaseerde NMEA multiplexer
    oorspronkelijke code gebaseerd op het gebruik van een arduino mega met 3 HW serial poorten
    Versie 1 zonder gebruik van pointers
    Versie 1b aangepast aan arduino nano en de alt-serial library
    versie 2 aangepast aan arduino nano met chksum berekening
    
    Onduidelijk of er effect is van het  if Serial.available > N construct

    Dick Koning dec 2016 jan 2017
*/

bool debug = false;
bool checksum = true;
AltSoftSerial GPSingang;

void setup() {
  // put your setup code here, to run once:


  Serial.begin (38400);         // NMEA Uitgang en AIS Ingang  (Tx =1 Rx=2)
  GPSingang.begin (4800);       // GPS Ingang (Tx=9 Rx=8)


  if (debug) {
    delay(1000);
    Serial.println();
    Serial.println("Test van NMEA multiplexing");
    Serial.println();
    Serial.println ("Arduino Nano met Alt soft serial");
    Serial.println();
    Serial.println ("Dick Koning dec 2016 vs 1a");
    delay(1000);
  }

}

void loop() {

  if (Serial.available() > 40) multiplex();       // poging de tijd van de multiplexer eerlijk te verdelen
  if (GPSingang.available() > 10) multiplex1();

}



void multiplex() {

  static char NMEAbuffer[100] ;     // Buffer tbv NMEA sentences, totaal 100 bytes lang
  static int i;                     // teller
  static int chk;                   // positie van het * in de nmea statement
  char c;

  while (Serial.available()) {

    c = Serial.read();
    if ( c == 0x24  || c == 0x21) {                                // '$' or '!'
      i = 0;                                                       // brute force sync on '$' to NMEAbuffer[0]
    }
    if (i < 99) {                                                  // hou een extra plek over voor het einde string karakter
      NMEAbuffer[i] = c;                                            // bij buffer overflow gebeurt er pas weer wat als de $ herkend wordt
      if (c == 0x2A) chk = i;                                       // chk is de positie van het * in de NMEA message
      i++;

      if (c == 0x0D) {                                              // cr
        NMEAbuffer[i] = 0x0A;                                       // voeg een newline
        i++ ;                                                       // de lengte van de buffer is 1 groter dan de index


        if (checksum) {
          int16_t Xor = 0;

          for (int j = 1; j < chk; j++) {                         // bereken de checksum   XOR alle data tussen $ of ! en *   teller i is 1 hoger dan in vorige sketch Andere syntax AIVD statement ??
            Xor = Xor ^ NMEAbuffer[j];                            // eventueel een separate index voor het * karakter gebruiken  (* = i-6 )
          }

          byte groot = (NMEAbuffer[(chk + 1)] < 0x39) ? NMEAbuffer[(chk + 1)] - 0x30 : NMEAbuffer[(chk + 1)] - 0x37; // To do : hard coderen ivm snelheid
          byte klein = (NMEAbuffer[(chk + 2)] < 0x39) ? NMEAbuffer[(chk + 2)] - 0x30 : NMEAbuffer[(chk + 2)] - 0x37;

          if (Xor == (16 * groot) + klein)  Serial.write(NMEAbuffer, i);


        }

        else {
          Serial.write(NMEAbuffer, i);
        }
        i = 0;
        return;                                                      // zorg er voor dat een snel inkomend signaal niet alles lamlegt
      }
    }
  }
}



void multiplex1() {

  static char NMEAbuffer[100] ;     // Buffer tbv NMEA sentences, totaal 100 bytes lang
  static int i;                     // teller
  static int chk;                   // positie van het * in de nmea statement
  char c;

  while (GPSingang.available()) {

    c = GPSingang.read();
    if ( c == 0x24  || c == 0x21) {                                // '$' or '!'
      i = 0;                                                       // brute force sync on '$' to NMEAbuffer[0]
    }
    if (i < 99) {                                                  // hou een extra plek over voor het einde string karakter
      NMEAbuffer[i] = c;                                            // bij buffer overflow gebeurt er pas weer wat als de $ herkend wordt
      if (c == 0x2A) chk = i;                                       // chk is de positie van het * in de NMEA message
      i++;

      if (c == 0x0D) {                                              // cr
        NMEAbuffer[i] = 0x0A;                                       // voeg een newline
        i++ ;                                                       // de lengte van de buffer is 1 groter dan de index


        if (checksum) {
          int16_t Xor = 0;

          for (int j = 1; j < chk; j++) {                         // bereken de checksum   XOR alle data tussen $ of ! en *   teller i is 1 hoger dan in vorige sketch Andere syntax AIVD statement ??
            Xor = Xor ^ NMEAbuffer[j];                            // eventueel een separate index voor het * karakter gebruiken  (* = i-6 )
          }

          byte groot = (NMEAbuffer[(chk + 1)] < 0x39) ? NMEAbuffer[(chk + 1)] - 0x30 : NMEAbuffer[(chk + 1)] - 0x37; // To do : hard coderen ivm snelheid
          byte klein = (NMEAbuffer[(chk + 2)] < 0x39) ? NMEAbuffer[(chk + 2)] - 0x30 : NMEAbuffer[(chk + 2)] - 0x37;

          if (Xor == (16 * groot) + klein)  Serial.write(NMEAbuffer, i);


        }

        else {
          Serial.write(NMEAbuffer, i);
        }
        i = 0;
        return;                                                      // zorg er voor dat een snel inkomend signaal niet alles lamlegt
      }
    }
  }
}

