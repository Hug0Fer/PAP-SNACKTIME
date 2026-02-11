#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino_LED_Matrix.h>

extern ArduinoLEDMatrix matrix;  // usar a mesma do .ino

// Animação "NO!" 

const uint32_t animation[][4] = {
	{
		0xcceed,
		0xffdbddbc,
		0xdfcce000,
		66
	},
	{
		0xcceed,
		0xffdbddbc,
		0xdfcce000,
		66
	},
	{
		0xcceed,
		0xffdbddbc,
		0xdfcce000,
		66
	}
};
// Delay de cada frame
const int frameDelay[] = {66, 66};

// Função para mostrar animação

void showObstacleAnimation() {
    for (int i = 0; i < sizeof(animation)/sizeof(animation[0]); i++) {
        matrix.loadFrame(animation[i]);   // carrega o frame
        delay(frameDelay[i]);             // delay do frame
    }

    matrix.clear();  // limpa no final
}

#endif
