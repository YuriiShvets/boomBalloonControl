#ifndef Config_h
#define Config_h

class Config {

  private:
    unsigned long gameTime;
    SdFile file;

  public:
    Config() {
      
      gameTime = 0;
      
      //--------------- ������ gameTime ------------------

      if(file.open("config.txt")) {
        while(file.available()) {
          gameTime *= 10;
          gameTime += (file.read() - 48);
        }
      }
      else {
        Serial.println("ERROR!!  (config.txt wasnt open)");
      }
      file.close();
      gameTime *= 1000;
//      Serial.print("gameTime = ");
//      Serial.println(gameTime);
    }

    unsigned long getGameTime() {
      return gameTime;
    }
};

#endif
