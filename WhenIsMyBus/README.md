# When's My Bus?


##### *What if, from a distance, you glanced at a bus sign and it told you what bus was coming next and when it would arrive?*

*If your hands were full of groceries and you'd rather not have to take your smart phone out to check your bus on the bus tracker app..*

*If you'd rather stay indoors as it snowed/ rained outside, only glancing at the bus sign that told you whenyour bus was nearing so you could hop out just in time to catch the bus?*

*If you were a tourist , new to the system, and didn't have to hassle downloading a bus tracker app..*

*What if, you were __visually impaired__, and the bus stop just __told__ you what bus and when it would arrive?*

-------------------------------------------------------------------------------------------------------------------------

#### _**When's my bus**_  is a smart bus sign that informs what buses arrive at a given stop and provides real-time data on the bus ETAs via ambient lighting/display mechanisms.
![Concept Schematic](https://github.com/srajend1/projects/blob/master/WhenIsMyBus/Schematic.png)

-------------------------------------------------------------------------------------------------------------------------


## The (Thought) Process
### The Thinking behind the Making
--------------------------------------------------------------------------------------------------------------------------
__Escape from the ‘Terminal Screen’__

Mark Wiser(Xerox, PARC), in his paper *The Computer for the 21st Century*, worried we’d be holed up in our offices stuck to our computer screens, with the advent of smart phones, millennials have their heads stuck into their smart phones. He suggested that ‘Future technology should involve more than a single screen interaction with humans…’ 

__Inclusion__

Smart apps tap into and (consequentially cater to) only the [64%](http://www.pewinternet.org/2015/04/01/chapter-one-a-portrait-of-smartphone-ownership/) of the population that actually has access to smart phones. What about the remaining 36% ? Our seniors, the older generation, what about the visually impaired among us?

*__Shouldn't Technology serve us all?__*

Our Profesoor asked us to make something that we'd want for ourselves, I figured he mentioned that so we understood the need well, and had the pasiion to see it through the end.

Missing bus to school has been among the most frustrating things for me, and from my experience, having spent almost a quarter of my everyday commute at bus stops(just a little exaggeration there), I know I am not alone - here are other’s who share the frustration.

Bus signs are an inevitable part of transit infrastructure, so I wondered, why not make them smart? Why not make them ‘enchanted objects’? (not too much though, just enough)   

While making them smart why not make them glance-able or rather keep them __just as glance-able as they were before__ (avoid requiring excessive attention or excessive interaction), but __way more informative now__.
Furthur still being informative __ambiently__ while ensuring __not being ambiguous__.

--------------------------------------------------------------------------------------------------------------------------


## The Prototype

### [Video](https://github.com/srajend1/projects/tree/master/WhenIsMyBus/Videos)
### [Image Gallery](https://github.com/srajend1/projects/blob/master/WhenIsMyBus/Images/README_IMAGES.md)

## The Poster
![Poster](https://github.com/srajend1/projects/blob/master/WhenIsMyBus/Images/POSTER%20FINAL%20BUS.png)

## The Software  : 
#### [Server Code](https://github.com/srajend1/projects/blob/master/WhenIsMyBus/PATRemoteServer/src/edu/cmu/patpublisher/PATRemoteServer.java)

#### [Particle Code](https://github.com/srajend1/projects/blob/master/WhenIsMyBus/PATdisp/PATdisp.ino)
##### References :
- https://learn.adafruit.com/assets/21024
- https://github.com/ensonic/photon-waveout

## The Hardware  :
### [Circuitry](https://github.com/srajend1/projects/blob/master/WhenIsMyBus/Images/Final%20Circuitry.png)
### Bill Of Materials


Component | Quantity | Note
----------|----------|-------
Particle Photon   |     1    | For circuit
OLED Display Screen    |     1  | For circuit
Neopixel Ring | 1 | For circuit
Speaker | 1 | For circuit
Push button | 1 | For circuit
Bread Board | 1 | For circuit
Jumper wires | Multiple | For circuit
Conecting wires| Multiple | For circuit
Portable (phone)battery | 1 | To Power circuit
PVC pipe | as per requirement(scale)| For Prototype
FoamCore | as per requirement(scale) | For Prototype
Cardboard | as per requirement(scale)| For Prototype
Thermocol| as per requirement(scale)| For Prototype
Magnets | 2 | For Prototype
Translucent grey(dark) screen |as per requirement| For Prototype
Sticky Tapes, Glue| as per requirement| For Prototype
Drill, Saw, Scissors, Paper Cutters | as per requirement| Tools for Prototype




## User Testing, Feedback and Design Changes :

The neopixel ring used to represent bus ETA initially had a __gradient__ that transitioned from red to green.
![Time Chunks](https://github.com/srajend1/projects/blob/master/WhenIsMyBus/Images/Time%20Chunks.png)
__Distinctly Coloured Quadrants as Time 'Chunks'__

It was however found to be more useful when ‘chunked’ into 3 __distinct quadrants__ of red, yellow and green that clearly represented 5mins each amounting to a whole of 15 mins. Users could now easily understand the and interpret the progression while also having a sense of the 'time' or duration each quadrant indicated.

## Future development :
- Selection of appropriate, optimal Hardware for Audio file storage (the Photon is currently unable to store audio data for more than one bus)
- Scaling : deployment design
- Refining Case design for installation : must be quick and adaptable to variable pole diameters
- Redesigning Push button case design to incorporate concise Braille instructions

### Co-Credits : My Sincere Thanks to..
- [Anand Bhat](https://github.com/anandbhat23)
- Prof. Daragh Byrne
- Hunt Ideate Lab
- 311 Studio
- User testing participants and friends

