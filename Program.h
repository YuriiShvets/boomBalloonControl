#ifndef Program_h
#define Program_h

#include "FileWorkerFat.h"
#include "AnalogOut.h"

class Program {

  private:
    AnalogButton *demoButton;                          //����� �� ������ ���� ������
    AnalogButton *button;
    AnalogButton *stopButton;
    AnalogOut *Rele1;
    DigitalOut *Rele2;
    AnalogOut *Rele3;
    AnalogOut *Rele4;
    Clock *Time;
    unsigned long Time2;                                //��� ��� ������������
    unsigned long Time3;                                //��� ��� �������� ������
    Music *music;
    Statistics *statistics;
    Sekyurotron *sekyurotron;
    boolean standartMode;                               //���� ������������ ����
    boolean PlayMode;                                   //���� �������� ����
    boolean buttonMode;                                  //���� ���� �� ������ ����������
    boolean demoModeState;
    GameCounter *counter;
    Config *attractionConfig;
    boolean motorsInStandBy = false;
    boolean musicInStandBy = false;
    unsigned long startStopSpeed = 5000;
    float maxVolume = 0;
    float minVolume = 65278;
    unsigned int standByMusicVolume = maxVolume;
    unsigned int playMusicVolume = maxVolume;

  public:
    Program() {		//nead to initial Serial
      Rele1 = new AnalogOut(3, startStopSpeed, 5);                        //�� �� ����� ������
      Rele2 = new DigitalOut(4);
      Rele3 = new AnalogOut(5, startStopSpeed, 5);
      Rele4 = new AnalogOut(10, startStopSpeed, 5);
      if (!sd.begin(9, SPI_HALF_SPEED)) {
        //Serial.println("SD EROR");
        resetFunc();
      }
      Serial.println("SD OK");
      delay(1000);
      //�� �� ����� ������, �������� ��������, ��������� �������, ������� �����
      //����������
      demoButton = new AnalogButton(A1, 512, 50, 500);
      button = new AnalogButton(A0, 512, 50, 500);
      stopButton = new AnalogButton(A2, 512, 50, 500);
      Time = new Clock();
      Time2 = 0;                                        //��������� ��� ������������
      Time3 = millis();
      sekyurotron = new Sekyurotron();
      statistics = new Statistics();
      music = new Music();
      standartMode = true;
      PlayMode = false;
      buttonMode = false;
      demoModeState = false;
      counter = new GameCounter();
      attractionConfig = new Config();

      FileWorkerFat *fileWorker = new FileWorkerFat();
      fileWorker->openFileForRead("config2.txt");
      motorsInStandBy = fileWorker->getNextNumber();
      //Serial.println(motorsInStandBy);
      musicInStandBy = fileWorker->getNextNumber();
      float bufVolume = fileWorker->getNextNumber();
      //Serial.print("bufVolume = ");
      //Serial.println(bufVolume);
      standByMusicVolume = minVolume - (minVolume / 100. * bufVolume);
      //Serial.println("standByMusicVolume = ");
      //Serial.println(standByMusicVolume);
      bufVolume = fileWorker->getNextNumber();
      playMusicVolume = minVolume - (minVolume / 100. * bufVolume);
      //Serial.println(playMusicVolume);
      fileWorker->closeFile();
      delete fileWorker;

    }

