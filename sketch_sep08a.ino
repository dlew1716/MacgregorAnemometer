/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/
#define arr_len 5000
#define PIECES  50
#define ONE_WIRE_BUS 15
#define TEMPERATURE_PRECISION 12 // Lower resolution

#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
int numberOfDevices; // Number of temperature devices found
DeviceAddress tempDeviceAddress;

// Set web server port number to 80
WiFiServer server(80);
IPAddress apIP(192, 168, 1, 1);
const int analogPin = 34;
double sensorValue;
double total;
double average;
double voltage;
double mph;
double tempC;
double tempF;
String mph_str;
char buffer [10];
const char* webSITE ="<!doctype html><meta content=\"width=device-width,initial-scale=1\"name=viewport><script>var ctx,c,slider,info,canvasHeight=375,canvasWidth=15e3,magic=15e3,windScale=10,tempScale=10,mphX=25,mphY=10,depthHours=72,minPixVertBars=40,tempPad=100,sortedWind=[1,2],sortedTemp=[1,2],unscaledTemp=[],wind_temp_sw=0;function stats(){var t,e=new Date,n=sortedWind.reduce(function(t,e){return Math.max(t,e)}),i=sortedWind.slice(magic-Math.round(magic/72)),a=i.reduce(function(t,e){return Math.max(t,e)}),o=sortedWind.reduce(function(t,e){return t<0&&(t=1e3),e<0&&(e=1e3),Math.min(t,e)}),r=i.reduce(function(t,e){return t<0&&(t=1e3),e<0&&(e=1e3),Math.min(t,e)}),s=4320-4320*sortedWind.indexOf(n)/magic;e.setMinutes(e.getMinutes()-s),t=n.toFixed(2).toString().length<5?\"&nbsp\"+n.toFixed(2).toString():n.toFixed(2).toString(),document.getElementById(\"max72\").innerHTML=\"Peak 72 Hours : \"+t+\" MPH,   \"+(e.getMonth()+1).toString()+\"-\"+e.getDate().toString()+\" \"+e.getHoursTwoDigits().toString()+\":\"+e.getMinutesTwoDigits().toString();e=new Date;var c,d=4320-4320*sortedWind.indexOf(o)/magic;e.setMinutes(e.getMinutes()-d),c=o.toFixed(2).toString().length<5?\"&nbsp\"+o.toFixed(2).toString():o.toFixed(2).toString(),document.getElementById(\"min72\").innerHTML=\"Min&nbsp 72 Hours&nbsp: \"+c+\" MPH,   \"+(e.getMonth()+1).toString()+\"-\"+e.getDate().toString()+\" \"+e.getHoursTwoDigits().toString()+\":\"+e.getMinutesTwoDigits().toString();e=new Date;var l,g=4320-4320*sortedWind.indexOf(a)/magic;e.setMinutes(e.getMinutes()-g),l=a.toFixed(2).toString().length<5?\"&nbsp\"+a.toFixed(2).toString():a.toFixed(2).toString(),document.getElementById(\"submax72\").innerHTML=\"Peak 1 Hours&nbsp&nbsp: \"+l+\" MPH,   \"+(e.getMonth()+1).toString()+\"-\"+e.getDate().toString()+\" \"+e.getHoursTwoDigits().toString()+\":\"+e.getMinutesTwoDigits().toString();var m;e=new Date,d=4320-4320*sortedWind.indexOf(r)/magic;e.setMinutes(e.getMinutes()-d),m=r.toFixed(2).toString().length<5?\"&nbsp\"+r.toFixed(2).toString():r.toFixed(2).toString(),document.getElementById(\"submin72\").innerHTML=\"Min&nbsp 1 Hours&nbsp&nbsp: \"+m+\" MPH,   \"+(e.getMonth()+1).toString()+\"-\"+e.getDate().toString()+\" \"+e.getHoursTwoDigits().toString()+\":\"+e.getMinutesTwoDigits().toString()}function getSilderSize(){return document.getElementById(\"zoom\").max=Math.ceil(magic/window.innerWidth)}function interpolateArray(t,e){var n,i,a=new Array,o=new Number((t.length-1)/(e-1));a[0]=t[0];for(var r=1;r<e-1;r++){var s=r*o,c=new Number(Math.floor(s)).toFixed(),d=new Number(Math.ceil(s)).toFixed(),l=s-c;a[r]=(n=t[c],i=t[d],n+(i-n)*l)}return a[e-1]=t[t.length-1],a}function scrollToElementD(){document.getElementById(\"parent\").scrollLeft=2e4}function loadHistTemp(){var t,e,n,i=new XMLHttpRequest;i.onreadystatechange=function(){4==this.readyState&&200==this.status&&(n=JSON.parse(this.responseText),t=n.temp_history.slice(0,n.currPosition),e=n.temp_history.slice(n.currPosition),sortedTemp=interpolateArray(sortedTemp=e.concat(t),magic),unscaledTemp=n.temp_history)},i.open(\"GET\",\"http://192.168.1.1/hist_temp\",!0),i.send()}function loadHist(){var t,e,n,i=new XMLHttpRequest;i.onreadystatechange=function(){4==this.readyState&&200==this.status&&(n=JSON.parse(this.responseText),t=n.wind_history.slice(0,n.currPosition),e=n.wind_history.slice(n.currPosition),sortedWind=interpolateArray(sortedWind=e.concat(t),magic),ctx.clearRect(0,0,canvas.width,canvas.height),drawH(),drawV(),drawHist())},i.open(\"GET\",\"http://192.168.1.1/hist_wind\",!0),i.send()}function loadWind(){var t=new XMLHttpRequest;t.onreadystatechange=function(){4==this.readyState&&200==this.status&&(document.getElementById(\"live\").innerHTML=this.responseText)},t.open(\"GET\",\"http://192.168.1.1/wind\",!0),t.send(),getSilderSize(),stats()}function loadTemp(){var t=new XMLHttpRequest;t.onreadystatechange=function(){4==this.readyState&&200==this.status&&(document.getElementById(\"live\").innerHTML=this.responseText)},t.open(\"GET\",\"http://192.168.1.1/temp\",!0),t.send(),getSilderSize(),stats()}function drawHist(){var t,e;ctx.beginPath(),ctx.strokeStyle=\"#000000\",ctx.setLineDash([2,0]);for(var n=1;n<canvas.width;n++)t=Math.floor(n*(magic/canvas.width)),e=Math.floor((n-1)*(magic/canvas.width)),0<=sortedWind[t]&&(ctx.moveTo(n-1,canvas.height-windScale*sortedWind[e]),ctx.lineTo(n,canvas.height-windScale*sortedWind[t]));ctx.stroke(),ctx.closePath()}function drawHistTemp(){var t,e;ctx.beginPath(),ctx.strokeStyle=\"#990000\",ctx.setLineDash([2,0]);var n=unscaledTemp.reduce(function(t,e){return t<0&&(t=1e3),e<0&&(e=1e3),Math.min(t,e)}),i=unscaledTemp.reduce(function(t,e){return Math.max(t,e)}),a=i-n;tempScale=0==a?375:(canvas.height-tempPad)/a,console.log(\"min\",n),console.log(\"max\",i),console.log(\"delta\",a),console.log(\"scale\",tempScale);for(var o=1;o<canvas.width;o++)t=Math.floor(o*(magic/canvas.width)),e=Math.floor((o-1)*(magic/canvas.width)),sortedTemp[e]>=n&&sortedTemp[t]>=n&&(ctx.moveTo(o-1,canvas.height-tempScale*sortedTemp[e]+tempScale*n-tempPad/2),ctx.lineTo(o,canvas.height-tempScale*sortedTemp[t]+tempScale*n-tempPad/2));ctx.stroke(),ctx.closePath(),ctx.beginPath();for(var r=Math.floor(n),s=[];r<i;)s.push(r),0==Math.ceil(20/tempScale)?r+=1:r+=Math.ceil(20/tempScale);console.log(s);for(o=0;o<s.length;o++){var c=canvas.height-tempScale*s[o]+tempScale*n-tempPad/2;ctx.setLineDash([2,2]),ctx.strokeStyle=\"#BBBBBB\",ctx.font=\"10px Arial\",ctx.fillStyle=\"#AAAAAA\",ctx.fillText(s[o],canvas.width-20,c-5),ctx.moveTo(0,c),ctx.lineTo(canvas.width,c)}ctx.stroke(),ctx.fillText(\"F\",canvas.width-mphX+10,mphY+10),ctx.closePath()}function drawHT(){}function drawH(){ctx.beginPath();for(var t=[2,5,8,10,12,14,16,18,20,22,24,26,28,30],e=0;e<t.length;e++){var n=(canvas.height/windScale-t[e])*windScale;ctx.setLineDash([2,2]),ctx.strokeStyle=\"#BBBBBB\",ctx.font=\"10px Arial\",ctx.fillStyle=\"#AAAAAA\",ctx.fillText(t[e],canvas.width-20,n-5),ctx.moveTo(0,n),ctx.lineTo(canvas.width,n)}ctx.stroke(),ctx.fillText(\"MPH\",canvas.width-mphX,mphY+10),ctx.closePath()}function drawV(){ctx.beginPath();for(var t=new Date,e=new Date,n=canvas.width/(60*depthHours),i=15*n,a=\"15\";i<minPixVertBars;)i<30*n?(i+=15*n,a=\"30\"):(i+=i<60*n?30*n:60*n,a=\"60\");var o=i,r=o/n;t.setMinutes(t.getMinutes()-r),o=n*(\"15\"==a?t.getMinutes()<15?t.getMinutes()-0:t.getMinutes()<30?t.getMinutes()-15:t.getMinutes()<45?t.getMinutes()-30:t.getMinutes()-45:\"30\"==a?t.getMinutes()<30?t.getMinutes()-0:t.getMinutes()-30:t.getMinutes()-0)+o;for(var s=canvas.width;0<s;s--)if(s<canvas.width-o){ctx.font=\"10px Arial\",ctx.fillStyle=\"#AAAAAA\";r=Math.round(o/n);t.setTime(e.getTime()),t.setMinutes(t.getMinutes()-r),59==t.getMinutes()&&t.setMinutes(t.getMinutes()+1);var c=t.getMinutesTwoDigits();ctx.fillText((t.getMonth()+1).toString()+\"-\"+t.getDate().toString(),s+6,20),ctx.fillText(t.getHours().toString()+\":\"+c,s+6,35),ctx.fillRect(s,0,2,canvas.height),o+=i}ctx.fillStyle=\"#000000\",ctx.closePath()}window.onload=function(){slider=document.getElementById(\"zoom\"),info=document.getElementById(\"info\"),c=document.getElementById(\"graph\"),ctx=c.getContext(\"2d\"),canvas=document.getElementsByTagName(\"canvas\")[0],scrollToElementD(),slider.oninput=function(){var t=magic/slider.value;t<window.innerWidth&&(t=window.innerWidth-20),canvas.width=t,wind_temp_sw?(ctx.clearRect(0,0,canvas.width,canvas.height),drawHT(),drawV(),drawHistTemp()):(ctx.clearRect(0,0,canvas.width,canvas.height),drawH(),drawV(),drawHist()),scrollToElementD()},info.onclick=function(){document.getElementById(\"live\").innerHTML=\"--\",wind_temp_sw?(document.getElementById(\"preambleID\").innerHTML=\"Wind speed is,\",document.getElementById(\"mphID\").innerHTML=\"MPH\",document.getElementById(\"info\").style.backgroundColor=\"#d3d3d3\",clearInterval(intervalTemp),intervalWind=setInterval(loadWind,1e3),ctx.clearRect(0,0,canvas.width,canvas.height),drawH(),drawV(),drawHist()):(document.getElementById(\"preambleID\").innerHTML=\"Temperature is,\",document.getElementById(\"mphID\").innerHTML=\"&#176F\",document.getElementById(\"info\").style.backgroundColor=\"#adffcc\",clearInterval(intervalWind),intervalTemp=setInterval(loadTemp,1e3),ctx.clearRect(0,0,canvas.width,canvas.height),drawHT(),drawV(),drawHistTemp()),wind_temp_sw=!wind_temp_sw}},Date.prototype.getHoursTwoDigits=function(){var t=this.getHours();return t<10?\"0\"+t.toString():t.toString()},Date.prototype.getMinutesTwoDigits=function(){var t=this.getMinutes();return t<10?\"0\"+t.toString():t.toString()},loadHist(),loadHistTemp();var intervalTemp,intervalWind=setInterval(loadWind,1e3)</script><div class=wind id=info><div class=preamble id=preambleID>Wind speed is,</div><div class=now id=live>--</div><div class=mph id=mphID>MPH</div></div><div class=slidecontainer></div><div id=parent><canvas height=375 id=graph style=\"border:1px solid #000\"width=15000></canvas></div><input class=slider id=zoom max=20 min=1 type=range value=0><div class=statistics><div class=statN>Stats</div><div class=statP id=max72>0</div><div class=statP id=min72>0</div><div class=statP id=submax72>0</div><div class=statP id=submin72>0</div></div><style>.slidecontainer{width:100%}.now{font-size:400%;font-family:sans-serif;text-align:center;font-weight:700}.statN{font-size:200%;font-family:sans-serif;text-align:center;font-weight:700}.statP{font-size:100%;font-family:monospace;text-align:left;font-weight:700;padding-left:5%}.mph{font-size:100%;font-family:sans-serif;text-align:center;padding-left:200px}.preamble{font-size:100%;font-family:sans-serif;text-align:center;padding-right:200px}#info{width:100%;border:0;background:#d3d3d3;padding-top:20px;padding-right:0;padding-bottom:20px;padding-left:0;margin-top:0;margin-bottom:0;margin-left:0;margin-right:0;border-radius:25px}.statistics{width:100%;border:0;background:#d3d3d3;padding-top:5px;padding-right:0;padding-bottom:20px;padding-left:0;margin-top:5px;margin-bottom:0;margin-left:0;margin-right:0;border-radius:25px}.slider{-webkit-appearance:none;width:100%;height:25px;background:#d3d3d3;outline:0;opacity:.7;-webkit-transition:.2s;transition:opacity .2s}.slider:hover{opacity:1}.slider::-webkit-slider-thumb{-webkit-appearance:none;appearance:none;width:25px;height:25px;background:#4caf50;cursor:pointer}.slider::-moz-range-thumb{width:25px;height:25px;background:#4caf50;cursor:pointer}#parent{width:100%;height:400px;border:0 solid #d3d3d3;overflow:auto;margin-bottom:2px;margin-top:10px}.graph{width:auto;height:100%}</style>";

