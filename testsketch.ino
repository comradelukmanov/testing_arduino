#include "button.h"
#include <PCA95x5.h>

PCA9535 ioex1;  // Объект расширителя ввода-вывода
PCA9535 ioex2;  // Объект расширителя ввода-вывода

button btn1(2);
button btn2(3);
button btn3(4);

int mode;  // Переменная режима устройства
// 0 - бездействие
// 1 - салют
// 2 - падение
// 3 - комета

int speed = 50;  // Скорость бега огней светодиодов

void off() {
  ioex1.write(PCA95x5::Level::L_ALL);
  ioex2.write(PCA95x5::Level::L_ALL);
}

void salute() {
  for (int i = 0; i < 15; i++) {
    ioex1.write(PCA95x5::Level::L_ALL);  // Гасим все огни
    ioex1.write(i, PCA95x5::Level::H);   // Зажигаем нужный
    delay(speed);
  }
  for (int i = 0; i < 15; i++) {
    ioex2.write(PCA95x5::Level::L_ALL);  // Гасим все огни
    ioex2.write(i, PCA95x5::Level::H);   // Зажигаем нужный
    delay(speed);
  }
}

void fall() {
  for (int i = 15; i != 0; i--) {
    ioex1.write(PCA95x5::Level::L_ALL);  // Гасим все огни
    ioex1.write(i, PCA95x5::Level::H);   // Зажигаем нужный
    delay(speed);
  }
  for (int i = 15; i != 0; i--) {
    ioex2.write(PCA95x5::Level::L_ALL);  // Гасим все огни
    ioex2.write(i, PCA95x5::Level::H);   // Зажигаем нужный
    delay(speed);
  }
}

void comet() {
  ioex2.write(15, PCA95x5::Level::H);  // Последний белый светоиод всегда горит
  for (int i = 14; i != 0; i--) {
    ioex2.write(PCA95x5::Level::H_ALL);  // Зажигаем все огни
    ioex2.write(i, PCA95x5::Level::L);   // Гасим нужный
    delay(speed);
  }
  for (int i = 15; i != 0; i--) {
    ioex1.write(PCA95x5::Level::H_ALL);  // Зажигаем все огни
    ioex1.write(i, PCA95x5::Level::L);   // Гасим нужный
    delay(speed);
  }
}

void readCommand() {
  String cmd;  // Переменная для хранения прочитанной команды
  if (Serial.available()) {
    cmd = Serial.readString();
  }
  if (cmd != "") {
    if (cmd == "off") mode = 0;
    if (cmd == "salute") mode = 1;
    if (cmd == "fall") mode = 2;
    if (cmd == "comet") mode = 3;
    if (cmd == "ison") {
      Serial.println(mode != 0);
    }
  }
}

void buttonProcessing() {
  if (btn1.click()) {
    if (mode == 1) {
      mode = 0;
    } else {
      mode = 1;
    }
  }
  if (btn2.click()) {
    if (mode == 2) {
      mode = 0;
    } else {
      mode = 2;
    }
  }
  if (btn3.click()) {
    if (mode == 3) {
      mode = 0;
    } else {
      mode = 3;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);  // Установка тайм-аута

  // Инициализация портов кнопок
  for (int i = 2; i < 5; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  Wire.begin();
  ioex1.attach(Wire, 0b0100000);
  ioex1.polarity(PCA95x5::Polarity::ORIGINAL_ALL);  // Установка прямой полярности пинов
  ioex1.direction(PCA95x5::Direction::OUT_ALL);     // Установка всех пинов расширителя на режим вывода
  ioex1.write(PCA95x5::Level::L_ALL);               // Установка 0 на всех пинах

  ioex2.attach(Wire, 0b0100001);
  ioex2.polarity(PCA95x5::Polarity::ORIGINAL_ALL);  // Установка прямой полярности пинов
  ioex2.direction(PCA95x5::Direction::OUT_ALL);     // Установка всех пинов расширителя на режим вывода
  ioex2.write(PCA95x5::Level::L_ALL);               // Установка 0 на всех пинах
}

void loop() {
  switch (mode) {
    case 0:
      off();
      break;
    case 1:
      salute();
      break;
    case 2:
      fall();
      break;
    case 3:
      comet();
      break;
  }
}

void yield() {
  readCommand();
  buttonProcessing();
}
