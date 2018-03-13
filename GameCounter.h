#ifndef GameCounter_h
#define GameCounter_h

class GameCounter {

  private:
    SdFile file;
    int multiplierDemo;
    int multiplierGames;
    int demo;
    int games; 

  public:
    GameCounter() {
      numberOfDemo = 0; //обнуляємо змінні
      numberOfGames = 0;
      multiplierDemo = 0;
      multiplierGames = 0;
      demo = 0;
      games = 0;

      //---читаємо із файла------
    
      if(file.open("counter.txt")){
        multiplierDemo = file.read();
        multiplierGames = file.read();
        demo = file.read();
        games = file.read();
      }
      file.close();  
      numberOfDemo = multiplierDemo * 250 + demo;
      numberOfGames = multiplierGames * 250 + games;
    }

    void incrementDemo() {
      numberOfDemo++;
      demo++;
      if(demo > 250) {
        multiplierDemo++;
        demo = 1;
      }
      updateFile();
    }

    void incrementGames() {
      numberOfGames++;
      games++;
      if(games > 250) {
        multiplierGames++;
        games = 1;
      }
      updateFile();
    }

    void updateFile() {   //обновляємо вміст файлу
      if(file.open("counter.txt", O_RDWR)){
        file.print((char)multiplierDemo);   //спочатку множники, потім доданки
        file.print((char)multiplierGames);
        file.print((char)demo);
        file.print((char)games);
      }
      file.close();
    }
};

#endif