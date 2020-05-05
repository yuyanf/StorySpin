
int motorPin1 = 2; //1A
int motorPin2 = 4; //2A
int enablePin = 6; // EN1,2
int knappPin = 8;
int arduinoLink = 9;
int motorHastighet = 100;



void setup() {
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(knappPin, INPUT);
  pinMode(arduinoLink, OUTPUT);
  digitalWrite(arduinoLink, LOW);
   
  //Initialisere motoren 
  //for å kjøre motor må motorPin1 og motorPin2 ha motsatt verdi. Hvilket verdier som er motsatt bestemmer også kjøre retning.
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);

}

void loop() {

  int val = digitalRead(knappPin); //naar knappen er trykket inn, vil denne verdien bli HIGH.


  if (val == HIGH) {

    digitalWrite(arduinoLink, HIGH);//Send signal til LCD/RFID-arduino.
    analogWrite(enablePin, motorHastighet); //Start motoren

    delay(3000);
    digitalWrite(enablePin, LOW);//Stopper motor på free-running modus.
    digitalWrite(arduinoLink, LOW);//Sender den andre arduinoen signal.
    
  }


}//loop
