
/* Read from SPIFFS: read string ReadSPIFFS(file name) */
void ReadSPIFFS(char readFromSPIFFS[])
{
  File file  = SPIFFS.open(SPIFFS_file_name, "r"); // Openning file from SPIFFS

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
  Serial.print("TOSPIFFS: ");
  Serial.println(toWrite);
  File file = SPIFFS.open(SPIFFS_file_name, "w"); // Openning file from SPIFFS

  if (!file) // Cheking if file opened normaly
  {
    Serial.println("Error opening file for writing");
    return;
  }

  uint16_t bytesWritten = file.println(toWrite); // Write string in SPIFFS

  if (bytesWritten == 0) // Check if written sucesseful
  {
    Serial.println("File write failed");
    file.close();
    return;
  }

  file.close();
}
