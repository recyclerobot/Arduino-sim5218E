<<<<<<< HEAD:nerdlab-gps-device.ino
// SCHEMA
// SETUP > LOOP > GET GPS > PUSH TO API > RECORD AUDIO > UPLOAD TO FTP > TAKE PICTURE > UPLOAD TO FTP > OK

/* PIN VARIABLES */
int onModulePin = 2;

/* DEBUG OMODE */
boolean debugMode = true;

/* SERVER API VARIALBLES */
char url[]         = "renaa.rs";
char host[]        = "www.renaa.rs";
char apiEndpoint[] = "/echo/track.php?g=";
int port           = 80;
char ftpServer[]   = "ftp.endlessecho.recyclerobot.com";
char ftpUser[]     = "endlessecho";
char ftpPassword[] = "charliebravotango";
char ftpDir[]      = "/endless_audio";
int ftpPort        = 21;

/* GLOBAL VARIABLES */
int    counter;
=======
/*  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 *  
 *  Author: Alejandro Gallego (http://www.libelium.com)
 *  Author: Thijs Bernolet (http://www.weworkweplay.com)
 *
 */

>>>>>>> FETCH_HEAD:nerdlab_gps_device.ino
int8_t answer;
int    aux;
int    data_size = 0;
int    end_file = 0;
int    x = 0;
long   previous;

/* STORE VARIABLES */
char gps_data[100];
char aux_str[250];
char data[250];
char request[250];

/* AUDIO VARS */
char audio_clip_dir[] = "C:/Audio";
char audio_clip_name[20];

/* PICTURE VARS */
char picture_dir[] = "C:/Picture/";
char picture_name[20];

/* SETUP AND INIT */
void setup(){

    pinMode(onModulePin, OUTPUT);
    Serial.begin(115200);    

    debugPrint("Starting...");

    // power our SIM5128
    power_on();

    delay(5000);
    
    // sets the PIN code
    sendATcommand("AT+CPIN=1111", "OK", 2000);
    
    delay(3000);

    // connect to the mobile network
    debugPrint("Connecting to the network...");

    while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
            sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

    sendATcommand("AT+CGSOCKCONT=1,\"IP\",\"web.be\"","OK",1000); 
    sendATcommand("AT+CSOCKAUTH=1,1,,\"web\",\"web\"","OK",1000);

    // set GPS server without certificate
    sendATcommand("AT+CGPSURL=\"supl.google.com:7276\"","OK",1000);
    sendATcommand("AT+CGPSSSL=0","OK",1000);
    
    // start gps node in GPS-MS
    answer = sendATcommand("AT+CGPS=1,2","OK",1000);
    if (answer == 0){
        debugPrint("Error starting the GPS");
        debugPrint("The code stucks here!!");
        while(1);
    }
}

/* MAIN LOOP */
void loop(){

    // get gps location
    answer = sendATcommand("AT+CGPSINFO","+CGPSINFO:",1000);
    
    if (answer == 1){
        counter = 0;
        do{
            while(Serial.available() == 0);
            gps_data[counter] = Serial.read();
            counter++;
        }while(gps_data[counter - 1] != '\r');
        
<<<<<<< HEAD:nerdlab-gps-device.ino
        gps_data[counter] = '\0';
        if(gps_data[0] == ','){
             debugPrint("No GPS data available");  
        }else{
            debugPrint("GPS data:");
            debugPrint(gps_data);
=======
        Serial.println(request);
        // Sends <Ctrl+Z>
        aux_str[0] = 0x1A;
        aux_str[1] = 0x00;
        answer = sendATcommand(aux_str, "+CHTTPACT: DATA,", 60000);

        x=0;
        do{
            if (answer == 1)
            {
                data_size = 0;
                while(Serial.available()==0);
                aux = Serial.read();
                do{
                    data_size *= 10;
                    data_size += (aux-0x30);
                    while(Serial.available()==0);
                    aux = Serial.read();        
                }
                while(aux != 0x0D);

                Serial.print("Data received: ");
                Serial.println(data_size);
                
            }
            else
            {
                Serial.println("Error getting the url");
                data_size = 0;
            }
            
        }while (answer != 1);

    }
    else
    {
        Serial.println("Error waiting the request");    
    }

    delay(10000);
}

void power_on(){

    uint8_t answer=0;

    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
        delay(3000);
        digitalWrite(onModulePin,LOW);

        // waits for an answer from the module
        while(answer == 0){    
            // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }

}


int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{

        if(Serial.available() != 0){    
            response[x] = Serial.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}

int8_t sendATcommand2(char* ATcommand, char* expected_answer1,
         char* expected_answer2, unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 
>>>>>>> FETCH_HEAD:nerdlab_gps_device.ino

            // push location to our api
            push_to_api(gps_data);

            // record audio and FTP to our server
            record_audio(audio_clip_name, audio_clip_dir);

            // take picture and upload to our server
            take_picture(picture_name, picture_dir);

            debugPrint("All Good!");
        }       
    }else{
      debugPrint("Error"); 
    }

<<<<<<< HEAD:nerdlab-gps-device.ino
    debugPrint("Loop finished!");
    delay(5000);
}
=======
    return answer;
}

>>>>>>> FETCH_HEAD:nerdlab_gps_device.ino
