//       ___ _    _ _ _           _  _
//      | _ \ |_ (_) (_)_ __ ___ | || |_  _ ___
//      |  _/ ' \| | | | '_ (_-< | __ | || / -_)
//      |_| |_||_|_|_|_| .__/__/ |_||_|\_,_\___|
//                     |_|
//       PicoC Integration for Loxone Miniserver
// ***************************************************
//  Generated on: 28 June 2025, 17:44 CEST        // Updated date and time
//  Simplified for Philips Hue scene triggering
//  This code is written in Pico C, a scripting language used within Loxone Config
//  to control smart home devices. It allows triggering Philips Hue scenes or
//  turning off groups via Loxone inputs (e.g., I0-I12). For beginners: Think of
//  this as a custom script that connects your Loxone system to Hue lights!
//  I0, I1: Scene on (group 3), I2: Group off (group 3)
//  I3, I4, I5: Scene on (group 1), I6: Group off (group 1)
//  I7, I8, I9: Scene on (group 4), I10: Group off (group 4)
//  Uses setoutput and sendcmd for feedback to Loxone
//  Logs prefixed with "HUE" for debugging
// ***************************************************

// Configuration constants
#define ENABLE_DEBUGGING (1) // Enable debugging for testing
#define RX_BUFFER_SIZE (40000)
#define RX_BLOCK_SIZE (128)

// Function prototypes
void SendCommand(int groupId, char *command);
void SetScene(int groupId, char *sceneId, int outputIndex);
void SetGroupOff(int groupId, int outputIndex);

// Input types
enum bulb_t__
{
  TYPE_SCENE,     // Scene activation
  TYPE_GROUP_OFF, // Turn off group
  TYPE_NONE
};

// Global arrays
int bulbType[13];  // Type of input (scene or none)
int bulbId[13];    // Group ID for the scene
char *sceneId[13]; // Scene ID for each input

// ***************************************************
//  Configure your system here                   START
// ***************************************************
char *IP_ADDRESS = "192.168.1.100";                  // Hue Bridge IP
char *PORT = "80";                                   // Hue Bridge port (HTTP)
char *USERNAME = "1028d66426293e821ecfd9ef1a0731df"; // Hue API key

// Set type of input (I0-I12)
bulbType[0] = TYPE_SCENE;      // I0: Scene on (group 3)
bulbType[1] = TYPE_SCENE;      // I1: Scene on (group 3)
bulbType[2] = TYPE_GROUP_OFF;  // I2: Group off (group 3)
bulbType[3] = TYPE_SCENE;      // I3: Scene on (group 1)
bulbType[4] = TYPE_SCENE;      // I4: Scene on (group 1)
bulbType[5] = TYPE_SCENE;      // I5: Scene on (group 1)
bulbType[6] = TYPE_GROUP_OFF;  // I6: Group off (group 1)
bulbType[7] = TYPE_SCENE;      // I7: Scene on (group 4)
bulbType[8] = TYPE_SCENE;      // I8: Scene on (group 4)
bulbType[9] = TYPE_SCENE;      // I9: Scene on (group 4)
bulbType[10] = TYPE_GROUP_OFF; // I10: Group off (group 4)
bulbType[11] = TYPE_NONE;
bulbType[12] = TYPE_NONE;

// Set group ID
bulbId[0] = 3;  // I0: Group 3
bulbId[1] = 3;  // I1: Group 3
bulbId[2] = 3;  // I2: Group 3
bulbId[3] = 1;  // I3: Group 1
bulbId[4] = 1;  // I4: Group 1
bulbId[5] = 1;  // I5: Group 1
bulbId[6] = 1;  // I6: Group 1
bulbId[7] = 4;  // I7: Group 4
bulbId[8] = 4;  // I8: Group 4
bulbId[9] = 4;  // I9: Group 4
bulbId[10] = 4; // I10: Group 4
bulbId[11] = 0;
bulbId[12] = 0;

// Set scene ID
sceneId[0] = "f5zBZNQ0tdzhJo-"; // I0: Group 3 scene  //Hoofdverlichting
sceneId[1] = "OxEpoBvjp4n2wB6"; // I1: Group 3 scene  //Ochtend
sceneId[2] = "";                // I2: Group 3 off    // Off command for group 3
sceneId[3] = "KvzUuemWUFcpOJN"; // I3: Group 1 scene
sceneId[4] = "6NxSiu3BgRRfEGd"; // I4: Group 1 scene
sceneId[5] = "kH8Xn-1FCSVgUaA"; // I5: Group 1 scene
sceneId[6] = "";                // I6: Group 1 off
sceneId[7] = "lbtvSuG1NgQcTs3"; // I7: Group 4 scene
sceneId[8] = "mU7fvDJsifmiGNz"; // I8: Group 4 scene
sceneId[9] = "ECLulCPGuSs5Xo3"; // I9: Group 4 scene
sceneId[10] = "";               // I10: Group 4 off
sceneId[11] = "";
sceneId[12] = "";
// ***************************************************
//  Configure your system here                     END
// ***************************************************

