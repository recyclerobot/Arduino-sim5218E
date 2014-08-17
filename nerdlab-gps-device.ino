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

int8_t answer;
int onModulePin= 2;
char gps_data[100];
int counter;

int aux;
int data_size = 0;
int end_file = 0;
char aux_str[250];
char data[250];
int x = 0;
long previous;
char url[ ]="renaa.rs";
int port= 80;
char request[250];

void setup(){

    pinMode(onModulePin, OUTPUT);
    Serial.begin(115200);    

    Serial.println("Starting...");
    power_on();

    delay(5000);
    
    // sets the PIN code
    sendATcommand("AT+CPIN=1111", "OK", 2000);
    
    delay(3000);

    Serial.println("Connecting to the network...");

    while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
            sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

    sendATcommand("AT+CGSOCKCONT=1,\"IP\",\"web.be\"","OK",1000); 
    // if not authentication requiered comment or remove the next line
    sendATcommand("AT+CSOCKAUTH=1,1,,\"web\",\"web\"","OK",1000);
    sendATcommand("AT+CGPSURL=\"supl.google.com:7276\"","OK",1000);    // sets GPS server
    sendATcommand("AT+CGPSSSL=0","OK",1000);    // without certificate
    
    answer = sendATcommand("AT+CGPS=1,2","OK",1000);    // starts GPS session in MS-based mode
    if (answer == 0)
    {
        Serial.println("Error starting the GPS");
        Serial.println("The code stucks here!!");
        while(1);
    }
}
void loop(){

    answer = sendATcommand("AT+CGPSINFO","+CGPSINFO:",1000);    // request info from GPS
    if (answer == 1)
    {
        counter = 0;
        do{
            while(Serial.available() == 0);
            gps_data[counter] = Serial.read();
            counter++;
        }
        while(gps_data[counter - 1] != '\r');
        gps_data[counter] = '\0';
        if(gps_data[0] == ',')
        {
             Serial.println("No GPS data available");  
        }
        else
        {
            Serial.print("GPS data:");
            Serial.println(gps_data);  
            Serial.println("");
            
            push_to_api();
    
        }       

    }
    else
    {
        Serial.println("Error"); 
    }

    delay(5000);
}

void push_to_api(){
  // request the url
    sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
    answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 60000);

    if (answer == 1){
        sprintf(request, "GET /echo/?g=%s HTTP/1.1\r\nHost: www.renaa.rs\r\n\r\n", gps_data);
        
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
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer2) != NULL)    
            {
                answer = 2;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}
