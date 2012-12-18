import ddf.minim.analysis.*;
import ddf.minim.*;
import processing.serial.*;
import xbee.*;
 
Minim minim;
AudioPlayer input;
FFT fft;
Serial myPort;
XBeeReader xbee;

int [] ad16 = {0xFFFF};
int [] payload = {0};

int LEDNUM = 125;

int lowStart = 0;
int lowEnd = 11;
int midStart = 12;
int midEnd = 68;
int highStart = 69;
int highEnd = 512;

float lowVol = 10;  // volume filter for low frequency
float midVol = 10;  // volume filter for mid frequency
float highVol = 5;  // volume filter for high frequency

int bplLow = 1;  // low frequency blocks/LED
int bplMid = 1;  // mid frequency blocks/LED
int bplHigh = 4;  // high frequency blocks/LED


void setup()
{
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  xbee = new XBeeReader(this, myPort);
  xbee.startXBee();
  
  size(600, 400);
  
  //initialization formusic 
  minim = new Minim(this);
  input = minim.loadFile("01 - Gun's & Roses.mp3", 1024);
  input.play();
  fft = new FFT(input.bufferSize(), input.sampleRate());
  
  // print out the Block number and freqency range
  for (int i = 0; i < fft.specSize(); i++) {  
    println("Block " + i + " is " + fft.getBandWidth()*i + "Hz ~ "+ fft.getBandWidth()*(i+1) + "Hz");
  }
}

void draw()
{
  background(0);
  fill(150);  
  stroke(255);
  strokeWeight(1);
  
  XBeeDataFrame data = xbee.getXBeeReading();
  
  fft.forward(input.mix);
  int specSize = fft.specSize();
  
  for(int i = 0; i < input.mix.size()-1; i++)
    point(i, 200 + input.mix.get(i)*200);
    
  for(int i = 0; i < specSize; i++) {
   float x = map(i, 0, specSize, 0, width);
    stroke(x, 100, 100, 100);
    strokeWeight(6);
    line(x, height, x, height - fft.getBand(i) * 10); 
  }
  
  int i = 0;
  int temp = 0;
  
  while(i < LEDNUM)
  {
      if (isLow(temp)) {
        if (getFrequency(temp, bplLow, lowVol)){
          payload[0] = i;
          xbee.sendDataString16(ad16[0], payload);
          print(i+1 + ", ");
        }
        temp += bplLow;
      }
      else if (isMid(temp)) {
        if (getFrequency(temp, bplMid, midVol)){
          payload[0] = i;
          xbee.sendDataString16(ad16[0], payload);
          print(i+1 + ", ");
        }
        temp += bplMid;
      }
      else if (isHigh(temp)) {
        if (getFrequency(temp, bplHigh, highVol)){
          payload[0] = i;
          xbee.sendDataString16(ad16[0], payload);
          print(i+1 + ", ");
        }
        temp += bplHigh;
      }
    i++;
  }
  println();  
} 
 
void stop()
{
  input.close();  
  minim.stop();

  super.stop();
}

boolean getFrequency (int n, int bpl, float vol)
{
  for (int i = 0; i < bpl; i++)
    if ((fft.getBand(i + n) > vol))
      return true;
    return false;
}

boolean isLow (int n)
{
  if (lowStart <= n && n <= lowEnd)
    return true;
  else
    return false;
}

boolean isMid(int n)
{
  if (midStart <= n && n <= midEnd)
    return true;
  else
    return false;
}

boolean isHigh(int n)
{
  if (highStart <= n && n + 3 <= highEnd)
    return true;
  else
    return false;
}

// Not sure if needed but for errer handle.
public void xBeeEvent(XBeeReader xbee) {
        println("Xbee Event!");
        // Grab a frame of data
        XBeeDataFrame data = xbee.getXBeeReading();

        // This version of the library only works with IOPackets
        if (data.getApiID() == xbee.SERIES1_IOPACKET) {
            // Get the transmitter address
            int addr = data.getAddress16();
            // Get the RSSI reading in dBM 
            int rssi = data.getRSSI();
            long addr64 = data.getAddress64();
            int totalSamples = data.getTotalSamples();
            
            for (int n = 0; n < totalSamples; n++) {
                print("Sample: " + n + "  ");
                // Current state of each digital channel (-1 indicates channel is not configured)
                int[] digital = data.getDigital(n);  
                // Current state of each analog channel (-1 indicates channel is not configured);
                int[] analog = data.getAnalog(n);   
                
                // This example simply prints the data to the message window
                print("16 address: " + addr + " rssi: " + rssi + "  64address: " + addr64);
                print("  digital: ");
                
                for (int i = 0; i < digital.length; i++) {
                    print(digital[i] + " ");
                }
                print("  analog: ");
                for (int i = 0; i < analog.length; i++) {
                    print(analog[i] + " ");
                }
                println("");
            }
        }
        else {
            println("Not I/O data: " + data.getApiID());
        }
}
