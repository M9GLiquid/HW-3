#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

void Init_LightSensor(void);

// Handlers
void ADC_Handler(void);
void Read_Light(void);
void Light_Measurement(void);

#endif