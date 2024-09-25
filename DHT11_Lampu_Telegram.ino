#include "CTBot.h"
#include "DHT.h"

CTBot myBot;
DHT dht(D1, DHT11); // Pin DHT11 terhubung ke pin D1

String ssid = "Jangan Lupa Solat";
String pass = "mar050899";
String token = "5764775266:AAEVqU2j6rOkWxQQQtjdQ7odCB5A3qOsi7g";

const int lampu = D2;

void setup() {
  // initialize the Serial
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  // set the pin connected to the LED to act as an output pin
  pinMode(lampu, OUTPUT);
  digitalWrite(lampu, HIGH); // turn off the led (inverted logic!)

  // initialize the DHT11 sensor
  dht.begin();
}

void loop() {

  // a variable to store telegram message data
  TBMessage msg;

  // if there is an incoming message...
  if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("ON")) {                   // Perintah dari telegram ke perangkat
      digitalWrite(lampu, HIGH);                              // Lampu dihidupkan
      myBot.sendMessage(msg.sender.id, "Lampu Menyala ON (Menyala)"); // Balasan dari perangkat ke Bot Telegram
    }
    else if (msg.text.equalsIgnoreCase("OFF")) {              // Perintah dari telegram ke perangkat
      digitalWrite(lampu, LOW);                              // Lampu dimatikan
      myBot.sendMessage(msg.sender.id, "Lampu Sekarang OFF (Mati)"); // Balasan dari perangkat ke Bot Telegram
    }
    else if (msg.text.equalsIgnoreCase("SUHU")) {      // Perintah dari telegram ke perangkat
      float temperature = dht.readTemperature();              // Membaca suhu dari sensor DHT11
      String reply = "Suhu ruangan " + String(temperature) + " °C";
      myBot.sendMessage(msg.sender.id, reply);                // Mengirimkan suhu melalui Bot Telegram
    }
    else if (msg.text.equalsIgnoreCase("KELEMBABAN")) {         // Perintah dari telegram ke perangkat
      float humidity = dht.readHumidity();                    // Membaca kelembaban dari sensor DHT11
      String reply = "Kelembaban ruangan " + String(humidity) + " %";
      myBot.sendMessage(msg.sender.id, reply);                // Mengirimkan kelembaban melalui Bot Telegram
    }
    else {                                                    // otherwise...
      // generate the message for the sender
      String reply;
      reply = "Selamat datang di QihBotLamp " + msg.sender.username + ". \n - Ketik ON = Menyalakan Lampu \n - Ketik OFF = Mematikan Lampu \n - Ketik SUHU = Mengecek Suhu ruangan \n - Ketik KELEMBABAN = Mengecek Kelembaban ruangan \n\n BOT DIBUAT OLEH FAQIH AMIRUDIN - TI20B - UNIVERSITAS BINA INSANI .";
      myBot.sendMessage(msg.sender.id, reply);             // and send it
    }

  }
  // wait 500 milliseconds
  delay(500);
}
