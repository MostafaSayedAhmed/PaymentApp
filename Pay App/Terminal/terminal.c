
#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    EN_terminalError_t ret = TERMINAL_OK;
    printf("Enter The Transaction Date : ");
    gets(termData->transactionDate);
    fflush(stdin);

    if((termData->transactionDate == NULL)      ||
       (strlen(termData->transactionDate) > 10) ||
       (termData->transactionDate[2] != '/')    ||
       (termData->transactionDate[5] != '/')    )
    {
        ret = WRONG_DATE;
    }
    else
    {
        ret = TERMINAL_OK;
    }

    return ret;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    EN_terminalError_t ret = TERMINAL_OK;

    if((NULL == cardData->cardExpirationDate )|| (NULL == termData->transactionDate) )
    {
        ret = INVALID_CARD;
    }
    else if( ((strcmp(cardData->cardExpirationDate , (termData->transactionDate + 3)) >= 0) &&
             (strcmp((cardData->cardExpirationDate + 3) , (termData->transactionDate + 8)) < 0))   ||

             (((strcmp(cardData->cardExpirationDate , (termData->transactionDate + 3)) < 0) &&
             (strcmp((cardData->cardExpirationDate + 3) , (termData->transactionDate + 8)) <= 0)))
            )
    {
         ret = EXPIRED_CARD;
    }
    else
    {
        ret =  TERMINAL_OK;
    }

    return ret;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    EN_terminalError_t ret = TERMINAL_OK;

    printf("Enter The Transaction Amount : ");
    scanf("%f",&(termData->transAmount));
    fflush(stdin);

    if(termData->transAmount <= 0)
    {
        ret = INVALID_AMOUNT;
    }
    else
    {
        ret = isBelowMaxAmount(termData);
    }

    return ret;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    EN_terminalError_t ret = TERMINAL_OK;
    if(termData->transAmount > termData->maxTransAmount)
    {
        ret = EXCEED_MAX_AMOUNT;
    }
    else
    {
        ret = TERMINAL_OK;
    }

    return ret;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, f32 maxAmount)
{
    EN_terminalError_t ret = TERMINAL_OK ;
    if(termData == NULL)
    {
        ret = INVALID_MAX_AMOUNT;
    }
    else
    {
        if(maxAmount <= 0)
        {
            ret = INVALID_MAX_AMOUNT;
        }
        else
        {
            termData->maxTransAmount = maxAmount;
            ret = TERMINAL_OK;

        }
    }

    return ret;

}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData); //Optional

void               getTransactionDateTest(void)
{
    ST_terminalData_t test = {0,0,NULL} ;
    EN_cardError_t ret ;

    printf("Tester Name     : Mostafa Sayed \n");
    printf("Function Name   : isValidCardPAN \n");

    printf("Test Case 1     : \n");
    printf("Input Data      : %s \n","NULL");
    ret = getTransactionDate(&test);
    printf("Expected Result : %s \n","WRONG_DATE");
    printf("Actual Result   : %s \n",(ret == WRONG_DATE)?"WRONG_DATE":"TERMINAL_OK");

    printf("Test Case 2     : \n");
    printf("Input Data      : %s \n","25/06/2022");
    ret = getTransactionDate(&test);
    printf("Expected Result : %s \n","TERMINAL_OK");
    printf("Actual Result   : %s \n",(ret == WRONG_DATE)?"WRONG_DATE":"TERMINAL_OK");

    printf("Test Case 3     : \n");
    printf("Input Data      : %s \n","2506/2022");
    ret = getTransactionDate(&test);
    printf("Expected Result : %s \n","WRONG_DATE");
    printf("Actual Result   : %s \n",(ret == WRONG_DATE)?"WRONG_DATE":"TERMINAL_OK");

    printf("Test Case 4     : \n");
    printf("Input Data      : %s \n","25/062022");
    ret = getTransactionDate(&test);
    printf("Expected Result : %s \n","WRONG_DATE");
    printf("Actual Result   : %s \n",(ret == WRONG_DATE)?"WRONG_DATE":"TERMINAL_OK");

}

