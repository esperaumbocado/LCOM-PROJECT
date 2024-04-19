#include "mouse.h"

void mouse_disable_data_reporting(){
  if(WRITE_KBC_CMD(KBC_WRITE_CMD,MOUSE_ACKNOWLEDGMENT)!=0)return 1;
  if(WRITE_KBC_CMD(KBC_IN_BUF,MOUSE_DISABLE_DATA_REPORTING)!=0)return 1;
  return 0;
}