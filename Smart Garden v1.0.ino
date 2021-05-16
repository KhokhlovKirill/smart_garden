void(* resetFunc) (void) = 0;  // Функция перезагрузки

// Подключение библиотек
#include "GyverButton.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
//_________________________


// Пресеты

// Название пресетов

const char *namesPreset[]  = {
  "B\273a\264o\273\306\262\270\263\303e",   // 0
  "C\263e\277o\273\306\262\270\263\303e",     // 1
  "Te\276\273o\273\306\262\270\263\303e",     // 2
  "Te\276\273oc\263e\277o\273\306\262\270\263\303e",     // 3
  "B\273a\264o\277e\276\273o\273\306\262\270\263\303e",     // 4
  "He\276p\270xo\277\273\270\263\303e",     // 5
};

//_____________________________________


  // Значения пресетов

  // Пресет 1
  const byte MoistureForWateringPreset1 = 0;
  const byte TemperaturePreset1 = 0;
  const byte LightingPreset1 = 0;
  //_____________

    // Пресет 2
  const byte MoistureForWateringPreset2 = 0;
  const byte TemperaturePreset2 = 0;
  const byte LightingPreset2 = 0;
  //_____________

    // Пресет 3
  const byte MoistureForWateringPreset3 = 0;
  const byte TemperaturePreset3 = 0;
  const byte LightingPreset3 = 0;
  //_____________

    // Пресет 4
  const byte MoistureForWateringPreset4 = 0;
  const byte TemperaturePreset4 = 0;
  const byte LightingPreset4 = 0;
  //_____________

    // Пресет 5
  const byte MoistureForWateringPreset5 = 0;
  const byte TemperaturePreset5 = 0;
  const byte LightingPreset5 = 0;
  //_____________

    // Пресет 6
  const byte MoistureForWateringPreset6 = 0;
  const byte TemperaturePreset6 = 0;
  const byte LightingPreset6 = 0;
  //_____________

//___________________________

//_____________________________________________________________



// Объявление директив #define
#define TERMIST_B 4300 
 #define PHOTORESIST A4
#define VIN 5.0

#define POMP 5 // Помпа (MOSFET)
#define TERMISTOR A1 // Помпа (MOSFET)
#define BUT_UP 4 // Кнопка вверх
#define BUT_DOWN 2 // Кнопка вниз
#define BUT_ENTER 3 // Кнопка ввод
#define BUT_STOP 5 // Кнопка аварийной остановки помпы
#define SOILT 7 // Датчик влажности почвы (Транзистор)
#define LED 6
#define SOIL A0 // Датчик влажности почвы
//_____________________________



// Настройка библиотек
GButton but_up(BUT_UP); // Кнопка вверх
GButton but_down(BUT_DOWN); // Кнопка вниз
GButton but_enter(BUT_ENTER); // Кнопка ввод
GButton but_stop(BUT_STOP); // Кнопка аварийной остановки помпы

LiquidCrystal lcd(13, 12, 11, 10,  9,  8); // Инициализация библиотеки LCD-экрана
//_________________________


// Объявление глобальных переменных
int soilResistance = 0; // Значение analogRead с датчик влажности почвы
int soilMoisture; // Процент влажности почвы
byte MoistureForWatering = 0; // Процент влажности почвы (EEPROM)
float temperature = 0; // Температура воздуха или почвы
int lighting = 0;
int lightingResist = 0;
byte regularControl = 0; // Время полива (EEPROM)
byte temperatureForSignal = 0; // Температура для сигнала (EEPROM)
byte lightingForSignal = 0; // Освещённость для сигнала (EEPROM)
unsigned long currentTime; 
unsigned long currentTime1; 
unsigned long currentTime2; 
unsigned long currentTime3; 
unsigned long currentTime4; 
boolean ledIsTurnTemp;
boolean ledIsTurnLighting;
boolean ledIsTurn;
//___________________________________


