
#define SERIAL_CALLBACKS_COUNT 8

typedef struct my_serial_command_s{
    char cmd[2];
    byte (*callback)( void *arg, void *val);
}my_serial_command;

byte _cmd_help(void *a, void *p);
byte _cmd_printTime(void *a, void *p);
byte _cmd_setTime(void *a, void *p);
byte _cmd_printAlarms(void *a, void *p);
byte _cmd_enableAlarm(void *a, void *p);
byte _cmd_disableAlarm(void *a, void *p);
byte _cmd_irrigationStart(void *a, void *p);
byte _cmd_irrigationStop(void *a, void *p);

my_serial_command serialCommands[SERIAL_CALLBACKS_COUNT] = {
 {"+h",_cmd_help},
 {"+p", _cmd_printTime},
 {"+s", _cmd_setTime},
 {"+a", _cmd_printAlarms},
 {"+e", _cmd_enableAlarm},
 {"+d", _cmd_disableAlarm},
 {"+1", _cmd_irrigationStart},
 {"+0", _cmd_irrigationStop}
};

byte _cmd_printTime(void *a, void *p){
    serialPrintNow();
}

byte _cmd_help(void *a, void *p){
   Serial.println("+h");
   for (int i=0; i<SERIAL_CALLBACKS_COUNT; i++){
     Serial.print("+ Funx @ 0x0");
     void *b= (void*)serialCommands[i].callback;
     Serial.print( int(b), HEX);     
     Serial.print(" CMD: ");     
     Serial.print( serialCommands[i].cmd[0] );
     Serial.println( serialCommands[i].cmd[1] ); 
   }
   return 1;
}

byte _cmd_setTime(void *a, void *p){
  char *buffer = (char *)calloc(sizeof(char), 8);
  strncpy(buffer, (char *)a, 8);
  char *indexStr = strtok(buffer, ".");
  char *dowStr = strtok(NULL, ".");
  char *durationStr = strtok(NULL, ".");

  int i = atoi(indexStr);
  int dow = atoi(dowStr);
  int duration = atoi(durationStr);

  alarms[i].dow = (byte)dow;
  alarms[i].enabled = true;
  alarms[i].duration = (byte)duration;

  free(buffer);

  buffer = (char *)calloc(sizeof(char),5);
  strncpy(buffer, (char *)p, 5);
  char *hStr = strtok(buffer, ":");
  char *mStr = strtok(NULL, ":");

  int h = atoi(hStr);
  int m = atoi(mStr);

  alarms[i].hour = (byte)h;
  alarms[i].min = (byte)m;

  free(buffer);

  Serial.println("SET OK");
  storeAlarms();

}

byte _cmd_printAlarms(void *a, void *p){
  printAlarms();
}

byte _cmd_enableAlarm(void *a, void *p){
  byte i = atoi((char *)a );
  alarms[i].enabled = true;
  Serial.print("Enabled alarm ");
  Serial.println(i);
  storeAlarms();
}

byte _cmd_disableAlarm(void *a, void *p){
  byte i = atoi((char *)a );
  alarms[i].enabled = false;
  Serial.print("Disabled alarm ");
  Serial.println(i);
  storeAlarms();
}

byte _cmd_irrigationStart(void *a, void *p){
  Serial.println("Irrigation starts now");
  irrigaStart();
}

byte _cmd_irrigationStop(void *a, void *p){
  Serial.println("Irrigation stops now");
  irrigaStop();
  irrigaReset();
}