    void standByMode() {
      if (musicInStandBy) {
        music -> setVolume(standByMusicVolume, standByMusicVolume);   //�� ����������� �������
        music -> continuePlay(2);         ////1 - ����� (������), 2 - �����(������)
      }
      //Serial.println("Stand by mode");
      if (motorsInStandBy) {
        //        Serial.print("1 ");
        //        Serial.println(Rele1 -> getState());
        //        Serial.print("2 ");
        //        Serial.println(Rele2 -> getState());
        //        Serial.print("3 ");
        //        Serial.println(Rele3 -> getState());
        //        Serial.print("4 ");
        //        Serial.println(Rele4 -> getState());
        unsigned long Delay = 20000;
        if (millis() - Time3 < Delay) {
          Rele1 -> setState(true);
          Rele2 -> setState(false);
          Rele3 -> setState(false);
          Rele4 -> setState(false);
        }
        else if (millis() - Time3 < Delay * 2) {
          Rele1 -> setState(false);
          Rele2 -> setState(true);
          Rele3 -> setState(false);
          Rele4 -> setState(false);
        }
        else if (millis() - Time3 < Delay * 3) {
          Rele1 -> setState(false);
          Rele2 -> setState(false);
          Rele3 -> setState(true);
          Rele4 -> setState(false);
        }
        else if (millis() - Time3 < Delay * 4) {
          Rele1 -> setState(false);
          Rele2 -> setState(false);
          Rele3 -> setState(false);
          Rele4 -> setState(true);
        }
        else
          Time3 = millis();
      }
      else {
        Rele1 -> setState(false);
        Rele2 -> setState(false);
        Rele3 -> setState(false);
        Rele4 -> setState(false);
      }
    }

    void playMode() {
      Rele1 -> setState(true);
      Rele2 -> setState(true);
      Rele3 -> setState(true);
      Rele4 -> setState(true);
      music -> setVolume(playMusicVolume, playMusicVolume);  //����������� �������
      music -> continuePlay(1);   //������� �����
    }

    void demoMode() {
      Rele1 -> setState(false);
      Rele2 -> setState(false);
      Rele3 -> setState(true);
      Rele4 -> setState(true);
      music -> setVolume(playMusicVolume, playMusicVolume);
      music -> continuePlay(1);
    }

    void startStandartMode() {
      music -> stopMusic();
      //Serial.println("Wait mode was started");
      standartMode = true;
      PlayMode = false;
      buttonMode = false;
      demoModeState = false;
    }

    void startPlayMode() {
      //Serial.println("Play mode was started");
      standartMode = false;
      PlayMode = true;
      buttonMode = false;
      demoModeState = false;
    }

    void startDemoMode() {
      music -> stopMusic();
      Time2 = millis();
      //Serial.println("Demo mode was started");
      standartMode = false;
      PlayMode = false;
      buttonMode = false;
      demoModeState = true;
      counter -> incrementDemo();
    }

    void startButtonMode() {
      music -> stopMusic();
      sekyurotron -> decrementTimer();
      statistics -> handControl();
      Time2 = millis();
      //Serial.println("Buttons mode was started");
      standartMode = false;
      PlayMode = false;
      buttonMode = true;
      demoModeState = false;
      counter -> incrementGames();
    }

    void Update() {
      button -> Update(false);                    //�����������
      demoButton -> Update(false);
      stopButton -> Update(false);
      Rele1->Update();
      Rele3->Update();
      Rele4->Update();

      if (standartMode)
        standByMode();
      if (demoModeState)
        demoMode();
      if (PlayMode || buttonMode)
        playMode();

      //Serial.print("gameTime = ");
      //Serial.println(attractionConfig->getGameTime());
      if (millis() > 2000 && button -> getState() && (sekyurotron -> getTimerState()
          || sekyurotron -> getFileIndexState()))                         //������ ���
        startButtonMode();
      else if ((millis() - Time2 > attractionConfig->getGameTime() || Time2 == 0) && buttonMode) { //��� ��� (����� ���� 166000)
        startStandartMode();
      }

      if (millis() > 2000 && demoButton -> getState() && !buttonMode
          && (sekyurotron -> getTimerState()
              || sekyurotron -> getFileIndexState()))                //������ ���� ������
        startDemoMode();
      else if ((millis() - Time2 > 40000 || Time2 == 0) && demoModeState)
        startStandartMode();
      if (stopButton->getState()) {
        //Serial.println("stopButton was presed");
        Rele1 -> setHardState(false);
        Rele2 -> setState(false);
        Rele3 -> setHardState(false);
        Rele4 -> setHardState(false);
        music -> stopMusic();
        while(true);
      }

    }

    Clock getClock() {
      return *Time;
    }
};

#endif