volatile short* wind_hist;
volatile short* temp_hist;
volatile int sec_counter;
volatile int tenTimes;
volatile short mphS;
volatile int interruptCounter;
unsigned int totalInterruptCounter;

//volatile short test[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};


hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Variable to store the HTTP request
String header;

String float_array_to_string(volatile short w_array[],int start_position, int end_position) {
  String returnstring = "";
  for (int temp = start_position; temp < end_position; temp++)
  {
    snprintf ( buffer, 10, "%.2f",double(w_array[temp % arr_len])/100.0);
    mph_str = buffer;
    if(temp == 0)
      returnstring += mph_str;
    else
      returnstring += "," + mph_str;
    
  }
  return returnstring;
}

void IRAM_ATTR onTimer(){
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}


void setup() {
  wind_hist = (volatile short *)malloc(sizeof(short)*arr_len);
  temp_hist = (volatile short *)malloc(sizeof(short)*arr_len);
  for(int i = 0; i < arr_len;i++)
  {
    wind_hist[i] = -1;
    temp_hist[i] = -1;
  }
//
//  for(int i = 0; i < 5000; i++)
//  {
//    if(i>(5000-72) && i<(5000-68))
//    {
//      wind_hist[i] = 16*100;
//    }
//    else if(i>(5000-140) && i<(5000-136))
//    {
//      wind_hist[i] = 20*100;
//    }
//    else if(i>(5000-848) && i<(5000-818))
//    {
//      wind_hist[i] = 30*100;
//    }
//    else
//    {
//      wind_hist[i] = 5*100;
//    }
//  }
  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
  sensors.getAddress(tempDeviceAddress, 0); //Theres only one...
  sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
  
  sec_counter = 0;
  tenTimes = 0;
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ArtificialReef");
  // Connect to Wi-Fi network with SSID and password

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  analogSetPinAttenuation(analogPin, ADC_6db);
  timer = timerBegin(0, 8000, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 51840, true);
  timerAlarmEnable(timer);
  Serial.println("start timer");
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;

    total = 0;
    int i;
    for(i=1;i<2000;i++)
    {
      sensorValue = analogRead(analogPin);  // Read the analog in value:
      total = total + sensorValue; 
    }
    average = total/(i-1);
  
    //0.0004350603x+0.1039808039
    //0.45.2979625 v/mph
    voltage = 0.0004350603*average+0.1039808039;
    mph = (voltage-0.4)*45.2979625;
    mphS = short((mph+0.615)*100);

    sensors.requestTemperaturesByIndex(0);
    tempF = sensors.getTempF(tempDeviceAddress);
    
    if(totalInterruptCounter % 10 == 0)
    {
      temp_hist[sec_counter] = short(tempF*100);
      wind_hist[sec_counter] = mphS;
      sec_counter++;
      if(sec_counter > (arr_len -1))
      {
        sec_counter = 0;
      }
      //Serial.print("Array Update \n");
    }
    //Serial.print("Current Temp: ");
    //Serial.println(DallasTemperature::toFahrenheit(tempC));
    //Serial.print("An interrupt as occurred. Total number: ");
    //Serial.println(totalInterruptCounter);
    
 
  }


  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Access-Control-Allow-Origin: *");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /wind") >= 0) {
              //Serial.println("wind");
              client.println(double(mphS)/100.0);
              client.println();
            }
