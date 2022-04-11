int lu_forward = 33;
int lu_backward = 15;
int ru_forward = 14;
int ru_backward = 32;

void setup() {
  // put your setup code here, to run once:
  pinMode(lu_forward, OUTPUT);
  pinMode(lu_backward, OUTPUT);
  pinMode(ru_forward, OUTPUT);
  pinMode(ru_backward, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(lu_backward, LOW);
  //digitalWrite(ru_backward, LOW);
  digitalWrite(lu_forward, HIGH);
  /*digitalWrite(ru_forward, HIGH);
  delay(1000);
  digitalWrite(lu_forward, LOW);
  digitalWrite(ru_forward, LOW);
  digitalWrite(lu_backward, HIGH);
  digitalWrite(ru_backward, HIGH);
  delay(1000);*/
}
