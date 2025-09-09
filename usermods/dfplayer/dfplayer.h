/*
  MP3-TF-16P
  21/12/2022
  credits: https://digitaltown.co.uk/components17dfminiplayer.php
*/
HardwareSerial mySerial(1); // RX/TX

// this function sends the actual command
// It receives the command byte and ParData is optional info such as track number or volume depending on the command
void sendDFCommand(byte Command, int ParData)
{
  byte commandData[10]; // This holds all the command data to be sent
  byte q;
  int checkSum;
  // Each command value is being sent in Hexadecimal
  commandData[0] = 0x7E;              // Start of new command
  commandData[1] = 0xFF;              // Version information
  commandData[2] = 0x06;              // Data length (not including parity) or the start and version
  commandData[3] = Command;           // The command that was sent through
  commandData[4] = 0x01;              // 1 = feedback
  commandData[5] = highByte(ParData); // High byte of the data sent over
  commandData[6] = lowByte(ParData);  // low byte of the data sent over
  checkSum = -(commandData[1] + commandData[2] + commandData[3] + commandData[4] + commandData[5] + commandData[6]);
  commandData[7] = highByte(checkSum); // High byte of the checkSum
  commandData[8] = lowByte(checkSum);  // low byte of the checkSum
  commandData[9] = 0xEF;               // End bit
  for (q = 0; q < 10; q++)
  {
    mySerial.write(commandData[q]);
  }
  // delay(100);
}

// play a specific track number
void playTrack(int tracknum)
{
  sendDFCommand(0x03, tracknum);
}

// plays the next track
void playNext()
{
  sendDFCommand(0x01, 0);
}

// volume increase by 1
void volumeUp()
{
  sendDFCommand(0x04, 0);
}
// volume decrease by 1
void volumeDown()
{
  sendDFCommand(0x05, 0);
}

// set volume to specific value
void changeVolume(int thevolume)
{
  sendDFCommand(0x06, thevolume);
}

void initDfPlayer()
{

  mySerial.begin(9600); // start the softwareSerial to DF player

  delay(3500);      // let everything initialise
}
