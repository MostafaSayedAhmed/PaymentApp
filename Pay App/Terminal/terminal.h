
#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "../LIB/STD_Types.h"
#include "../Card/card.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ST_terminalData_t
{
    float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
    TERMINAL_OK,
    WRONG_DATE,
    EXPIRED_CARD,
    INVALID_CARD,
    INVALID_AMOUNT,
    EXCEED_MAX_AMOUNT,
    INVALID_MAX_AMOUNT
}EN_terminalError_t ;

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData);

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, f32 maxAmount);

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData); //Optional

/*      Test Functions        */
void               getTransactionDateTest  (void);
void               isCardExpriedTest       (void);
void               getTransactionAmountTest(void);
void               isBelowMaxAmountTest    (void);
void               setMaxAmountTest        (void);


#endif
