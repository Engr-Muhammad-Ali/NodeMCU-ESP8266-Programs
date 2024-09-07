int D = 1000;
int Button_State;

void setup() {
  // put your setup code here, to run once:
  pinMode (D1, OUTPUT);
  pinMode (D2, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Button_State = digitalRead(D2);

  if (Button_State == LOW)
  {
    digitalWrite (D1, 0);
  }
  else
  {
    digitalWrite (D1, 1);
  }
}
