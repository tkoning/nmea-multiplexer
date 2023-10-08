#include <AltSoftSerial.h>

bool debug = false;
bool checksum = true;

AltSoftSerial GPSingang;

void setup() {
  Serial.begin(38400);
  GPSingang.begin(4800);

  if (debug) {
    delay(1000);
    Serial.println(F("Test av NMEA multiplexing"));
    Serial.println(F("Versjon 2"));
  }
}

void loop() {
  if (Serial.available() > 40) {
    multiplex(Serial);
  }
  if (GPSingang.available() > 10) {
    multiplex(GPSingang);
  }
}

void multiplex(Stream &inputStream) {
  static char NMEAbuffer[100];
  static int i = 0;
  static int chk = 0;
  char c;

  while (inputStream.available()) {
    c = inputStream.read();

    if (c == '$' || c == '!') {
      i = 0;
    }

    if (i < 99) {
      NMEAbuffer[i] = c;
      if (c == '*') chk = i;
      i++;

      if (c == '\r') {
        NMEAbuffer[i] = '\n';
        i++;
        
        if (checksum && validateChecksum(NMEAbuffer, chk)) {
          Serial.write(NMEAbuffer, i);
        } else if (!checksum) {
          Serial.write(NMEAbuffer, i);
        }

        i = 0;
        return;
      }
    } else {
      i = 0; // reset buffer på overflow
    }
  }
}

bool validateChecksum(const char *sentence, int pos) {
  int16_t Xor = 0;
  for (int j = 1; j < pos; j++) {
    Xor ^= sentence[j];
  }

  byte groot = (sentence[pos + 1] <= '9') ? sentence[pos + 1] - '0' : sentence[pos + 1] - 'A' + 10;
  byte klein = (sentence[pos + 2] <= '9') ? sentence[pos + 2] - '0' : sentence[pos + 2] - 'A' + 10;

  return Xor == (16 * groot) + klein;
}
