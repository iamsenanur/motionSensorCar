#include <SoftwareSerial.h>

#define echopin 13 // Ultrasonik sensörün echo pini(alınan ses dalgası)
#define trickpin 12 // Ultrasonik sensörün trigger pini(yayılan ses dalgası)
#define buzzer 4 // Buzzer pini(ses çıkaran parça)
#define led 11 // LED pini
const int motorA_IN1 = 10; // Motor A IN1 pini//sol motor ileri
const int motorA_IN2 = 9;  // Motor A IN2 pini //sol motor geri
const int motorB_IN1 = 8;  // Motor B IN1 pini //sağ motor geri 
const int motorB_IN2 = 7;  // Motor B IN2 pini//sağ motor ileri
const int enableA = 5;     // Motor A enable pini//sol motorun hızını kontrol eden pin
const int enableB = 6;     // Motor B enable pini//sağ motorun hızını kontrol eden pin

int veri; // Bluetooth üzerinden gelen veri
int maxirange = 20; // Maksimum mesafe aralığı
int minirange = 0;   // Minimum mesafe aralığı

void setup() {
  Serial.begin(9600); // Seri haberleşmeyi başlat
  
  // Pin modlarını ayarla
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorB_IN1, OUTPUT);
  pinMode(motorB_IN2, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(trickpin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // Bluetooth üzerinden veri gelirse oku
  if (Serial.available() > 0) {      
    veri = Serial.read();   // Gelen veriyi oku
  }

  // Gelen veriye göre motorları kontrol et
  if (veri == 'i') { 
    // İleri hareket
    digitalWrite(motorB_IN2, HIGH);//sağ motor ileri
    digitalWrite(motorB_IN1, LOW);//sağ motor geri
    analogWrite(enableB, 20);
    digitalWrite(motorA_IN2, LOW);//sol motor geri
    digitalWrite(motorA_IN1, HIGH);//sol motor ileri
    analogWrite(enableA, 20);
  }
  else if (veri == 'g') { //interrupt
    // Geri hareket
    digitalWrite(motorB_IN2, LOW);//sağ motor ileri
    digitalWrite(motorB_IN1, HIGH);//sağ  motor geri
    analogWrite(enableB, 20);
    digitalWrite(motorA_IN2, HIGH);//sol motor geri
    digitalWrite(motorA_IN1, LOW);//sol motor ileri
    analogWrite(enableA, 20);
    int olcum = mesafe(maxirange, minirange); // Mesafe ölçümü yap
    melodi(olcum * 10); // Buzzer çal
    ledf(olcum * 10);   // LED yanıp sön
  }
  else if (veri == 's') { 
    // Sağ motor geri, sol motor ileri hareket
    digitalWrite(motorB_IN2, HIGH);//sağ motor ileri
    digitalWrite(motorB_IN1, LOW);//sağ motor geri
    analogWrite(enableB, 60);
    digitalWrite(motorA_IN2, LOW);//sol motor geri
    digitalWrite(motorA_IN1, HIGH);//sol motor ileri
    analogWrite(enableA, 60);
  }
  else if (veri == 'o') { 
    // Sağ motor ileri, sol motor geri hareket
    digitalWrite(motorB_IN2, HIGH);//sağ motor ileri
    digitalWrite(motorB_IN1, LOW);//sağ motor geri
    analogWrite(enableB, 20);
    digitalWrite(motorA_IN2, LOW);//sol motor geri
    digitalWrite(motorA_IN1, HIGH);//sol motor ileri
    analogWrite(enableA, 60);
  }
  else if (veri == 'd') { 
    // Motorları durdur
    digitalWrite(motorB_IN2, LOW);//sağ motor ileri
    digitalWrite(motorB_IN1, LOW);//sağ motor geri
    analogWrite(enableB, 20);
    digitalWrite(motorA_IN2, LOW);//sol motor geri
    digitalWrite(motorA_IN1, LOW);//sol motor ileri
    analogWrite(enableA, 20);
  }
  else if (veri == 'm') { 
    // İleri hareket
    digitalWrite(motorB_IN2, HIGH);//sağ motor ileri
    digitalWrite(motorB_IN1, LOW);//sağ motor geri
    analogWrite(enableB, 20);
    digitalWrite(motorA_IN2, HIGH);//sol motor geri
    digitalWrite(motorA_IN1, LOW);//sol motor ileri
    analogWrite(enableA, 20);
  }
  else {
    // Varsayılan durumda motorları durdur
    digitalWrite(motorB_IN2, LOW);
    digitalWrite(motorB_IN1, LOW);
    analogWrite(enableB, 20);
    digitalWrite(motorA_IN2, LOW);
    digitalWrite(motorA_IN1, LOW);
    analogWrite(enableA, 20);
  }
}

// Ultrasonik sensör ile mesafe ölçümü//timer
int mesafe(int maxrange, int minrange) {
  long duration, distance;//duration(yankının geri dönme süresini mikrosaniyee)//distance u süreyi kullanarak hesaplanan mesafe santimetre
  digitalWrite(trickpin, LOW);//tiger pinini low yaparak sensörü başlatır
  delayMicroseconds(2);//2 mikrosaniye bekler
  digitalWrite(trickpin, HIGH);
  delayMicroseconds(10);//tiger pinini high yaparak 10 mikrosaniye boyunca bir ses dalgası gönderir
  digitalWrite(trickpin, LOW);//tekrar low yaparak sensörün yankıyı dinlemesini sağlar

  duration = pulseIn(echopin, HIGH);//eco pinin high olduğu süreyi mikrosaniye cinsinden ölçer ve bu süre suration değişkenine atanır.
  distance = duration / 58.2;&&yankının geri dönme süresi kulanılarak mesafe hesaplanır. 58.2 sabiti mikrosaniye cinsinden yankının dönüş süresinin santimetreye çevirmek için kullanılır.
  delay(50);

  if (distance >= maxrange || distance <= minrange)
    return 0;

  return distance;//geçerli ölçüm sonucu olarak hesaplanan mesafeyi döner
}

// Buzzer ile ses çıkarma
int melodi(int dly) {//dly parametrei, sesin ne kadar süreyle çalacağını belirler
  tone(buzzer, 470);//tone fonksiyonu belirtilen frekansta(470 Hz)buzzerdan ses çıakrmak için kullanılır
  delay(dly);//dly süresi boyunca bekler
  noTone(buzzer);//buzzerı kapatır
  delay(dly);
}

// LED'i yanıp söndürme
int ledf(int ledy) {//ledi belirli bir süre boyunca yanıp söndürür
  digitalWrite(led, HIGH);//led pinine high sinyali göndererek ledi yakar
  delay(ledy);//ledy süresi boyunca bekler
  digitalWrite(led, LOW);//led pinine low sinyali göndererek sinyali söndürür
  delay(ledy);//ledy süresi boyunca tekrar bekler. bu, led'in yanıp sönmesini sağlar
}
