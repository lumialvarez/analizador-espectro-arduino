# Analizador de Espectro con Arduino
Este analizador de espectro permite la visualización del comportamiento de una señal análoga en el dominio de la frecuencia. Dicho prototipo usa un método de muestreo que consiste en la toma de valores sucesivos, los cuales al terminar se procesan con por medio de la transformada rápida de Fourier ([FFT](https://es.wikipedia.org/wiki/Transformada_r%C3%A1pida_de_Fourier)).

El hardware se basa sobre la placa Arduino junto a un circuito electrónico y una pantalla LCD Multicolor que se conecta directamente a la placa.

#### Demostracion
Este es el proyecto funcionando
![Codigo Morse](https://github.com/lumialvarez/analizador-espectro-fft-arduino/blob/main/Recursos/foto_analizador_funcionamiento.jpg?raw=true)

#### Componentes Electronicos
Estos son los siguientes componentes usados:
 - Arduino (UNO, MEGA, Leonardo,etc)
 - Pantalla tactil TFT LCD SPFD5408 de 2.4 pulgadas
 - Shield Arduino para prototipado
 - Jack Hembra 3.5mm
 - 1 Resistencia 22kΩ
 - 1 Condensador 0.1µF

#### Esquema Electrico 
Este es el esquema electrico usado para el analizador de espectro
![Codigo Morse](https://github.com/lumialvarez/analizador-espectro-fft-arduino/blob/main/Recursos/esquema_electrico_analizador.png?raw=true)

#### Ensamble final
Estos son los componentes electronicos ya montados segun el esquema electrico
![Codigo Morse](https://github.com/lumialvarez/analizador-espectro-fft-arduino/blob/main/Recursos/ensamble_final.jpg?raw=true)

> Written with [StackEdit](https://stackedit.io/).
