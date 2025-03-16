#include <Arduino_LSM9DS1.h>  
void setup() {
  Serial.begin(9600);
  while (!Serial);  

  // Initialise l'IMU
  if (!IMU.begin()) {
    Serial.println("Erreur : capteur IMU non détecté !");
    while (1); 
  }
  Serial.println("IMU initialisé avec succès !");
}

void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    // Envoie les valeurs sur le Serial Monitor, séparées par une virgule
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.println(z);
  }
  delay(100); 
