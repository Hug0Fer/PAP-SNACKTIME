#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino_LED_Matrix.h>

extern ArduinoLEDMatrix matrix;  // usar a mesma do .ino

// ======================
// Animação "NO!" - cada frame tem 3 cores
// ======================
const uint32_t animation[][3] = {
    {
        0x00cceed,
        0xffdbddfc,
        0xce000000
        },
    {
        0x20cceed, 
        0xffdbddfc, 
        0xce000000
        }
};

// Delay de cada frame
const int frameDelay[] = {66, 66};

// ======================
// Função para mostrar animação
// ======================
void showObstacleAnimation() {
    for (int i = 0; i < sizeof(animation)/sizeof(animation[0]); i++) {
        matrix.loadFrame(animation[i]);   // carrega o frame (3 cores)
        delay(frameDelay[i]);             // delay do frame
    }

    matrix.clear();  // limpa no final
}

#endif
