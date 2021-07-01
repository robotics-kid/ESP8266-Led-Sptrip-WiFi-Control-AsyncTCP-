
/* Read from SPIFFS: read string ReadSPIFFS(file name) */
void ReadSPIFFS(char readFromSPIFFS[])
{

  File file  = LittleFS.open(SPIFFS_file_name, "r"); // Openning file from SPIFFS

  if (!file) // Cheking if file opened normaly
  {
    Serial.println("Failed to open file for reading");
    strcat(readFromSPIFFS, "-1");
  }

  while (file.available()) // Open while() loop while file is available
  {
    file.readStringUntil('\n').toCharArray(readFromSPIFFS, argsLen); // Read from file unti  l \n while file is available
  }

  file.close();
}

/* Write string to SPIFFS: writed sucesefuly or not WriteSPIFFS(file name, writting string) */
void WriteSPIFFS(char toWrite[])
{
  Serial.println(millis());
  //Serial.printf("4.1: %d\n", millis()); // 32994
  File file = LittleFS.open(SPIFFS_file_name, "w"); // Openning file from SPIFFS
  //Serial.printf("4.2: %d\n", millis()); // 32999
  if (!file) // Cheking if file opened normaly
  {
    Serial.println("Error opening file for writing");
    return;
  }
  //Serial.printf("4.3: %d\n", millis()); // 32999
  uint16_t bytesWritten = file.println(toWrite); // Write string in SPIFFS
  //Serial.printf("4.4: %d\n", millis()); // 32999
  if (bytesWritten == 0) // Check if written sucesseful
  {
    Serial.println("File write failed");
    file.close();
    return;
  }
  //Serial.printf("4.5: %d\n", millis()); / 33000
  file.close();
  Serial.println(millis());
  Serial.println();
}
