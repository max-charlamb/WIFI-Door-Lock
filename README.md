# Web-Enabled-Door-Lock
Control a deadbolt through a website!

### Devices used:
<ul>
<li><a href="https://www.adafruit.com/product/2821">Feather Huzzah</a> </li>
<li><a href="https://www.amazon.com/gp/product/B07NQJ1VZ2/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1">Standard Servo</a></li>
<li><a href="https://www.command.com/3M/en_US/command/products/~/?N=5924736+3294529207+3294774739+3294857497&rt=rud">Command Strips</a></li>
<li>3D Printer</li>
</ul>

### Web Server:
The web servo code is provided in the ```doorwebserver``` directory. It is designed to be uploaded to a Feather Huzzah through the Arduino IDE.
This requires extra libraries found in [this Adafruit](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide) tutorial.
The directly also contains HTML files that the Feather sends to the client. These are uploaded using SPIFFS and the Arduino IDE. To do so follow [this tutorial](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html).  

The code is based on [A Beginner's Guide to the ESP8266](https://tttapa.github.io/ESP8266/Chap01%20-%20ESP8266.html). That tutorial is great reference tool for using the Feather board.


### STL Files:
There are 3 STL files provided. The servo housing, split into two parts, and the servo horn addition. These are meant for 
a standard size servo but may need to be filed down.
All three models are designed so that they can be printed without support. I used epoxy to glue the two halves of the lock
body together and the servo horn addition on to the servo horn. 
The files are designed to be used with command strips to affix it to the door.