// Символы для LCD-экрана
byte water[8]= { // Капля
            B00100,
            B00100,
            B01110,
            B01110,
            B11111,
            B11111,
            B01110

};

byte temp[8]= { // Температура
            B00101,
            B00100,
            B00100,
            B01110,
            B00100,
            B00101,
            B00111

};


byte lamp[8]= { // Лампочка
            B01110,
            B10001,
            B10101,
            B10101,
            B01110,
            B01110,
            B00100

};
//________________________________________



void setup() {
  // put your setup code here, to run once:

  
// Установка режимов для пинов
pinMode(SOIL, OUTPUT); // Датчик влаж почвы
pinMode(LED, OUTPUT); // Светодиод
pinMode(POMP, OUTPUT); // Помпа (Реле)
//______________________________

  
lcd.begin(16, 2); // Инициализация экрана

// Инициализация символов
lcd.createChar(0, water);
lcd.createChar(1, temp);
lcd.createChar(2, lamp);
//____________________________


// Установка режима опроса кнопки на автоматический
 but_up.setTickMode(AUTO);
 but_down.setTickMode(AUTO);
 but_enter.setTickMode(AUTO);
 but_stop.setTickMode(AUTO);
//______________________________  

Serial.begin(9600); // Инициализация для монитора порта (Отладка)

// Получение настроек с EEPROM

if (EEPROM.read(5) == 255) EEPROM.update(5, 0);
regularControl = EEPROM.read(5); // Время полива

if (EEPROM.read(6) == 255) EEPROM.update(6, 0);
MoistureForWatering = EEPROM.read(6); // Влажность для полива

if (EEPROM.read(7) == 255) EEPROM.update(7, 0);
temperatureForSignal = EEPROM.read(7); // Температура для сигнала

if (EEPROM.read(8) == 255) EEPROM.update(8, 0);
lightingForSignal = EEPROM.read(8); // Освещенность для сигнала
//_____________________________________



digitalWrite(POMP, LOW); // Отключение помпы
}

void loop() {
  // put your main code here, to run repeatedly:
but_enter.tick(); // Принудительное считывание значения с кнопки Enter


// Получение данных о освещенности
if (millis() - currentTime3 > 5000) 
{ 
    currentTime3 = millis();        
    lightingResist = analogRead(PHOTORESIST);
lighting = map(lightingResist, 0, 1023, 0, 100);
}
//____________________________________________________


// Получение данных о температуре
if (millis() - currentTime2 > 5000) 
{ 
    currentTime2 = millis();        
float voltage = analogRead(TERMISTOR) * VIN / 1024.0;
   float r1 = voltage / (VIN - voltage);
   temperature = 1./( 1./(TERMIST_B)*log(r1)+1./(25. + 273.) ) - 273;
Serial.println(temperature); 
}
//___________________________________________________________



// Сигнализация при отклонениях
  // Температура
if (temperature <= temperatureForSignal){
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  
  ledIsTurnTemp = true;
  }
  if (temperature > temperatureForSignal){
  lcd.setCursor(15, 0);
  lcd.print(" ");

  ledIsTurnTemp = false;
  }
  //________________________________________

  // Освещенность
  if (lighting <= lightingForSignal){
  lcd.setCursor(15, 1);
  lcd.write(byte(2));

  ledIsTurnLighting = true;
  }
  if (lighting > lightingForSignal){
  lcd.setCursor(15, 1);
  lcd.print(" ");

  ledIsTurnLighting = false;
  }
  //____________________________________ 

  // Включение светодиода
  if (ledIsTurnTemp || ledIsTurnLighting) ledIsTurn = true;
  else ledIsTurn = false;


  if (ledIsTurn) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);
  //_______________________________________________________________
//_________________________________________________________
  

