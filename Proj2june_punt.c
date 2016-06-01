//
//  Progetto per la board
//  Consegna: 2 Giugno 2016
//
//  Matteo Greco - Matricola: 807205
//  Nicolò Grassi - Matricola: 808940
//

#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "xil_io.h"
#include "xparameters.h"
#include "xgpio_l.h"
#include "microblaze_sleep.h"

#define BTNADDR XPAR_BTNS_GPIO_BASEADDR
#define LEDADDR XPAR_SWSLEDS_GPIO_BASEADDR
#define LED_OFFSET XGPIO_DATA2_OFFSET

void codifica_gray(uint8_t *, uint8_t *);
int buttons_operations(uint8_t *);

int main() {
    
    init_platform();
    
    uint8_t val_switch, val_gray;
    int check;
    
    // Leggo il valore degli switch
    val_switch = Xil_In8(LEDADDR);
    
    // Richiamo una funzione che lo converte in codifica Gray
    codifica_gray(&val_switch, &val_gray);
    
    // Lo stampo sui led
    Xil_Out8(LEDADDR+LED_OFFSET, val_gray);
    
    while (1) {
        
        // Richiamo una funzione che effettua l'operazione in base al bottone premuto e mi ritorna 1 se è stato premuto un bottone.
        check = buttons_operations(&val_switch);
        
        // Se il check è uguale a 1 significa che è stato premuto un bottone. Quindi converto il val_switch in codifica gray e lo stampo sui Led.
        if(check==1) {
            codifica_gray(&val_switch, &val_gray);
            Xil_Out8(LEDADDR+LED_OFFSET, val_gray);
        }
    }
    
    cleanup_platform();
    
    return 0;
}

// Funzione che converte il valore degli switch in codifica Gray
void codifica_gray(uint8_t *val1, uint8_t *val2) {
    
    int i;
    
    for(i=0; i<8; i++)
        *val2 = *val1 ^ (*val1 >> 1);
}


// Funzione che, in base al bottone premuto, effettua la relativa operazione sul val degli switch
int buttons_operations (uint8_t *val){
    
    // Leggo il valore dei bottoni
    uint8_t val_buttons = Xil_In8(BTNADDR);
    
    // Applico una MB_Sleep per evitare di leggere più volte il bottone durante un'unica pressione del tasto
    MB_Sleep(250);
    
    // Ho aggiunto dei controlli sui valori poichè ho solo 4 led (4 bit) e quindi posso rappresentare solo 16 valori sui led (0, 1, ..., 15). Senza queste condizioni, superato il num 15, sarei riuscito a rappresentare sui led solo gli ultimi 4 bit del numero. Ho aggiunto anche una condizione per l'operazione di sottrazione perchè ho notato che, partendo da zero e premendo due volte il bottone -, il val_switch ripartiva da 1 e, premendo di nuovo il bottone -, aumentava di 1 il valore, come se fosse il bottone +.
    if((val_buttons == 1) && (*val<15)) {
        (*val)++;
        return 1;
    }
        else if((val_buttons == 2) && (!(*val==0))) {
            (*val)--;
            return 1;
        }
            else if((val_buttons == 4) && (*val<8)) {
                *val = *val * 2;
                return 1;
            }
                else if(val_buttons == 8) {
                    *val = *val / 2;
                    return 1;
                }
        
    return 0;
    
    }