//            else if (header.indexOf("GET /cust_wind") >= 0) { 
//              int query = header.indexOf("?");
//              int equal = header.indexOf("=",query);
//              int space = header.indexOf(" ",equal);
//              int lower = header.substring(query+1,equal).toInt();
//              int higher = header.substring(equal+1,space).toInt();
//              Serial.println(lower);
//              Serial.println(higher);
//              client.println("{\"wind_history\":[");         
//              client.println(float_array_to_string(wind_hist,sec_counter+lower,sec_counter+higher));  
//              client.println("]}");
//              client.println();
//              
//            }
            else if (header.indexOf("GET /hist_wind") >= 0) { 
              client.println("{\"histDepth\":");
              client.println(arr_len);
              client.println(",\"freeHeap\":");
              client.println(ESP.getFreeHeap());
              client.println(",\"currPosition\":");
              client.println(sec_counter);
              client.println(",\"wind_history\":[");         
              for(int k = 0; k < PIECES; k++)
              { 
                  client.println(float_array_to_string(wind_hist,arr_len/PIECES*k,arr_len/PIECES*(k+1)));  
              }
              client.println("]}");
              client.println();
            }
            else if (header.indexOf("GET /stat") >= 0) { 
              client.println("{\"histDepth\":");
              client.println(arr_len);
              client.println("\"freeHeap\":");
              client.println(ESP.getFreeHeap());
              client.println(",\"currPosition\":");
              client.println(sec_counter);
              client.println("}");
              client.println();
            } 
            else if (header.indexOf("GET /hist_temp") >= 0) { 
              client.println("{\"histDepth\":");
              client.println(arr_len);
              client.println(",\"freeHeap\":");
              client.println(ESP.getFreeHeap());
              client.println(",\"currPosition\":");
              client.println(sec_counter);
              client.println(",\"temp_history\":[");         
              for(int k = 0; k < PIECES; k++)
              { 
                  client.println(float_array_to_string(temp_hist,arr_len/PIECES*k,arr_len/PIECES*(k+1)));  
              }
              client.println("]}");
              client.println();
            }
            else if (header.indexOf("GET /temp") >= 0) { 
              client.println(tempF);
              client.println();
            } 
            else if (header.indexOf("GET /") >= 0) {
              //Serial.println("index");
              //client.println("<!doctype html><script>function loadDoc(){var t=new XMLHttpRequest;t.onreadystatechange=function(){4==this.readyState&&200==this.status&&(document.getElementById(\"demo\").innerHTML=this.responseText,loadDoc())},t.open(\"GET\",\"http://192.168.1.1/wind\",!0),t.send()}loadDoc()</script><h1>Anemometer</h1><p id=demo>temp");
              client.println(webSITE);
              client.println();
            }


            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
}
