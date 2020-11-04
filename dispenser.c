
#include <LiquidCrystal_I2C.h> //Display
#include <Wire.h> //PadrÃ£o
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27,20,4);//LCD address 0x27 para display de 20x4
#include <EEPROM.h>
#define EEPROM_SIZE 512
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {26,27,14,12}; //Pinos das linhas do teclado
byte colPins[COLS] = {32,33,25}; //Pinos das colunas do teclado
Keypad teclado = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Inicializando o Sistema");
  delay(10);
  EEPROM.begin(EEPROM_SIZE);
}
void adicionaPreco(){
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Digite o Valor");
  char escolha ;
  int i;
 
  char capsula = teclado.waitForKey();
  capsula = capsula -0x30;
  char preco[5];
  for(int j=0; j<5; j++){
   preco[j] = teclado.waitForKey();
  if(preco[j]=='*'&&j>-1){
    j=j-1;
    lcd.setCursor(j+1, 2);
    lcd.print(' ');
        j--;
  }else{
  EEPROM.write(30+j+(capsula*5), preco[j]);
  EEPROM.commit();
  lcd.setCursor(j+1, 2);
  char leitura=EEPROM.read(30+j+(capsula*5));
  lcd.print(leitura);
    }
  }
  lcd.setCursor(1,3);
  lcd.print("Deseja adicionar outro valor? *'sim', #'voltar'.");
  escolha = teclado.waitForKey();
  if(escolha == '*'){
    adicionaPreco();
    
  }else{
    lcd.setCursor(1,1);
    lcd.clear();
    lcd.print("preco adicionado");
  }
}       
void escolherCapsula(char adolfo){
  int id = (int)adolfo - 48;
  int quantidade = (int)EEPROM.read((id-1)*2) - 48;
  quantidade = quantidade +  ((int)EEPROM.read((id-1)*2) - 48)*10;
  if(quantidade>0){ 
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Capsula escolhida:");
  lcd.setCursor(1,1);
  lcd.print(id);
  lcd.setCursor(1,2);
  lcd.print("Confirma: digite *");
  char confirmar = teclado.waitForKey();
  if(confirmar=='*'){
    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("Capsula ");
    lcd.setCursor(9, 1);
    lcd.print(id);
    lcd.setCursor(1, 2);
    lcd.print("escolhida");
    delay(500);
    lcd.setCursor(1,3);
    lcd.print("aproveite :)");
    delay(1000);
    quantidade--;
    int Quantidade[2];
    Quantidade[0] = quantidade/10;
    Quantidade[1] = quantidade%10;
    EEPROM.write((id-1)*2+1, Quantidade[0]);
    EEPROM.commit();
    EEPROM.write((id-1)*2, Quantidade[1]);
    EEPROM.commit();
    
  }     
  }else{
  lcd.setCursor(1, 1);
  lcd.clear();
  lcd.print("infelizmente acabou");
  delay(500);
  lcd.setCursor(1, 2);
  lcd.print("   :(");
  delay(1000);
  }
}
void alterarSenha(){
  int i=0;
  lcd.setCursor(1,1);
  lcd.clear();
  lcd.print("digite nova senha");
  char senhaNova[5];
  senhaNova[i] = teclado.waitForKey();
 
  while(i!=5){
  if(i!=0) senhaNova[i] = teclado.waitForKey();
  lcd.setCursor(i,2);
  lcd.print(senhaNova[i]);
  if(i==4)delay(1000);
  i++;
  }
  lcd.setCursor(1,1);
  lcd.clear();
  lcd.print("se confirma digite #");
  char confirma = teclado.waitForKey();
  if(confirma != '#') alterarSenha();
  i = 0;
  char Senha;
  while(i!=5){
  EEPROM.write(i+11, senhaNova[i]);
  EEPROM.commit();
  i++;
  }
  lcd.setCursor(1,1);
  lcd.clear();
  lcd.print("senha atualizada");
}
void acessoAdmin(){
  lcd.setCursor(1, 1);
  lcd.clear();
  lcd.print("digite a senha");
  
  char senhaIn[5];
  int j=0;
  int i=0;
    
   for(i=0; i<5; i=i+1){
    
    lcd.setCursor(i+1, 2);
    senhaIn[i]= teclado.waitForKey();
    if(senhaIn[i] == '*'){
      i=i-1;
      lcd.setCursor(i+1, 2);
      
    
      
    }else{
    
    lcd.print(senhaIn[i]);
    if(EEPROM.read(i+11) == senhaIn[i]){
      j++;
    }
  }
   }
      
    if(j==5){
          areaAdmin();
    }else{
          lcd.clear();
          lcd.setCursor(2,1);
          lcd.print("Senha Incorreta!");
          lcd.setCursor(2,2);
          lcd.print("Voltando para o menu");
          loop();
          
       
       }
}