//   Считывание показайний с датчика влажности почвы       
if (millis() - currentTime > (regularControl * 60000)) 
{ 
    currentTime = millis();        
    digitalWrite(SOILT, HIGH);
    soilResistance = analogRead(SOIL);
    Serial.println(soilResistance);
    soilMoisture = map(soilResistance, 0, 650, 0, 100);
    digitalWrite(SOILT, LOW);    
}
//___________________________________________________________

// Полив
if (soilMoisture < MoistureForWatering){
  if (millis() > (regularControl * 60000)){
  watering();
  } else digitalWrite(POMP, LOW);
  }   
//__________________________



// Основной экран

  // Влажность почвы
  lcd.setCursor(1, 0);
  lcd.write(byte(0));
  lcd.print("-");
  lcd.print(soilMoisture);
  lcd.print("%");
  //____________________________

    // Температура
  lcd.setCursor(1, 1);
  lcd.write(byte(1));
  lcd.print("-");
  lcd.print(temperature);
  lcd.print("\x99""C");
  //____________________________


      // Освещенность
  lcd.setCursor(8, 0);
  lcd.write(byte(2));
  lcd.print("-");
  lcd.print(lighting);
  lcd.print("%");

  //____________________________
  
//______________________________________

// Переключение с основного экрана на настройки
if (but_enter.hasClicks()) {
  byte clicks = but_enter.getClicks();
  switch (clicks) {
    case 1:
    menuSettings();
    break;

    case 2:
    menuPresets();
    break;

    }
  }
  //_________________________________________________
}




void menuSettings() { // Меню настроек
byte posMenu = 0;
  
  lcd.clear();
  for (;;){
  lcd.setCursor(1, 0);
  lcd.print("Pe\264y\273. \272o\275\277po\273\307"); // Время полива

    lcd.setCursor(1, 1);
  lcd.print("B\273a\266.\343\273\307 \276o\273\270\263a"); // Влаж для полива

  if (but_down.isClick()) posMenu = posMenu+ 1;
  if (but_up.isClick()) posMenu = posMenu - 1;

  if (posMenu == 0) {
  lcd.setCursor(0, 1);
  lcd.print(" ");  
  lcd.setCursor(0, 0);
  lcd.print("\x13"); }

  if (posMenu == 1) {
  lcd.setCursor(0, 0);
  lcd.print(" ");  
  lcd.setCursor(0, 1);
  lcd.print("\x13"); }

    if (posMenu == 2) {
  menuSettings2();
  }


  if (but_enter.isSingle()){
    switch (posMenu) {
      case 0:
      
      regularControlSet();
      break;
      case 1:
      MoistureForWateringSet();
      break;
      
      }
    }


    if (but_enter.isDouble()){
      lcd.clear();
     resetFunc();
      break;
      }
  
  
   }
   }
//__________________________________________________


// Меню настроек времени полива
void regularControlSet(){
   lcd.clear();
   for(;;){
    lcd.setCursor(0,0);
  lcd.print("Pe\264y\273. \272o\275\277po\273\307");
  lcd.setCursor(4,1);
  lcd.print(regularControl);
  lcd.print(" \274\270\275"); // мин


  if (but_up.isClick()) {           
    regularControl++;                       
  }
  if (but_down.isClick()) {         
    regularControl--;                        
  }
  if (but_up.isStep()) {          
    regularControl++;                       
  }
  if (but_down.isStep()) {            
    regularControl--;                       
  }

    if (but_enter.isSingle()){
      EEPROM.update(5, regularControl);
      lcd.clear();
      break;
      }
    }
}
//________________________________________________



void MoistureForWateringSet(){
   lcd.clear();
   for(;;){
    lcd.setCursor(0,0);
  lcd.print("B\273a\266. \343\273\307 \276o\273\270\263a");
  lcd.setCursor(4,1);
  lcd.print(MoistureForWatering);
  lcd.print(" %");


  if (but_up.isClick()) {           
    MoistureForWatering++;                       
  }
  if (but_down.isClick()) {         
    MoistureForWatering--;                        
  }
  if (but_up.isStep()) {          
    MoistureForWatering++;                       
  }
  if (but_down.isStep()) {            
    MoistureForWatering--;                       
  }

    if (but_enter.isSingle()){
      EEPROM.update(6, MoistureForWatering);
      lcd.clear();
      break;
      }
    }
}



