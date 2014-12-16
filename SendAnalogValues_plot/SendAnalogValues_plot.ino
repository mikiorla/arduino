/* ******************************************************
    Send data to MegunoLink for plotting. 
    This program reads the first three analog input values
    and sends them to MegunoLink for plotting.
    
    Download MegunoLink, a free tool for uploading programs,
    monitoring, logging and plotting Arduino data, from: 
    http://www.MegunoLink.com/Download_MegunoLink
   ****************************************************** */

#include <GraphSeries.h>

GraphSeries g_aGraphs[] = {"ADC3", "ADC4", "ADC5"};

void setup()
{
  // The data is sent via the serial port. Initialize it.
  Serial.begin(9600);
}


void loop()
{
  int nADCInput;

  // Read values from the ADC converter and send them out the serial port.
  for (nADCInput = 3; nADCInput <= 5; ++nADCInput)
    g_aGraphs[nADCInput].SendData(analogRead(nADCInput));

  delay(300);
}
