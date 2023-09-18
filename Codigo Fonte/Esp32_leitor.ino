#include <Keypad.h>
#include <LiquidCrystal.h>

const int rs = 5, en = 4, d4 = 3, d5 = 2, d6 = A4, d7 = A5;//fixo fiz para facilitar o entedimento do codigo, blz?
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int iLineNumber = 1;    //Número da linha para mostrar sua string (0 ou 1)                                                               

int iCursor = 0;

int screenWidth = 16;
int screenHeight = 2;

String line1 = "    EcoSpark"; // sem scroll
String line2 = "Insira o codigo de barras"; // scroll na linha 

int stringStart, stringEnd = 0;
int scrollCursor = screenWidth;
int next =0;


const byte ROWS = 4; //quatro linhas
const byte COLS = 4; //quatro colunas
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10}; //conecte aos pinos de linha do teclado
byte colPins[COLS] = {9, 8, 7, 6}; //conecte aos pinos de coluna do teclado
int LCDRow = 7; //import
const String code_plastic = "A123";
const String code_glass = "B456";
const String code_metal = "C789";
String input;


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {

  lcd.begin(screenWidth, screenHeight);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(LCDRow, 0);
  input.reserve(32);

}



void loop() {

  send_codeQR(); // Para enviar dados dos materias
  delay(5000); //atraso de 5 segundos para inicia sequência de coleta
}


void init_wifi(void) {

  Serial.println("Conectando Wifi...");

  Serial.print("SSID: ");

  Serial.println(wifiSsid);

  Serial.print("PASS: ");

  Serial.println(wifiPass);

  WiFi.begin(wifiSsid, wifiPass);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println("WiFi Conectado!");

  Serial.print("IP is ");

  Serial.println(WiFi.localIP());

}


void send_codeQR(void) {


  while (next != 24){
	
  
  lcd.setCursor(0, 0); // set na primeira linha
  lcd.print(line1); // print a messagem na primeira linha
  lcd.setCursor(scrollCursor, 1); // Definir o cursor na primeira linha e (rolar da extremidade esquerda para a direita)
  lcd.print(line2.substring(stringStart,stringEnd)); // To print line1 first character "inicial"(atenção)

  delay(150);

  lcd.clear(); // limpa o diplay
 
  if(stringStart == 0 && scrollCursor > 0){
  scrollCursor--; // Move cursor para 16, 0
  stringEnd++; // caracteres u,g,i...
  // imprimi todos os caracteres de acordo com a logica, pestar atenção nessa soma 
  }
  else if (stringStart == stringEnd){ // começar tudo de novo
  stringStart = stringEnd = 0;
  scrollCursor = screenWidth;
  }
  else if (stringEnd == line1.length() && scrollCursor == 0) { // se chegar ao caractere final
  stringStart++;
  }
  else { //ele imprimirá o caractere de (1 a 16) até o caractere final (atenção)SOMA.SE
  stringStart++;
  stringEnd++;
  next++;
   }
  }
  
  
  char key = keypad.getKey();

  if (key){
    Serial.println(key);

    if(key == '*') {
      lcd.clear();
      input = "";
      LCDRow = 0;

    } else if(key == '#') {
      if(input == code_plastic){
        lcd.clear();
        lcd.print("Plastico");
      } else if(input == code_glass){
        lcd.clear();
        lcd.print("Vidro");
      } else if(input == code_metal){
        lcd.clear();
        lcd.print("Metal");
      } else {
        lcd.clear();
        lcd.print("Codigo Invalido");
      }
      delay(1000);
      lcd.clear();
      input = "";
      LCDRow = 0;
    } else {
      input += key;
      lcd.setCursor(LCDRow, 8);//lcd.setCursor(LCDRow, 0);
      LCDRow = LCDRow + 1;
      
      lcd.print(key);


      char anyData[30];

      char postData[300];

      char anyData1[30];

      char bAny[30];

      int statusCode = 0;


      strcpy(postData, "{\n\t\"variable\": \"material\",\n\t\"value\": ");

      dtostrf(key, 6, 2, anyData);

      strncat(postData, anyData, 100);

      strcpy(anyData1, ",\n\t\"unit\": \"lux\"\n\t}\n");

      strncat (postData, anyData1, 100);

      Serial.println(postData);

      lient.begin(serverAddress);

      client.addHeader("Content-Type", contentHeader);

      client.addHeader("Device-Token", tokenHeader);

      statusCode = client.POST(postData);


      Serial.print("Status code: ");

      Serial.println(statusCode);

      Serial.println("End of POST to TagoIO");

      Serial.println();
    }
  }




  



}
