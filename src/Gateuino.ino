/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 *
 **************************************************************
 * This project runs directly on WeMos D1 R2 & mini  chip.
 *
 * You need to install this for ESP8266 development:
 *   https://github.com/esp8266/Arduino
 *
 * Please be sure to select the right ESP8266 module
 * in the Tools -> Board menu!
 *
 * Change WiFi ssid, pass, and Blynk auth token to run :)
 *
 **************************************************************/

#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

//********* Auth Token in the Blynk App ********

// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// ************ WiFi credentials ***************

// Set password to "" for open networks
char ssid[] = "YourSsid";
char pass[] = "YourPassword";

//************ Pin configuration ***************
     
const int door1Pin = D1;      
const int door2Pin = D2;
const int lockDoor1Pin = D3;
const int lockDoor2Pin = D4;
const int btnDoorsPin = D5;
const int btnWicketPin = D6;
const int wicketPin = D7;
const int lockWicketPin = D8;

//************** Global variables ****************

SimpleTimer timer;
long lastDebounceTime = 0;       // the last time doors button was pressed
long lastDebounceTime2 = 0;      // the last time wicket button was pressed
long debounceDelay = 200;        // the debounce time
bool buttonPressed = false;      // doors button pressed flag
bool buttonPress = false;        // doors button pressing now flag
bool btnWicketPressed = false;   // wicket button pressed flag
bool btnWicketPress = false;     // wicket button pressing now flag
long currTime = 0;               // doors timer current time (s)
long currWicketTime = 0;         // wicket timer current time (s)
long doorOpenDelay = 5;          // delay open doors (s)
long doorCloseDelay = 5;         // delay close doors (s) 
long doorLockDelay = 15;         // delay lock doors (s)
long wicketOpenDelay = 5;        // delay open wicket (s)
long wicketLockDelay = 15;       // delay lock wicket (s) 
int remoteCmd = 0;               // open doors command from blynk 
bool remotePressed = false;      // flag doors remote button pressed
int remWicketCmd = 0;            // open wicket command from blynk 
bool remWicketPressed = false;   // flag remote wicket button pressed
bool cmdDoorsOpen = false;       // doors activation command
bool cmdWicketOpen = false;      // wicket activation command 
bool doorsUnlocked = false;      // flag doors unlocked
bool doorsLocked = true;         // flag doors locked
bool door1Opened = false;        // flag door1 opened
bool door2Opened = false;        // flag door2 opened
bool door1Closed = false;        // flag door1 closed
bool door2Closed = false;        // flag door2 closed
bool wicketUnlocked = false;     // flag wicket unlocked
bool wicketLocked = true;        // flag wicket locked
bool wicketOpened = false;       // flag wicket opened
bool wicketClosed = false;       // flag wicket closed
bool connectedToBlynk = false;   // flag connected to blynk

//*********** Local functions ***************

// Function set/reset doors state flag
void getDoorsState()
{
  if (checkDoorsButton() || checkDoorsRemCmd())
  {
    cmdDoorsOpen = !cmdDoorsOpen;
  }
  if (cmdDoorsOpen)
  {
    Blynk.virtualWrite(V12, "OPEN");
  }
  else
  {
    Blynk.virtualWrite(V12, "CLOSE");
  }
}

// Function set/reset wicket state flag
void getWicketState()
{
  if (checkWicketButton() || checkWicketRemCmd())
  {
    cmdWicketOpen = !cmdWicketOpen;
  }
  if (cmdWicketOpen)
  {
    Blynk.virtualWrite(V13, "OPEN");
  }
  else
  {
    Blynk.virtualWrite(V13, "CLOSE");
  }
}

// Function fires every second
void everySecond()
{
  // Open/close doors
  doorsUpdate();
  // Open/close wicket
  wicketUpdate();
}

// Function open/close the doors
void doorsUpdate()
{
  // Open doors
  if (cmdDoorsOpen && !door2Opened)
  {
    // Reset flags for closing action 
    doorsLocked = false;  
    door1Closed = false;
    door2Closed = false;
    // Unlock doors
    if (!doorsUnlocked)
    {
      // Activate 1th and 2nd locks
      digitalWrite(lockDoor1Pin, HIGH);
      digitalWrite(lockDoor2Pin, HIGH);
      // Set doors unlocked flag
      doorsUnlocked = true;
      currTime = 0;
    }
    // Open 1th door
    if (currTime >= doorOpenDelay && !door1Opened)
    {
        openDoor(1);
        door1Opened = true;
        currTime = 0;
    } 
    // Open 2nd door    
    if (currTime >= doorOpenDelay && door1Opened)
    {        
        openDoor(2);
        door2Opened = true;
        currTime = 0;
    }
    currTime++;
  }

  // Close doors
  if (!cmdDoorsOpen && !doorsLocked)
  {
    // Reset flags for opening action 
    doorsUnlocked = false;
    door1Opened = false;
    door2Opened = false;
    // Close 2nd door
    if (!door2Closed)
    {
      closeDoor(2);
      door2Closed = true;
      currTime = 0;
    }
    // Close 1th door
    if (currTime >= doorCloseDelay && !door1Closed)
    {        
        closeDoor(1);
        door1Closed = true;
        currTime = 0;
    }
    // Lock the doors     
    if (currTime >= doorLockDelay && door1Closed)
    {        
        digitalWrite(lockDoor1Pin, LOW);
        digitalWrite(lockDoor2Pin, LOW);
        // Set doors locked flag
        doorsLocked = true;
        currTime = 0;
    }
    currTime++;    
  }
}