// Меню питания
void menuPower(){ // Меню питания и сброса настроек
  byte posMenu = 1;
  
  lcd.clear();
  for (;;){
  lcd.setCursor(1, 0);
  lcd.print("\250epe\267a\264py\267\272a");

    lcd.setCursor(1, 1);
  lcd.print("C\262poc \275ac\277poe\272");

  if (but_down.isClick()) posMenu = posMenu+ 1;
  if (but_up.isClick()) posMenu = posMenu - 1;

  
    if (posMenu == 0) {
menuSettings2();
 }
  
  
  if (posMenu == 1) {
  lcd.setCursor(0, 1);
  lcd.print(" ");  
  lcd.setCursor(0, 0);
  lcd.print("\x13"); }

  if (posMenu == 2) {
  lcd.setCursor(0, 0);
  lcd.print(" ");  
  lcd.setCursor(0, 1);
  lcd.print("\x13"); }

    if (posMenu == 3) {
posMenu = 2;
 }


  if (but_enter.isSingle()){
    switch (posMenu) {
      case 1:
      Serial.println("Restart");
      restart();
      break;
      case 2:
      resetSet();
      break;
      
      }
    }


    if (but_enter.isDouble()){
      lcd.clear();
           resetFunc();
      break;
      }
  
  
   }
  }
//______________________________________________


// Функция перезагрузки
void restart(){
lcd.clear();
lcd.setCursor(1, 0);
lcd.print("\250epe\267a\264py\267\272a...");
delay(1500);
resetFunc();
}
//_____________________________


// Функция сброса к заводским настройкам
void resetSet(){
byte posMenu = 0;
  
  lcd.clear();
  for (;;){
    lcd.setCursor(1,0);
    lcd.print("C\262poc \275ac\277poe\272?");
    lcd.setCursor(2,1);
    lcd.print("\340a");
    lcd.setCursor(12,1);
    lcd.print("He\277");
    
  if (but_down.isClick()) posMenu = 1;
  if (but_up.isClick()) posMenu = 0;

  if (posMenu == 1) {
  lcd.setCursor(1, 1);
  lcd.print(" ");  
  lcd.setCursor(11, 1);
  lcd.print("\x13"); }

  if (posMenu == 0) {
  lcd.setCursor(11, 1);
  lcd.print(" ");  
  lcd.setCursor(1, 1);
  lcd.print("\x13"); }


  if (but_enter.isSingle()){
    switch (posMenu) {
      case 1:
       resetFunc();
      case 0:
      EEPROM.update(5, 0);
      EEPROM.update(6, 0);
      resetFunc();
      break;
      
      }
    }
  }
}
//_________________________________________________

// Функция полива
void watering(){
  lcd.clear();
  for (;;) {
  // Вывод на экран
  lcd.setCursor(5, 0);
  lcd.print("\250o\273\270\263");
  lcd.setCursor(5, 1);
    lcd.write(byte(0));
  lcd.print("-");
  lcd.print(soilMoisture);
  lcd.print("%");
  //____________________________

  // Включение помпы
  digitalWrite(POMP, HIGH);
  delay(50);
  //_________________________

  // Отключение помпы
      digitalWrite(SOILT, HIGH);
    soilResistance = analogRead(SOIL);
    Serial.println(soilResistance);
    soilMoisture = map(soilResistance, 0, 650, 0, 100);
    digitalWrite(SOILT, LOW);  
  if (soilMoisture >= MoistureForWatering){
  digitalWrite(POMP, LOW);
  lcd.clear();
  break;
  }
  //_______________________
  
  }}
