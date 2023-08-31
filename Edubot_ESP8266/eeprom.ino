
void saveConfig(){
  for (unsigned int t = 0; t < sizeof(robotConfig); t++) {
    EEPROM.write(CONFIG_START + t, *((char*)&robotConfig + t));
  }
  //Saving the version ID to pull from EEPROM the next time you load
  EEPROM.write(VERSION_START + 0, CONFIG_VERSION[0]);
  EEPROM.write(VERSION_START + 1, CONFIG_VERSION[1]);
  EEPROM.commit();
}

void loadConfig(){
  if (EEPROM.read(VERSION_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(VERSION_START + 1) == CONFIG_VERSION[1]) {
    for (unsigned int t = 0; t < sizeof(robotConfig); t++)
      *((char*)&robotConfig + t) = EEPROM.read(CONFIG_START + t);
  }
}

void changeState(String txtstate){
  txtstate.toCharArray(robotConfig.state, 5);
  saveConfig();
}
