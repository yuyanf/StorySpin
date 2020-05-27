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
String adjListe[10] = {"Ekte", "Flau", "Fremmed", "Midlertidig", "Modig", "Nysgjerrig", "Tilstrekkelig", "Ekkel", "Dyster", "Skummel"};
int adjTeller = 0;
int adjForrige = -1;

const char verb = '8';
String verbListe[10] = {"Eie", "Glede seg", "Hjelpe", "Miste", "Overtale", "Savne", "Drepe", "Dyrke", "Hindre", "Nekte"};
int verbTeller = 0;
int verbForrige = -1;

const char sub = '7';
String subListe[12] = {"Framtid", "Kjole", "Katt", "Strand", "Penger", "Sommer", "Venn", "Skog", "Plante", "Troll", "Tur", "Planet"};
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

  //Ventetid etter at batteriene er slaatt paa.
  for(int i = 15; i > 0; i--) 
  {
    lcd.clear();
    lcd.print(i);
    Serial.println(i);
    delay(1000);
  }

  
  
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
      if(pronoTeller < 3)//sjekk om kortet har blitt skannet 3 ganger allerede, pronoTellers initialverdi er 0
      {
        //bruker listeLengde som variabel slik at vi senere kan legge til eller fjerne ord fra listen uten a maatte endre pa storrelsen i koden.
        int listeLengde = sizeof(pronoListe)/sizeof(pronoListe[0]);// total antall bytes dele på en string element som er 6 byte gir storrelse til array.
        printOrd(pronoListe, listeLengde, pronoForrige, "Pronomen:");
        pronoTeller++;
      }
      else
      {
        bruktOpp();
      }//brukere får skanne maks 2 ganger per kort hver spillerunde
      break;


    case preposi: 
      if(preposiTeller < 3)
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
      if(determTeller < 3)
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
      if(adjTeller < 3)
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
      if(verbTeller < 3)
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
      if(subTeller < 3)
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
  Serial.println("Resetta alt");
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
   
  Serial.println(nyOrd);
  Serial.println(forrigeOrd);

  
  Serial.println("Foor while");
  while(forrigeOrd == nyOrd) //dersom forrigeords randomtall er samme som ny ords randomtall, generer random tall på nytt
  {
     nyOrd = random(listeLengde);
     Serial.print("Forrige ord: ");
     Serial.println(forrigeOrd);
     Serial.print("Ny ord: "); 
     Serial.println(nyOrd);
  }
  Serial.println("Etter while");

  //sett forrigeords random tall til ny ords random tall etter første scan 
  switch(ordklasse.charAt(2)) 
  {
    case 'o':
      pronoForrige = nyOrd;
      lcd.print(ordklasse);
      lcd.setCursor(0,1);
      lcd.print(lista[pronoForrige]);
      break;
      
    case 'e':
      preposiForrige = nyOrd;
      lcd.print(ordklasse);
      lcd.setCursor(0,1);
      lcd.print(lista[preposiForrige]);
      break;
      
    case 't':
      determForrige = nyOrd;
      lcd.print(ordklasse);
      lcd.setCursor(0,1);
      lcd.print(lista[determForrige]);
      break;
      
    case 'j':
      adjForrige = nyOrd;
      lcd.print(ordklasse);
      lcd.setCursor(0,1);
      lcd.print(lista[adjForrige]);
      break;
      
    case 'r':
      verbForrige = nyOrd;
      lcd.print(ordklasse);
      lcd.setCursor(0,1);
      lcd.print(lista[verbForrige]);
      break;
      
    case 'b':
      subForrige = nyOrd;
      lcd.print(ordklasse);
      lcd.setCursor(0,1);
      lcd.print(lista[subForrige]);
      break;  
  }
  
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