void SendCommand(int id, char *command)
{
  char streamname[100];
  char selector[30];
  char txBuffer[1024];
  char rxBuffer[4096]; // Reduced from 40000 to 4096
  char szTmpBuffer[128];
  int nCnt;
  int nBytesReceived = 0;

  if (ENABLE_DEBUGGING)
  {
    printf("HUE Entering SendCommand for group %d: %s\n", id, command);
  }

  if (strlen(command) > 500)
  {
    if (ENABLE_DEBUGGING)
    {
      printf("HUE Error: Command too long\n");
    }
    setoutput(12, 1);
    sendcmd("Error: Command too long", 0);
    return;
  }

  sprintf(streamname, "/dev/tcp/%s/%s/", IP_ADDRESS, PORT);
  if (ENABLE_DEBUGGING)
  {
    printf("HUE Creating stream: %s\n", streamname);
  }
  STREAM *TcpStream = stream_create(streamname, 0, 0);
  if (TcpStream == NULL)
  {
    if (ENABLE_DEBUGGING)
    {
      printf("HUE Error: Creating Stream failed\n");
    }
    setoutput(12, 1);
    sendcmd("Error: Creating Stream failed", 0);
    return; // Avoid closing NULL stream
  }

  sprintf(txBuffer, "PUT /api/%s/groups/%d/action HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nContent-Length: %d\r\n\r\n%s",
          USERNAME, id, IP_ADDRESS, strlen(command), command);
  if (ENABLE_DEBUGGING)
  {
    printf("HUE Sending request:\n%s\n", txBuffer);
  }
  int writeResult = stream_write(TcpStream, txBuffer, strlen(txBuffer));
  if (writeResult <= 0)
  {
    if (ENABLE_DEBUGGING)
    {
      printf("HUE Error: Failed to write to stream\n");
    }
    setoutput(12, 1);
    sendcmd("Error: Failed to write to stream", 0);
    stream_close(TcpStream);
    return;
  }
  stream_flush(TcpStream);

  // Read response with a max iteration limit
  int maxIterations = 32; // Limit to 32 * 128 = 4096 bytes
  while (maxIterations-- > 0)
  {
    nCnt = stream_read(TcpStream, szTmpBuffer, 128, 4000);
    if (nCnt <= 0)
      break;
    if (nCnt + nBytesReceived > 4096)
    {
      nBytesReceived = -1;
      if (ENABLE_DEBUGGING)
      {
        printf("HUE Error: Response too large\n");
      }
      break;
    }
    strncpy((char *)rxBuffer + nBytesReceived, szTmpBuffer, nCnt);
    nBytesReceived += nCnt;
  }

  if (ENABLE_DEBUGGING && nBytesReceived > 0)
  {
    if (strstr(rxBuffer, "[{") != NULL)
    {
      printf("HUE Response received:\n%s\n", strstr(rxBuffer, "[{"));
    }
    else
    {
      printf("HUE No valid response received\n");
    }
  }
  stream_close(TcpStream);
  setoutput(12, 0);
}

void SetScene(int groupId, char *sceneId, int outputIndex)
{
  char command[100];
  sprintf(command, "{\"scene\": \"%s\"}", sceneId);
  if (ENABLE_DEBUGGING)
  {
    printf("HUE Activating scene %s on group %d\n", sceneId, groupId);
  }
  SendCommand(groupId, command);
  setoutput(outputIndex, 1); // Set corresponding output to indicate success
}

void SetGroupOff(int groupId, int outputIndex)
{
  char command[50];
  sprintf(command, "{\"on\": false}");
  if (ENABLE_DEBUGGING)
  {
    printf("HUE Turning off group %d\n", groupId);
  }
  SendCommand(groupId, command);
  setoutput(outputIndex, 1); // Set corresponding output to indicate success
}

void UpdateScene(int i, int value)
{
  int id = bulbId[i];
  char *scene = sceneId[i];

  if (ENABLE_DEBUGGING)
  {
    printf("HUE UpdateLamp: Input %d = %d\n", i, value);
  }

  if (bulbType[i] == TYPE_SCENE)
  {
    if (value > 0) // Trigger scene only when input is non-zero
    {
      SetScene(id, scene, i);
    }
    else
    {
      setoutput(i, 0); // Reset output when input is 0
    }
  }
  else if (bulbType[i] == TYPE_GROUP_OFF)
  {
    if (value > 0) // Trigger group off when input is non-zero (CHANGE THIS LINE)
    {
      SetGroupOff(id, i);
    }
    else
    {
      setoutput(i, 0); // Optional: Reset output when input is 0, for consistency with scenes
    }
  }
}

// Main application
int inputsThatChanged;
int i;
int refreshCounter = 0;

while (1)
{
  if (ENABLE_DEBUGGING)
  {
    // printf("HUE Main loop iteration\n");
  }

  // Get a bitmask of the changed inputs
  inputsThatChanged = getinputevent();

  // Fast round to update all scenes that changed
  for (i = 0; i < 13; i++)
  {
    if (inputsThatChanged & (0x8 << i))
    {
      if (ENABLE_DEBUGGING)
      {
        printf("HUE Input %d changed\n", i);
      }
      UpdateScene(i, (int)getinput(i));
    }
  }

  // Slower round with sleep to handle network congestion
  for (i = 0; i < 13; i++)
  {
    if (inputsThatChanged & (0x8 << i))
    {
      UpdateScene(i, (int)getinput(i));
      sleep(10);
    }
  }

  refreshCounter = refreshCounter + 1;

  // Refresh all scenes approximately every 60s
  if (3000 < refreshCounter)
  {
    if (ENABLE_DEBUGGING)
    {
      printf("HUE Refreshing all inputs\n");
    }
    refreshCounter = 0;
    for (i = 0; i < 13; i++)
    {
      UpdateScene(i, (int)getinput(i));
      sleep(10);
    }
  }
  sleep(100);
}
