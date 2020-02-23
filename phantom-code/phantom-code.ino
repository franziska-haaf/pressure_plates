colorCycle //current color cycle. Start new if color changes. Save steps

void setup() {
  setupListening() //Udp.begin(port)
}

void loop() {
  rotateColors();
  waitForIncomingMessages();

  // I got stepped on
  if (isSteppedOn) {
    tellOtherPlate(); //Udp.beginPacket(IP,port)
    iWon = receiveAnswerIfWon(); 
    if (iWon) {
      winnerLights();
    } else {
      loosingLights();
    }
  }

  // The other one got stepped on
  message = readIncomingMessage() //Udp.parsePacket()
  if (message) {
    // got stepped on aswell
    if (isSteppedOn) {
      var stepTimeStamp = colorCycle.stepTimeStamp;
      
      //Other one stepped first:
      if(message.stepTimeStamp < stepTimeStamp){
        loosingLights();
        tellWinner(); //tell we lost
       }
       //I stepped first
      else{
          winnerLights();
          tellLooser();
      }
    }
    else{
     //todo? I am not stepped on? 
     }
  }
}
}








Ok so both need to do the server part in the setup, meaning:
connectToTheWifiStuff(); //as done in your AP-UDP.ino
Udp.begin(localPort); // wait if someone wants to talk
In the loop they both wait for a packet aka
int packetSize = Udp.parsePacket();
  if (packetSize){
//todo MAGIC HAPPENS
}
If they get one, they do the stuff they’re supposed to do, and send an answer. All in this if-statement.

BUT to send messages, they need to be a client aswell, correct?
How does this work? 

I am confused about the fact that both AccessPoint and Client