//_________________________________________________


  void menuSettings2() { // Меню настроек
byte posMenu = 1;
  
  lcd.clear();
  for (;;){
  lcd.setCursor(1, 0);
  lcd.print("Te\274\276epa\277ypa"); // Температура

    lcd.setCursor(1, 1);
  lcd.print("Oc\263e\346e\275\270e"); // Освещение

  if (but_down.isClick()) posMenu = posMenu+ 1;
  if (but_up.isClick()) posMenu = posMenu - 1;

  if (posMenu == 0) {
  menuSettings();
  }
  
  
  if (posMenu == 1) {
  lcd.setCursor(0, 1);
  lcd.print(" ");  
  lcd.setCursor(0, 0);
  lcd.print("\x13"); }

  if (posMenu == 2) {
  lcd.setCursor(0, 0);
  lcd.print(" ");  
  lcd.setCursor(0, 1);
  lcd.print("\x13"); }

    if (posMenu == 3) {
menuPower();
 }


  if (but_enter.isSingle()){
    switch (posMenu) {
      case 1:
      
      temperatureSet();
      break;
      case 2:
      lightingSet();
      break;
      
      }
    }


    if (but_enter.isDouble()){
      lcd.clear();
           resetFunc();
      break;
      }
  
  
   }
   }
//__________________________________________________



void temperatureSet(){
   lcd.clear();
   for(;;){
    lcd.setCursor(0,0);
  lcd.print("Te\274\276epa\277ypa:");
  lcd.setCursor(4,1);
  lcd.print(temperatureForSignal);
  lcd.print("\x99""C");


  if (but_up.isClick()) {           
    temperatureForSignal++;                       
  }
  if (but_down.isClick()) {         
    temperatureForSignal--;                        
  }
  if (but_up.isStep()) {          
    temperatureForSignal++;                       
  }
  if (but_down.isStep()) {            
    temperatureForSignal--;                       
  }

    if (but_enter.isSingle()){
      EEPROM.update(7, temperatureForSignal);
      lcd.clear();
      break;
      }
    }
}


void lightingSet(){
   lcd.clear();
   for(;;){
    lcd.setCursor(0,0);
  lcd.print("Oc\263e\346\265\275\275oc\277\304:");
  lcd.setCursor(4,1);
  lcd.print(lightingForSignal);
  lcd.print(" %");


  if (but_up.isClick()) {           
    lightingForSignal++;                       
  }
  if (but_down.isClick()) {         
    lightingForSignal--;                        
  }
  if (but_up.isStep()) {          
    lightingForSignal = lightingForSignal + 10;                       
  }
  if (but_down.isStep()) {            
    lightingForSignal = lightingForSignal - 10;                       
  }

    if (lightingForSignal >= 101) lightingForSignal = 100;

    if (but_enter.isSingle()){
      EEPROM.update(8, lightingForSignal);
      lcd.clear();
      break;
      }
    }
}


void menuPresets() { // Меню пресетов 1
byte posMenu = 0;
  
  lcd.clear();
  for (;;){
  lcd.setCursor(1, 0);
  lcd.print(namesPreset[0]); // Пресет 1

    lcd.setCursor(1, 1);
  lcd.print(namesPreset[1]); // Пресет 2

  if (but_down.isClick()) posMenu = posMenu+ 1;
  if (but_up.isClick()) posMenu = posMenu - 1;

  if (posMenu == 0) {
  lcd.setCursor(0, 1);
  lcd.print(" ");  
  lcd.setCursor(0, 0);
  lcd.print("\x13"); }

  if (posMenu == 1) {
  lcd.setCursor(0, 0);
  lcd.print(" ");  
  lcd.setCursor(0, 1);
  lcd.print("\x13"); }

    if (posMenu == 2) {
  menuPresets2();
  }


  if (but_enter.isSingle()){
    switch (posMenu) {
      case 0:
      
      preset1();
      break;
      case 1:
      preset2();
      break;
      
      }
    }


    if (but_enter.isDouble()){
      lcd.clear();
     resetFunc();
      break;
      }
  
  
   }
   }
