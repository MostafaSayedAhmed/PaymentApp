
#include "app.h"

typedef enum
{
    SAME_ACC = 1,
    DIFF_ACC,
    END_PROG
}options_t;


int main(void)
{
    ST_cardData_t           *userCard  = NULL;
    ST_terminalData_t       *userTerm  = NULL;
    ST_transaction_t        *userTrans = NULL;

    EN_cardError_t          retCard  = CARD_OK;
    EN_terminalError_t      retTerm  = TERMINAL_OK;
    EN_serverError_t        retServ  = SERVER_OK;
    EN_transState_t         retstate = RUNNING;

    options_t input = END_PROG;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date[11] = {0};
    sprintf(date,"%02d/%02d/%d",tm->tm_mday,tm->tm_mon + 1,tm->tm_year + 1900);
//  printf("Date = %s \n",date);

    Again:
    printf("Card Module : \n");
    printf("################################\n");
    userCard = (ST_cardData_t *) malloc(sizeof(ST_cardData_t ));


    do{
        retCard = getCardHolderName(userCard);
        if(retCard != CARD_OK ) printf("Wrong Name !!\nTry Again !\n");
    }
    while(retCard != CARD_OK);


    do{
        retCard = getCardExpiryDate(userCard);
        if(retCard != CARD_OK ) printf("Wrong Expiry Date !!\nTry Again !\n");
    }
    while(retCard != CARD_OK);


    do{
        retCard = getCardPAN(userCard);
        if(retCard != CARD_OK ) printf("Wrong PAN !!\nTry Again !\n");
    }
    while(retCard != CARD_OK);


    printf("Terminal Module : \n");
    printf("################################\n");
    userTerm = (ST_terminalData_t *) calloc(1,sizeof(ST_terminalData_t ));
    strcpy(userTerm->transactionDate,date);
    printf("Transaction Date        : %s \n",userTerm->transactionDate) ;

    setMaxAmount(userTerm,MAXAMOUNT);


    if(isCardExpired(userCard,userTerm) == EXPIRED_CARD)
    {
        printf("Invalid Card Date : Restarting \n");
        Here:
        free(userCard);
        free(userTerm);
        goto Again;
    }
    else                                                 printf("Valid Card Date : Proceeding \n");
    Transaction:
    do
    {
    retTerm = getTransactionAmount(userTerm);
    switch(retTerm)
    {
        case INVALID_AMOUNT   : printf("Invalid Amount    : Try Again \n");goto Here   ;break;
        case EXCEED_MAX_AMOUNT: printf("Exceed Max Amount : Try Again \n");             break;
    }
    }while(retTerm != TERMINAL_OK);
    printf("Valid Amount  : Proceeding \n");


    printf("Server Module : \n");
    printf("################################\n");

    userTrans = (ST_terminalData_t *) calloc(1,sizeof(ST_transaction_t ));

    strcpy(userTrans->cardHolderData.cardExpirationDate,userCard->cardExpirationDate);
    strcpy(userTrans->cardHolderData.cardHolderName,userCard->cardHolderName);
    strcpy(userTrans->cardHolderData.primaryAccountNumber,userCard->primaryAccountNumber);

    userTrans->terminalData.maxTransAmount = userTerm->maxTransAmount;
    strcpy(userTrans->terminalData.transactionDate , userTerm->transactionDate);
    userTrans->terminalData.transAmount = userTerm->transAmount;


    retstate = recieveTransactionData(userTrans);
    switch(retstate)
    {
    case    APPROVED:
        printf("Transaction Approved And Saved \n");
        break;
    case    DECLINED_INSUFFECIENT_FUND:
        printf("Insufficient Fund !!\n");
        break;
    case    DECLINED_STOLEN_CARD:
            printf("Declined Stolen Card !!\n");
        break;
    case    FRAUD_CARD:
        printf("Non Existing Card : Fraud Card !!\n");
        break;
    case    INTERNAL_SERVER_ERROR:
        printf("Internal Server Error \n");
        break;
    }
    listSavedTransactions();
    printf("Thank you for your Transaction \n");
    printf("Would you like other service ?\n");
    printf("1.Yes, another transaction using this account ?\n");
    printf("2.Yes, different account transaction ?\n");
    printf("3.No, Thanks (End Program) ?\n");
    printf("Enter Option Number : ");
    scanf ("%d",&input);

    free(userTrans);
    switch(input)
    {
    case SAME_ACC:
        goto Transaction;
        break;

    case DIFF_ACC:
        goto Here;
        break;

    case END_PROG:
        printf("Goodbye !");
        break;
    }



    //saveTransactionTest();

    return 0;


}

void appStart(void);

