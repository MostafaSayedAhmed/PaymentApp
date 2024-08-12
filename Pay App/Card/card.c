
#include "card.h"

/**
 * @brief Get the Card Holder Name object
 *
 * @param cardData
 * @return EN_cardError_t
 */
EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    EN_cardError_t ret = CARD_OK;

    printf("Enter Card Holder Name : ");
    fflush(stdin);
    gets(cardData->cardHolderName);
    fflush(stdin);

    if((cardData == NULL) || (strlen(cardData->cardHolderName) < 20) || (strlen(cardData->cardHolderName) > 24) )
    {
        ret = WRONG_NAME;
    }
    else
    {
        ret = CARD_OK;
    }

    return ret;
}

/**
 * @brief Get the Card Expiry Date object
 *
 * @param cardData
 * @return EN_cardError_t
 */
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    EN_cardError_t ret = CARD_OK;

    printf("Enter Expiry Date : ");
    gets(cardData->cardExpirationDate);
    fflush(stdin);

    if((cardData == NULL) || (strlen(cardData->cardExpirationDate) < 5) || (strlen(cardData->cardExpirationDate) > 5) )
    {
        ret = WRONG_EXP_DATE;
    }
    else
    {
        ret = CARD_OK;
    }

    return ret;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    EN_cardError_t ret = CARD_OK;

    printf("Enter Card Primary Account Number : ");
    gets(cardData->primaryAccountNumber);
    fflush(stdin);

    if((cardData == NULL) || (strlen(cardData->primaryAccountNumber) > 19) || (strlen(cardData->primaryAccountNumber) < 16) )
    {
        ret = WRONG_PAN;
    }
    else
    {
        ret = CARD_OK;
    }

    return ret;
}

void           getCardHolderNameTest(void)
{
   ST_cardData_t test = {NULL,NULL,NULL} ;
    EN_cardError_t ret ;

    printf("Tester Name     : Mostafa Sayed \n");
    printf("Function Name   : getCardHolderNameTest \n");

    printf("Test Case 1     : \n");
    printf("Input Data      : %s \n","NULL");
    ret = getCardHolderName(&test);
    printf("Expected Result : %s \n","WRONG_NAME");
    printf("Actual Result   : %s \n",ret?"WRONG_NAME":"CARD_OK");

    printf("Test Case 2     : \n");
    printf("Input Data      : %s \n","Mostafa Sayed");
    ret = getCardHolderName(&test);
    printf("Expected Result : %s \n","WRONG_NAME");
    printf("Actual Result   : %s \n",ret?"WRONG_NAME":"CARD_OK");

    printf("Test Case 3     : \n");
    printf("Input Data      : %s \n","Mostafa Sayed Ahmed Taha");
    ret = getCardHolderName(&test);
    printf("Expected Result : %s \n","CARD_OK");
    printf("Actual Result   : %s \n",ret?"WRONG_NAME":"CARD_OK");

    printf("Test Case 4     : \n");
    printf("Input Data      : %s \n","Mostafa Sayed Ahmed");
    ret = getCardHolderName(&test);
    printf("Expected Result : %s \n","WRONG_NAME");
    printf("Actual Result   : %s \n",ret?"WRONG_NAME":"CARD_OK");

    printf("Test Case 5     : \n");
    printf("Input Data      : %s \n","Mostafa Sayed Ahmed Taha Ahmed");
    ret = getCardHolderName(&test);
    printf("Expected Result : %s \n","WRONG_NAME");
    printf("Actual Result   : %s \n",ret?"WRONG_NAME":"CARD_OK");

}

void           getCardExpiryDateTest(void)
{
    ST_cardData_t test = {NULL,NULL,NULL} ;
    EN_cardError_t ret ;

    printf("Tester Name     : Mostafa Sayed \n");
    printf("Function Name   : getCardExpiryDateTest \n");

    printf("Test Case 1     : \n");
    printf("Input Data      : %s \n","NULL");
    ret = getCardExpiryDate(&test);
    printf("Expected Result : %s \n","WRONG_EXP_DATE");
    printf("Actual Result   : %s \n",(ret == WRONG_EXP_DATE)?"WRONG_EXP_DATE":"CARD_OK");

    printf("Test Case 2     : \n");
    printf("Input Data      : %s \n","05/23");
    ret = getCardExpiryDate(&test);
    printf("Expected Result : %s \n","CARD_OK");
    printf("Actual Result   : %s \n",(ret == WRONG_EXP_DATE)?"WRONG_EXP_DATE":"CARD_OK");

    printf("Test Case 3     : \n");
    printf("Input Data      : %s \n","05/2023");
    ret = getCardExpiryDate(&test);
    printf("Expected Result : %s \n","WRONG_EXP_DATE");
    printf("Actual Result   : %s \n",(ret == WRONG_EXP_DATE)?"WRONG_EXP_DATE":"CARD_OK");

    printf("Test Case 4     : \n");
    printf("Input Data      : %s \n","05");
    ret = getCardExpiryDate(&test);
    printf("Expected Result : %s \n","WRONG_EXP_DATE");
    printf("Actual Result   : %s \n",(ret == WRONG_EXP_DATE)?"WRONG_EXP_DATE":"CARD_OK");
}

void           getCardPANTest       (void)
{
    ST_cardData_t test = {NULL,NULL,NULL} ;
    EN_cardError_t ret ;

    printf("Tester Name     : Mostafa Sayed \n");
    printf("Function Name   : getCardPAN \n");

    printf("Test Case 1     : \n");
    printf("Input Data      : %s \n","NULL");
    ret = getCardPAN(&test);
    printf("Expected Result : %s \n","WRONG_PAN");
    printf("Actual Result   : %s \n",(ret == WRONG_PAN)?"WRONG_PAN":"CARD_OK");

    printf("Test Case 2     : \n");
    printf("Input Data      : %s \n","0123456789012345678");
    ret = getCardPAN(&test);
    printf("Expected Result : %s \n","CARD_OK");
    printf("Actual Result   : %s \n",(ret == WRONG_PAN)?"WRONG_PAN":"CARD_OK");

    printf("Test Case 3     : \n");
    printf("Input Data      : %s \n","01234567890123456");
    ret = getCardPAN(&test);
    printf("Expected Result : %s \n","CARD_OK");
    printf("Actual Result   : %s \n",(ret == WRONG_PAN)?"WRONG_PAN":"CARD_OK");

    printf("Test Case 4     : \n");
    printf("Input Data      : %s \n","0123456789012345");
    ret = getCardPAN(&test);
    printf("Expected Result : %s \n","CARD_OK");
    printf("Actual Result   : %s \n",(ret == WRONG_PAN)?"WRONG_PAN":"CARD_OK");

    printf("Test Case 5     : \n");
    printf("Input Data      : %s \n","012345678901234");
    ret = getCardPAN(&test);
    printf("Expected Result : %s \n","WRONG_PAN");
    printf("Actual Result   : %s \n",(ret == WRONG_PAN)?"WRONG_PAN":"CARD_OK");

    printf("Test Case 6     : \n");
    printf("Input Data      : %s \n","01234567890123456789");
    ret = getCardPAN(&test);
    printf("Expected Result : %s \n","WRONG_PAN");
    printf("Actual Result   : %s \n",(ret == WRONG_PAN)?"WRONG_PAN":"CARD_OK");
}
