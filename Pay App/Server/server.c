
#include "server.h"

static ST_accountsDB_t accountsDB[255] = {
                                   {20000.0,   RUNNING,  "8989374615436851"},
                                   {100000.0, BLOCKED,  "5807007076043875"},
                                   {546627.0, BLOCKED,  "705571243707987"},
                                   {743557.0, RUNNING,  "2332285098668637"},
                                   {253899.0, RUNNING,  "6025406564483340"}
                                  };

static ST_transaction_t    transDB[255] = {0};

static uint8_t               DB_Counter   = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    EN_transState_t   ret     = FRAUD_CARD ;
    EN_serverError_t  retServ = SERVER_OK ;
    uint8_t counter = 0;
    if(NULL == transData)
    {
        ret = INTERNAL_SERVER_ERROR;
    }
    else{
         for(counter = 0 ; counter < 255 ; counter++)
        {
        ret = FRAUD_CARD;

        if(strcmp(transData->cardHolderData.primaryAccountNumber , accountsDB[counter].primaryAccountNumber) == 0)
        {
            if(isBlockedAccount(&accountsDB[counter]) != SERVER_OK)
//          if(accountsDB[counter].state == BLOCKED)
            {

                ret = DECLINED_STOLEN_CARD;
            }
            else if((transData->terminalData.transAmount > accountsDB[counter].balance) ||
                    (isBelowMaxAmount(&(transData->terminalData)) == EXCEED_MAX_AMOUNT))
            {
                ret = DECLINED_INSUFFECIENT_FUND;
            }
            else if ((isCardExpired(&(transData->cardHolderData), &(transData->terminalData)) == EXPIRED_CARD))
            {
                 ret = INTERNAL_SERVER_ERROR;
            }
            else
            {
               // accountsDB[counter].balance = accountsDB[counter].balance - transData->terminalData.transAmount;
                ret = APPROVED;
                retServ = isAmountAvailable(&(transData->terminalData),&accountsDB[counter]);
                if(retServ == SERVER_OK)                {
                        do{
                            retServ = saveTransaction(transData);
                        }while(retServ == SAVING_FAILED);
                }
                else
                {
                    ret = DECLINED_INSUFFECIENT_FUND;
                }

            }
            break;
        }
        else if(accountsDB[counter].primaryAccountNumber[0] == '\0')   break;
        }

    }
        transData->transState = ret;
