
/* clients events */
static void handleError(void* arg, AsyncClient* client, int8_t error) {
  Serial.printf("\n Connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleDisconnect(void* arg, AsyncClient* client) {
  Serial.printf("\n Client %s disconnected \n", client->remoteIP().toString().c_str());
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) {
  Serial.printf("\n Client ACK timeout IP: %s \n", client->remoteIP().toString().c_str());
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
  Serial.printf("\n data received from client %s \n", client->remoteIP().toString().c_str());
  //Serial.write((uint8_t*)data, len);

  char recv[len];
  memset(recv, 0, sizeof(char)*(len+10));
  
  for (int i = 0; i < len; i++) {
    recv[i] = (char)((uint8_t*)data)[i];

  }
  
  Serial.println();
  Serial.println(len);
  Serial.println(recv);
  
  char toSPIFFS[len];
  strcpy(toSPIFFS, recv);

  Tokenizer(recv);
  effectHandler(toSPIFFS);
  /* reply to client
    if (client->space() > 32 && client->canSend()) {
    char reply[32];
    sprintf(reply, "this is from %s", SERVER_HOST_NAME);
    client->add(reply, strlen(reply));
    client->send();
    }*/
}

/* server events */
static void handleNewClient(void* arg, AsyncClient* client) {
  Serial.printf("\n New client has been connected to server, IP: %s", client->remoteIP().toString().c_str());

  // add to list
  clients.push_back(client);

  // register events
  client->onData(&handleData, NULL);
  client->onError(&handleError, NULL);
  client->onDisconnect(&handleDisconnect, NULL);
  client->onTimeout(&handleTimeOut, NULL);
}
