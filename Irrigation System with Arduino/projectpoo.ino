#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);
int stare;
//enumerare care se foloseste pentru setarea sistemului
typedef enum
{
 stare_oprit, stare_pornit, stare_inceput
}stare_sistem;

//clasa de baza
class Componenta
{
 protected:
 int pin;
 public:
 //constructor
 Componenta(int p)
 {
 pin = p;
 }
 // functie pentru setarea intrarii sau iesirii
 void setareIntrare_iesire(int intrare_iesire)
 {
 
 pinMode(pin, intrare_iesire);
 }
 // functie pentru returnarea pin-ului
 int returnarePin()
 {
 return pin;
 }
 //functie virtuala pentru oprire a unei componente
 virtual void SetareOprit() { }
 //functie virtuala pentru pornire a unei componente
 virtual void SetarePornit() { }
};
//clasa derivata Led din clasa de baza Componenta
class Led : public Componenta {
private:
 stare_sistem sist;
public:
 Led(int pin, stare_sistem s) : Componenta(pin)  //constructorul clasei derivate
{
 sist = s;
}
//setare pornire led
void SetarePornit()
{
 digitalWrite(pin, HIGH);
 sist = stare_pornit;
}
//setare oprire led
void SetareOprit()
{
 digitalWrite(pin, LOW);
 sist = stare_oprit;
}
};
 
// se foloseste pentru senzorul de umiditate
class SenzorUmiditate : public Componenta {
private:
 // valoarea citita de la senzor
 int valoareCitita;
 
// valoare pragului
 
int prag;
public:
 SenzorUmiditate(int pin, int prg) : Componenta(pin)
{
 prag=prg;
 
}
//returneaza valoarea de min/max a umiditatii
int returnarePrag()
{
 return prag;
}
 
//functie folosita care ajuta la aflarea valorii maxime a umiditatii dintr-un interval de tip
int umiditateMaxima(int , int );
//functie care returneaza valoarea umiditatii
int citesteUmiditate();
};
//functie care returneaza valoarea maxima a senzorului de umiditate
int SenzorUmiditate::umiditateMaxima(int pin, int count)
{
 //variabila care se foloseste pentru determinarea maximului
 int valMaxima = 0;
 
 for(int i = 0; i < count; i++)
 
{
 valMaxima = max(analogRead(pin), valMaxima);
 
}
 return valMaxima; }
//functie care returneaza valoare citita de la senzorul de vibratii
int SenzorUmiditate :: citesteUmiditate() {
 valoareCitita = umiditateMaxima(pin, 1000);
 return valoareCitita; }
 
//clasa derivata din clasa de baza Componenta_sistem
//butonul este folosit pentru a opri sau a incepe functionarea sistemului
class Buton : public Componenta
{
private:
 stare_sistem sist_stat;
public:
 //constructor
 Buton(int pin, stare_sistem s) : Componenta(pin) {
 sist_stat = s; }
//setarea butonului
void SetareStatus(stare_sistem s) {
 sist_stat = s; }
};
//declararea variabilelor folosite in program
stare_sistem stareSistem = stare_inceput;
Led LedAlbastru1(47, stare_oprit);
Led LedVerde1(49, stare_oprit);  //pentru stare
Led LedVerde2(33, stare_oprit); //pentru udat
Led LedAlbastru(31, stare_oprit);
int umiditate;
Buton buton1(43, stare_oprit);  //butonul pentru oprirea si pornirea sistemului
SenzorUmiditate s_umiditate(A8, 100);  //senzorul de umiditate


//functie folosita pentru a schimba starea sistemului la apasarea butonului
void schimbareStareSistem()
{
 // daca sistemul este oprit sistemul se va intoarce la inceput
 if (stareSistem == stare_oprit)
 {
 stareSistem = stare_pornit;  //in loc de inceput
 }
 else
 if (stareSistem == stare_pornit)
 {
 stareSistem = stare_oprit;
 }
}
//initializarea modului in care sunt setati pinii fiecarei componente (intrare / iesire)

void setup()
{
 lcd.begin(16,2);
 lcd.clear();
 // 1 umed, 2 uscat;
 LedAlbastru1.setareIntrare_iesire(OUTPUT);
 LedVerde1.setareIntrare_iesire(OUTPUT);  //stare
 LedVerde2.setareIntrare_iesire(OUTPUT);  //udare
 LedAlbastru.setareIntrare_iesire(OUTPUT);
 //buton
 buton1.setareIntrare_iesire(INPUT);
 s_umiditate.setareIntrare_iesire(INPUT);
stareSistem= stare_oprit;
}

//codul propriu-zis ce indeplineste functia ceruta
void loop()
{
 //se verifica in ce stare este sistemul
 switch (stareSistem)
 {
 //cazul in care sistemul este oprit; toate ledurile sunt oprite si difuzorul nu emite niciun sunet
 case stare_oprit:
 lcd.clear();
 lcd.setCursor(0,0);
lcd.print("Sistem Irigatii");
 LedAlbastru1.SetarePornit();
 LedVerde1.SetareOprit();
 LedVerde2.SetareOprit();
 LedAlbastru.SetareOprit();
 delay(1000);
 if (digitalRead(buton1.returnarePin()) == HIGH)
{
 schimbareStareSistem();
}
 break;
 //cazul in care sistemul a intrat in starea de inceput a programului
 case stare_inceput:
 LedVerde1.SetarePornit();
 stareSistem = stare_pornit;
 break;
 case stare_pornit:
 umiditate = s_umiditate.citesteUmiditate();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Umiditate: ");
lcd.print(umiditate);
delay(5000);
 if (s_umiditate.citesteUmiditate() < s_umiditate.returnarePrag())
 stare=1;
else
stare=2;
 
if(stare==1)
{
  LedAlbastru1.SetareOprit();
  LedVerde1.SetarePornit();
  LedVerde2.SetarePornit();
  LedAlbastru.SetareOprit();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sol uscat");
  lcd.setCursor(0,1);
  lcd.print("Pornim udarea");
  delay(1000);
  }
  else
  if(stare==2){
  LedAlbastru1.SetareOprit();
  LedVerde1.SetarePornit();
  LedVerde2.SetareOprit();
  LedAlbastru.SetarePornit();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sol umed");
  lcd.setCursor(0,1);
  lcd.print("Oprim udarea");
  delay(5000);
  }
 
 
 if (digitalRead(buton1.returnarePin()) == HIGH)
{
 schimbareStareSistem();
 break;
}
 
 break;
 }
}
