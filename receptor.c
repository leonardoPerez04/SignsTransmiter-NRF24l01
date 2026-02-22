#include <Pantalla.h>
#include <string.h>
#include <stdlib.h>

#define BTN_ROJO     GPIOB_IDR.B0
#define BTN_AZUL     GPIOB_IDR.B1
#define BTN_AMAR     GPIOB_IDR.B2
#define BTN_VERDE    GPIOB_IDR.B8

// --- HARDWARE NRF24 ---
sbit CE  at GPIOC_ODR.B0;
sbit CSN at GPIOC_ODR.B2;

char buffer_tx[32];
char ultimo_estado[32] = "";

// --------------------- PANTALLA TX (MEJORADA) ---------------------
void Init_TFT_UI() {
    InitPantalla();
    TFT_Fill_Screen(CL_BLACK);

    // --- ESTILO VISUAL: MARCO "PADRE" ---
    // Dibujamos un marco blanco alrededor de la zona de información
    TFT_Set_Pen(CL_WHITE, 2);
    TFT_Set_Brush(0, CL_BLACK, 0, 0, 0, 0); // Brush transparente
    TFT_Rectangle(5, 5, 315, 235); // Borde de pantalla

    // Título con fondo de color para que resalte
    TFT_Set_Pen(CL_WHITE, 1);
    TFT_Set_Brush(1, 0x8410, 0, 0, 0, 0); // Un gris/azul oscuro de fondo para el titulo
    TFT_Rectangle(6, 6, 314, 40);

    TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);
    TFT_Write_Text("CONTROL DE MANDO RF", 80, 15);

    TFT_Set_Font(TFT_defaultFont, CL_SILVER, FO_HORIZONTAL);
    TFT_Write_Text("ESTADO ACTUAL:", 20, 70);
}


void MostrarTX(char *txt, unsigned int color) {
    // Solo actualizamos si cambia el mensaje
    if (strcmp(ultimo_estado, txt) != 0) {

        // 1. Limpiamos el área de dibujo anterior (Rectángulo negro)
        //    Abarcamos zona de texto y zona del círculo LED
        TFT_Set_Pen(CL_BLACK, 1);
        TFT_Set_Brush(1, CL_BLACK, 0, LEFT_TO_RIGHT, CL_BLACK, CL_BLACK);
        TFT_Rectangle(10, 90, 310, 200);

        // 2. DIBUJAR "LED" GIGANTE (Personalización Visual)
        // Si es "ESPERANDO...", dibujamos el círculo vacío (solo borde)
        // Si es un color, lo rellenamos.

        if(strcmp(txt, "ESPERANDO...") == 0) {
             TFT_Set_Pen(CL_GRAY, 3); // Borde gris
             TFT_Set_Brush(0, CL_BLACK, 0, 0, 0, 0); // Sin relleno
             TFT_Circle(160, 130, 30); // Círculo en el centro
        } else {
             TFT_Set_Pen(CL_WHITE, 2); // Borde blanco para resaltar
             TFT_Set_Brush(1, color, 0, 0, 0, 0); // Relleno del color del botón
             TFT_Circle(160, 130, 30); // Círculo "Encendido"
        }

        // 3. Escribimos el nombre del color debajo del LED
        TFT_Set_Font(TFT_defaultFont, color, FO_HORIZONTAL);

        // Centramos un poco el texto (ajuste manual aproximado)
        if (strlen(txt) > 6)
            TFT_Write_Text(txt, 110, 170); // Texto largo
        else
            TFT_Write_Text(txt, 135, 170); // Texto corto (ROJO, AZUL)

        // 4. Guardamos el estado
        strcpy(ultimo_estado, txt);
    }
}

// --------------------- NRF24 TX (Igual que antes) ---------------------
void Init_SPI() {
    SPI1_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT |
                       _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION |
                       _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE |
                       _SPI_SSI_1, &_GPIO_MODULE_SPI1_PA567);
}

void nrf24_write_reg(char reg, char value) {
    CSN = 0;
    SPI1_Write(0x20 | reg);
    SPI1_Write(value);
    CSN = 1;
}

void nrf24_config_tx() {
    CE = 0;
    nrf24_write_reg(0x00, 0x0E);
    nrf24_write_reg(0x01, 0x00);
    nrf24_write_reg(0x03, 0x03);
    nrf24_write_reg(0x04, 0x00);
    nrf24_write_reg(0x05, 0x42);
    nrf24_write_reg(0x06, 0x0F);

    CSN = 0;
    SPI1_Write(0x20 | 0x10);
    SPI1_Write('A'); SPI1_Write('A'); SPI1_Write('A'); SPI1_Write('A'); SPI1_Write('A');
    CSN = 1;

    CE = 1;
    Delay_ms(2);
}

void nrf24_send(char *data_out) {
    char i;
    CE = 0;
    CSN = 0;
    SPI1_Write(0xA0);
    for (i = 0; i < 32; i++) {
        SPI1_Write(data_out[i]);
    }
    CSN = 1;
    CE = 1;
    Delay_us(20);
    CE = 0;
}


void main() {

    GPIO_Config(&GPIOB_BASE,
                _GPIO_PINMASK_0 | _GPIO_PINMASK_1 | _GPIO_PINMASK_2 | _GPIO_PINMASK_8,
                _GPIO_CFG_MODE_INPUT | _GPIO_CFG_PULL_UP);


    GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_0 | _GPIO_PINMASK_2);

    Init_SPI();
    Init_TFT_UI();
    nrf24_config_tx();

    while (1) {
        memset(buffer_tx, 0, 32);


        if (BTN_ROJO == 0) {
            Delay_ms(20);
            if (BTN_ROJO == 0) {
                strcpy(buffer_tx, "ROJO");
                MostrarTX("ROJO", CL_RED);
            }
        }
        else if (BTN_AZUL == 0) {
            Delay_ms(20);
            if (BTN_AZUL == 0) {
                strcpy(buffer_tx, "AZUL");
                MostrarTX("AZUL", CL_BLUE);
            }
        }
        else if (BTN_AMAR == 0) {
            Delay_ms(20);
            if (BTN_AMAR == 0) {
                strcpy(buffer_tx, "AMARILLO");
                MostrarTX("AMARILLO", CL_YELLOW);
            }
        }
        else if (BTN_VERDE == 0) {
            Delay_ms(20);
            if (BTN_VERDE == 0) {
                strcpy(buffer_tx, "VERDE");
                MostrarTX("VERDE", CL_GREEN);
            }
        }


        nrf24_send(buffer_tx);
        Delay_ms(50);
    }
}
