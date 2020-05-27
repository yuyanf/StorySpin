
int motorPin1 = 2; //1A
int motorPin2 = 4; //2A
int enablePin = 6; // EN1,2
int knappPin = 8;
int arduinoLink = 13;
int motorHastighet = 147;




void setup() {
  Serial.begin(9600);

  //Kode linjen endrer frekvens paa PMW-pin-6 fra default verdi 976.56Hz til 62.5kHz.
  //Dette for aa unngaa pipelyd fra motor. Mennesker hoorer frekvenser opp til 20kHz.
  TCCR0B = TCCR0B & B11111000 | B00000001;
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(knappPin, INPUT);
  pinMode(arduinoLink, OUTPUT);
  digitalWrite(arduinoLink, LOW);
   
  //Initialisere motoren 
  //for å kjøre motor må motorPin1 og motorPin2 ha motsatt verdi.
  //Hvilket verdier som er motsatt bestemmer også kjøre retning.
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);

}

void loop() {

  int val = digitalRead(knappPin); //naar knappen er trykket inn, vil denne verdien bli HIGH.


  if (val == HIGH) {

    digitalWrite(arduinoLink, HIGH);//Send signal til LCD/RFID-arduino.
    Serial.println("Motor sender: HIGH");
    analogWrite(enablePin, motorHastighet); //Start motoren

    //4.5sec delay = 4500ms. 4500 * 64 = 288 000.
    delay(288000);//Delay er vanligvis 1000ms = 1 sec. Etter endring av frekvensen til PMW-pin 6 (EnablePin)
    //Vil denne sin timer divisor bli 1 (opprinnelig 64). Derfor ganges tiden med 64.
    digitalWrite(enablePin, LOW);//Stopper motor på free-running modus.
    digitalWrite(arduinoLink, LOW);//Sender den andre arduinoen signal.
    
  }


}//loop
