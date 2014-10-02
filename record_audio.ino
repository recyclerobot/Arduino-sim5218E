void record_audio(){
  
    char targetLocalFilename[250];
    char targetLocalFileDir[250];

    // start record
    answer = sendATcommand2("AT+CQCPREC=0,amr", "C:/Audio/", "ERROR", 5000);
    
    if (answer == 1){
        counter = 0;
        while(Serial.available()==0);
        do{
            targetLocalFilename[counter] = Serial.read();
            counter++;
            while(Serial.available()==0);
        }
        while (targetLocalFilename[counter-1] != 0x0D);

        targetLocalFilename[counter-1] = '\0';

        debugPrint("Clip name: ");                
        debugPrint(targetLocalFilename);

        // record clip for 10 seconds
        delay(10000);

        // stop record          
        sendATcommand2("AT+CQCPSTOP", "OK", "ERROR", 2000);

        // push audio and location to our api
        upload_to_ftp(targetLocalFilename, targetLocalFileDir);
    }
    else{
        debugPrint("Error recording");    
    }
}

void play_audio(char* targetLocalFilename, char* targetLocalFileDir){

    sprintf(aux_str, "AT+FSCD=\"%s\"", targetLocalFileDir);
    sendATcommand2(aux_str, "OK", "ERROR", 2000);
    
    sprintf(aux_str, "AT+CCMXPLAY=\"%s\",0", targetLocalFilename);
    answer = sendATcommand2(aux_str, "OK", "ERROR", 2000);
    if (answer == 1)
    {            
        delay(5000);
        sendATcommand2("AT+CCMXPAUSE", "OK", "ERROR", 2000);
        delay(5000);            
        sendATcommand2("AT+CCMXRESUME", "OK", "ERROR", 2000);
        delay(5000);            
        sendATcommand2("AT+CCMXSTOP", "OK", "ERROR", 2000);
    }
    else
    {
        debugPrint("Error playing");    
    }
}
