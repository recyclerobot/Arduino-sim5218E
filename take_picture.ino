void take_picture(char* targetLocalFilename, char* targetLocalFileDir){

	// Start the camera
    answer = sendATcommand2("AT+CCAMS", "OK", "CAMERA NO SENSOR", 3000);
    if (answer == 1)
    {
        // Sets resolution (Old camera)
        //sendATcommand2("AT+CCAMSETD=640,480", "OK", "ERROR", 2000);

        // Sets resolution (New camera)
        sendATcommand2("AT+CCAMSETD=1600,1200", "OK", "ERROR", 2000);
        
        // Takes a picture, but not saved it
        answer = sendATcommand2("AT+CCAMTP", "OK", "ERROR", 5000);
        delay(1000);
        if (answer == 1)
        {
            // Saves the picture into C:/Picture
            answer = sendATcommand2("AT+CCAMEP", targetLocalFileDir, "ERROR", 2000);
            
            if (answer == 1)
            {
                counter = 0;
                while(Serial.available()==0);
                do{
                    targetLocalFilename[counter] = Serial.read();
                    counter++;
                    while(Serial.available()==0);
                }while (targetLocalFilename[counter-1] != 0x0D);
                
                targetLocalFilename[counter] = '\0';
                
                Serial.print("Picture name: ");                
                Serial.println(targetLocalFilename);
                
                // stop camera
                sendATcommand2("AT+CCAME", "OK", "", 2000);

                // upload to FTP
                upload_to_ftp(targetLocalFilename, targetLocalFileDir);
                
            }
            else
            {
                Serial.println("Error saving the picture");    
            }  
        }
        else if (answer == 2)
        {
            Serial.println("Camera invalid state");    
        }
        else
        {
            Serial.println("Error taking the picture");    
        }
    }
    else if (answer == 2)
    {
        Serial.println("Camera not detected");    
    }
    else
    {
        Serial.println("Error starting the camera");    
    }
}