// Function open/close wicket
void wicketUpdate()
{
  // Open wicket
  if (cmdWicketOpen && !wicketOpened)
  {
    // Reset flags for closing action 
    wicketLocked = false;  
    wicketClosed = false;
    // Unlock wicket
    if (!wicketUnlocked)
    {
      // Activate wicket lock
      digitalWrite(lockWicketPin, HIGH);
      // Set doors unlocked flag
      wicketUnlocked = true;
      currWicketTime = 0;
    }
    // Open wicket
    if (currWicketTime >= wicketOpenDelay && !wicketOpened)
    {
        digitalWrite(wicketPin, HIGH);
        wicketOpened = true;
        currWicketTime = 0;
    } 
    currWicketTime++;
  }

  // Close wicket
  if (!cmdWicketOpen && !wicketLocked)
  {
    // Reset flags for opening action 
    wicketUnlocked = false;
    wicketOpened = false;
    // Close wicket
    if (!wicketClosed)
    {
      digitalWrite(wicketPin, LOW);
      wicketClosed = true;
      currWicketTime = 0;
    }
    // Lock the wicket     
    if (currWicketTime >= wicketLockDelay)
    {        
        digitalWrite(lockWicketPin, LOW);
        // Set doors locked flag
        wicketLocked = true;
        currWicketTime = 0;
    }
    currWicketTime++;    
  }
}

// Function checks doors button state
bool checkDoorsButton() 
{
  bool result = false;

  if(!buttonPressed) 
  {
    lastDebounceTime = millis();
    if(digitalRead(btnDoorsPin) == LOW)
    {
      buttonPressed = true;      
    }    
  }
  if((millis() - lastDebounceTime) > debounceDelay)
  {    
    if(digitalRead(btnDoorsPin) == LOW && !buttonPress)
    {
      buttonPressed = false;
      result = true;
      buttonPress = true;
    }
    if(digitalRead(btnDoorsPin) == HIGH)
    {
      buttonPress = false;   
      lastDebounceTime = millis();
    }    
  }
  return result;
}

// Function checks wicket button state
bool checkWicketButton() 
{
  bool result = false;

  if(!btnWicketPressed) 
  {
    lastDebounceTime2 = millis();
    if(digitalRead(btnWicketPin) == LOW)
    {
      btnWicketPressed = true;      
    }    
  }
  if((millis() - lastDebounceTime2) > debounceDelay)
  {    
    if(digitalRead(btnWicketPin) == LOW && !btnWicketPress)
    {
      btnWicketPressed = false;
      result = true;
      btnWicketPress = true;
    }
    if(digitalRead(btnWicketPin) == HIGH)
    {
      btnWicketPress = false;   
      lastDebounceTime2 = millis();
    }    
  }
  return result;
}

// Function check doors remote button state in blynk app
bool checkDoorsRemCmd() 
{
  bool result = false;
  if(remoteCmd)
  {
    if (!remotePressed)
    {
      result = true;
    }    
    remotePressed = true;
  }
  else
  {
    remotePressed = false;
  }
  return result;
}

// Function check wicket button state state in blynk app
bool checkWicketRemCmd() 
{
  bool result = false;
  if(remWicketCmd)
  {
    if (!remWicketPressed)
    {
      result = true;
    }    
    remWicketPressed = true;
  }
  else
  {
    remWicketPressed = false;
  }
  return result;
}

// Function to open the door
void openDoor(int doorNo)
{
  if (doorNo == 1)
  {
    digitalWrite(door1Pin, HIGH);    
  }
  if (doorNo == 2)
  {
    digitalWrite(door2Pin, HIGH);
  }
}

// Function to close the door
void closeDoor(int doorNo)
{
  if (doorNo == 1)
  {
    digitalWrite(door1Pin, LOW);
  }
  if (doorNo == 2)
  {
    digitalWrite(door2Pin, LOW);
  }
}

void checkConnection(){
  connectedToBlynk = Blynk.connected();
  if(!connectedToBlynk){
    Serial.println("Not connected to Blynk server"); 
    Blynk.connect(3333);  // timeout set to 10 seconds and then continue without Blynk  
  }
  else{
    Serial.println("Connected to Blynk server");     
  }
}

//*************** Blynk pins configuration ***************

BLYNK_WRITE(V1)
{
  remoteCmd = param.asInt();
}
BLYNK_WRITE(V2)
{
  remWicketCmd = param.asInt();
}
BLYNK_READ(V12)
{
  if (cmdDoorsOpen)
  {
    Blynk.virtualWrite(V12, "OPEN");
  }
  else
  {
    Blynk.virtualWrite(V12, "CLOSE");
  }
  
}
BLYNK_READ(V13)
{
  if (cmdWicketOpen)
  {
    Blynk.virtualWrite(V13, "OPEN");
  }
  else
  {
    Blynk.virtualWrite(V13, "CLOSE");
  }
  
}

//************** Initialization ******************

void setup()
{
  // init pin mode
  pinMode(btnDoorsPin, INPUT_PULLUP);
  pinMode(door1Pin, OUTPUT); 
  pinMode(door2Pin, OUTPUT);
  pinMode(lockDoor1Pin, OUTPUT); 
  pinMode(lockDoor2Pin, OUTPUT);
  pinMode(btnWicketPin, INPUT_PULLUP);
  pinMode(wicketPin, OUTPUT);
  pinMode(lockWicketPin, OUTPUT);
  timer.setInterval(1000L, everySecond);
  timer.setInterval(11000L, checkConnection); 
  Serial.begin(115200);
  //Blynk.begin(auth, ssid, pass);
  
  WiFi.begin(ssid, pass);  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
  
  Blynk.config(auth);  
  checkConnection();
}

//************ Main loop **************

void loop() 
{
  if(connectedToBlynk)
  {
    Blynk.run();
  }
  timer.run();
  getDoorsState();
  getWicketState();
}

