int D = 500;

void setup() {
  // put your setup code here, to run once:
  pinMode (D4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (D4, 1);
  delay (D);

  digitalWrite (D4, 0);
  delay (D);
}