void areaAdmin(){
  lcd.setCursor(1,1);
  lcd.clear();
   lcd.print("Area do Admin");
  delay(1000);
  lcd.setCursor(1,1);
  delay(1000);
  lcd.clear();
  lcd.setCursor(1,0);
 
  lcd.print("1:mudar senha");
 
  lcd.setCursor(1,1);
 
  lcd.print("2:atualizar capsulas");
 
  lcd.setCursor(1,2);
 
  lcd.print("3:atualizar preco");
 
  lcd.setCursor(1,3);
 
  lcd.print("outro:logoff");
  char  c = teclado.waitForKey();
  if(c=='1') alterarSenha();
  else if(c=='2') atualizarQuantidadeCapsulas();
  else if(c=='3') adicionaPreco();
}
void atualizarQuantidadeCapsulas(){ 
  char recebe[3];
  int i=1;
 
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("id entre 1 e 5");
  delay(1000);
 
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("dgt id da caps:");
  recebe[0] = teclado.waitForKey();
  int id = (int)recebe[0] - 48;
  if(id>6 || id<1) return atualizarQuantidadeCapsulas();
 
  lcd.setCursor(1,2);
  lcd.print(recebe[0]);
  delay(1000);
 
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("confirma? digite #");
 
  char confirma = teclado.waitForKey();
  if(confirma == '#'){
  
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("qtd de capsulas (2 dgts):");
  
  recebe[i] = teclado.waitForKey();
  lcd.clear();
  while(i!=3){
    if(i==2) recebe[i] = teclado.waitForKey();
    lcd.setCursor(i,2);
    lcd.print(recebe[i]);
    delay(1000);
    i++;
  }
  
  int quantidade = (int)recebe[2] - 48;
  quantidade = quantidade + ((int)recebe[1] - 48)*10;
  lcd.clear();
  lcd.setCursor(1,1); 
  lcd.print("quantidade = ");
  delay(1000);
    
  lcd.clear();
  lcd.setCursor(1,1); 
  lcd.print(quantidade);
  delay(1000);
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("confirma? digite #");
  confirma = teclado.waitForKey();  
  if(confirma == '#'){
    i=0;
    int Quantidade[2];
    Quantidade[0] = quantidade/10;
    Quantidade[1] = quantidade%10;
    while(i!=2){
      EEPROM.write((id-1)*2+i+1, Quantidade[i]);
      EEPROM.commit();
      i++;
    }
    
    
  }
  else
    atualizarQuantidadeCapsulas();  
  }else
  atualizarQuantidadeCapsulas();  
}
void loop(){
  
  lcd.setCursor(1, '0');
  lcd.clear();
  lcd.print("Boa noite");
  delay(500);
  lcd.setCursor(1, 1);
  lcd.print("Escolha a capsula");
  delay(500);
  lcd.setCursor(1, 2);
  lcd.print("digitando de 1 a 5");
  delay(500);
  delay(500);
  char capsula = teclado.waitForKey();
  if(capsula != '*'){
  escolherCapsula(capsula);
  }else{
  acessoAdmin();
  }   
}






