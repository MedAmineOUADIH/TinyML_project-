Partie 1 : Classification des Vibrations
Description
Cette partie du projet vise à classer différents types de vibrations en utilisant une carte Arduino Nano 33 BLE et son capteur IMU intégré. Le processus se déroule en trois étapes principales :
Collecte des données : Les données de vibrations sont générées et collectées à l'aide du capteur IMU de l'Arduino.
Entraînement du modèle : Un modèle de machine learning est entraîné pour reconnaître et classer les vibrations.
Inférence en temps réel : Le modèle est déployé sur l'Arduino pour effectuer des prédictions en temps réel.

Types de Fichiers :
Code Arduino :
data_generation.ino : Collecte les données de vibrations.
inference_vibrations.ino : Effectue l'inférence en temps réel.

Données :
vibrations.csv : Fichier contenant les données collectées.

Modèle :
model.tflite : Modèle entraîné au format TensorFlow Lite.

Notebook Jupyter :
training_vibrations.ipynb : Notebook pour l'entraînement du modèle.
