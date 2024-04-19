#include <stdio.h>
#include <stdlib.h>
#define ESTADO_ERROR 0
#define ESTADO_ABIERTO 1
#define ESTADO_CERRADO 2
#define ESTADO_ABRIENDO 3
#define ESTADO_CERRANDO 4
#define ESTADO_INTERMEDIO 5
#define ESTADO_INICIAL 6
#define FALSO 0
#define VERDADERO 1
#define TIEMPO_CIERRE 60

int Func_ESTADO_ERROR(void);
int Func_ESTADO_ABIERTO(void);
int Func_ESTADO_CERRADO(void);
int Func_ESTADO_ABRIENDO(void);
int Func_ESTADO_CERRANDO(void);
int Func_ESTADO_INTERMEDIO(void);
int Func_ESTADO_INICIAL(void);

volatile int ESTADO_ANT = ESTADO_INICIAL;
volatile int ESTADO_ACT = ESTADO_INICIAL;
volatile int ESTADO_SIG = ESTADO_INICIAL;
volatile int CntTiempo = 0; 

volatile struct EntradasSalidas
{
    unsigned int SensorCierre : 1;
    unsigned int SensorApertura : 1;
    unsigned int MotorCierre : 1;
    unsigned int MotorApertura : 1;
    unsigned int BotonCierre : 1;
    unsigned int BotonApertura : 1;
    unsigned int LED : 1;
} io;

int main()
{

    for (;;)
    {
        if (ESTADO_SIG == ESTADO_INICIAL)
        {
            ESTADO_SIG = Func_ESTADO_INICIAL();
        }
        if (ESTADO_SIG == ESTADO_ABIERTO)
        {
            ESTADO_SIG = Func_ESTADO_ABIERTO();
        }
        if (ESTADO_SIG == ESTADO_CERRADO)
        {
            ESTADO_SIG = Func_ESTADO_CERRADO();
        }
        if (ESTADO_SIG == ESTADO_ABRIENDO)
        {
            ESTADO_SIG = Func_ESTADO_ABRIENDO();
        }
        if (ESTADO_SIG == ESTADO_CERRANDO)
        {
            ESTADO_SIG = Func_ESTADO_CERRANDO();
        }
        if (ESTADO_SIG == ESTADO_INTERMEDIO)
        {
            ESTADO_SIG = Func_ESTADO_INTERMEDIO();
        }
        if (ESTADO_SIG == ESTADO_ERROR)
        {
            ESTADO_SIG = Func_ESTADO_ERROR();
        }
    }
}

int Func_ESTADO_ERROR(void)
{
    static int estado_led1 = 0;
    static int estado_led2 = 0;
    for (;;)
    {
        estado_led1 = io.LED;
        io.LED = estado_led2;
        estado_led2 = estado_led1;
        delay_ms(199);
    }
}

int Func_ESTADO_ABIERTO(void)
{
    ESTADO_ANT = ESTADO_ACT;
    ESTADO_ACT = ESTADO_ABIERTO;
    io.MotorApertura = FALSO;
    io.MotorCierre = FALSO;
    io.LED = FALSO;
    CntTiempo = 0; 

    for (;;)
    {
        if (CntTiempo > TIEMPO_CIERRE)
        {
            return ESTADO_CERRANDO;
        }
        if (io.BotonCierre == VERDADERO)
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_CERRADO(void)
{
    ESTADO_ANT = ESTADO_ACT;
    ESTADO_ACT = ESTADO_CERRADO;
    io.MotorCierre = FALSO;
    io.MotorApertura = FALSO;
    io.LED = VERDADERO;

    for (;;)
    {
        if (io.BotonApertura == VERDADERO)
        {
            return ESTADO_ABRIENDO;
        }
    }
}
int Func_ESTADO_ABRIENDO(void)
{
    ESTADO_ANT = ESTADO_ACT;
    ESTADO_ACT = ESTADO_ABRIENDO;
    io.LED = FALSO;
    io.MotorCierre = FALSO;
    io.MotorApertura = VERDADERO;

    for (;;)
    {
        if (io.SensorApertura == VERDADERO)
        {
            return ESTADO_ABIERTO;
        }
        else if (io.BotonCierre == VERDADERO)
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_CERRANDO(void)
{
    ESTADO_ANT = ESTADO_ACT;
    ESTADO_ACT = ESTADO_CERRANDO;
    io.MotorCierre = VERDADERO;
    io.MotorApertura = FALSO;
    io.LED = FALSO;

    for (;;)
    {
        if (io.SensorCierre == VERDADERO)
        {
            return ESTADO_CERRADO;
        }
        else if (io.BotonApertura == VERDADERO)
        {
            return ESTADO_ABRIENDO;
        }
    }
}
int Func_ESTADO_INTERMEDIO(void)
{
    ESTADO_ANT = ESTADO_ACT;
    ESTADO_ACT = ESTADO_INTERMEDIO;
    io.MotorCierre = FALSO;
    io.MotorApertura = FALSO;
    io.LED = VERDADERO;
    for (;;)
    {
        if ((io.BotonApertura == VERDADERO))
        {
            return ESTADO_ABRIENDO;
        }

        if ((io.BotonCierre == VERDADERO))
        {
            return ESTADO_CERRANDO;
        }
    }
}
int Func_ESTADO_INICIAL(void)
{
    ESTADO_ANT = ESTADO_ACT;
    ESTADO_ACT = ESTADO_INICIAL;

    io.LED = VERDADERO;
    io.MotorApertura = FALSO;
    io.MotorCierre = FALSO;

    if ((io.SensorApertura == VERDADERO) && (io.SensorCierre == VERDADERO))
    {
        return ESTADO_ERROR; 
    }
    if ((io.SensorApertura == VERDADERO) && (io.SensorCierre == FALSO))
    {
        return ESTADO_ABIERTO;
    }
    if ((io.SensorCierre == VERDADERO) && (io.SensorApertura == FALSO))
    {
        return ESTADO_CERRADO;
    }
    if ((io.SensorApertura == FALSO) && (io.SensorCierre == FALSO))
    {
        return ESTADO_INTERMEDIO;
    }
}

void intGeneral100ms(void)
{

    static int cont_int = 0;

    cont_int++;

    if (cont_int == 9)
    {
        CntTiempo++;
        cont_int = 0;
    }
    if (leer_GPIO(PUERTO, PIN))
    {
    }
}
