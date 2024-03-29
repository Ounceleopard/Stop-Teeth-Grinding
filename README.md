## How does it work?
  
When the patient begins to bite down the strain gauge will read this force and a small amount of voltage will be released via a TENS attachment to relax the muscles so that their mouth remains open. Further testing is needed but as a result snoring can also be affected by this action. 

- This is all done wirelessly so that the patient doesn't experience discomfort in their sleep. Why WIFI instead of BLE because it was more stable and reliable. 
  - Power consumption is minimal due to the fact that both sender and client are only listening every other second.
- Please note that the gauge threshold and TENS deployment high to low voltage output duration can be adjusted.
- ESP32-WIFI-Strain-Guage-TENS
  
### Sender & Receiver
- Obtain your devices MAC addresses by using [Mac-Address-Code.ino](https://github.com/Ounceleopard/Stop-Teeth-Grinding/blob/0793f728ac65816c50a5f721e32866592153aeb7/Mac-Address-Code.ino)
- Server.ino (Sender)
MAC Address: C8:F0:9E:4E:55:B0  
0xC8, 0xF0, 0x9E, 0x4E, 0x55, 0xB0 // MAC address of server 
 
- Client.ino (Receiver)
MAC Address: EC:62:60:1D:24:88    
0xEC, 0x62, 0x60, 0x1D, 0x24, 0x88 // MAC address of client

### Hardware 
- Board module: DOITESP32 DEVKIT V1 

### Final Product
<img width="554" alt="Final Product" src="https://github.com/Ounceleopard/ESP32-Wireless-Modules-StrainGuage-Tensor/assets/40043757/5414573e-500d-4b56-825f-93d8fbc72c36">

### Highly recommend reading
Client ESP32 Understand these concepts

https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/

https://linuxhint.com/use-analog-pin-as-digital/

https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/

https://www.oreilly.com/library/view/getting-started-with/9781491900550/ch04.html

### Disclaimer
This code for educational purposes only, do not attempt. I am not liable for any damages or injuries.
