# rtl_433_transmit
Retransmit any 433Mhz Signal captured by the https://github.com/merbanan/rtl_433 with a 433Mhz transmitter for Arduino

## Hardware 
- Arduino compatible board
- SDR Receiver (RTL-SDR)
- 433Mhz Transmitter for Arduino

## Workflow
- Capture the 433Mhz signal with the ***rtl_433 -A*** command 
- Copy the End of the url: https://triq.org/pdv/#AA....
- Call the c++ script `./convert "AA..."` (if you compiled the script like that `g++ -o convert main.cpp`) inside the folder
- Upload the code to the Arduino

