#include <iostream>
#include <string>
#include <unistd.h>
#include "bbb_simple_api.h"

using namespace std;

unsigned TILT_DIR_GPIO  = 60;   // GPIO1_28 = (1x32) + 28 = 60
unsigned int PAN_DIR_GPIO = 48;   // GPIO1_16 = (1x32) + 16 = 48

int main(int argc, char *argv[]){

	cout << "Setting GPIO pins.." << endl;

	//Set direction pins
        gpio_export(TILT_DIR_GPIO);    // Tilt motor direction pin
	gpio_export(PAN_DIR_GPIO);   // Pin motor direction pin
        gpio_set_dir(TILT_DIR_GPIO, OUTPUT_PIN);   // The LED is an output
	gpio_set_dir(PAN_DIR_GPIO, OUTPUT_PIN);   // The push button input

	for(int i=0; i<5; i++){
		cout << "Setting the LED on" << endl;
                gpio_set_value(TILT_DIR_GPIO, HIGH);
		gpio_set_value(PAN_DIR_GPIO, LOW);
		usleep(200000);         // on for 200ms

                gpio_set_value(TILT_DIR_GPIO, LOW);
		gpio_set_value(PAN_DIR_GPIO, HIGH);
		usleep(200000);         // off for 200ms
	}

	cout << "Finished Testing the GPIO Pins" << endl;
	gpio_unexport(TILT_DIR_GPIO);     // unexport the LED
	gpio_unexport(PAN_DIR_GPIO);  // unexport the push button
	return 0;
}