//__________________________________________________

  void menuPresets2() { // Меню пресетов 2
byte posMenu = 1;
  
  lcd.clear();
  for (;;){
  lcd.setCursor(1, 0);
  lcd.print(namesPreset[2]); // Пресет 3

    lcd.setCursor(1, 1);
  lcd.print(namesPreset[3]); // Пресет 4

  if (but_down.isClick()) posMenu = posMenu+ 1;
  if (but_up.isClick()) posMenu = posMenu - 1;

  if (posMenu == 0) {
  menuPresets();
  }
  
  
  if (posMenu == 1) {
  lcd.setCursor(0, 1);
  lcd.print(" ");  
  lcd.setCursor(0, 0);
  lcd.print("\x13"); }

  if (posMenu == 2) {
  lcd.setCursor(0, 0);
  lcd.print(" ");  
  lcd.setCursor(0, 1);
  lcd.print("\x13"); }

    if (posMenu == 3) {
menuPresets3();
 }


  if (but_enter.isSingle()){
    switch (posMenu) {
      case 1:
      
      preset3();
      break;
      case 2:
      preset4();
      break;
      
      }
    }


    if (but_enter.isDouble()){
      lcd.clear();
           resetFunc();
      break;
      }
  
  
   }
   }
//__________________________________________________

  void menuPresets3() { // Меню пресетов 3
byte posMenu = 1;
  
  lcd.clear();
  for (;;){
  lcd.setCursor(1, 0);
  lcd.print(namesPreset[4]); // Пресет 5

    lcd.setCursor(1, 1);
  lcd.print(namesPreset[5]); // Пресет 6

  if (but_down.isClick()) posMenu = posMenu+ 1;
  if (but_up.isClick()) posMenu = posMenu - 1;

  if (posMenu == 0) {
  menuPresets2();
  }
  
 
  if (posMenu == 1) {
  lcd.setCursor(0, 1);
  lcd.print(" ");  
  lcd.setCursor(0, 0);
  lcd.print("\x13"); }

  if (posMenu == 2) {
  lcd.setCursor(0, 0);
  lcd.print(" ");  
  lcd.setCursor(0, 1);
  lcd.print("\x13"); }

    if (posMenu == 3) {
posMenu = 2;
 }


  if (but_enter.isSingle()){
    switch (posMenu) {
      case 1:
      
      preset5();
      break;
      case 2:
      preset6();
      break;
      
      }
    }


    if (but_enter.isDouble()){
      lcd.clear();
           resetFunc();
      break;
      }
  
  
   }
   }
//__________________________________________________


// Настройка устройства при выборе пресетов
void preset1(){
EEPROM.update(6, MoistureForWateringPreset1);
EEPROM.update(7, TemperaturePreset1);
EEPROM.update(8, LightingPreset1);
resetFunc();
}

void preset2(){
EEPROM.update(6, MoistureForWateringPreset2);
EEPROM.update(7, TemperaturePreset2);
EEPROM.update(8, LightingPreset2);
resetFunc();
}

void preset3(){
EEPROM.update(6, MoistureForWateringPreset3);
EEPROM.update(7, TemperaturePreset3);
EEPROM.update(8, LightingPreset3);
resetFunc();
}

void preset4(){
EEPROM.update(6, MoistureForWateringPreset4);
EEPROM.update(7, TemperaturePreset4);
EEPROM.update(8, LightingPreset4);
resetFunc();
}

void preset5(){
EEPROM.update(6, MoistureForWateringPreset5);
EEPROM.update(7, TemperaturePreset5);
EEPROM.update(8, LightingPreset5);
resetFunc();
}

void preset6(){
EEPROM.update(6, MoistureForWateringPreset6);
EEPROM.update(7, TemperaturePreset6);
EEPROM.update(8, LightingPreset6);
resetFunc();
}
//________________________________________________
