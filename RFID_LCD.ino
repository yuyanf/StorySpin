#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h> 
#define SS_PIN 10
#define RST_PIN 9
#define resetPin 8
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2,3,4,5,6,7);

boolean harSpunnet = false; //status når roulette ikke har spunnet

const char pronomen = '6';
String pronoListe[6] = {"Oss", "Meg", "Dere", "Dem", "Deg", "Den"};
int pronoTeller = 0;
int pronoForrige = -1;//satt til -1 slik at det er aldri lik generert random tall i første scan

const char preposi = '2';
String preposiListe[7] = {"Mellom", "Over", "Av", "Bak", "Hos", "Gjennom", "Utenom"};
int preposiTeller = 0;
int preposiForrige = -1;

const char determ = 'C';
String determListe[6] = {"Hennes", "Ditt", "Sitt", "Sine", "Noen", "Noe"};
int determTeller = 0;
int determForrige = -1;

const char adj = '5';
String adjListe[15] = {"Dyp", "Ekte", "Flau", "Fremmed", "Heldig", "Midlertidig", "Modig", "Nysgjerrig", "Tilstrekkelig", "Vellykket", "Ekkel", "Dyster", "Hyppig", "Skummel"};
int adjTeller = 0;
int adjForrige = -1;

const char verb = '8';
String verbListe[15] = {"Danse", "Eie", "Glede seg", "Hjelpe", "Leke", "Miste", "Overtale", "Reise", "Savne", "Trenge", "Drepe", "Dyrke", "Grue seg", "Hindre", "Nekte"};
int verbTeller = 0;
int verbForrige = -1;

const char sub = '7';
String subListe[15] = {"Familie", "Framtid", "Kjole", "Katt", "Strand", "Penger", "Sommer", "Venn", "Kjæreste", "Skog", "Plante", "Troll", "Tur", "Teater", "Planet"};
int subTeller = 0;
int subForrige = -1;



 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  randomSeed(analogRead(0)); //Gir en nyt sett med random naar man skal uploade kode paa nytt.
  pinMode(resetPin, INPUT);
  // set up for intro 
  lcd.begin(16, 2);
  delay(2500);
  lcd.print(" Velkommen til StorySpin! Kos dere:)");
  for (int i = 0; i < 24; i++){
    lcd.scrollDisplayLeft();
    delay(385); 
  }
  lcd.clear();
  delay(500);
  lcd.print("Skann for ord: ");
}//setup



void loop() 
{
  if(digitalRead(resetPin))//når motor spinner er resetPin også satt til High 
  {
    harSpunnet = true;
  }
  
  // ser etter ny kort
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // velge et av kortene
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //hent kort uid
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();


  if(harSpunnet) //Dersom motor er spunnet vil en ny runde starte og variabler resettes.
  { 
    resetAll();
  }


  //switch case sjekk alle kort og siden alle kort har forskjellige karakterer på den 4.karakter
  switch(content.charAt(4))
  {
    case pronomen:
      if(pronoTeller < 2)//sjekk om kortet har blitt skannet 2 ganger allerede
      {
        int listeLengde = sizeof(pronoListe)/sizeof(pronoListe[0]);// total antall bytes dele på en string element som er 6 byte gir storrelse til array
        printOrd(pronoListe, listeLengde, pronoForrige, "Pronomen:");
        pronoTeller++;
      }
      else
      {
        bruktOpp();
      }//brukere får skanne maks 2 ganger per kort hver spillerunde
      break;


    case preposi: 
      if(preposiTeller < 2)
      {
        int listeLengde = sizeof(preposiListe)/sizeof(preposiListe[0]);
        printOrd(preposiListe, listeLengde, preposiForrige, "Preposisjon:");
        preposiTeller++;
             
      }
      else
      {
        bruktOpp();
      }
      break;

    
    case determ:
      if(determTeller < 2)
      {
        int listeLengde = sizeof(determListe)/sizeof(determListe[0]);
        printOrd(determListe, listeLengde, determForrige, "Determinativ:");
        determTeller++;    
      }
      else
      {
        bruktOpp();
      }
      break;
      

    case adj:
      if(adjTeller < 2)
      {
        int listeLengde = sizeof(adjListe)/sizeof(adjListe[0]);
        printOrd(adjListe, listeLengde, adjForrige, "Adjektiv:");
        adjTeller++;   
      }
      else
      {
        bruktOpp();
      }
      break;

      
    case verb:
      if(verbTeller < 2)
      {
        int listeLengde = sizeof(verbListe)/sizeof(verbListe[0]);
        printOrd(verbListe, listeLengde, verbForrige, "Verb:");
        verbTeller++;       
      }
      else
      {
        bruktOpp();
      }
      break;


    case sub:
      if(subTeller < 2)
      {
        int listeLengde = sizeof(subListe)/sizeof(subListe[0]);
        printOrd(subListe, listeLengde, subForrige, "Substantiv:");
        subTeller++;      
      }
      else
      {
        bruktOpp();
      }
      break;
              
  }//Switch-case  
}//mainloop 


void resetAll()
{
  pronoTeller = 0;
  pronoForrige = -1;
  
  preposiTeller = 0;
  preposiForrige = -1;
  
  determTeller = 0;
  determForrige = -1;
  
  adjTeller = 0;
  adjForrige = -1;

  verbTeller = 0;
  verbForrige = -1;
  
  subTeller = 0;
  subForrige = -1;

  harSpunnet = false;//Settes lav igjen for at spillere kan starte runden som normalt.
  
}//resetAll


void printOrd(String lista[], int listeLengde, int forrigeOrd, String ordklasse) 
{
  lcd.clear();
  int nyOrd = random(listeLengde); //generer random tall, altså random posisjon i array for å printe ut random element

  while(forrigeOrd == nyOrd) //dersom forrigeords randomtall er samme som ny ords randomtall, generer random tall på nytt
  {
     nyOrd = random(listeLengde);
  }

  forrigeOrd = nyOrd; // sett forrigeords random tall til ny ords random tall etter første scan 
  lcd.print(ordklasse);
  lcd.setCursor(0,1);
  lcd.print(lista[forrigeOrd]);
  delay(1500); 
  
}//printOrd


void bruktOpp() 
{ 
  lcd.clear();
  lcd.print("O P S! Sjanse er");
  lcd.setCursor(0,1);
  lcd.print("brukt opp.");
  delay(1500);
  lcd.clear();
  delay(500);
  lcd.print("Skann for ord: "); 
    
}//bruktOpp
