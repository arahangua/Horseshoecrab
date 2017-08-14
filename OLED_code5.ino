#include <Wire.h>  // Include rWire if you're using I2C
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library



//below is the given standard declaration format for Microoled control.
#define PIN_RESET 9  // Connect RST to pin 9
#define PIN_DC    8  // Connect DC to pin 8
#define PIN_CS    10 // Connect CS to pin 10
#define DC_JUMPER 0



const char OUTPUT_PIN=3; //Pin 3 will going to send out impulse signal. By putting delay right after sending-out function it introduces square pulse. Square pulse is fed into opal kelly board.
const char number_of_stim=20; //When declaring stimulation groups, maximum 20 inputs can be stored.
const char num_of_seq=10; // maximum length of unique sequence.


#define x_width 64 //defining x axis width of the OLED
#define y_width 48 //defining y axis width of the OLED

char seq_input[num_of_seq]; //This is for parsing sequence input it stores each digit in "len_seq" var.
char len_seq; // This directly stores sequence input.
char group_num; // How many groups are there
char command; // variable to store input during the loop
char x,y; // x,y variable to store x,y coordinate
bool x_entered; //flag variable for x input
char iter; //total iterations of the whole sequence
char inputs; // number of inputs
char c,d; //variables for "seq_input" calculations
char bb; // just for loop control variable
char a[num_of_seq][number_of_stim]; 
char b[num_of_seq][number_of_stim];// a,b are arrays that stores group-inputs information
char interval; //interval between light stimulation
char pulse_width; //length of the square pulse
char count; //variable for "seq_input" calculation
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // SPI declaration




// Below function just clears the oled.
void clearDisplay()
{
  oled.clear(ALL);
  oled.display();
  oled.clear(PAGE);
}


// setting up OLED device.
void setup() {
  // put your setup code here, to run once:
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory 
  oled.display();
  oled.clear(PAGE); 
  Serial.begin(115200); 
  Serial.println("Waiting for Python input, a- light all and each LEDs, c - light up the column, r - light up the row, s - customized LED input.");
}


//Below is the loop
void loop() {
    if(Serial.available())
    {
    //serial will be receiving one of 'a', 'c', 'r', 's' inputs. 
      command = Serial.read();
      
      
      
      if(command=='a')
      // Below will be go through all the pixels.
      {   
       Serial.print("\nlighting up all");
       for(y=1; y<=y_width; y++)
       {
        for(x=1; x<=x_width; x++)
        {
          oled.pixel(x,y);
          oled.display();
          digitalWrite(OUTPUT_PIN,HIGH);
          delay(5); //giving delay to generate nicely shaped square pulse.
          digitalWrite(OUTPUT_PIN,LOW);
          //delay(0.5);
         clearDisplay();
        }
       }
      }
      
      else if(command=='c')
      //Will go through all the columns
      {
        Serial.print("\nlighting up columns");
    // 
        for(x=1; x<=x_width; x++)
        {
          oled.line(x, 1, x, y_width);
          oled.display();
          digitalWrite(OUTPUT_PIN,HIGH);
          delay(5); 
          digitalWrite(OUTPUT_PIN,LOW);
          clearDisplay();
        }
      }
      else if(command=='r')
      //will go through all the rows
      {
         Serial.print("\nlighting up rows");
         
        for(y=1; y<=y_width; y++)
        {
          oled.line(1,y,x_width,y);
          oled.display();
          digitalWrite(OUTPUT_PIN,HIGH);
          delay(5);
          digitalWrite(OUTPUT_PIN,LOW);
          clearDisplay();
        }
      }
      else if(command=='s')
      //custom inputs
      {
        // flushing the serial
       while(Serial.available()) //checking if there is anything to read.
       {  
       char getData = Serial.read(); // just reads-in everything so that nothing is left in the buffer.
        }

       Serial.println("\ntype in total number of groups");
       while(!Serial.available()){}
       group_num=Serial.parseInt();
       Serial.println(group_num);
       


       Serial.println("\ntype in the sequence of cycle with group numbers ex) 1324:");
       while(!Serial.available()){}
       len_seq=Serial.parseInt();
       Serial.println(len_seq);
       
       //Below lines are for parsing len_seq into digits.
       count=0;
       c=len_seq;
       while(c>0)
       {
        d=c/10 ;
        seq_input[count] = c-(10*d)-1 ;
        c=d ;
        count++ ; // stores sequence of input in reverse order to the seq_input array
       }


       Serial.println("\nNow declaring groups, start by declaring the first group");
       
       for(bb=0;bb<group_num;bb++)
       {
     

       Serial.println("\ntype in total number of inputs:");
       while(!Serial.available()){}
       inputs=Serial.parseInt();
       Serial.println(inputs);

       //declaring inputs for each group
       for(int k=0;k<inputs;k++)
       {
        
        // flushing the serial
       while(Serial.available()) //checking if there is anything to read.
       {  
       char getData = Serial.read(); // just reads in everything so that nothing is left in the buffer.
        }   
       Serial.println("\nEnter x,y coordinate(two digit number for each)");
       Serial.println("\nx: ");
       while(!Serial.available()){} //This just introduces waiting time for the user to make input.
       a[bb][k] = Serial.parseInt(); //Takes in the input as integer
       Serial.println(a[bb][k]);
       x_entered=true; //make a flag that x coordinate already has a value.
       
      Serial.println("y: ");
      while(Serial.available() == 0){} //Again, same this line gives waiting time for the user.
      if (Serial.available() and x_entered) 
  {
    while(!Serial.available()){}
    b[bb][k] = Serial.parseInt();
    Serial.println(b[bb][k]);
    x_entered = false; //Turn the flag variable back to false.
  }
    Serial.println("Added point");
   // oled.pixel(a[bb][k],b[bb][k]); //Add point in the oled array.
  }
  Serial.println("breaking input");
       }

       Serial.println("\nfinished declaring groups");

       Serial.println("\ntype in a number for iteration:");
       while(!Serial.available()){}
       iter=Serial.parseInt();
       Serial.println(iter);


       Serial.println("\ntype in delay(milisecond) between blinks:");
       while(!Serial.available()){}
       interval=Serial.parseInt();
       Serial.println(interval);

       Serial.println("\ntype in pulse width length in milisecond:");
       while(!Serial.available()){}
       pulse_width=Serial.parseInt();
       Serial.println(pulse_width);
       
Serial.println("\now lighting the LEDs up");
  //Below lines light up the LED   
  for(int j=1;j<=iter;j++)
  {
    for(bb=count-1;bb>=0;bb--)
    {
    for(int k=0;k<inputs;k++) //This loop assigns pixels to the buffer
    {
      oled.pixel(a[seq_input[bb]][k],b[seq_input[bb]][k]);
    }
      oled.display(); //light up the pixels in buffer
      digitalWrite(OUTPUT_PIN,HIGH); //sends out signal
      delay(pulse_width);
      digitalWrite(OUTPUT_PIN,LOW); //return it back to normal(default is low)
      oled.clear(PAGE); 
      oled.clear(ALL);//erasing lit up pixels
      delay(interval); //just introducing delay for testing purpose.
    }
  }
  oled.clear(ALL);
  //again flushing out all the inputs in the buffer.
       while(Serial.available())
       {  //is there anything to read?
       char getData = Serial.read(); 
       }      
      }   
      }
clearDisplay(); //clears display after each iteration in the loop.

   }

   
  

