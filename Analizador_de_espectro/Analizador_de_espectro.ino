/*
   Analizador_de_espectro_2.0.ino
    Autor: Luis Miguel Alvarez
    Fecha: 20/04/2014
    Analizador de espectro, que permite el analisis la visualizacion del comportamiento de una senal analoga en el dominio de la frecuencia.
    Analizador de espectro Arduino, permite la medicion de las senales electricas en el tiempo con un rango de operacion
    de 0 a 5 Voltios (Capaz de soportar hasta 20).
    IMPORTANTE!!! Este codigo requiere de la libreria fft.ino

    Probado con Arduino IDE 1.6.2, tarjetas Arduino MEGA y Leonardo.
*/

// Definicion de los colores en hexadecimal
#include <Adafruit_GFX.h>    // Core graphics library
#include <SWTFT.h> // Hardware-specific library
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Definicion del Objeto que gestiona el funcionamiento de la pantalla
SWTFT tft;

// Maxima cantidad de datos que se muestran en la pantalla (Un barrido completo)
const int n_datos_lectura = 128;
const int n_datos_graficados = 64;

// estructuras dondese guardan los datos
char im[n_datos_lectura];
char datos[n_datos_lectura];
char datos_anteriores[n_datos_graficados];

//datos de configuracion
int pin_captura_datos = A5; //pin de entrada de datos en el dispositivo
float altura_barra_multiplo = 2;
int ancho_barra = 3;

//Frecuencias maxima y minimas teoricas (que se muesttran en pantalla)
float min_frecuencia = 7.8;
float max_frecuencia = 500;

unsigned long tt;

/* 
   Este metodo se encarga de inicializar variables, objetos y la presentacion de inicial del dispositivo.
   Adicionalmente se envian datos por el Serial (El ID, ancho y alto de la pantalla) a modo informativo.
*/
void setup() {
  Serial.begin(9600);

  tft.reset();
  uint16_t identifier = tft.readID();
  Serial.print("LCD driver chip: ");
  Serial.println(identifier, HEX);
  tft.begin(identifier);
  Serial.print("width: "); 
  Serial.println(tft.width());
  Serial.print("height: "); 
  Serial.println(tft.height());
  tft.setRotation(3);
  graficar_presentacion_inicial();
  graficar_adornos();
}


/* 
  Este metodo principal que se repite "infinitamente", se encarga de la captura de los datos y posteriormente llama al 
   metodo que grafica el dato obtenido.
*/
void loop() {
  int static i = 0;

  int val;
  if (millis() > tt) {
    if (i < n_datos_lectura) {
      val = analogRead(pin_captura_datos);
      datos[i] = val / 4 - 128;
      im[i] = 0;
      i++;

    }
    else {
      //this could be done with the fix_fftr function without the im array.
      fix_fft(datos, im, 7, 0);
      // I am only interessted in the absolute value of the transformation
      for (i = 0; i < n_datos_graficados; i++) {
        datos[i] = sqrt(datos[i] * datos[i] + im[i] * im[i]);
      }


      //do something with the datos values 1..64 and ignore im
      graficar_datos_pantalla();
    }
    tt = millis();
  }

}


/**
*  Funcion para mostrar en la pantalla el valor de los datos capturados LCD
*/
void graficar_datos_pantalla() {
  for (int k = 0 ; k < n_datos_graficados ; k++) {
    int indice = k; //n_datos_graficados - k;  para invertir la formna en que se grafica
    char dato = abs((datos[k] * altura_barra_multiplo));
    char dato_anterior = abs((datos_anteriores[k] * altura_barra_multiplo));
    dibujar_barra(ancho_barra, 30 + (indice * (ancho_barra + 1)), 200, dato, dato_anterior);
    datos_anteriores[k] = datos[k];
  }
}


/* 
   Este metodo muestra graficamente la magnitud del dato de entrada en la pantalla. Esto funciona al borrar la magnitud 
   graficada anteior y grafica el nuevo valor.
  
   @param magnitud Entero que representa la magnitud de la senal a graficar (entre 0 y 'altura_graficos').
*/
void dibujar_barra(int ancho_barra, int x, int y, char dato, char dato_anterior) {
  if ( dato_anterior > dato) {
    tft.fillRect(x, y - dato_anterior, ancho_barra, dato_anterior - dato, BLACK);
  } else {
    tft.fillRect(x, y - dato, ancho_barra, dato - dato_anterior, WHITE);
  }
}

/* 
   Este metodo muestra graficamente una presentacion que se muestra al inicio de la opeacion del dispositivo.
*/
void graficar_presentacion_inicial() {
  tft.fillScreen(BLACK);
  tft.setCursor(30, 50);
  tft.setTextColor(RED);    
  tft.setTextSize(3);
  tft.println("Analizador de");
  tft.setCursor(50, 90);
  delay(300);
  tft.println("Espectro V2.0");


  tft.setCursor(30, 180);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("By:  Luis Miguel Alvarez A.");
  tft.setCursor(60, 195);
  tft.println("Yulian Andres Zapata M.");

  for (int i = 0 ; i < 320 ; i++) {
    tft.fillRect(i, 238, 2, 1, WHITE);
    delay(10);
  }
  delay(100);
  tft.fillScreen(BLACK);
}

/* 
   Este metodo muestra graficamente las marcas de referencia y etiquetas que van a quedar estaticas una vez que se inicie
   la operaciond del dispositivo.
*/
void graficar_adornos() {
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.setCursor(80, 0);
  tft.print("Analizador de Espectro V2.0");
  delay(300);
  tft.setCursor(30, 30);
  tft.print("Las frecuencias son un aproximado\n      Tasa de muestreo aprox: 1ms");

  tft.setCursor(30, 210);
  tft.print(min_frecuencia); tft.print(" Hz");

  tft.setCursor(((ancho_barra + 1) * n_datos_graficados) - 20, 210);
  tft.print(max_frecuencia); tft.print(" Hz");
  delay(300);
  tft.drawRect(29, 50, ((ancho_barra + 1) * n_datos_graficados) + 1, 151, WHITE);
  delay(300);
}