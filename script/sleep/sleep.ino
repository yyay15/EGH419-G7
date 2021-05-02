#define WAKEUP_MASK_37 0x2000000000
#define WAKEUP_MASK_38 0x4000000000
#define WAKEUP_MASK_ALL (WAKEUP_MASK_37 | WAKEUP_MASK_38)

void printWakeupReason(){
  uint32_t wakeup_status = esp_sleep_get_ext1_wakeup_status();
  Serial.println(wakeup_status);
  if (wakeup_status == WAKEUP_MASK_37)
  {
    Serial.println("Wakeup caused by button");
  }
  else if (wakeup_status == WAKEUP_MASK_38)
  {
    Serial.println("Wakeup caused by NFC");
  }
  else
  {
    Serial.println("Wakeup not caused by interrupt");
  }
  /*esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }*/
}

void setup() {
  Serial.begin(115200);
  
  esp_sleep_enable_ext1_wakeup(WAKEUP_MASK_ALL, ESP_EXT1_WAKEUP_ANY_HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  printWakeupReason();
  Serial.println("Going to sleep...");
  delay(1000);
  esp_deep_sleep_start();
}
