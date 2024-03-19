// DORA VERSION 1
/* Se cambia toda la navegacion mediante un mapa de control*/
#include <Arduino.h>
// TARJETA DE MEMORIA
#include <SPI.h>
#include <SD.H>
// DISPLAY MAX  CAMBIO
// #include <MD_MAX72xx.h>
// #include <MD_Parola.h>

// #include <WiFi.h>
// #include <WebServer.h>
// #include <AutoConnect.h>
#include <time.h>
#include "Esp32Time.h"
#include <Preferences.h>
#include <Wire.h> //i2c
#include "Auth.h"
#include <Adafruit_NeoPixel.h>
#include <esp_now.h>

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

#define CERO 0
#define UNO 1
#define DOS 2
#define CUATRO 4
#define OCHO 8

// seleccion de tipo de round
#define HIBRIDO 1
#define RAFAGA 3
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
#define GANADOR_A_POSICION_ROJO 1
#define GANADOR_A_POSICION_AZUL 2

// Cantidad de duelos por juego
#define CUATRO_DUELOS 4

// cantidad de round por duelo
#define REALIZA_3_ROUNDS 3
#define REALIZA_5_ROUNDS 5
#define REALIZA_7_ROUNDS 7
#define REALIZA_8_ROUNDS 8
#define SEG_PAUSA_ENTRE_ROUNDS 3
#define SEGUNDOS_MENSAJE 2

#define LISTO 1
#define NOLISTO 0

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
#define GANADOR_A_CASILLA_8 8

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
int total_duelos_ganados_azul = 0;
int total_rounds_ganados_rojo = 0;
int total_rounds_ganados_azul = 0;

int velocidad = 1;
int rafaga = 2;
int tipo_de_duelo = 1;

// variables juego
int reiniciar_jugadores_juego_1 = SI; // Se usa para reiniciar los datos antes de cada juego
int reiniciar_jugadores_juego_2 = SI;
int reiniciar_jugadores_juego_4 = SI;
int reiniciar_jugadores_juego_8 = SI;
int Sumatoria_duelos_ganados_rojo = 0; // Valido solo para juego1 y juego2
int Sumatoria_duelos_ganados_azul = 0; // Valido solo para juego1 y juego2
int round_ganados_rojo = 0;            // Valido solo para juego1 y juego2
int round_ganador_azul = 0;            // Valido solo para juego1 y juego2
int mejor_velocidad_rojo = 0;          // Valido solo para juego1 y juego2
int mejor_velocidad_azul = 0;          // Valido solo para juego1 y juego2
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
void ejecutajuegoDos();
void ejecutajuegoCuatro();
void ejecutajuegoOcho();
void muestraGanadorjuego4y8();
void muestraGanadorjuego2();
void acumulaDatosParajuego1y2();
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
void reiniciaDatosJuego1();
void reiniciaDatosJuego2();
void reiniciaDatosJuego4();
void reiniciaDatosJuego8();

void reiniciaJugadoresUNO();

// DORA VOID -------------------
// Bienvenida
void iniciaBienvenida();
// Juego UNO
void iniciaJuegoUno();
void reiniciaJugadoresUNO();
void ejecutaDuelosUNO();
void ejecutaRounds();

int jugadorRojoPreparado();
int jugadorAzulPreparado();
void deteccionFrecuenciaApuntaRojo();
void deteccionFrecuenciaApuntaAzul();

void muestraResultadofinal();
void reiniciaDatosDeTiros();
void detectaTiro();

void reiniciaDatosRounds001();
void escribeMensaje001();
void mensajeIniciaTiroteo002();
void escribeMensajeIniciaTiroteo002();
void mensaje333_003();
void escribeMensaje003();
void mensaje222_004();
void escribeMensaje004();
void mensaje111_005();
void escribeMensaje005();
void mensaje3Verdes_006();
void escribeMensaje006();
void mensaje2Verdes_007();
void escribeMensaje007();
void mensaje1Verdes_008();
void escribeMensaje008();
void mensajeTiempoTotalTiroteo_009();
void escribeTiempoTotalTiroteo009(ulong tiempo_total_round);
void mensajeEsperaTiempoAzul_010();
void mensajeGanadorTiroteo_011();
void mensajeGanasteRojo013();
void mensajePerdisteRojo014();

void mensajeNombreJuego_100();
void escribeMensaje100();
void mensajeKrambitSystems_101();
void escribeKrambitSystems101();
void mensajeContrataEn_102();
void escribeContrataEn102();
void mensajeGuacamaya_103();
void escribeGuacamaya103();
void mensajeCreditosDisponibles_104();
void escribeCreditosDisponibles104();
void escribeCuadrorojo006();
void escribeCuadroRojo007();
void escribeCuadroRojo008();
void escribeCuadroRojo201();
void escribeCuadroRojo200();

// control de inicio de mensajes toda la aplicacion
bool comienza_mensaje_001 = true; // Mensaje de listos para iniciar duelo
ulong contador_inicial_001 = 0;
ulong contador_actual_001 = 0;
ulong duracion_mensaje_001 = 3000; // duracion en milisegundos

bool comienza_mensaje_002 = true; // Mensaje de Ready
ulong contador_inicial_002 = 0;
ulong contador_actual_002 = 0;
ulong duracion_mensaje_002 = 3000; // duracion en milisegundos 2 seg