void               isCardExpriedTest     (void)
{
    ST_cardData_t       cardData ;
    ST_terminalData_t   termData ;
    EN_terminalError_t  ret     ;


    printf("Test Case 1     : \n");
    printf("Input     Data      :  \n");
    printf("Card      Data      : %s \n","07/23");
    printf("Terminal  Data      : %s \n","25/06/2022");
    strcpy(cardData.cardExpirationDate,"07/23");
    strcpy(termData.transactionDate,"25/06/2022");

    ret = isCardExpired(&cardData,&termData);
    printf("Expected Result : %s \n","TERMINAL_OK");

    switch(ret)
    {
        case EXPIRED_CARD: printf("Actual Result   : %s \n","EXPIRED_CARD"); break;
        case TERMINAL_OK : printf("Actual Result   : %s \n","TERMINAL_OK") ; break;
        case INVALID_CARD: printf("Actual Result   : %s \n","INVALID_CARD"); break;
    }


    printf("Test Case 2     : \n");
    printf("Input     Data      :  \n");
    printf("Card      Data      : %s \n","05/23");
    printf("Terminal  Data      : %s \n","25/06/2022");
    strcpy(cardData.cardExpirationDate,"05/23");
    strcpy(termData.transactionDate,"25/06/2022");

    ret = isCardExpired(&cardData,&termData);
    printf("Expected Result : %s \n","TERMINAL_OK");

    switch(ret)
    {
        case EXPIRED_CARD: printf("Actual Result   : %s \n","EXPIRED_CARD"); break;
        case TERMINAL_OK : printf("Actual Result   : %s \n","TERMINAL_OK") ; break;
        case INVALID_CARD: printf("Actual Result   : %s \n","INVALID_CARD"); break;
    }


    printf("Test Case 3     : \n");
    printf("Input     Data      :  \n");
    printf("Card      Data      : %s \n","07/22");
    printf("Terminal  Data      : %s \n","25/06/2022");
    strcpy(cardData.cardExpirationDate,"07/22");
    strcpy(termData.transactionDate,"25/06/2022");

    ret = isCardExpired(&cardData,&termData);
    printf("Expected Result : %s \n","TERMINAL_OK");

    switch(ret)
    {
        case EXPIRED_CARD: printf("Actual Result   : %s \n","EXPIRED_CARD"); break;
        case TERMINAL_OK : printf("Actual Result   : %s \n","TERMINAL_OK") ; break;
        case INVALID_CARD: printf("Actual Result   : %s \n","INVALID_CARD"); break;
    }

    printf("Test Case 4     : \n");
    printf("Input     Data      :  \n");
    printf("Card      Data      : %s \n","05/22");
    printf("Terminal  Data      : %s \n","25/06/2022");
    strcpy(cardData.cardExpirationDate,"05/22");
    strcpy(termData.transactionDate,"25/06/2022");

    ret = isCardExpired(&cardData,&termData);
    printf("Expected Result : %s \n","EXPIRED_CARD");

    switch(ret)
    {
        case EXPIRED_CARD: printf("Actual Result   : %s \n","EXPIRED_CARD"); break;
        case TERMINAL_OK : printf("Actual Result   : %s \n","TERMINAL_OK") ; break;
        case INVALID_CARD: printf("Actual Result   : %s \n","INVALID_CARD"); break;
    }

    printf("Test Case 5     : \n");
    printf("Input     Data      :  \n");
    printf("Card      Data      : %s \n","07/21");
    printf("Terminal  Data      : %s \n","25/06/2022");
    strcpy(cardData.cardExpirationDate,"07/21");
    strcpy(termData.transactionDate,"25/06/2022");

    ret = isCardExpired(&cardData,&termData);
    printf("Expected Result : %s \n","EXPIRED_CARD");

    switch(ret)
    {
        case EXPIRED_CARD: printf("Actual Result   : %s \n","EXPIRED_CARD"); break;
        case TERMINAL_OK : printf("Actual Result   : %s \n","TERMINAL_OK") ; break;
        case INVALID_CARD: printf("Actual Result   : %s \n","INVALID_CARD"); break;
    }
}

