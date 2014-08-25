void upload_to_ftp(char* targetLocalFilename, char* targetLocalFileDir){
	char ftpServerString[250];
	char ftpServerPortString[250];
	char ftpServerUserString[250];
	char ftpServerPassString[250];

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
    sprintf(aux_str, "AT+CFTPPUTFILE=\"%s\",\"%s\"", audio_clip_name, ftpDir);
    answer = sendATcommand(aux_str, "+CFTPPUTFILE: 0", 60000);
    
    if (answer == 1)
    {        
        debugPrint("Upload done"); 
        // delete our local file
        sprintf(targetLocalFilename, "AT+DEL=\"%s\"", audio_clip_name);
    	sendATcommand2(targetLocalFilename, "OK", "ERROR", 2000);   
    }
    else
    {
        debugPrint("Upload fail");    
        // dont delete our file
        // #TODO: implement disk size left to prevent overflow
    }
    
}