//DORA VERSION 1
#include <Arduino.h>
// TARJETA DE MEMORIA
#include <SPI.h>
#include <SD.H>
// DISPLAY MAX  CAMBIO
// #include <MD_MAX72xx.h>
// #include <MD_Parola.h>

#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <time.h>
#include "Esp32Time.h"
#include <Preferences.h>
#include <Wire.h> //i2c
#include "Auth.h"
#include <Adafruit_NeoPixel.h>

#include <esp_now.h>

// variables para mostrar fecha en posicion año/mes_dia para envio firebase
String dia_envio = "2021/01/01";
String hora_envio = "00:00:01";

// inicializacin de fecha
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -21600;
const int daylightOffset_sec = 0; // falta el void de ajuste verano +3600

//------------------------------

// DISPLAY MAX
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // Módulo a usar para el control de led FC16
#define MAX_DEVICES 4                     // Número de matrices 8x8 conectadas

// estados booleanos
#define SI 1
#define NO 0

// MEMORIA SD
#define SSpin 5 // memoria SD , pin en ESP32

// seleccion de juego por WebServer
int num_juego = 1;
#define UNO 1
#define DOS 2
#define CUATRO 4
#define OCHO 8

// seleccion de tipo de round
#define VELOCIDAD_Y_RAFAGA 1
#define TIROS_POR_RAFAGA 3
#define TIRADOR_1 1
#define TIRADOR_2 2
#define TIRADOR_3 3
#define TIRADOR_4 4
#define TIRADOR_5 5
#define TIRADOR_6 6
#define TIRADOR_7 7
#define TIRADOR_8 8

#define DUELO_FINAL_SI 1
#define DUELO_FINAL_NO 0

// posicion de jugador en matriz TiradoresRojo[] o TiradoresAzul[]
#define GANADOR_EN_POSICION_ROJO 1
#define GANADOR_A_POSICION_AZUL 2

// cantidad de round por duelo
#define REALIZA_3_ROUNDS 3
#define REALIZA_5_ROUNDS 5
#define REALIZA_7_ROUNDS 7
#define REALIZA_8_ROUNDS 8
#define SEG_PAUSA_ENTRE_ROUNDS 3

// tipos de juegos
#define juego_1 1
#define juego_2 2
#define juego_4 4
#define juego_8 8

// avance del ganador en duelos
#define GANADOR_A_DUELO_2 2
#define GANADOR_A_DUELO_4 4
#define GANADOR_A_DUELO_5 5
#define GANADOR_A_DUELO_6 6
#define GANADOR_A_DUELO_7 7 // sin usar
#define GANADOR_CASILLA_8 8

#define VERDE 1
#define AZUL 2
#define ROJO 3

#define FIJO 1
#define FLASHEO 2
#define CIRCULO 3
#define IMPACTO 4

int inicia_juego = 1;
int tiradores_preparados = NO;
int tirador_1_listo = SI;
int tirador_2_listo = SI;
int num_duelo = 0;
int ganador_duelo = 0;

// variables por round
int termino_el_round = 0;

// variables de grupo de round
int roundi_nicial = 1;
int terminaron_rounds = NO;
int total_duelos_ganados_rojo = 0;
int total_duelos_ganado_sazul = 0;
int velocidad = 1;
int rafaga = 2;
int tipo_de_duelo = 1;

// variables juego
int reiniciar_jugadores_juego_1 = SI; // Se usa para reiniciar los datos antes de cada juego
int reiniciar_jugadores_juego_2 = SI;
int reiniciar_jugadores_juego_4 = SI;
int reiniciar_jugadores_juego_8 = SI;
int duelos_ganados_rojo = 0;  // Valido solo para juego1 y juego2
int duelos_ganados_azul = 0;  // Valido solo para juego1 y juego2
int round_ganados_rojo = 0;   // Valido solo para juego1 y juego2
int round_ganador_azul = 0;   // Valido solo para juego1 y juego2
int mejor_velocidad_rojo = 0; // Valido solo para juego1 y juego2
int mejor_rafaga_rojo = 0;    // Valido solo para juego1 y juego2
int mejor_velocidad_azul = 0; // Valido solo para juego1 y juego2
int mejor_rafaga_azaul = 0;   // Valido solo para juego1 y juego2
volatile unsigned long reloj = 0;

