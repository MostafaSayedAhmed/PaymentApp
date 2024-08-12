# Payment App

Payment systems are now available everywhere and everyone interacts with these
systems every day. <br />
There are different types of transactions you can make, SALE, REFUND, Pre-Authorization,
and VOID. <br />
<ul>
  <li> SALE: means to buy something and its price will be deducted from your bank account. </li>
  <li> REFUND: this means that you will return something and wants your money back to your bank account. </li>
  <li> Pre-Authorization: means holding an amount of money from your account, e.g Hotel reservation. </li>
  <li> VOID: this means canceling the transaction, e.g if the seller entered the wrong amount. </li>
</ul> 

<p>
The project is to implement the SALE transaction only by simulating the card, terminal(ATM), and the server.
</p>

## Code
<p>
  Project Code is divide into four main modules :
  <ul>
    <li>Card     Module : contains all information about holder name , PAN and expire date (entered by user during transaction)</li>
    <li>Terminal Module : contains all information about maximum amount of transaction allowed , inserted amount of transaction desired , check it with maximum allowed transaction and check expiration date </li>
    <li>Server   Module : perform all operation of checking whether card is running or not , applying change in balance and save transaction to list showing all transaction performed</li>
  </ul>
</p>

## Future Work
<p>
  Although the project is completed but some part can be improved which are :
  <ul>
    <li>Unfortunately the code isn't very well documented so the next phase is documenting the whole code</li>
    <li>Pan lynn Number check isn't implemented in this project </li>
    <li>Techincal Support mode : ability to modify database simulating bank side of operation </li>
    <li>Allowing program to perform more than one successful transaction and ability to detect same user even if different account option was chosen </li>
  </ul>
</p>
