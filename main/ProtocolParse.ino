
void Tokenizer(char recv[])
{
  Serial.print("Recv: ");
  Serial.println(recv);

  if (!strcmp(recv, "")) {
    Serial.print("Recv in if: ");
    Serial.println(recv);
    Serial.println("NULL string recieved");
    return;
  }

  memset(WiFiHandler, '\0', sizeof(handler)*argsLen); // Filling WiFiHandler with NULL values

  char* token = strtok(recv, del); // Initialize token to split recv by delimiter

  uint16_t i = 0;
  bool flag = false;
  while (token != NULL)
  {
    // Checks if token[0] is equal to root_previx
    if (i == 0 and strcmp(token, root_previx))
    {
      Serial.println("Root prefix does not match");
      return;
    }

    // If it is not first root_previx
    if (flag)
    {
      if ((i % 2) == 0)
      {
        WiFiHandler[i / 2 - 1].handlerVal = atoi(token);
      }
      else if ((i % 2) == 1)
      {
        strcpy(WiFiHandler[(i + 1) / 2 - 1].handlerChar, token);
      }
    }
    token = strtok(NULL, del); // Incrementing token
    flag = true;
    i++;
  }

  /* Serial.println();
    for(int i =0; i < argsLen; i++){
    Serial.print("Char: ");
    Serial.println(WiFiHandler[i].handlerChar);

    Serial.print("Val: ");
    Serial.println(WiFiHandler[i].handlerVal);
    }
    Serial.println(); */
}

void effectHandler(char toSPIFFS[])
{
  if (!strcmp(WiFiHandler[0].handlerChar, "EFF"))
  {
    WriteSPIFFS(toSPIFFS);
    char tmp[100];
    ReadSPIFFS(tmp);
    Serial.print("Read SPIFFS: ");
    Serial.println(tmp);

    switch (WiFiHandler[0].handlerVal) // Tutn on effect prepare functions proceeding from WiFiHandler array value
    {
      case 1:
        // Extract from WiFiHandler arguments for SPECIAL EFFECT
                colorEffect(); // Color solid effect
        break;
      case 2:
        //Extract from WiFiHandler arguments for SPECIAL EFFECT
        whiteEffect(); // White solid effect
        break;
      case 3:
        gradientEffect_2Val();
        break;
    }

  }
}