// CREDITOS
uint16_t num_creditos = 200;

// variables de control de solicitar firma
int rojo_listo = NO;
int azul_listo = NO;

// son 8 duelos del 0 al 7 y se fijan quien compite contra quien
// Ejemplo juego 8: el resultado de duelo 0 (1 contra 2) el ganador se pone en tirador_rrojo[4]
// El resultado de duelo1 (3 vs 4) el ganador se escribe en tiradorazul[4]
//             0,1,2,3,4,5,6,7,8
int TiradoresRojo[9] = {1, 3, 5, 7, 0, 0, 0, 0, 0};
int TiradoresAzul[9] = {2, 4, 6, 8, 0, 0, 0, 0, 0};

// declaracion de funciones
void ejecutajuegoUno();
void ejecutajuegoDos();
void ejecutajuegoCuatro();
void ejecutajuegoOcho();
void muestraGanadorjuego4y8();
void muestraGanadorjuego2();
void acumulaDatosParajuego1y2();
void esperaFirmaJugadores(int Ref_duelo, int duelo_final, int tirador_rrojo, int tiradorazul, int num_rounds);
void ejecutaCiclosRound();
void ejecutaDuelo(int juego, int Ref_duelo, int duelo_final, int tirador_rrojo, int tiradorazul, int tipoduelo,
                  int tirosrafaga, int num_rounds, int segundosentrerouds, int nuevoduelo, int posicioncolor);
void mensajeDuelo(int Ref_duelo, int tirador_rrojo, int num_rounds);
void solicitaFirmaRojo(int Ref_duelo, int tirador_rrojo, int num_rounds);
void monitoreaFirmaRojo();
void solicitaFirmaAzul(int Ref_duelo, int tiradorazul, int num_rounds);
void recibeFirmaAzul();
void enciendeBox(int tipoencendido, int verde);
void mensajeSolicitudFirma();

// reloj para mensajes y led
ulong previous_time, current_time; // para sincronizar encendido led y duracion de mensajes matriz

// estructura para ESPNOW firma de jugadores inicio de duelo y round OK
typedef struct flag_listo
{
  int rojolisto; // Duelo tirador rojo  listo 0,1
  int azullisto; // Duelo tirador azul listo  0,1
  int numduelos;
  int numjugador;
} flag_listo;
flag_listo MasterBanderasDuelo;
flag_listo AzulBanderasDuelo;
flag_listo RojoBanderasDuelo;

// Informacion de round
typedef struct send_master_round_param
{
  int dfinal;   // duelo final SI/NO
  int nround;   // round a pelear
  int tipo;     // tipo de round
  int nrafaga;  // num de tiros en caso de round de rafaga
  int segpausa; // espera entre rounds
} send_master_round_param;
send_master_round_param ParametrosParaCadaRound;

// direccion MAC del receptor
/*en ejemplo se pone la direccion MAC del receptor ( 30:AE:A4:07:0D:64)
debe cambiarse por la nueva del receptor  ver nota 1*/

uint8_t broadcastAddress[] = {0x30, 0xAE, 0xA4, 0x07, 0x0D, 0x64};