bool comienza_mensaje_003 = true; // Mensaje de "3 3 3"
ulong contador_inicial_003 = 0;
ulong contador_actual_003 = 0;
ulong duracion_mensaje_003 = 500; // duracion en milisegundos .5seg

bool comienza_mensaje_004 = true; // Mensaje de "2 2 2"
ulong contador_inicial_004 = 0;
ulong contador_actual_004 = 0;
ulong duracion_mensaje_004 = 500; // duracion en milisegundos .5seg

bool comienza_mensaje_005 = true; // Mensaje de "1 1 1"
ulong contador_inicial_005 = 0;
ulong contador_actual_005 = 0;
ulong duracion_mensaje_005 = 500; // duracion en milisegundos .5seg

bool comienza_mensaje_006 = true; // Espera de primer impacto
ulong contador_inicial_006 = 0;
ulong contador_actual_006 = 0;
ulong duracion_mensaje_006 = 5000; // timeout o deteccion de 1 tiro

bool comienza_mensaje_007 = true; // Espera de segundo impacto
ulong contador_inicial_007 = 0;
ulong contador_actual_007 = 0;
ulong duracion_mensaje_007 = 5000; // timeout o deteccion de 2 tiro

bool comienza_mensaje_008 = true; // Espera de segundo impacto
ulong contador_inicial_008 = 0;
ulong contador_actual_008 = 0;
ulong duracion_mensaje_008 = 5000; // timeout o deteccion de 2 tiro

bool comienza_mensaje_009 = true; // Mensaje tiempo de tiro
ulong contador_inicial_009 = 0;
ulong contador_actual_009 = 0;
ulong duracion_mensaje_009 = 3000; // Salida a 3 segundos
bool detecta_impacto = false;      // detecta el impacto

bool comienza_mensaje_010 = true; // Mensaje soliciud tiempo azul ESPNOW
ulong contador_inicial_010 = 0;
ulong contador_actual_010 = 0;
ulong duracion_mensaje_010 = 3000; // Salida a 3 segundos

bool comienza_mensaje_011 = true; // <Mensaje ganador
ulong contador_inicial_011 = 0;
ulong contador_actual_011 = 0;
ulong duracion_mensaje_011 = 3000; //

bool comienza_mensaje_012 = true; // Cantidad de tiros
ulong contador_inicial_012 = 0;
ulong contador_actual_012 = 0;
ulong duracion_mensaje_012 = 3000; // 3 seg

bool comienza_mensaje_100 = true; // mensaje nombre del juego
ulong contador_inicial_100 = 0;
ulong contador_actual_100 = 0;
ulong duracion_mensaje_100 = 3000;

bool comienza_mensaje_101 = true; // mensaje Krambit Systems
ulong contador_inicial_101 = 0;
ulong contador_actual_101 = 0;
ulong duracion_mensaje_101 = 3000;

bool comienza_mensaje_102 = true; // mensaje guacamaya.com.mx
ulong contador_inicial_102 = 0;
ulong contador_actual_102 = 0;
ulong duracion_mensaje_102 = 3000;

bool comienza_mensaje_103 = true; // mensaje nombre del juego
ulong contador_inicial_103 = 0;
ulong contador_actual_103 = 0;
ulong duracion_mensaje_103 = 3000;

bool comienza_mensaje_104 = true; // <Mensaje Creditos
ulong contador_inicial_104 = 0;
ulong contador_actual_104 = 0;
ulong duracion_mensaje_104 = 3000; //

bool comienza_mensaje_200 = true; // <Cuadro Rojo
ulong contador_inicial_200 = 0;
ulong contador_actual_200 = 0;
ulong duracion_mensaje_200 = 3000; //

bool comienza_mensaje_201 = true; // Mensaje de ganador juego
ulong contador_inicial_201 = 0;
ulong contador_actual_201 = 0;
ulong duracion_mensaje_201 = 3000; //

// dora fin void ++++++++++++++++++++++

// reloj para mensajes y led
ulong previous_time, current_time; // para sincronizar encendido led y duracion de mensajes matriz

