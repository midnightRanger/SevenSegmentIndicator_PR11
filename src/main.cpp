#include <Arduino.h>

#define latchPin 4
#define clockPin 7
#define dataPin 8
#define COUNTDOWN_DELAY 1000

//коды для цифр от 0 до 9 под соответствующими индексами
int digitArray[] = {~B11111100, ~B01100000, ~B11011010, ~B11110010, ~B01100110, ~B10110110, ~B10111110, ~B11100000, ~B11111110, ~B11110110};
int countdown_digit = 0;
void setup()
{

  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void set_number(int num)
{
  if (num < 0 || num > 9)
  {
    Serial.println("Введите цифру от 0 до 9");
    return;
  }

  int digitCode = digitArray[num];
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, digitCode);
  shiftOut(dataPin, clockPin, MSBFIRST, 0xF1);
  digitalWrite(latchPin, HIGH);
}

long countdown_millis = 0;
void timer_start()
{
  if (millis() - countdown_millis > COUNTDOWN_DELAY)
  {
    set_number(countdown_digit);
    countdown_digit = countdown_digit - 1;
    countdown_millis = millis();
  }
}

void command_handle()
{
  String input_number;
  int number;

  if (Serial.available() > 0)
  {
    Serial.print("Было введено значение: ");
    input_number = Serial.readString();
    number = input_number.toInt();
  }
  else
    return;

  Serial.println(number);
  countdown_digit = number;
  set_number(number);
}

void loop()
{
  command_handle();
  if (countdown_digit >= 0)
  {
    timer_start();
  }
}