void iniciaDatosBanderasDuelo()
{ // reinicia estructuras de banderas de inicio de duelo
  MasterBanderasDuelo.rojolisto = 0;
  MasterBanderasDuelo.azullisto = 0;
  MasterBanderasDuelo.numduelo = 0;
  MasterBanderasDuelo.numjugador = 0;
  RojoBanderasDuelo.rojolisto = 0;
  RojoBanderasDuelo.azullisto = 0;
  RojoBanderasDuelo.numduelo = 0;
  RojoBanderasDuelo.numjugador = 0;
  AzulBanderasDuelo.rojolisto = 0;
  AzulBanderasDuelo.azullisto = 0;
  AzulBanderasDuelo.numduelo = 0;
  AzulBanderasDuelo.numjugador = 0;
}

void setup()
{ // Inicia comunicación serial
  Serial.begin(115200);

  // Inicia tarjeta de memoria, debe estar en FAT32
  Serial.println("Iniciando tarjeta de memoria ...2");
  if (SD.begin(SSpin)) // iniciando en el pin10, checar para esp32
  {
    Serial.println("fallo de inicialización de tarjeta");
    return;
  }

  // Inicia Reloj
  ESP32Time rtc32;
  // Inicia los valores de las estructuras de banderas
  iniciaDatosBanderasDuelo();
}

void loop()
{ //
  current_time = millis();
  switch (num_juego) // se selecciona mediante pagina web
  {
  case UNO:
  {
    ejecutajuegoUno();
    break;
  }
  case DOS:
  {
    ejecutajuegoDos();
    break;
  }
  case CUATRO:
  {
    ejecutajuegoCuatro();
    break;
  }
  case OCHO:
  {
    ejecutajuegoOcho();
    break;
  }
  }
}