void               getTransactionAmountTest(void)
{
    ST_terminalData_t termData;
    EN_terminalError_t ret    ;

    printf("Test Case 1     : \n");
    printf("Input     Data      :  15.5\n");
    ret = getTransactionAmount(&termData);

    printf("Stored Value    : %f\n",termData.transAmount);
    printf("Expected Result : %s \n","TERMINAL_OK");
    printf("Actual Result   : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"INVALID_AMOUNT");

    printf("Test Case 2     : \n");
    printf("Input     Data      :  0\n");
    ret = getTransactionAmount(&termData);

    printf("Stored Value    : %f\n",termData.transAmount);
    printf("Expected Result : %s \n","INVALID_AMOUNT");
    printf("Actual Result   : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"INVALID_AMOUNT");

    printf("Test Case 3     : \n");
    printf("Input     Data      :  -15.5\n");
    ret = getTransactionAmount(&termData);

    printf("Stored Value    : %f\n",termData.transAmount);
    printf("Expected Result : %s \n","INVALID_AMOUNT");
    printf("Actual Result   : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"INVALID_AMOUNT");



}

void               isBelowMaxAmountTest    (void)
{
    ST_terminalData_t termData;
    EN_terminalError_t ret    ;
    f32                temp   ;

    printf("Test Case 1     : \n");
    printf("Input     Data      :  \n");

    printf("Max Amount          : %f\n",50.5);

    printf("Enter Max Amount    : ");
    scanf("%f",&temp);
    fflush(stdin);
    ret = setMaxAmount(&termData,temp);
    printf("Transaction Amount  : %f\n",10.0);
    ret = getTransactionAmount(&termData);
    ret = isBelowMaxAmount(&termData);
    printf("Expected Result     : %s \n","TERMINAL_OK");
    printf("Actual Result       : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"EXCEED_MAX_AMOUNT");


    printf("Test Case 2     : \n");
    printf("Input     Data      :  \n");

    printf("Max Amount          : %f\n",50.5);

    printf("Enter Max Amount    : ");
    scanf("f",&temp);
    fflush(stdin);
    ret = setMaxAmount(&termData,temp);
    printf("Transaction Amount  : %f\n",50.5);
    ret = getTransactionAmount(&termData);
    ret = isBelowMaxAmount(&termData);
    printf("Expected Result     : %s \n","TERMINAL_OK");
    printf("Actual Result       : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"EXCEED_MAX_AMOUNT");


    printf("Test Case 3     : \n");
    printf("Input     Data      :  \n");

    printf("Max Amount          : %f\n",50.5);

    printf("Enter Max Amount    : ");
    scanf("f",&temp);
    fflush(stdin);
    ret = setMaxAmount(&termData,temp);
    printf("Transaction Amount  : %f\n",60.0);
    ret = getTransactionAmount(&termData);
    ret = isBelowMaxAmount(&termData);
    printf("Expected Result     : %s \n","EXCEED_MAX_AMOUNT");
    printf("Actual Result       : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"EXCEED_MAX_AMOUNT");
}

void               setMaxAmountTest(void)
{
    ST_terminalData_t termData;
    EN_terminalError_t ret    ;
    f32                temp   ;

    printf("Test Case 1     : \n");
    printf("Input     Data      :  \n");

    printf("Max Amount          : %f\n",50.5);
    printf("Enter Max Amount    : ");
    scanf("%f",&temp);
    fflush(stdin);
    ret = setMaxAmount(&termData,temp);

    printf("Expected Result     : %s \n","TERMINAL_OK");
    printf("Actual Result       : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"INVALID_MAX_AMOUNT");

    printf("Test Case 2     : \n");
    printf("Input     Data      :  \n");

    printf("Max Amount          : %f\n",0.0);
    printf("Enter Max Amount    : ");
    scanf("%f",&temp);
    fflush(stdin);
    ret = setMaxAmount(&termData,temp);

    printf("Expected Result     : %s \n","INVALID_MAX_AMOUNT");
    printf("Actual Result       : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"INVALID_MAX_AMOUNT");

    printf("Test Case 3     : \n");
    printf("Input     Data      :  \n");

    printf("Max Amount          : %f\n",-10.0);
    printf("Enter Max Amount    : ");
    scanf("%f",&temp);
    fflush(stdin);
    ret = setMaxAmount(&termData,temp);

    printf("Expected Result     : %s \n","INVALID_MAX_AMOUNT");
    printf("Actual Result       : %s \n",(ret == TERMINAL_OK)? "TERMINAL_OK" :"INVALID_MAX_AMOUNT");

}

