unsigned long previousMillis = 0;
const long interval = 604800016.56;

//Variabel perhitungan konversi rotasi ke translasi kendali motor stepper
float step = 200; //jumlah step dalam satu putaran motor steper
float dpulley = 40; // diameter pulley
float degperstep = 360 / step; // sudut per 1 step
float kllpulley = dpulley * (22 / 7); // panjang lintasan perkeliling pulley
float lpersatstep = (degperstep / 360) * kllpulley; //panjang perstep pulley
float la = 500;// step asumsi 0.5 meter 
float lb = 1000;// step asumsi 1 meter
float lc = 1800; // step asumsi 1,5 meter
int stepla = la / lpersatstep;
int steplb = lb / lpersatstep;
int steplc = lc / lpersatstep;
int steplaa = stepla + 1;
int steplba = steplb + 1;
int steplca = steplc + 1;
int x = 0;

//parameter soilmoisture
int moisture1;
int moisture2;
int moisture3;
int percentage1;
int percentage2;
int percentage3;
int map_low1 = 300; //Kondisi basah sensor 88/540
int map_high1 = 592; // Kondisi kering sensor 1
int map_low2 = 275; //Kondisi basah sensor 2
int map_high2 = 618; // Kondisi kering sensor 2
int map_low3 = 270; //Kondisi basah sensor 3
int map_high3 = 643; // Kondisi kering sensor 3
int val = 0; // variabel pembantu

//I/O device
int sensorSoil1 = A0;
int sensorSoil2 = A1;
int sensorSoil3 = A2;
int relay = 11;
int relay2 = 10;
const int stepPin = 3;
const int dirPin = 4;

