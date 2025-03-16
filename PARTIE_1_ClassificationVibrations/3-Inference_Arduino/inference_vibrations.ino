#include <Arduino_LSM9DS1.h> 
#include <TensorFlowLite.h>   
#include "vibrations_model.h" 

#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

const int tensor_arena_size = 2 * 1024;  
uint8_t tensor_arena[tensor_arena_size];

tflite::MicroErrorReporter error_reporter;
tflite::AllOpsResolver resolver;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!IMU.begin()) {
        Serial.println(" Impossible d'initialiser l'IMU ");
        while (1);
    }

    Serial.println(" IMU initialisé ");

    model = tflite::GetModel(vibrations_model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("modèle incompatible ");
        while (1);
    }

    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, tensor_arena_size, &error_reporter);
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        Serial.println(" Erreur !");
        while (1);
    }

    Serial.println(" Modèle chargé ");
    input = interpreter->input(0);
    output = interpreter->output(0);
}

void loop() {
    float x, y, z;

    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);

        Serial.print(" Accélération : ");
        Serial.print(x, 2);
        Serial.print(", ");
        Serial.print(y, 2);
        Serial.print(", ");
        Serial.println(z, 2);

        input->data.f[0] = x;
        input->data.f[1] = y;
        input->data.f[2] = z;

        if (interpreter->Invoke() != kTfLiteOk) {
            Serial.println("Échec de l'inférence ");
            return;
        }

        float prediction = output->data.f[0];

        Serial.print(" Résultat du modèle");
        Serial.println(prediction, 4);

        if (prediction > 0.5) {
            Serial.println("Etat Stable.");
        } else {
            Serial.println("Forte Vibration ");
        }
    }

    delay(500);  
}
