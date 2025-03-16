
import numpy as np
import pandas as pd
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder

data = pd.read_csv("data1.csv")
print(data.head())

encoder = LabelEncoder()
data["label"] = encoder.fit_transform(data["label"])

print(data.head())
print("Classes encodées :", dict(enumerate(encoder.classes_)))

data["label"].value_counts().plot(kind='bar')
plt.xlabel("Classes de vibration")
plt.ylabel("Nombre d'échantillons")
plt.title("Répartition des classes")
plt.show()

X = data[['x', 'y', 'z']].values
y = data['label'].values

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

print("Taille des données d'entraînement :", X_train.shape)
print("Taille des données de test :", X_test.shape)

model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(3,)),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dropout(0.2),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(8, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')
])

model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),
              loss='binary_crossentropy',
              metrics=['accuracy'])

history = model.fit(X_train, y_train, epochs=100, batch_size=8, validation_data=(X_test, y_test))

converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

with open("vibrations_model.tflite", "wb") as f:
    f.write(tflite_model)

print("Modèle sauvegardé sous vibrations_model.tflite")

from google.colab import files
files.download("vibrations_model.tflite")

with open("vibrations_model.tflite", "rb") as f:
    tflite_model = f.read()

hex_array = ", ".join("0x{:02x}".format(c) for c in tflite_model)

cpp_code = f"""
#ifndef VIBRATIONS_MODEL_H
#define VIBRATIONS_MODEL_H

alignas(8) const unsigned char vibrations_model_tflite[] = {{
    {hex_array}
}};
const int vibrations_model_tflite_len = {len(tflite_model)};

#endif  // VIBRATIONS_MODEL_H
"""

with open("vibrations_model.h", "w") as f:
    f.write(cpp_code)

print("Modèle converti")