void setup() {

  pinMode(relay, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(relay, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  Serial.begin(9600);
}

void loop() {

  unsigned long currentMillis = millis();
  moisture1 = analogRead(sensorSoil1);// aktivasi object sensor soil 1
  moisture2 = analogRead(sensorSoil2);// aktivasi object sensor soil 2
  moisture3 = analogRead(sensorSoil3);// aktivasi object sensor soil 3
  percentage1 = map(moisture1, map_low1, map_high1, 0, 100);// Persentase kekeringan sensor 1
  percentage2 = map(moisture2, map_low2, map_high2, 0, 100);// Persentase kekeringan sensor 2
  percentage3 = map(moisture3, map_low3, map_high3, 0, 100);// Persentase kekeringan sensor 3

  Serial.print(" |Soil1 ");
  Serial.print(percentage1);
  Serial.print("%; ");
  Serial.print(" |Soil2 ");
  Serial.print(percentage2);
  Serial.print("%; ");
  Serial.print(" |Soil3 ");
  Serial.print(percentage3);
  Serial.println("%");

  delay(1000);

   if (currentMillis - previousMillis >= interval) 
  {
    digitalWrite(relay2, HIGH);
    previousMillis = currentMillis;
    val = 4;
  }
  else 
  {
    val = 0;
  }

  if (percentage1 > 50) {
    Serial.println("Sektor 1 akan disiram");
    delay(2000);
    val = 1;
  }
  else if (percentage2 > 75) {
    Serial.println("Sektor 2 akan disiram");
    delay(2000);
    val = 2;
  }
  else if (percentage3 > 70) {
    Serial.println("Sektor 3 akan disiram");
    delay(2000);
    val = 3;
  }
  else {
    Serial.println("Semua sektor dalam kondisi basah");
    delay(2000);
    val = 0;
  }
  switch (val) {
    case 0 :
      cektanah1 ();
      cektanah2 ();
      cektanah3 ();
      break;
    case 1 :
      point1();
      while ( percentage1 > 35) {
        cektanah1();
        delay(2000);
        digitalWrite(relay, LOW);
        Serial.println("Keran penyiram sektor 1 diaktifkan");
        delay(1000);
        penyiraman1();
        digitalWrite(relay, HIGH);
        Serial.println("Keran sektor 1 dinonaktifkan");
        cektanah1();
        delay(1000);
      }
      homing1();
      break;
    case 2 :
      point2();
      while ( percentage2 > 60 ) {
        cektanah2();
        digitalWrite(relay, LOW);
        Serial.println("Keran penyiram sektor 2 diaktifkan");
        delay(1000);
        penyiraman2();
        digitalWrite(relay, HIGH);
        Serial.println("Keran sektor 2 dinonaktifkan");
        cektanah2();
        delay(1000);
      }
      homing2();
      break;
    case 3 :
      point3();
      while ( percentage3 > 60 ) {
        cektanah3();
        digitalWrite(relay, LOW);
        Serial.println("Keran penyiram sektor 3 diaktifkan");
        delay(1000);
        delay(1000);
        penyiraman3();
        digitalWrite(relay, HIGH);
        Serial.println("Keran sektor 3 dinonaktifkan");
        cektanah3();
        delay(1000);
      }
      homing3();
      break;
    case 4 :
      point3();
      while ( val = 4 ) {
        digitalWrite(relay2, LOW);
        Serial.println("Keran penyiram pupuk diaktifkan");
        delay(1000);
        delay(1000);
        penyiraman4();
        digitalWrite(relay2, HIGH);
        Serial.println("Keran penyiraman pupuk dinonaktifkan");
        delay(1000);
      }
      homing3();
      break;
  }
}
void point1 () {
  digitalWrite(dirPin, HIGH);
  for (int x = 0; x < steplaa ; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENUJU POSISI 1 ");
    Serial.print(x * lpersatstep);
    Serial.println("mm");
  }
}
void point2 () {
  digitalWrite(dirPin, HIGH);
  for (int x = 0; x < steplba ; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENUJU POSISI 2 ");
    Serial.print(x * lpersatstep);
    Serial.println("mm");
  }
}
void point3 () {
  digitalWrite(dirPin, HIGH);
  for (int x = 0; x < steplca ; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENUJU POSISI 3 ");
    Serial.print(x * lpersatstep);
    Serial.println("mm");
  }
}
void homing1() {
  digitalWrite(dirPin, LOW);
  for (int x = steplaa; x > 0 ; x--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENUJU HOMING1 JARAK ");
    Serial.print(x * lpersatstep);
    Serial.println(" mm");
  }
  delay(5000);
}
void homing2() {
  digitalWrite(dirPin, LOW);
  for (int x = steplba; x > 0 ; x--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENUJU HOMING2 JARAK ");
    Serial.print(x * lpersatstep);
    Serial.println(" mm");
  }
  delay(5000);
}
void homing3() {
  digitalWrite(dirPin, LOW);
  for (int x = steplca; x > 0 ; x--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENUJU HOMING3 JARAK ");
    Serial.print(x * lpersatstep);
    Serial.println(" mm");
  }
  delay(5000);
}

void cektanah1 () {
  //digitalWrite(stepPin, LOW);
  baca();
  Serial.print(" Tingkat kekeringan tanah 1 : ");
  Serial.print(percentage1);
  Serial.print("%");
  Serial.print("| Raw 1 : ");
  Serial.println(moisture1);
  delay(3000);
}
void cektanah2 () {
  //digitalWrite(stepPin, LOW);
  baca();
  Serial.print(" Tingkat kekeringan tanah 2 : ");
  Serial.print(percentage2);
  Serial.print("%");
  Serial.print("| Raw 2 : ");
  Serial.println(moisture2);
  delay(3000);
}
void cektanah3 () {
  //digitalWrite(stepPin, LOW);
  baca();
  Serial.print(" Tingkat kekeringan tanah 3 : ");
  Serial.print(percentage3);
  Serial.print("%");
  Serial.print("| Raw 3 : ");
  Serial.println(moisture3);
  delay(3000);
}
void penyiraman1 () {
  digitalWrite(dirPin, HIGH);// arah putaran stepper berlawanan jarum jam
  for (int x = steplaa; x > steplaa - 200 ; x--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("LANGKAH PENYIRAMAN KANAN ");
    Serial.print(x * lpersatstep);
    Serial.println("mm");

  }
  delay(1000);
  digitalWrite(dirPin, LOW);// arah putaran stepper berlawanan jarum jam
  for (int x = steplaa - 200; x < steplaa; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("LANGKAH PENYIRAMAN KIRI ");
    Serial.print(x * lpersatstep);
    Serial.println("mm");
  }
  delay(1000);
}
void penyiraman2 () {
  digitalWrite(dirPin, HIGH);// arah putaran stepper berlawanan jarum jam
  for (int x = steplba; x > steplba - 200 ; x--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("LANGKAH PENYIRAMAN KANAN");
    Serial.print(x * lpersatstep);
    Serial.println("mm");

  }
  delay(1000);
  digitalWrite(dirPin, LOW);// arah putaran stepper berlawanan jarum jam
  for (int x = steplba - 200; x < steplba; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("LANGKAH PENYIRAMAN KIRI");
    Serial.print(x * lpersatstep);
    Serial.println("mm");
  }
  delay(1000);
}
void penyiraman3 () {
  digitalWrite(dirPin, HIGH);// arah putaran stepper berlawanan jarum jam
  for (int x = steplca; x > steplca - 200 ; x--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("LANGKAH PENYIRAMAN KANAN");
    Serial.print(x * lpersatstep);
    Serial.println("mm");

  }
  delay(1000);
  digitalWrite(dirPin, LOW);// arah putaran stepper berlawanan jarum jam
  for (int x = steplca - 200; x < steplca; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("LANGKAH PENYIRAMAN KIRI");
    Serial.print(x * lpersatstep);
    Serial.println("mm");
  }
  delay(1000);
}

void penyiraman4 () {
  digitalWrite(dirPin, HIGH);// arah putaran stepper berlawanan jarum jam
  for (int x = 0; x < steplca ; x++) { //int x = 0; x < steplca ; x++
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENYIRAM PUPUK KE KANAN");
    Serial.print(x * lpersatstep);
    Serial.println("mm");

  }
  delay(1000);
  digitalWrite(dirPin, LOW);// arah putaran stepper berlawanan jarum jam
  for (int x = steplca; x > 0 ; x--) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    Serial.print("MENYIRAM PUPUK KE KIRI");
    Serial.print(x * lpersatstep);
    Serial.println("mm");
  }
  delay(1000);
}
void baca() {
  moisture1 = analogRead(sensorSoil1);// aktivasi object sensor soil 1
  moisture2 = analogRead(sensorSoil2);// aktivasi object sensor soil 2
  moisture3 = analogRead(sensorSoil3);// aktivasi object sensor soil 3
  percentage1 = map(moisture1, map_low1, map_high1, 0, 100);// Persentase kekeringan sensor 1
  percentage2 = map(moisture2, map_low2, map_high2, 0, 100);// Persentase kekeringan sensor 2
  percentage3 = map(moisture3, map_low3, map_high3, 0, 100);// Persentase kekeringan sensor 3
}
void motoroff() {
  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, LOW);
}