//     printf("%d\n",counter);
    return ret;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t   ret = ACCOUNT_NOT_FOUND ;
    uint8_t counter = 0;

    if(NULL == cardData || NULL == accountRefrence)
    {
        ret = ACCOUNT_NOT_FOUND;
    }
    else
    {
        for(counter = 0 ; counter < 255 ; counter++)
        {
            if(strcmp(cardData->primaryAccountNumber,accountsDB[counter].primaryAccountNumber) == 0)
           {
               ret = SERVER_OK;
               *accountRefrence = accountsDB[counter];
               break;
           }
           else if(accountsDB[counter].primaryAccountNumber[0] == '\0')
           {
               ret = ACCOUNT_NOT_FOUND;
               memset(accountRefrence->primaryAccountNumber ,'\0',20);
               accountRefrence->balance              = 0;
               accountRefrence->state                = BLOCKED;

               break;
           }
        }

    }
    return ret;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{

     EN_serverError_t   ret = ACCOUNT_NOT_FOUND ;

    if( NULL == accountRefrence)
    {
        ret = ACCOUNT_NOT_FOUND;
    }
    else
    {

        switch(accountRefrence->state)
        {
            case BLOCKED : ret = BLOCKED_ACCOUNT   ; break;
            case RUNNING : ret = SERVER_OK         ; break;
            default      : ret = ACCOUNT_NOT_FOUND ; break;
        }

    }
    return ret;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData,ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t ret = ACCOUNT_NOT_FOUND;

    if(termData == NULL || accountRefrence == NULL)
    {
        ret = TRANSACTION_NOT_FOUND;
    }
    else
    {
        if(termData->transAmount <= accountRefrence->balance)
        {
            accountRefrence->balance -= termData->transAmount;
            ret = SERVER_OK;
        }
        else
        {
            ret = LOW_BALANCE;
        }
    }
    return ret ;
}
EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    EN_serverError_t ret = SAVING_FAILED;
    if(NULL == transData)
    {
        ret = SAVING_FAILED;
    }
    else
    {

        transData->transactionSequenceNumber = DB_Counter ;
        transDB[DB_Counter] = *transData;
        ret = SERVER_OK;
        DB_Counter++;

    }
    return ret;
}
void listSavedTransactions(void)
{
    uint8_t fun_counter = 0;
    uint8_t counter     = 0;
    printf("List of All Save Transaction : \n");
    printf("######################### \n");
    printf("\n");
    for(fun_counter = 0 ; fun_counter < DB_Counter ; fun_counter++)
    {
    /*
        #########################
        Transaction Sequence Number:
        Transaction Date:
        Transaction Amount:
        Transaction State:
        Terminal Max Amount:
        Cardholder Name:
        PAN:
        Card Expiration Date:
        #########################
    */
        printf("Transaction Sequence Number      : %d    \n",transDB[fun_counter].transactionSequenceNumber);
        printf("Transaction Date                 : %s    \n",transDB[fun_counter].terminalData.transactionDate);
        printf("Transaction Amount               : %0.3f \n",transDB[fun_counter].terminalData.transAmount);

        printf("Transaction State                : ");
        switch(transDB[fun_counter].transState)
        {
            case APPROVED                  :  printf("APPROVED    \n")                        ; break;
            case DECLINED_INSUFFECIENT_FUND:  printf("DECLINED_INSUFFECIENT_FUND    \n")      ; break;
            case DECLINED_STOLEN_CARD      :  printf("DECLINED_STOLEN_CARD    \n")            ; break;
            case FRAUD_CARD                :  printf("FRAUD_CARD    \n")                      ; break;
            case INTERNAL_SERVER_ERROR     :  printf("INTERNAL_SERVER_ERROR    \n")           ; break;
        }

        printf("Terminal Max Amount              : %0.3f \n",transDB[fun_counter].terminalData.maxTransAmount);
        printf("Card Holder Name                 : %s    \n",transDB[fun_counter].cardHolderData.cardHolderName);
        printf("PAN                              : %s    \n",transDB[fun_counter].cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date             : %s    \n",transDB[fun_counter].cardHolderData.cardExpirationDate);
        for(counter = 0 ; counter < 255 ; counter++)
        {
            if(strcmp(transDB[fun_counter].cardHolderData.primaryAccountNumber , accountsDB[counter].primaryAccountNumber) == 0)
            {
                printf("Balance                          : %0.3f \n",accountsDB[counter].balance);
            }
            else if(accountsDB[counter].primaryAccountNumber[0] == '\0')   break;
        }


        printf("\n");
        printf("#########################\n");

    }
}

/* Test Functions */
void recieveTransactionDataTest(void)
{
    ST_transaction_t Test  ;
    EN_transState_t ret ;
    printf("Tester Name             : Mostafa Sayed \n");
    printf("Function Name           : recieveTransactionDataTest \n");

    printf("\n");
    printf("Test Case 1             : \n\n");

    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"8989374615436851");

    Test.terminalData.maxTransAmount = 2000.0;
    strcpy(Test.terminalData.transactionDate,"22/02/2022");
    Test.terminalData.transAmount = 1000.0;

    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);

    printf("Maximum Trans           : %f \n",Test.terminalData.maxTransAmount);
    printf("Transaction Date        : %s \n",Test.terminalData.transactionDate);
    printf("Trans Amount            : %f \n\n",Test.terminalData.transAmount);

    printf("Expected Result         : %s \n","APPROVED");

    ret = recieveTransactionData (&Test);

    printf("Actual Result           : ");

    switch(ret)
    {
        case APPROVED                   : printf("APPROVED \n")                  ; break;
        case DECLINED_INSUFFECIENT_FUND : printf("DECLINED_INSUFFECIENT_FUND \n"); break;
        case DECLINED_STOLEN_CARD       : printf("DECLINED_STOLEN_CARD \n")      ; break;
        case FRAUD_CARD                 : printf("FRAUD_CARD \n")                ; break;
        case INTERNAL_SERVER_ERROR      : printf("INTERNAL_SERVER_ERROR \n")     ; break;
    }

    printf("\n");
    printf("Test Case 2             : \n\n");

    strcpy(Test.cardHolderData.cardExpirationDate,"02/2021");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"8989374615436851");

    Test.terminalData.maxTransAmount = 2000.0;
    strcpy(Test.terminalData.transactionDate,"22/02/2022");
    Test.terminalData.transAmount = 1000.0;

    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);

    printf("Maximum Trans           : %f \n",Test.terminalData.maxTransAmount);
    printf("Transaction Date        : %s \n",Test.terminalData.transactionDate);
    printf("Trans Amount            : %f \n\n",Test.terminalData.transAmount);

    printf("Expected Result         : %s \n","INTERNAL_SERVER_ERROR");

    ret = recieveTransactionData (&Test);

    printf("Actual Result           : ");

    switch(ret)
    {
        case APPROVED                   : printf("APPROVED \n")                  ; break;
        case DECLINED_INSUFFECIENT_FUND : printf("DECLINED_INSUFFECIENT_FUND \n"); break;
        case DECLINED_STOLEN_CARD       : printf("DECLINED_STOLEN_CARD \n")      ; break;
        case FRAUD_CARD                 : printf("FRAUD_CARD \n")                ; break;
        case INTERNAL_SERVER_ERROR      : printf("INTERNAL_SERVER_ERROR \n")     ; break;
    }

    printf("\n");
    printf("Test Case 3             : \n");
    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"8989374615436851");

    Test.terminalData.maxTransAmount = 21000.0;
    strcpy(Test.terminalData.transactionDate,"22/02/2022");
    Test.terminalData.transAmount = 20000.0;

    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);

    printf("Maximum Trans           : %f \n",Test.terminalData.maxTransAmount);
    printf("Transaction Date        : %s \n",Test.terminalData.transactionDate);
    printf("Trans Amount            : %f \n\n",Test.terminalData.transAmount);

    printf("Expected Result         : %s \n","DECLINED_INSUFFECIENT_FUND");

    ret = recieveTransactionData (&Test);

    printf("Actual Result           : ");

    switch(ret)
    {
        case APPROVED                   : printf("APPROVED \n")                  ; break;
        case DECLINED_INSUFFECIENT_FUND : printf("DECLINED_INSUFFECIENT_FUND \n"); break;
        case DECLINED_STOLEN_CARD       : printf("DECLINED_STOLEN_CARD \n")      ; break;
        case FRAUD_CARD                 : printf("FRAUD_CARD \n")                ; break;
        case INTERNAL_SERVER_ERROR      : printf("INTERNAL_SERVER_ERROR \n")     ; break;
    }

    printf("\n");
    printf("Test Case 4             : \n");
    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"8989374615436951");

    Test.terminalData.maxTransAmount = 21000.0;
    strcpy(Test.terminalData.transactionDate,"22/02/2022");
    Test.terminalData.transAmount = 20000.0;

    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);

    printf("Maximum Trans           : %f \n",Test.terminalData.maxTransAmount);
    printf("Transaction Date        : %s \n",Test.terminalData.transactionDate);
    printf("Trans Amount            : %f \n\n",Test.terminalData.transAmount);

    printf("Expected Result         : %s \n","FRAUD_CARD");

    ret = recieveTransactionData (&Test);

    printf("Actual Result           : ");

    switch(ret)
    {
        case APPROVED                   : printf("APPROVED \n")                  ; break;
        case DECLINED_INSUFFECIENT_FUND : printf("DECLINED_INSUFFECIENT_FUND \n"); break;
        case DECLINED_STOLEN_CARD       : printf("DECLINED_STOLEN_CARD \n")      ; break;
        case FRAUD_CARD                 : printf("FRAUD_CARD \n")                ; break;
        case INTERNAL_SERVER_ERROR      : printf("INTERNAL_SERVER_ERROR \n")     ; break;
    }

    printf("\n");
    printf("Test Case 5             : \n");
    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"5807007076043875");

    Test.terminalData.maxTransAmount = 21000.0;
    strcpy(Test.terminalData.transactionDate,"22/02/2022");
    Test.terminalData.transAmount = 20000.0;

    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);

    printf("Maximum Trans           : %f \n",Test.terminalData.maxTransAmount);
    printf("Transaction Date        : %s \n",Test.terminalData.transactionDate);
    printf("Trans Amount            : %f \n\n",Test.terminalData.transAmount);

    printf("Expected Result         : %s \n","DECLINED_STOLEN_CARD");

    ret = recieveTransactionData (&Test);

    printf("Actual Result           : ");

    switch(ret)
    {
        case APPROVED                   : printf("APPROVED \n")                  ; break;
        case DECLINED_INSUFFECIENT_FUND : printf("DECLINED_INSUFFECIENT_FUND \n"); break;
        case DECLINED_STOLEN_CARD       : printf("DECLINED_STOLEN_CARD \n")      ; break;
        case FRAUD_CARD                 : printf("FRAUD_CARD \n")                ; break;
        case INTERNAL_SERVER_ERROR      : printf("INTERNAL_SERVER_ERROR \n")     ; break;
    }

    printf("\n");
    printf("Test Case 6             : \n");
    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"5807007076043875");

    Test.terminalData.maxTransAmount = 20000.0;
    strcpy(Test.terminalData.transactionDate,"22/02/2022");
    Test.terminalData.transAmount = 20000.0;

    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);

    printf("Maximum Trans           : %f \n",Test.terminalData.maxTransAmount);
    printf("Transaction Date        : %s \n",Test.terminalData.transactionDate);
    printf("Trans Amount            : %f \n\n",Test.terminalData.transAmount);

    printf("Expected Result         : %s \n","INTERNAL_SERVER_ERROR");

    ret = recieveTransactionData (NULL);

    printf("Actual Result           : ");

    switch(ret)
    {
        case APPROVED                   : printf("APPROVED \n")                  ; break;
        case DECLINED_INSUFFECIENT_FUND : printf("DECLINED_INSUFFECIENT_FUND \n"); break;
        case DECLINED_STOLEN_CARD       : printf("DECLINED_STOLEN_CARD \n")      ; break;
        case FRAUD_CARD                 : printf("FRAUD_CARD \n")                ; break;
        case INTERNAL_SERVER_ERROR      : printf("INTERNAL_SERVER_ERROR \n")     ; break;
    }



}

