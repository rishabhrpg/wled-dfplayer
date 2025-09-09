#include "wled.h"
#include "./dfplayer.h"

class DFPLAYER : public Usermod
{
private:
  static const char _name[];
  int volume = 20;

public:
  inline bool isEnabled() { return true; }
  void setup()
  {
    Serial.println("Hello from my dfplayerusermod!");
  }

  void connected()
  {
    initDfPlayer();
    changeVolume(volume);
    delay(100);
    Serial.println("dfplayerusermod! connected play track");
    playTrack(1);
  }

  void loop()
  {
    // Nothing needed here for this usermod
  }

  void addToConfig(JsonObject &root) override
  {
    JsonObject top = root.createNestedObject(FPSTR(_name));
    // save these vars persistently whenever settings are saved
    top["volume"] = volume;
  }

  /*
   * readFromJsonState() is called when WLED receives a POST request to /json/state.
   */
  void readFromJsonState(JsonObject &root) override
  {
    // Log that the function was called

    // Check if our "audiomod" key exists
    JsonObject audiomod = root["audiomod"];

    if (audiomod.isNull())
    {
      // This is expected for most API calls, so we won't print anything here.
      // It just means the call was not for us.
      return;
    }

    // If we get here, the "audiomod" key was found.
    if (audiomod.containsKey("track"))
    {
      int trackToPlay = audiomod["track"].as<int>();

      if (trackToPlay > 0)
      {
        playTrack(trackToPlay);
      }
    }

    if (audiomod.containsKey("volume"))
    {
      int vol = audiomod["volume"].as<int>();
      changeVolume(vol);
    }
  }

  bool readFromConfig(JsonObject &root) override
  {
    JsonObject audiomod = root[FPSTR(_name)];
    if (!audiomod.isNull())
    {
      this->volume = audiomod["volume"].as<int>();
      changeVolume(this->volume); // <-- apply after loading config
      return true;
    }
    return false;
  }

  uint16_t getId()
  {
    return USERMOD_ID_DFPLAYER; // Make sure this ID is defined in const.h
  }
};

// add more strings here to reduce flash memory usage
const char DFPLAYER::_name[] PROGMEM = "DFPLAYERMOD";

// Create an instance of the usermod class to register it.
static DFPLAYER myAudioMod;
REGISTER_USERMOD(myAudioMod);