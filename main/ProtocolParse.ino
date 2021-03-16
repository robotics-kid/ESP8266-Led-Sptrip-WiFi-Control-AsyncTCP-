
uint8_t Tokenizer(char recv_msg[])
{

  if (!strcmp(recv_msg, "")) {
    //Serial.println("NULL string recieved");
    return 0;
  }

  char* token = strtok(recv_msg, del); // Initialize token to split recv by delimiter

  uint16_t i = 0;
  bool flag = false;
  while (token != NULL)
  {
    // Checks if token[0] is equal to root_previx
    if (i == 0 and strcmp(token, root_previx))
    {
      Serial.println("Root prefix does not match");
      return 0;
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

  if (!strcmp(WiFiHandler[0].handlerChar, "REQ")) {
    Serial.println("2 returned");
    RequestRecieved = true;
    return 2;
  }



  /* Serial.println();
    for(int i =0; i < argsLen; i++){
    Serial.print("Char: ");
    Serial.println(WiFiHandler[i].handlerChar);

    Serial.print("Val: ");
    Serial.println(WiFiHandler[i].handlerVal);
    }
    Serial.println(); */
  return 1;
}

void effectHandler(char toSPIFFS[])
{
  //Serial.printf("4: %d\n", millis()); // 51657
  if (!strcmp(WiFiHandler[0].handlerChar, "EFF"))
  {
    WriteSPIFFS(toSPIFFS);
    //Serial.printf("5: %d\n", millis()); // 51664
    /*char tmp[100];
      ReadSPIFFS(tmp);
      Serial.print("Read SPIFFS: ");
      Serial.println(tmp);*/

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
    //Serial.printf("6: %d\n", millis()); // 51666
  }
}

void handshaking(char mSend[])
{
  memset(mSend, 0, sizeof(char)*argsLen);
  char tmp[3];
  if (!strcmp(WiFiHandler[0].handlerChar, "REQ"))
  {

    switch (WiFiHandler[0].handlerVal) // Tutn on effect prepare functions proceeding from WiFiHandler array value
    {
      case 1:
        //FoViBalTLight;LDT:1;STT:1!
        strcat(mSend, "FoViBalTLight;LDT:");
        itoa(LEDS_TYPE, tmp, 10);
        strcat(mSend, tmp);
        strcat(mSend, ";STT:");
        memset(tmp, 0, sizeof(char)*strlen(tmp));
        itoa(STRIP_TYPE, tmp, 10);
        strcat(mSend, tmp);
        strcat(mSend, "!");
        Serial.println(mSend);
        break;
      case 2:
        //FoViBalTLight;DEV:1;DEV:2;DEV:3 ...
        strcat(mSend, "FoViBalTLight");
        String preIp = "";
        AsyncClient* preIP = 0;
        for (uint16_t i = 0; i < clients.size(); i++)
        {
          if (clients[i]->canSend() and preIp != clients[i]->remoteIP().toString().c_str()) {
            strcat(mSend, ";DEV:");
            itoa(i, tmp, 10);
            strcat(mSend, tmp);
            preIp = clients[i]->remoteIP().toString().c_str();
            Serial.println(clients[i]->remoteIP().toString().c_str());
            //preIP = clients[i];
          }
          
        }
        strcat(mSend, "!");
        Serial.println(mSend);
        break;
    }
  }
}
