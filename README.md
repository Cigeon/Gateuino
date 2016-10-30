Welcome to Gateuino!
===================


Gateuino is a simple project to control gate and wicket in your house. It is built using WeMos D1 mini board and Blynk platform.

----------
Software apps and online services:
-------------
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Blynk](http://www.blynk.cc/)

About Arduino IDE:
-------------
The open-source Arduino Software (IDE) makes it easy to write code and upload it to the board. It runs on Windows, Mac OS X, and Linux. The environment is written in Java and based on Processing and other open-source software.
This software can be used with any Arduino board.
Refer to the [Getting Started](https://www.arduino.cc/en/Guide/HomePage) page for Installation instructions.

Arduino IDE configuration:
-------------
- Select File, then Preferences
- In the Additional Boards Manager URLs text box, enter http://arduino.esp8266.com/stable/package_esp8266com_index.json and hit OK
- Select Tools, mouse over your currently selected board, and choose Boards Manager from the popup menu
- In the Boards Manager window that appears, enter ESP in the Filter your search… box, click on the esp8266 by ESP8266 Community entry that appears, and then select Install
- Wait for a while, while the Board Definitions and Tools are downloaded – and note, you can use this same download to support a whole heap of other boards.
- Once complete, select Tools again, and choose the WeMos D1 mini from the Boards dropdown.

> #####Important! Don't forget to enter your wifi ssid and password in the sketch.

About Blynk:
-------------
Blynk is a Platform with iOS and Android apps to control Arduino, Raspberry Pi and the likes over the Internet.
It's a digital dashboard where you can build a graphic interface for your project by simply dragging and dropping widgets.
It's really simple to set everything up and you'll [start tinkering](http://www.blynk.cc/getting-started)  in less than 5 mins.
Blynk  is not tied to some specific board or shield. Instead, it's supporting hardware of your choice. Whether your Arduino or Raspberry Pi  is linked to the Internet over Wi-Fi, Ethernet or this new ESP8266 chip, Blynk will get you online and ready for the Internet Of Your Things. 
Blynk app configuration:
-------------
After installing Blynk app to your device, create a new project and generate authentication token then copy and paste it into a sketch:
```sh
char auth[] = "YourAuthToken";
```
Add Button widgets and Value Display S widgets to your project:

![Add button widget](https://github.com/Cigeon/Gateuino/blob/master/screens/Screenshot1_cc.blynk.png)

![Add Value Display S  widget](https://github.com/Cigeon/Gateuino/blob/master/screens/Screenshot2_cc.blynk.png)

Then configure yout widgets like this:

![Configure gate's button widget](https://github.com/Cigeon/Gateuino/blob/master/screens/Screenshot3_cc.blynk.png)

![Configure wicket's button widget](https://github.com/Cigeon/Gateuino/blob/master/screens/Screenshot4_cc.blynk.png)

![Configure gate's Value Display S  widget](https://github.com/Cigeon/Gateuino/blob/master/screens/Screenshot5_cc.blynk.png)

![Configure wicket's Value Display S  widget](https://github.com/Cigeon/Gateuino/blob/master/screens/Screenshot6_cc.blynk.png)

Finally you should get something like this:

![Configure wicket's Value Display S  widget](https://drive.google.com/file/d/0B8BYBsqsRl2gTEFWRXFVazFpcTg/view?usp=sharing)

Schematics:
-------------
Gate and wicket view:

![Gate and wicket view](https://github.com/Cigeon/Gateuino/blob/master/screens/GateAndWicket.png)

Electrical diagram:

![Electrical diagram](https://github.com/Cigeon/Gateuino/blob/master/screens/GateuinoScheme.png)

Hardware components:
-------------
- WeMos D1 mini x 1
- Button x 2
- Relay 3VDC x 6
- Gate drive 24VDC x 2
- Wicket drive 24VDC x 1
- Lock x 3

----------
Now when you passed all steps you can try to play with your gate and wicket.