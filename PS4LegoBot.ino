#include <PS4Controller.h>

uint16_t button_state;
uint8_t PS4_state;

uint8_t Rx_buffer[9];

uint8_t data;

bool PS4set;
bool Serialflag;

void onConnect(){
  PS4set = true;
  PS4.setLed(255,0,0);
}

void onReport(){
  button_state = 0;
  button_state |= (PS4.Cross()<<0);
  button_state |= (PS4.Square()<<1);
  button_state |= (PS4.Triangle()<<2);
  button_state |= (PS4.Circle()<<3);
  button_state |= (PS4.Up()<<4);
  button_state |= (PS4.Right()<<5);
  button_state |= (PS4.Down()<<6);
  button_state |= (PS4.Left()<<7);
  button_state |= (PS4.L1()<<8);
  button_state |= (PS4.R1()<<9);
  button_state |= (PS4.L3()<<10);
  button_state |= (PS4.R3()<<11);
  button_state |= (PS4.Share()<<12);
  button_state |= (PS4.Options()<<13);
  button_state |= (PS4.PSButton()<<14);
  button_state |= (PS4.Touchpad()<<15);

  Rx_buffer[1] = button_state & 0xFF;
  Rx_buffer[2] = (button_state >> 8) & 0xFF;
  Rx_buffer[3] = PS4.LStickX();
  Rx_buffer[4] = PS4.LStickY();
  Rx_buffer[5] = PS4.RStickX();
  Rx_buffer[6] = PS4.RStickY();
  Rx_buffer[7] = PS4.L2Value();
  Rx_buffer[8] = PS4.R2Value();
}


  uint8_t btmac[6];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  esp_read_mac(btmac,ESP_MAC_BT);
  char MacAdd[32];
  sprintf(MacAdd, "%02X:%02X:%02X:%02X:%02X:%02X", btmac[0], btmac[1], btmac[2], btmac[3], btmac[4], btmac[5]);
  PS4.begin(MacAdd);
  PS4.attachOnConnect(onConnect);
}
int16_t gyroX;
int16_t gyroY;
int16_t gyroZ;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    data = Serial.read();
    Serialflag = (data & 0x01);
    if(Serialflag){
      Serial.write(Rx_buffer,9);
    }
  }
  onReport();
  PS4.sendToController();
//  gyroX = PS4.GyrX();
//  gyroY = PS4.GyrY();
//  gyroZ = PS4.GyrZ();
//  Serial.printf("X:%d Y:%d Z:%d\n",gyroX,gyroY,gyroZ);

}