void ejecutajuegoUno()
{
  if (reiniciar_jugadores_juego_1 == SI)
  {
    reiniciaDatosJuego1();
    reiniciar_jugadores_juego_1 = NO;
  }
  switch (num_duelo)
  {
  case 0: // duelo 0 - Tirador 1 vs tirador 0  ganador a TiradoresRojo[8]
  {
    ejecutaDuelo(juego_1, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 1: // duelo 1 tirador 1 vs tirador 0  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_1, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 2: // duelo 1 tirador 1 vs tirador 0  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_1, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 3: // duelo 3  tirador 1 vs tirador 0  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_1, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 4: // duelo 4 tirador 1 vs tirador 0  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_1, num_duelo, DUELO_FINAL_SI, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 8: // Resultados final con ganador juego en posicion TiradoresRojo[8]
  {
    muestraGanadorjuego2(); // datos obtenidos de duelos_ganados_rojo y duelos_ganados_azul
    reiniciar_jugadores_todos_los_juegos();

    break;
  }
  default:
  {
    Serial.println(" Error 001 - NO de duelo desconocido en CASE");
    break;
  }
  }
}
void reiniciar_jugadores_todos_los_juegos()
{
  reiniciar_jugadores_juego_1 = SI;
  reiniciar_jugadores_juego_2 = SI;
  reiniciar_jugadores_juego_4 = SI;
  reiniciar_jugadores_juego_8 = SI;
  num_creditos = num_creditos - 1;
}
void reiniciaDatosJuego1()
{
  TiradoresRojo[0] = 1;
  TiradoresRojo[1] = 1;
  TiradoresRojo[2] = 1;
  TiradoresRojo[3] = 1;
  TiradoresRojo[4] = 1;
  TiradoresAzul[0] = 0;
  TiradoresAzul[1] = 0;
  TiradoresAzul[2] = 0;
  TiradoresAzul[3] = 0;
  TiradoresAzul[4] = 0;
  duelos_ganados_rojo = 0;
  duelos_ganados_azul = 0;
  round_ganados_rojo = 0;
  round_ganador_azul = 0;
  mejor_velocidad_rojo = 0;
  mejor_rafaga_rojo = 0;
  mejor_velocidad_azul = 0;
  mejor_rafaga_azaul = 0;
}
void ejecutajuegoDos()
{
  if (reiniciar_jugadores_juego_2 == SI)
  {
    reiniciaDatosJuego2();
    reiniciar_jugadores_juego_2 = NO;
  }
  switch (num_duelo)
  {
  case 0: // duelo 0 - Tirador 1 vs tirador 2  ganador a TiradoresRojo[8]
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 1: // duelo 1 tirador 1 vs tirador 2  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 2: // duelo 2 es el final , define al ganadorjuego
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 3: // duelo 3 tirador 1 vs tirador 2  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 4: // duelo 4 tirador 1 vs tirador 2  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_SI, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 8: // Resultados de ganador final de juego en posicion TiradoresRojo[8]
  {
    muestraGanadorjuego2(); // datos obtenidos de duelos_ganados_rojo y duelos_ganados_azul
    reiniciar_jugadores_todos_los_juegos();
    num_creditos = num_creditos - 2;
    break;
  }
  default:
  {
    Serial.println(" Error 001 - NO de duelo desconocido en CASE");
    break;
  }
  }
}

void reiniciaDatosJuego2()
{
  TiradoresRojo[0] = 1;
  TiradoresRojo[1] = 1;
  TiradoresRojo[2] = 1;
  TiradoresRojo[3] = 1;
  TiradoresRojo[4] = 1;
  TiradoresAzul[0] = 2;
  TiradoresAzul[1] = 2;
  TiradoresAzul[2] = 2;
  TiradoresAzul[3] = 1;
  TiradoresAzul[4] = 1;
  duelos_ganados_rojo = 0;
  duelos_ganados_azul = 0;
  round_ganados_rojo = 0;
  round_ganador_azul = 0;
  mejor_velocidad_rojo = 0;
  mejor_rafaga_rojo = 0;
  mejor_velocidad_azul = 0;
  mejor_rafaga_azaul = 0;
}

void acumulaDatosParajuego1y2() // Se acumula duelos para un total final en juego1 y juego 2 solamente
{                               // Caso especial para juego1 y juego2 Solo dos jugadores todos los duelos,1 y 2,
  if (TiradoresRojo[8] == 1)
  {
    duelos_ganados_rojo++; // Se acumula para el jugador No 1 siempre
  }
  if (TiradoresRojo[8] == 2)
  {
    duelos_ganados_azul++; // Se acumula para el jugador No.2 siempre
  }
}

void muestraGanadorjuego2() // en posicion TiradoresRojo[8]
{
}

void ejecutajuegoCuatro()
{
  if (reiniciar_jugadores_juego_4 == SI)
  {
    reiniciaDatosJuego4();
    reiniciar_jugadores_juego_4 = NO;
  }
  switch (num_duelo)
  {
  case 0: // duelo 0 - Tirador 1 vs tirador 2  ganador a TiradoresRojo[2]
  {
    ejecutaDuelo(juego_4, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_7_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_2, GANADOR_EN_POSICION_ROJO);
    break;
  }
  case 1: // duelo 1 tirador 3 vs tirador 4  ganador a TiradoresAzul[2]
  {
    ejecutaDuelo(juego_4, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_7_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_2, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 2: // duelo 2 es el final , define al ganadorjuego
  {
    ejecutaDuelo(juego_4, num_duelo, DUELO_FINAL_SI, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_7_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_EN_POSICION_ROJO);
    break;
  }
  case 8: // Resultados de ganador juego en posicion TiradoresRojo[8]
  {
    muestraGanadorjuego4y8(); // en posicion TiradoresRojo[8]
    reiniciar_jugadores_todos_los_juegos();
    num_creditos = num_creditos - 4;
    break;
  }
  default:
  {
    Serial.println(" Error 001 - NO de duelo desconocido en CASE");
    break;
  }
  }
}

void reiniciaDatosJuego4()
{
  TiradoresRojo[0] = 1;
  TiradoresRojo[1] = 3;
  TiradoresAzul[0] = 2;
  TiradoresAzul[1] = 4;
  duelos_ganados_rojo = 0;
  duelos_ganados_azul = 0;
  round_ganados_rojo = 0;
  round_ganador_azul = 0;
}
void ejecutajuegoOcho()
{
  if (reiniciar_jugadores_juego_8 == SI)
  {
    reiniciaDatosJuego8();
    reiniciar_jugadores_juego_8 = NO;
  }
  switch (num_duelo)
  {
  case 0: // duelo 0 - Tirador 1 vs tirador 2  ganador a TiradoresRojo[4]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_4, GANADOR_EN_POSICION_ROJO);
    break;
  }
  case 1: // duelo 1 tirador 3 vs tirador 4  ganador a TiradoresAzul[4]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_4, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 2: // duelo 2 tirador 5 vs tirador 6 ganador a TiradoresRojo[5]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_5, GANADOR_EN_POSICION_ROJO);
    break;
  }
  case 3: // duelo 3 tirador 7 vs tirador 8 ganador a TiradoresAzul[5]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_5, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 4: // duelo 4  ganador DUELO 0 vs ganador DUELO 1  ganador a TiradoresRojo[6]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_6, GANADOR_EN_POSICION_ROJO);
    break;
  }
  case 5: // duelo 5  ganador DUELO 2 vs ganador DUELO 3  ganador a TiradoresAzul[6]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_6, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 6: // duelo 6  ganador DUELO 4 vs ganador DUELO 5  ganador a TiradoresRojo[8]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_SI, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], VELOCIDAD_Y_RAFAGA, TIROS_POR_RAFAGA,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_CASILLA_8, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 8: // Resultados de ganador de juego en posicion TiradoresRojo[8]
  {
    muestraGanadorjuego4y8(); // en posicion TiradoresRojo[8]
    reiniciar_jugadores_todos_los_juegos();
    num_creditos = num_creditos - 8;
    break;
  }
  default:
  {
    Serial.println(" Error 001 - NO de duelo desconocido en CASE");
    break;
  }
  }
}

void reiniciaDatosJuego8()
{
  TiradoresRojo[0] = 1;
  TiradoresAzul[0] = 2;
  TiradoresRojo[1] = 3;
  TiradoresAzul[1] = 4;
  TiradoresRojo[2] = 5;
  TiradoresAzul[2] = 6;
  TiradoresRojo[3] = 7;
  TiradoresAzul[3] = 8;
  duelos_ganados_rojo = 0;
  duelos_ganados_azul = 0;
  round_ganados_rojo = 0;
  round_ganador_azul = 0;
}
void muestraGanadorjuego4y8()
{
}

void ejecutaDuelo(int ref_duelo, int duelo_final, int tirador_rojo, int tiradorazul, int tipoduelo, int tirosrafaga, int num_rounds, int segundosentrerouds, int nuevoduelo, int posicioncolor)
{
  // al ejecutar por primera  ves checa los tiradores se firmen
  tiradores_preparados = NO;
  do
  {
    enviaSolicitudListorojo(ref_duelo,tirador_rojo);
    enviaSolicitudListoazul(ref_duelo,tirador_azul);
    recibeSolicitudListorojo(); // recibe y cambia MasterBanderaDuelo.rojolisto=SI
    recibeSolicitudListoazul(); // recibe y cambia MasterBanderaDuelo.azullisto=SI
    if ((MasterBanderasDuelo.rojolisto == SI) & (MasterBanderasDuelo.azullisto = 0; == SI))
    {
      tiradores_preparados = SI;
    }
  } while (tiradores_preparados == NO);

  do
  {
    enviaaRojoSolicitudIniciaRound();
    enviaaAzulSolicitudIniciaRound();
    recibeconfirmaciónInicioRojo();
    recibeconfirmaciónInicioAzul();
  } while (terminaron_rounds == NO);

  if (terminaron_rounds == SI)
  { // saca el no de jugador y lo asigna como ganador
    if (total_duelos_ganados_rojo > total_duelos_ganado_sazul)
    {
      ganador_duelo = TiradoresRojo[num_duelo];
    }
    else
    {
      ganador_duelo = TiradoresAzul[num_duelo];
    }
    /* ahora lo manda al ganador a un nuevo duelo y en la posición de color definida*/
    if (posicioncolor == GANADOR_EN_POSICION_ROJO)
    {
      TiradoresRojo[nuevoduelo] = ganador_duelo;
    }
    if (posicioncolor == GANADOR_A_POSICION_AZUL)
    {
      TiradoresAzul[nuevoduelo] = ganador_duelo;
    }
    num_duelo++;          // incrementa para el nuevo duelo
    if (duelo_final == 1) // detecta ultimo duelo, sale para mostrar ganador
    {
      num_duelo = 8;
    }
    // inicializa variables para nuevo duelo
    tiradores_preparados = NO;
    roundi_nicial = 1;
    terminaron_rounds = NO;
  }
}

void ejecutaCiclosRound()
{
  // como hacer un dato radnom para seleccionar velocidad o rafaga MinutoDeLista=1+rand()%16;
  tipo_de_duelo = 1 + rand() % 2; // entrega calor 1 o 2
}

void FirmaJugadores(int Ref_duelo, int duelo_final, int tirador_rrojo, int tiradorazul, int num_rounds)
{
  /*se queda en while hasta que esten listos, envia informacion a local y remoto, espera las firmas
  y cuando se tiene los dos se libera para continuar con tiradores_preparados=SI */
  // si estan listos

  solicitaFirmaRojo(Ref_duelo, tirador_rrojo, num_rounds);
  monitoreaFirmaRojo();
  solicitaFirmaAzul(Ref_duelo, tiradorazul, num_rounds);
  recibeFirmaAzul();
}

void solicitaFirmaRojo(int Ref_duelo, int duelo_final, int tirador_rrojo, int num_rounds)
{
  enciendeBox(FIJO, VERDE);
  mensajeDuelo(Ref_duelo, tirador_rrojo, num_rounds);
  mensajeSolicitudFirma();
}

void monitoreaFirmaRojo()
{
  /* checa si se recibe frecuencia de laser de apunta, si la recibe se cambia la variable rojolisto=SI
   y se deja prendida, se llama la fucion de monitorear frecuencia de puerto
   */
  rojo_listo = SI;
}

void enciendeBox(int tipoencendido, int verde)
{
  // 1 FIJO, 2 FLASHEO, 3 CIRCULAR, 4 IMPACTO
  // VERDE 1, AZUL 2, ROJO 3
}
void mensajeDuelo(int Ref_duelo, int tirador_rrojo, int num_rounds)
{
}

void mensajeSolicitudFirma()
{
}

void solicitaFirmaAzul(int Ref_duelo, int tiradorazul, int num_rounds)
{
}

void recibeFirmaRojo(int Ref_duelo, int duelo_final, int tirador_rrojo, int tiradorazul, int num_rounds) {}
void recibeFirmaAzul() {}

void cicloRounds() // ejecuta los round
{
  // al terminar round marca la salida con
  termino_el_round = 1;
}

void actualizaMarcadorJuego() // actualiza los marcadores de duelo y acumulado
{
}

void reiniciaContadoresDueloYRound()
{
}

void recibeRespuesta()
{
  /* detecta la respuesta de los dos equipos y cuando
  cuando los dos equipo estan listos se cambia
  tiradores_preparados=true;*/
}

void round()
{
  if (!tiradores_preparados)
  {
    solicitaPrepararse();
    recibeRespuesta();
  }
  else
  {
  }
}

/*  Definicion de errores
Errores
-------------
001 - al cambiar de duelo se regresa un valor de duelo no programado en el CASE

Notas 1---ESPNOW------------
Se obtiene la dirección mac del esp32 con el siguiente programa
// Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/
#include "WiFi.h"
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}
void loop(){}
---------------

*/