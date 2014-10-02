void upload_to_ftp(char* targetLocalFilename, char* targetLocalFileDir){
  char ftpServerString[255];
  char ftpServerPortString[255];
  char ftpServerUserString[255];
  char ftpServerPassString[255];
  char deleteFileNameConstruct[255];
  
  int answerrr;
  
  sprintf(ftpServerString, "AT+CFTPSERV=\"%s\"", ftpServer);
  sprintf(ftpServerPortString, "AT+CFTPPORT=%d", ftpPort);
  sprintf(ftpServerUserString, "AT+CFTPUN=\"%s\"", ftpUser);
  sprintf(ftpServerPassString, "AT+CFTPPW=\"%s\"", ftpPassword);

  // sets the paremeters for the FTP server
  sendATcommand(ftpServerString, "OK", 2000);
  sendATcommand(ftpServerPortString, "OK", 2000);
  sendATcommand("AT+CFTPMODE=1", "OK", 2000);
  sendATcommand(ftpServerUserString, "OK", 2000);
  sendATcommand(ftpServerPassString, "OK", 2000);
    
  // switch to audio dir
  sprintf(aux_str, "AT+FSCD=\"%s\"", targetLocalFileDir);
  sendATcommand2(aux_str, "OK", "ERROR", 2000);

  // send our file
  sprintf(aux_str, "AT+CFTPPUTFILE=\"%s\",\"%s\"", targetLocalFilename, targetLocalFileDir);
  answerrr = sendATcommand(aux_str, "+CFTPPUTFILE: 0", 60000);
  
//  Serial.print(answerrr);
//  if (answerrr == 1){
//    debugPrint("Upload done");
//    debugPrint(targetLocalFilename);
////    sprintf(deleteFileNameConstruct, "AT+DEL=\"%s\"", targetLocalFilename);
////    sendATcommand2(deleteFileNameConstruct, "OK", "ERROR", 2000);   
//  }else{
//    debugPrint("Upload fail");
//    // dont delete our file
//  }
}