// estructura para ESPNOW firma de jugadores inicio de duelo y round OK
typedef struct flag_listo
{
  int rojolisto; // Duelo tirador rojo  listo 0,1
  int azullisto; // Duelo tirador azul listo  0,1
  int numduelo;
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

// direccion MAC del receptor
/*en ejemplo se pone la direccion MAC del receptor ( 30:AE:A4:07:0D:64)
debe cambiarse por la nueva del receptor  ver nota 1*/

uint8_t broadcastAddress[] = {0x30, 0xAE, 0xA4, 0x07, 0x0D, 0x64};

int calcula_tiros = 1;
int tiros_a_realizar = 1;
int tiros_acumulados = 0;

// DORA INICIA- PARAMETROS ------------------------------------

typedef struct struct_mapa
{
  int creditosdisponibles;
  int juego;          // Sala 1 seleccion de juego vis RF
  int ubicacionjuego; // Sala 2 Ubicación de juego
                      // 2-1:reseteo de valores 2-2:Firma de jugadores 2-3: Pasa a duelo

  int numerodeduelos; // Sala 3 parametro de cantidad de duelos
  int dueloactual;    // posicion del duelo a realizar

  int numeroderounds; // cantidad de rounds por duelo
  int flujoround;     // round que se realizara

  int nround;   // round a pelear
  int tipo;     // tipo de round
  int nrafaga;  // num de tiros en caso de round de rafaga
  int segpausa; // espera entre rounds
  int mensajebienvenida;

  int flujodetiro; // el avance en el proceso del round desde mensjae hata fin de tiros
  int tiradoressincronizados;
  int flujosolicitudtiempoazulespnow;
  int tiemporojo;
  int tiempoazul;
  int tiradorrojo;
  int tiradorazul;
  int tirosacumulados;
  int tirosarealizar;

  // tiempo de medicion de cada tiroteo
  ulong tiempoinicialtiroteo; // milisegundos
  ulong tiempofinaltiroteo;
  ulong tiempototaltiroteo;

  int resultadoenvio;

} struct_mapa;
struct_mapa Mapa;

typedef struct datos_control
{
  int vsolicitada; // variable solicitada
  int venviada;    // variable enviada
  int vleida;      // variable enviada ya procesada

} datos_control;

void reiniciarvaloresMapa()
{
  Mapa.mensajebienvenida = 1;
  Mapa.juego = 0;
  Mapa.ubicacionjuego = 1;

  Mapa.numerodeduelos = 0;
  Mapa.dueloactual = 0;
  Mapa.flujoround = 0;

  Mapa.numeroderounds = 0;
  Mapa.flujodetiro = 1;
  Mapa.mensajebienvenida = 1;

  Mapa.flujosolicitudtiempoazulespnow = 0;
  Mapa.tiempoazul = 0;
  Mapa.tiemporojo = 0;

  Mapa.resultadoenvio = 0;
}

typedef struct struct_mensaje
{
  int leido;          // 0 NO, 1) SI
  int dueloactual;    // numero de duelo  ejecutando
  int roundactual;    // numero de round ejecutado
  int flagrojo;       // 0) no se envia / 1)se envia
  float segundosrojo; // rojo duracion en segundos
  int flagazul;       // 0) no se envia / 1)se envia
  float segundosazul; // azul duracion en segundos
} struct_mensaje;

// Create a struct_message called myData
struct_mensaje paqueteOut;
struct_mensaje paqueteIn;

// DORA FINAL PARAMETROS ++++++++++++++++++++++++++++++++++++++++++

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

  // Dora inicio setup-------------------------
  reiniciarvaloresMapa();

  // Dora final SETUP +++++++++++++++++++++++++

  // Inicia los valores de las estructuras de banderas
  iniciaDatosBanderasDuelo();
}