void             isValidAccountTest         (void)
{
   ST_transaction_t Test  ;
   ST_accountsDB_t  accountRefTest = { 0 , .primaryAccountNumber = {0} , .state = BLOCKED};
    EN_serverError_t ret ;
    printf("Tester Name             : Mostafa Sayed \n");
    printf("Function Name           : isValidAccountTest \n");

    printf("\n");
    printf("Test Case 1             : \n\n");

    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"8989374615436851");


    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);


    printf("Expected Result         : %s \n","SERVER_OK");

    ret = isValidAccount (&Test , &accountRefTest);

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("SERVER_OK \n")                  ; break;
        case ACCOUNT_NOT_FOUND           : printf("ACCOUNT_NOT_FOUND \n"); break;
    }
    if(accountRefTest.primaryAccountNumber[0] == '\0') printf("NULL Returned Reference : %s \n",accountRefTest.state == RUNNING ? "RUNNING":"BLOCKED");
    else                                               printf("Reference Account State : %s \n", accountRefTest.state == RUNNING ? "RUNNING":"BLOCKED");

    printf("\n");
    printf("Test Case 2             : \n\n");

    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"8989374615036850");


    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);


    printf("Expected Result         : %s \n","ACCOUNT_NOT_FOUND");

    ret = isValidAccount (&Test , &accountRefTest);

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("APPROVED \n")                  ; break;
        case ACCOUNT_NOT_FOUND           : printf("ACCOUNT_NOT_FOUND \n"); break;
    }
    if(accountRefTest.primaryAccountNumber[0] == '\0') printf("NULL Returned Reference : %s \n",accountRefTest.state == RUNNING ? "RUNNING":"BLOCKED");
    else                                               printf("Reference Account State : %s \n", accountRefTest.state == RUNNING ? "RUNNING":"BLOCKED");

    printf("\n");
    printf("Test Case 3             : \n\n");

    strcpy(Test.cardHolderData.cardExpirationDate,"02/2022");
    strcpy(Test.cardHolderData.cardHolderName,"Mostafa Sayed Ahmed Taha");
    strcpy(Test.cardHolderData.primaryAccountNumber,"8989374615036850");


    printf("Input Data              :  \n\n");
    printf("Card Holder Name        : %s \n",Test.cardHolderData.cardHolderName);
    printf("Expiration Date         : %s \n",Test.cardHolderData.cardExpirationDate);
    printf("Primary Account Number  : %s \n",Test.cardHolderData.primaryAccountNumber);


    printf("Expected Result         : %s \n","ACCOUNT_NOT_FOUND");

    ret = isValidAccount (NULL , &accountRefTest);

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("APPROVED \n")                  ; break;
        case ACCOUNT_NOT_FOUND           : printf("ACCOUNT_NOT_FOUND \n")         ; break;
    }
    if(accountRefTest.primaryAccountNumber[0] == '\0') printf("NULL Returned Reference : %s \n",accountRefTest.state == RUNNING ? "RUNNING":"BLOCKED");
    else                                               printf("Reference Account State : %s \n", accountRefTest.state == RUNNING ? "RUNNING":"BLOCKED");



}
void             isBlockedAccountTest       (void)
{
    ST_accountsDB_t *Test  ;
    EN_serverError_t ret ;
    printf("Tester Name             : Mostafa Sayed \n");
    printf("Function Name           : isBlockedAccountTest \n");

    printf("\n");
    printf("Test Case 1             : \n\n");

    Test = &accountsDB[0];


    printf("Input Data              :  \n\n");
    printf("Balance                 : %0.3f \n",Test->balance);
    printf("State                   : %s \n",Test->state == RUNNING ? "RUNNING" : "BLOCKED" );
    printf("Primary Account Number  : %s \n",Test->primaryAccountNumber);


    printf("Expected Result         : %s \n","SERVER_OK");

    ret = isBlockedAccount (Test );

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("SERVER_OK \n")                  ; break;
        case ACCOUNT_NOT_FOUND           : printf("ACCOUNT_NOT_FOUND \n")          ; break;
        case BLOCKED_ACCOUNT             : printf("BLOCKED_ACCOUNT \n")            ; break;
    }

    printf("\n");
    printf("Test Case 2             : \n\n");

    Test = &accountsDB[1];

    printf("Input Data              :  \n\n");
    printf("Balance                 : %0.3f \n",Test->balance);
    printf("State                   : %s \n",Test->state == RUNNING ? "RUNNING" : "BLOCKED" );
    printf("Primary Account Number  : %s \n",Test->primaryAccountNumber);


    printf("Expected Result         : %s \n","BLOCKED_ACCOUNT");

    ret = isBlockedAccount (Test );

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("SERVER_OK \n")                  ; break;
        case ACCOUNT_NOT_FOUND           : printf("ACCOUNT_NOT_FOUND \n")          ; break;
        case BLOCKED_ACCOUNT             : printf("BLOCKED_ACCOUNT \n")            ; break;
    }

    printf("\n");
    printf("Test Case 3             : \n\n");

    Test = NULL;

    printf("Input Data              :  \n\n");
    printf("Balance                 :  \n");
    printf("State                   :  \n");
    printf("Primary Account Number  :  \n");


    printf("Expected Result         : %s \n","ACCOUNT_NOT_FOUND");

    ret = isBlockedAccount (Test);

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("SERVER_OK \n")                  ; break;
        case ACCOUNT_NOT_FOUND           : printf("ACCOUNT_NOT_FOUND \n")          ; break;
        case BLOCKED_ACCOUNT             : printf("BLOCKED_ACCOUNT \n")            ; break;
    }


}
void             isAmountAvailableTest      (void)
{
    ST_accountsDB_t *TestRefDB  ;
    ST_terminalData_t TestTerm  ;

    EN_serverError_t ret ;

    printf("Tester Name             : Mostafa Sayed \n");
    printf("Function Name           : isAmountAvailableTest \n");

    printf("\n");
    printf("Test Case 1             : \n\n");

    TestRefDB = &accountsDB[0];


    printf("Input Data              :  \n\n");
    printf("Balance                 : %0.3f \n",TestRefDB->balance);
    printf("State                   : %s \n",TestRefDB->state == RUNNING ? "RUNNING" : "BLOCKED" );
    printf("Primary Account Number  : %s \n",TestRefDB->primaryAccountNumber);

    TestTerm.maxTransAmount  = 100000 ;
    strcpy(TestTerm.transactionDate , "25/06/2022");
    TestTerm.transAmount     = 1000 ;

    printf("Max Transaction Amount  : %0.3f \n",TestTerm.maxTransAmount);
    printf("Transaction Date        : %s \n",TestTerm.transactionDate );
    printf("Transaction Amount      : %0.3f \n",TestTerm.transAmount);


    printf("Expected Result         : %s \n","SERVER_OK");

    ret = isAmountAvailable (&TestTerm,TestRefDB );

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("SERVER_OK \n")                  ; break;
        case TRANSACTION_NOT_FOUND       : printf("TRANSACTION_NOT_FOUND \n")      ; break;
        case LOW_BALANCE                 : printf("LOW_BALANCE \n")                ; break;
    }

    printf("\n");
    printf("Test Case 2             : \n\n");

    TestRefDB = &accountsDB[0];


    printf("Input Data              :  \n\n");
    printf("Balance                 : %0.3f \n",TestRefDB->balance);
    printf("State                   : %s \n",TestRefDB->state == RUNNING ? "RUNNING" : "BLOCKED" );
    printf("Primary Account Number  : %s \n",TestRefDB->primaryAccountNumber);

    TestTerm.maxTransAmount  = 100000 ;
    strcpy(TestTerm.transactionDate , "25/06/2022");
    TestTerm.transAmount     = 50000 ;

    printf("Max Transaction Amount  : %0.3f \n",TestTerm.maxTransAmount);
    printf("Transaction Date        : %s \n",TestTerm.transactionDate );
    printf("Transaction Amount      : %0.3f \n",TestTerm.transAmount);


    printf("Expected Result         : %s \n","LOW_BALANCE");

    ret = isAmountAvailable (&TestTerm,TestRefDB );

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("SERVER_OK \n")                  ; break;
        case TRANSACTION_NOT_FOUND       : printf("TRANSACTION_NOT_FOUND \n")      ; break;
        case LOW_BALANCE                 : printf("LOW_BALANCE \n")                ; break;
    }

    printf("\n");
    printf("Test Case 2             : \n\n");

    TestRefDB = &accountsDB[0];


    printf("Input Data              :  \n\n");
    printf("Balance                 : %0.3f \n",TestRefDB->balance);
    printf("State                   : %s \n",TestRefDB->state == RUNNING ? "RUNNING" : "BLOCKED" );
    printf("Primary Account Number  : %s \n",TestRefDB->primaryAccountNumber);

    TestTerm.maxTransAmount  = 100000 ;
    strcpy(TestTerm.transactionDate , "25/06/2022");
    TestTerm.transAmount     = 2000 ;

    printf("Max Transaction Amount  : %0.3f \n",TestTerm.maxTransAmount);
    printf("Transaction Date        : %s \n",TestTerm.transactionDate );
    printf("Transaction Amount      : %0.3f \n",TestTerm.transAmount);


    printf("Expected Result         : %s \n","TRANSACTION_NOT_FOUND");

    ret = isAmountAvailable (NULL,TestRefDB );

    printf("Actual Result           : ");

    switch(ret)
    {
        case SERVER_OK                   : printf("SERVER_OK \n")                  ; break;
        case TRANSACTION_NOT_FOUND       : printf("TRANSACTION_NOT_FOUND \n")      ; break;
        case LOW_BALANCE                 : printf("LOW_BALANCE \n")                ; break;
    }

}
void             saveTransactionTest        (void)
{
    ST_transaction_t Test ;


    strcpy(Test.cardHolderData.cardExpirationDate       , "02/2022");
    strcpy(Test.cardHolderData.cardHolderName           , "Mostafa Sayed Ahmed");
    strcpy(Test.cardHolderData.primaryAccountNumber     , "8989374615436851");
    Test.terminalData.maxTransAmount       = 30000;
    strcpy(Test.terminalData.transactionDate      , "03/01/2021");
    Test.terminalData.transAmount          = 2000;
    Test.transState                        = APPROVED;

    saveTransaction(&Test);

    strcpy(Test.cardHolderData.cardExpirationDate       , "04/2002");
    strcpy(Test.cardHolderData.cardHolderName           , "Momen Zakaryia");
    strcpy(Test.cardHolderData.primaryAccountNumber     , "5807007076043875");
    Test.terminalData.maxTransAmount       = 100000;
    strcpy(Test.terminalData.transactionDate      , "03/02/2020");
    Test.terminalData.transAmount          = 4000;
    Test.transState                        = INTERNAL_SERVER_ERROR;

    saveTransaction(&Test);

    strcpy(Test.cardHolderData.cardExpirationDate , "01/2000");
    strcpy(Test.cardHolderData.cardHolderName     , "Someone Unknown");
    strcpy(Test.cardHolderData.primaryAccountNumber     , "5807007896043875");
    Test.terminalData.maxTransAmount       = 0;
    strcpy(Test.terminalData.transactionDate      , "31/12/1999");
    Test.terminalData.transAmount          = 0;
    Test.transState                        = FRAUD_CARD;

    saveTransaction(&Test);


    strcpy(Test.cardHolderData.cardExpirationDate , "02/2024");
    strcpy(Test.cardHolderData.cardHolderName     , "Ahmed Abdelrahman");
    strcpy(Test.cardHolderData.primaryAccountNumber     , "2332285098668637");
    Test.terminalData.maxTransAmount       = 743557;
    strcpy(Test.terminalData.transactionDate      , "03/01/2021");
    Test.terminalData.transAmount          = 6000000;
    Test.transState                        = DECLINED_INSUFFECIENT_FUND;

    saveTransaction(&Test);

    strcpy(Test.cardHolderData.cardExpirationDate , "02/2022");
    strcpy(Test.cardHolderData.cardHolderName     , "Samir Ahmed");
    strcpy(Test.cardHolderData.primaryAccountNumber     , "705571243707987");
    Test.terminalData.maxTransAmount       = 2;
    strcpy(Test.terminalData.transactionDate      , "03/01/2021");
    Test.terminalData.transAmount          = 1;
    Test.transState                        = DECLINED_STOLEN_CARD;

    saveTransaction(&Test);

    listSavedTransactions();

}
void             listSavedTransactionsTest  (void);
