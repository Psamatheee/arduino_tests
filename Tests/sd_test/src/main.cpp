#include <Arduino.h>
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <SD.h>

const int _MISO = 16;
const int _MOSI = 19;
const int _CS = 17;
const int _SCK = 18;

File myFile;
MPU6050 mpu6050(Wire);

int16_t ax,ay,az;
int16_t gx,gy,gz;

char* dataLine = new char[18];

void getMotion6(void){
  ax=mpu6050.getRawAccX();//gain the values of X axis acceleration raw data
  ay=mpu6050.getRawAccY();//gain the values of Y axis acceleration raw data
  az=mpu6050.getRawAccZ();//gain the values of Z axis acceleration raw data
  gx=mpu6050.getRawGyroX();//gain the values of X axis Gyroscope raw data
  gy=mpu6050.getRawGyroX();//gain the values of Y axis Gyroscope raw data
  gz=mpu6050.getRawGyroX();//gain the values of Z axis Gyroscope raw data
}

void writeData(){
  myFile = SD.open("sensor_data.txt", FILE_WRITE);
  String data = "";
  data += String(ax) + " " + String(ay) + " " + String(az) + " " + String(gx) + " " + String(gy) + " " + String(gz);
  myFile.println(data);
  myFile.close();
}

void setup() {
  Wire.begin();
  mpu6050.begin();      
  mpu6050.calcGyroOffsets(true);

  Serial.begin(115200);
  while(!Serial);
  Serial.print("Initializing SD card...");

  SPI.setRX(_MISO);
  SPI.setTX(_MOSI);
  SPI.setSCK(_SCK);

  if (!SD.begin(_CS)) {
    Serial.println("initialization failed!");
    while(true);
  }
  Serial.println("initialization done.");

  if(SD.exists("sensor_data.txt")){
    myFile = SD.open("sensor_data.txt", FILE_READ);
    Serial.println(myFile.readString());
    myFile.close();
  }
}

void loop() {
  mpu6050.update();
  getMotion6();
  writeData();
  delay(100);
}