void loop()
{
  current_time = millis();
  switch (Mapa.juego) // sala 1 seleccion de juego
  {
  case CERO: // información de la empresa
  {
    iniciaBienvenida();
    break;
    // el Mapa.juego solo se cambia mediante el control RF
  }
  case UNO:
  {
    iniciaJuegoUno();
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

// Dora inicio voids----------------------------------

void iniciaBienvenida() // case de mensajes de bienvenida
{
  switch (Mapa.mensajebienvenida)
  {
  case 1: // Duelo de Gamers
  {
    mensajeNombreJuego_100();
    break;
  }
  case 2: // KrambitSystems
  {
    mensajeKrambitSystems_101();
    break;
  }
  case 3: // contrata en www.guacamaya.com.mx
  {
    mensajeContrataEn_102();
    break;
  }
  case 4: // distribuciones disponibles
  {
    mensajeGuacamaya_103();
    break;
  }
  case 5: // Creditos disponibles
  {
    mensajeCreditosDisponibles_104();
    break;
  }
  }
}

void mensajeNombreJuego_100() // nombre DEULO DE GAMERS
{
  {
    if (comienza_mensaje_100)
    { // inicia tiempo de mensaje
      contador_inicial_100 = millis();
      comienza_mensaje_100 = false;
    }
    // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
    contador_actual_100 = millis();
    if (contador_actual_100 < (contador_inicial_100 + duracion_mensaje_100))
    {
      escribeMensaje100(); // nombre del juego DUELOS GAMERS
    }
    else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
    {
      // Salida de mensaje y avanza a primer round
      Mapa.mensajebienvenida = 3; // avanza  pagina web por el momento
    }
  }
}

void mensajeKrambitSystems_101() // nombre KS
{
  if (comienza_mensaje_101)
  { // inicia tiempo de mensaje
    contador_inicial_101 = millis();
    comienza_mensaje_101 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_101 = millis();
  if (contador_actual_101 < (contador_inicial_101 + duracion_mensaje_101))
  {
    escribeKrambitSystems101(); // nombre Krambit Systems
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.mensajebienvenida = 3; // avanza  siguiente mensaje
  }
}

void mensajeContrataEn_102() // nombre "Contrata en:"
{
  if (comienza_mensaje_102)
  { // inicia tiempo de mensaje
    contador_inicial_102 = millis();
    comienza_mensaje_102 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_102 = millis();
  if (contador_actual_102 < (contador_inicial_102 + duracion_mensaje_102))
  {
    escribeContrataEn102(); //
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.mensajebienvenida = 4; // avanza  siguiente mensaje
  }
}

void mensajeGuacamaya_103() // nombre "Guacamaya.com.mx"
{
  if (comienza_mensaje_103)
  { // inicia tiempo de mensaje
    contador_inicial_103 = millis();
    comienza_mensaje_103 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_103 = millis();
  if (contador_actual_103 < (contador_inicial_103 + duracion_mensaje_103))
  {
    escribeGuacamaya103(); // nombre Krambit Systems
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.mensajebienvenida = 5; // Regresa a nombre de juego
  }
}

void mensajeCreditosDisponibles_104() // nombre "Creditos"
{
  if (comienza_mensaje_104)
  { // inicia tiempo de mensaje
    contador_inicial_104 = millis();
    comienza_mensaje_104 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_104 = millis();
  if (contador_actual_104 < (contador_inicial_104 + duracion_mensaje_104))
  {
    escribeCreditosDisponibles104(); // nombre Krambit Systems
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.mensajebienvenida = 1; // Regresa a nombre de juego
  }
}
void escribeMensaje100()
{
  /* texo = Duelo de Gamers*/
  Serial.println("Duelo de Gamers");
}

void escribeKrambitSystems101()
{
  /* texo = >Krambit Systems*/
  Serial.println("Krambit Systems");
}

void escribeContrataEn102()
{
  /* texo = Contrata en:*/
  Serial.println("Contrata en : ");
}

void escribeGuacamaya103()
{
  /* guacamaya.com.mx*/
  Serial.println("guacamaya.com.mx");
}

void escribeCreditosDisponibles104()
{
  /* Saldo :  Mapa.creditosdisponibles*/
  Serial.print("Mapa.creditosdisponibles : ");
  Serial.println(Mapa.creditosdisponibles);
}

// JUEGO 1
void iniciaJuegoUno() // case (Mapa.ubicacionjuego) 1)reinicio- 2)firma- 3)ejecutaduelos
{
  switch (Mapa.ubicacionjuego) // sala 2 Ubicación en juego
  {
  case 1: // inicia parametros duelos  y limpieza de datos
  {
    reiniciaJugadoresUNO();
    Mapa.numerodeduelos = 5;
    Mapa.dueloactual = 0;
    Mapa.ubicacionjuego = 2; // avanza
    break;
  }
  case 2: // solicita firma de jugadores para iniciar juego
  {
    if ((jugadorRojoPreparado() == SI) & (jugadorAzulPreparado() == SI))
    {
      Mapa.ubicacionjuego = 3; // avanza a siguiente case
    }
    break;
  }
  case 3: // ejecuta duelos
  {
    ejecutaDuelosUNO();
    break;
  }
  case 4: // Muestra Ganador
  {
    muestraResultadofinal();
    break;
  }
  case 5: // Regresa a bienvenida de juegos
  {
    Mapa.juego = 0;          // regresa a bienvenida
    Mapa.ubicacionjuego = 1; // reinicia la ubicacion de cualquier juego
    break;
  }
  }
}

void reiniciaJugadoresUNO() // 1) reinicia jugadores y contadores
{
  // base de jugadores en duelos
  // - Rojo
  TiradoresRojo[0] = 1;
  TiradoresRojo[1] = 1;
  TiradoresRojo[2] = 1;
  TiradoresRojo[3] = 1;
  TiradoresRojo[4] = 1;
  //- Azul
  TiradoresAzul[0] = 0;
  TiradoresAzul[1] = 0;
  TiradoresAzul[2] = 0;
  TiradoresAzul[3] = 0;
  TiradoresAzul[4] = 0;

  // acumuladores solo para juego1 y juego2
  total_duelos_ganados_rojo = 0;
  total_duelos_ganados_azul = 0;
  total_rounds_ganados_rojo = 0;
  total_rounds_ganados_azul = 0;
  mejor_velocidad_rojo = 0;
  mejor_velocidad_azul = 0;
}

void ejecutaDuelosUNO() // 3) ejecuta duelos cib duelos a realizs y numero de duelos
{
  switch (Mapa.dueloactual) // case (Mapa.dueloactual)  1)Reinicio 2-4) duelos
  {
  case 0: // Reiniciar Parametros ROUND  para  duelo
  {
    Mapa.numeroderounds = 3;
    Mapa.flujoround = 0;
    Mapa.segpausa = 5;
    Mapa.dueloactual = 11;
    // contadores de los mensajes para que siguan apareciendo insertos
    break;
  }
  case 1: // duelo 1 - Tirador 1 vs tirador 0  si ganador suma a duelos_ganados_rojo o duelos_ganados_azul
  {
    ejecutaRounds();
    acumulaDatosParajuego1y2();
    // el dueloactual se cambia en el siguiente nivel Round en el case 10: que es fin de r
    break;
  }
  case 2: // duelo 2 - Tirador 1 vs tirador 0  si ganador suma a duelos_ganados_rojo o duelos_ganados_azul
  {
    ejecutaRounds();
    acumulaDatosParajuego1y2();
    break;
  }
  case 3: // duelo 3 - Tirador 1 vs tirador 0  si ganador suma a duelos_ganados_rojo o duelos_ganados_azul
  {
    ejecutaRounds();
    acumulaDatosParajuego1y2();
    break;
  }
  case 4: // duelo 4 - Tirador 1 vs tirador 0  si ganador suma a duelos_ganados_rojo o duelos_ganados_azul
  {
    ejecutaRounds();
    acumulaDatosParajuego1y2();
    break;
  }
  case 5: // duelo 5 - Tirador 1 vs tirador 0  si ganador suma a duelos_ganados_rojo o duelos_ganados_azul
  {
    ejecutaRounds();
    acumulaDatosParajuego1y2();
    Mapa.dueloactual = 10; // envia a calcular totales
    break;
  }
  case 10: // Calcula totales
  {
    acumulaDatosParajuego1y2();
    Mapa.ubicacionjuego = 4; // mostrar resultado final
    break;
  }
  }
}

void muestraResultadofinal()
{
  if (comienza_mensaje_201)
  { // inicia tiempo de mensaje
    contador_inicial_201 = millis();
    comienza_mensaje_201 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_201 = millis();
  if (contador_actual_201 < (contador_inicial_201 + duracion_mensaje_201))
  {
    escribeMensaje201();
  }
  else // termina mostrar mensaje
  {
    // Salida de mensaje y avanza a primer round
    Mapa.ubicacionjuego = 5; // avanza al mensaje de contador descendente "2 2 2 "
  }
}

void escribeMensaje201() // ganador del torneo, mejor tiro velocidad, mejor tiro rafaga
{
  // Muestra quien es el ganador de lo acumulado de duelos
}

// subrutinas copartidas por juegos
int jugadorRojoPreparado() // Dora
{
  // informacion jugador local mediante
  deteccionFrecuenciaApuntaRojo();
  return SI;
}

void deteccionFrecuenciaApuntaRojo()
{
}

int jugadorAzulPreparado() // Dora
{                          // ESP-NOW
  // información obtenida mediante espnow
  // la deteccion de azulse usara para chaleco esclavo
  deteccionFrecuenciaApuntaAzul();
  return SI;
}

void deteccionFrecuenciaApuntaAzul()
{
}

void ejecutaRounds() // Del 1 al 5
{                    //
  switch (Mapa.flujoround)
  {
  case 0:                     // reinicia rounds
  {                           // mensajes de inicio de round
    reiniciaDatosRounds001(); // despues acabar el periodo de mostrado se cambia dentro el Mapa.flujoround
    break;
  }
  case 1: // round 1
  {
    ejecutaTiroteo();
    break;
  }
  }
}

void ejecutaTiroteo()
{
  switch (Mapa.flujodetiro)
  {
  case 0: // reinicia datos de tiros
  {
    reiniciaDatosDeTiros();
    // define el tipo de tiros
    calcula_tiros = 1 + rand() % (2); // genera aleatorio tipo de disparo 1:velocidad o 2:rafaga
    if (calcula_tiros == 1)
    {
      Mapa.tirosarealizar = 1; // velocidad
    }
    else
    {
      Mapa.tirosarealizar = 3; // rafaga
    }
    Mapa.flujodetiro = 20;
    break;
  }
  case 20:
  {
    mensajeCantidadDeTiros012();
    break;
  }

  case 1: // sincroniza tiradores
  {
    sincronizaInicioDeTiros();
    if ((Mapa.tiradorrojo == LISTO) && (Mapa.tiradorazul == LISTO))
    { // INICIO DUELO
      Mapa.flujodetiro = 2;
    }
    break;
  }
  case 2: // Mensaje "Inicia Tiroteo"
  {
    mensajeIniciaTiroteo002();
    break;
  }
  case 3: // 3 3 3
  {
    mensaje333_003();
    break;
  }
  case 4: // 2 2 2
  {
    mensaje222_004();
    break;
  }
  case 5: // 1 1 1
  {
    mensaje111_005(); // se inicia (var tiempoinicialtiroteo)
    break;
  }
  case 6:
  {
    mensaje3Verdes_006(); // primer disparo /avanza case 6
    break;
  }
  case 7:
  {
    mensaje2Verdes_007(); // segundo disparo /avanza case 7
    break;
  }
  case 8:
  {
    mensaje1Verdes_008(); // tercer disparo /avanza case 10
    break;
  }
  case 10: // calcula tiempo toral de tiroteo
  {
    Mapa.tiempofinaltiroteo = millis();
    Mapa.tiempototaltiroteo = Mapa.tiempofinaltiroteo - Mapa.tiempoinicialtiroteo;
    Mapa.flujodetiro = 9; // avanza a mostrar tiempo
    break;
  }
  case 9: // tiempo de disparo
  {
    mensajeTiempoTotalTiroteo_009();
    break;
  }

  case 11: // Revisa informacion de azul
  {
    /*
    Se obtiene el archivo recibio por ESPNOW del loop
    se revisa si viene el dato de la duracion del azul
    si es correcto se saca la duración azul
    se cambia Mapa.tiempoazul= dato recibido
    Se avanza a mostrar ganador
    if (ESPNOWRECIEVE_OK)
       {se revisa combinacion de datoenviado y identificador del mismo
        Mapa.tiempoazul=Archivo.ta;
        Mapa.flujodetiro=12;
        break;
       }
    */
    if (Mapa.juego == 1) // no hay equipo azul, se manda un valor default
    {
      Mapa.tiempoazul = 5000;
      Mapa.flujodetiro = 12;
    }
    else
    {
      mensajeEsperaTiempoAzul_010();
    }
    break;
  }

  case 12: // envia informacion a azul ESPNOW
  {
    /* se envia información, se revisa si se recibio y continua
    si no se recibio se manda mensaje de sin conexion */
    Mapa.flujodetiro = 13;
    break;
  }
  case 13: // Mostrar ganador y mejor tiempo de tiro
  {
    apuntador;
    mensajeGanadorTiroteo_011();
    break;
  }
  case 14: // acumula el ganador en round
  {
    break;
  }
  case 15: // cambia de round y reinicializa flujo de tiro
  {
    Mapa.flujoround++;
    Mapa.flujodetiro=0;
    break;
  }
  }
}

void mensajeEsperaTiempoAzul_010()
{
  if (comienza_mensaje_010)
  { // inicia tiempo de mensaje
    contador_inicial_010 = millis();
    comienza_mensaje_010 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_010 = millis();
  if (contador_actual_010 < (contador_inicial_010 + duracion_mensaje_010))
  {
    escribeEsperandoTiempoAzul010();
  }
  else
  {
    Mapa.tiempoazul = 5000;
    Mapa.flujodetiro = 12; // envia por espnow tiempo de rojo a azul
  }
}

void escribeEsperandoTiempoAzul010()
{
}

void mensajeCantidadDeTiros012()
{
  if (comienza_mensaje_012)
  { // inicia tiempo de mensaje
    contador_inicial_012 = millis();
    comienza_mensaje_012 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_012 = millis();
  if (contador_actual_012 < (contador_inicial_012 + duracion_mensaje_012))
  {
    escribeMensajeCantidadDeTiros012(); // mensaje "1 tiro / 3 tiros"
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.flujodetiro = 1; // avanza al mensaje de contador descendente "3 3 3 "
  }
}

void escribeMensajeCantidadDeTiros012()
{
}

void reiniciaDatosTiros()
{
  Mapa.tiradorrojo = NOLISTO;
  Mapa.tiradorazul = NOLISTO;
  Mapa.tirosacumulados = 0;
}

void sincronizaInicioDeTiros()
{
  // checa tirador rojo que apunte a blanco y cambia
  // Mapa.tiradorrojo=1

  // manda ESP-NOW orden de inicio duelo a azul
  // si recibio mensaje == ESPNOW_OK entonces Mapa.tiradorazul=LISTO
}

void reiniciaDatosRounds001()
{
  if (comienza_mensaje_001)
  { // inicia tiempo de mensaje
    contador_inicial_001 = millis();
    comienza_mensaje_001 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_001 = millis();
  if (contador_actual_001 < (contador_inicial_001 + duracion_mensaje_001))
  {
    escribeMensaje001();
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.flujoround = 1;
    Mapa.flujodetiro = 1;
  }
}

void mensajeGanadorTiroteo_011() // case 11 Mapa.flujodetiro
{
  if (comienza_mensaje_011)
  { // inicia tiempo de mensaje
    contador_inicial_011 = millis();
    comienza_mensaje_011 = false;
    apuntador2;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_011 = millis();
  if (contador_actual_011 < (contador_inicial_011 + duracion_mensaje_011))
  {
    if (Mapa.tiemporojo <= Mapa.tiempoazul)
    {
      mensajeGanasteRojo013(); // ganaste
    }
    else
    {
      mensajePerdisteRojo014(); // perdiste
    }
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.flujoround = 1;
    Mapa.flujodetiro = 1;
    Mapa.dueloactual++; // incrementa el numero de duelo actual para iniciar un nuevo duelo
  }
}

void mensajeGanasteRojo013()
{
}
void mensajePerdisteRojo014()
{
}

void mensajeIniciaTiroteo002()
{
  if (comienza_mensaje_002)
  { // inicia tiempo de mensaje
    contador_inicial_002 = millis();
    comienza_mensaje_002 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_002 = millis();
  if (contador_actual_002 < (contador_inicial_002 + duracion_mensaje_002))
  {
    escribeMensajeIniciaTiroteo002(); // mensaje " ¡ INICIA !"
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.flujodetiro = 3; // avanza al mensaje de contador descendente "3 3 3 "
  }
}

void mensaje333_003()
{
  if (comienza_mensaje_003)
  { // inicia tiempo de mensaje
    contador_inicial_003 = millis();
    comienza_mensaje_003 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_003 = millis();
  if (contador_actual_003 < (contador_inicial_003 + duracion_mensaje_003))
  {
    escribeMensaje003();
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.flujodetiro = 4; // avanza al mensaje de contador descendente "2 2 2 "
  }
}

void mensaje222_004()
{
  if (comienza_mensaje_004)
  { // inicia tiempo de mensaje
    contador_inicial_004 = millis();
    comienza_mensaje_004 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_004 = millis();
  if (contador_actual_004 < (contador_inicial_004 + duracion_mensaje_004))
  {
    escribeMensaje004();
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.flujodetiro = 5; // avanza al mensaje de contador descendente "1 1 1 "
  }
}

void mensaje111_005()
{
  if (comienza_mensaje_005)
  { // inicia tiempo de mensaje
    contador_inicial_005 = millis();
    comienza_mensaje_005 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_005 = millis();
  if (contador_actual_005 < (contador_inicial_005 + duracion_mensaje_005))
  {
    escribeMensaje005();
  }
  else // termina mostrar mensaje, se sale de aqui y brinca a siguiente paso
  {
    // Salida de mensaje y avanza a primer round
    Mapa.flujodetiro = 6; // avanza a "3 CUADROS VERDES"
    Mapa.tiempoinicialtiroteo = millis();
  }
}

void mensaje3Verdes_006()
{
  if (comienza_mensaje_006)
  { // inicia tiempo de mensaje
    contador_inicial_006 = millis();
    comienza_mensaje_006 = false;
    Mapa.tirosacumulados = 0; // solo el primer tiro
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_006 = millis();
  if (contador_actual_006 < (contador_inicial_006 + duracion_mensaje_006))
  {
    escribeMensaje006();
  }
  else
  {
    Mapa.tirosacumulados = 1;
    if (Mapa.tirosarealizar == 3)
    {
      Mapa.flujodetiro = 7; // avanza 2o tiro
    }
    else
    {
      Mapa.flujodetiro = 10; // avanza calcular duraciont tiroteo
    }
  }
  if (detecta_impacto == SI) // sale por impacto
  {
    Mapa.tirosacumulados = 1;
    if (Mapa.tirosarealizar == 3)
    {
      Mapa.flujodetiro = 7; // avanza 2o tiro
    }
    else
    {
      Mapa.flujodetiro = 10; // avanza calcular duracion tiroteo
    }
  }

  do // espera tiro mientras muestra mensaje
  {
    escribeMensaje006();
    detectaTiro(); // cambia el valor detecta_impacto=true;
  } while ((detecta_impacto == false) || (contador_actual_006 < (contador_inicial_006 + duracion_mensaje_006)));

  // Salida de mensaje y avanza a 2 cuadros verdes si es rafaga, si es un solo tiro avanza afinal
  Mapa.tirosacumulados = 1;
  if (Mapa.tirosarealizar == 3)
  {
    Mapa.flujodetiro = 7; // avanza 2o tiro
  }
  else
  {
    Mapa.flujodetiro = 10; // avanza a fin de round
    Mapa.tiempofinaltiroteo = millis();
    Mapa.tiempototaltiroteo = Mapa.tiempofinaltiroteo - Mapa.tiempoinicialtiroteo;
  }
}

void mensaje2Verdes_007() // espera segundo disparo
{
  if (comienza_mensaje_007)
  { // inicia tiempo de mensaje
    contador_inicial_007 = millis();
    comienza_mensaje_007 = false;
    detecta_impacto = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_007 = millis();
  do
  {
    escribeCuadroRojo201();
    escribeMensaje007();
    detectaTiro(); // cambia el valor de detecta_impacto=true;
  } while ((detecta_impacto == false) || (contador_actual_006 < (contador_inicial_006 + duracion_mensaje_006)));
  // Salida de mensaje y avanza a 2 cuadros verdes si es rafaga, si es un solo tiro avanza afinal
  tiros_acumulados++;

  if (tiros_acumulados < tiros_a_realizar)
  {
    Mapa.flujodetiro = 7; // avanza a detectar tercer disparo
  }
  else // no se ejectua al menos que sean la rafaga de dos disparos
  {
    Mapa.flujodetiro = 10; // avanza a fin de round
  }
}

void mensaje1Verdes_008() // espera tercer disparo avanza a final
{
  if (comienza_mensaje_008)
  { // inicia tiempo de mensaje
    contador_inicial_008 = millis();
    comienza_mensaje_008 = false;
    detecta_impacto = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_008 = millis();
  do
  {
    escribeCuadroRojo008();
    escribeMensaje008();
    detectaTiro(); // cambia el valor de detecta_impacto=true;
  } while ((detecta_impacto == false) || (contador_actual_008 < (contador_inicial_008 + duracion_mensaje_008)));
  // Salida de mensaje y avanza a 2 cuadros verdes si es rafaga, si es un solo tiro avanza afinal
  tiros_acumulados++;
  if (tiros_acumulados = tiros_a_realizar) // detecto 3 disparos y avanza a final de round
  {
    Mapa.flujodetiro = 10;         // avanza a fin de round
    tiempo_final_round = millis(); // marca el tiempo dinal del round
  }
}

void mensajeTiempoTotalTiroteo_009()
{
  if (comienza_mensaje_009)
  { // inicia tiempo de mensaje
    contador_inicial_009 = millis();
    comienza_mensaje_009 = false;
  }
  // despliege el mensaje a usuarios mientras tiempo sea menor del rango definido
  contador_actual_009 = millis();
  if (contador_actual_009 < (contador_inicial_009 + duracion_mensaje_009))
  {
    escribeTiempoTotalTiroteo009();
  }
  else
  {
    Mapa.flujodetiro = 11; // Solicita azul y calcula ganador
  }
}

void escribeCuadroRojo007() {}
void escribeCuadroRojo008() {}

void escribeGanaste() {}
void escribePerdiste() {}

void detectaTiro() // checa se reciba impacto
{
  /* Checa que se reciba el impacto
  Cuando detecta el laser de apuntar cambia el color a azul
  Cuando detecta el impacto cambi la variable detecta_impacto=true
  para salir de dowhile y brinca al siguiente nivel*/
  detecta_impacto = true;
}

void escribeCuadroRojo200() {} // ilumia cuadro flasheando en rojo
void escribeMensaje001()
{

} // Información de Round
void escribeMensajeIniciaTiroteo002() {} // Ready
void escribeMensaje003() {}              // 3_3_3
void escribeMensaje004() {}              // 2_2_2
void escribeMensaje005() {}              // 1_1_1
void escribeMensaje006() {}              // 3 bloques verdes para marcar el incio del tiro
void escribeMensaje007() {}              // 2 bloques verdes y 1 azul para marcar 1 impacto ya
void escribeMensaje008() {}

void escribeTiempoTotalTiroteo009()
{
  // muestra el tiempo total del round
  Serial.println(Mapa.tiempototaltiroteo);
}

void reiniciaDatosJuego1()
{
}

// Dora final ------------------

// revisar los que se eliminan ya que no tienen uso casi todos
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
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_3_ROUNDS,
                 SEG_PAUSA_ENTRE_ROUNDS,
                 GANADOR_A_CASILLA_8,
                 GANADOR_A_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 1: // duelo 1 tirador 1 vs tirador 2  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_CASILLA_8, GANADOR_A_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 2: // duelo 2 es el final , define al ganadorjuego
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_CASILLA_8, GANADOR_A_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 3: // duelo 3 tirador 1 vs tirador 2  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_CASILLA_8, GANADOR_A_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 4: // duelo 4 tirador 1 vs tirador 2  ganador a ROJO[8]
  {
    ejecutaDuelo(juego_2, num_duelo, DUELO_FINAL_SI, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_3_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_CASILLA_8, GANADOR_A_POSICION_ROJO);
    acumulaDatosParajuego1y2();
    break;
  }
  case 8: // Resultados de ganador final de juego en posicion TiradoresRojo[8]
  {
    muestraGanadorjuego2(); // datos obtenidos de duelos_ganados_rojo y duelos_ganados_azul
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
}

void acumulaDatosParajuego1y2() // Se acumula duelos para un total final en juego1 y juego 2 solamente
{                               // Caso especial para juego1 y juego2 Solo dos jugadores todos los duelos,1 y 2,
  if (TiradoresRojo[8] == 1)
  {
    total_duelos_ganados_rojo++; // Se acumula para el jugador No 1 siempre
  }
  if (TiradoresRojo[8] == 2)
  {
    total_duelos_ganados_azul++; // Se acumula para el jugador No.2 siempre
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
    ejecutaDuelo(juego_4, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_7_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_2, GANADOR_A_POSICION_ROJO);
    break;
  }
  case 1: // duelo 1 tirador 3 vs tirador 4  ganador a TiradoresAzul[2]
  {
    ejecutaDuelo(juego_4, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_7_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_2, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 2: // duelo 2 es el final , define al ganadorjuego
  {
    ejecutaDuelo(juego_4, num_duelo, DUELO_FINAL_SI, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_7_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_CASILLA_8, GANADOR_A_POSICION_ROJO);
    break;
  }
  case 8: // Resultados de ganador juego en posicion TiradoresRojo[8]
  {
    muestraGanadorjuego4y8(); // en posicion TiradoresRojo[8]
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
  total_duelos_ganados_rojo = 0;
  total_duelos_ganados_azul = 0;
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
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_4, GANADOR_A_POSICION_ROJO);
    break;
  }
  case 1: // duelo 1 tirador 3 vs tirador 4  ganador a TiradoresAzul[4]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_4, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 2: // duelo 2 tirador 5 vs tirador 6 ganador a TiradoresRojo[5]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_5, GANADOR_A_POSICION_ROJO);
    break;
  }
  case 3: // duelo 3 tirador 7 vs tirador 8 ganador a TiradoresAzul[5]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_5, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 4: // duelo 4  ganador DUELO 0 vs ganador DUELO 1  ganador a TiradoresRojo[6]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_6, GANADOR_A_POSICION_ROJO);
    break;
  }
  case 5: // duelo 5  ganador DUELO 2 vs ganador DUELO 3  ganador a TiradoresAzul[6]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_NO, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_DUELO_6, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 6: // duelo 6  ganador DUELO 4 vs ganador DUELO 5  ganador a TiradoresRojo[8]
  {
    ejecutaDuelo(juego_8, num_duelo, DUELO_FINAL_SI, TiradoresRojo[num_duelo], TiradoresAzul[num_duelo], HIBRIDO, tiros_a_realizar,
                 REALIZA_5_ROUNDS, SEG_PAUSA_ENTRE_ROUNDS, GANADOR_A_CASILLA_8, GANADOR_A_POSICION_AZUL);
    break;
  }
  case 8: // Resultados de ganador de juego en posicion TiradoresRojo[8]
  {
    muestraGanadorjuego4y8(); // en posicion TiradoresRojo[8]
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
    // enviaSolicitudListorojo(ref_duelo,tirador_rojo);
    // enviaSolicitudListoazul(ref_duelo,tirador_azul);
    // recibeSolicitudListorojo(); // recibe y cambia MasterBanderaDuelo.rojolisto=SI
    // recibeSolicitudListoazul(); // recibe y cambia MasterBanderaDuelo.azullisto=SI
    if ((MasterBanderasDuelo.rojolisto == SI) & (MasterBanderasDuelo.azullisto == SI))
    {
      tiradores_preparados = SI;
    }
  } while (tiradores_preparados == NO);

  do
  {
    /*enviaaRojoSolicitudIniciaRound();
    //enviaaAzulSolicitudIniciaRound();
    //recibeconfirmaciónInicioRojo();
    //recibeconfirmaciónInicioAzul();*/
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
    if (posicioncolor == GANADOR_A_POSICION_ROJO)
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
    // solicitaPrepararse();
    // recibeRespuesta();
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