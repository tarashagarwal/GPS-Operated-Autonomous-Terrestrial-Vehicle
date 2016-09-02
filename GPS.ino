#include<LiquidCrystal.h>

int flag=0;
int value=50;
int count=0;
LiquidCrystal lcd(8, 7, 10, 11, 12, 13);
void setup() {
  lcd.begin(16,2);
  // put your setup code here, to run once:
  Serial.begin(4800);


}

String GPSRMC="";
String latitude;
String longitude;
String latDir;
String lonDir;
String velocity;
String checkSum;
int checkSumInt;
String course;
String validity;
boolean isValid;
int i=0;
float distance;
float bearing;
String reached;
int left;
int right;
int maxSpeed;
String turn;
float error;

void loop() {
//Serial.println("Stuck Here 1");

//String GPSRMC="$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68";
  latitude="";
  longitude="";
  lonDir="";
  latDir="";
  velocity="";
  checkSum="";
  course="";
  checkSumInt=0;
  validity="";
  distance=0.0;
  bearing=0.0;
  
  isValid=false;
  reached="N";
  
  turn="S";
  maxSpeed=255;
  left=255;
  right=255;
  error=0.0;
  
  
 // Serial.println("READY TO READ");
  
  //delay(5000);
  
//GPSRMC=getGPSString(GPSRMC);
//Serial.println("Check 1");
 distance= getDistanceFromLatLonInKm(2709.7362,07759.9916,2709.7363,07759.9917);//getting distance from the lab as destination
 bearing=(getBearing(2709.7362,07759.9916,2709.7363,07759.9917)*180)/PI;//getting location from lab as destination
 //Serial.println("Reached");
Serial.println(GPSRMC);
Serial.println(distance);
Serial.println(bearing);


if(isValid==1)
{
  Serial.println("Valid");
  //Serial.println("Valid");
 if(distance<0.015)
 reached="Y";
 else 
 reached="N";
 
 
 roboCode();
//Serial.println(GPSRMC);
//Serial.print(latitude);
//Serial.print(" ");
//Serial.print(longitude);
//Serial.print(" ");
//Serial.print(lonDir);
//Serial.print(" ");
//Serial.print(latDir);
//Serial.print(" ");
//Serial.print(velocity);
//Serial.print(" ");
//Serial.print(checkSum);
//Serial.print(" ");
//Serial.print(course);
//Serial.print(" ");
//
//Serial.print(validity);
//Serial.print(" ");
//Serial.print(checkSumInt,HEX);
//Serial.print(" ");
//Serial.println(isValid);

lcd.clear();
lcd.setCursor(0,0);
lcd.print(velocity);
lcd.print("/");
//lcd.setCursor(5,0);
lcd.print(course);
lcd.print("/");
//lcd.setCursor(11,0);
lcd.print(distance);
//lcd.setCursor(0,1);
//lcd.print(reached);


lcd.setCursor(0,1);
lcd.print(turn);
lcd.print("/");
//lcd.setCursor(2,1);
lcd.print(left);
lcd.print("/");
//lcd.setCursor(7,1);
lcd.print(right);
lcd.print("/");
//lcd.setCursor(11,1);

lcd.print(error);

if(reached.equals("Y"))
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Pahunch Gaye");
lcd.setCursor(0,1);
lcd.print(distance);
Serial.println("Pahucnch Gaye");
}

}
else
{
  //lcd.clear();
  //lcd.setCursor(0,0);
  Serial.println("Not Valid");
 // Serial.print(checkSumInt,HEX);
  //Serial.print(" ");
  //Serial.print(checkSum);
  //Serial.print(" ");
  //Serial.println(isValid);
  
  //lcd.print("Not Valid");
}
delay(100);

}

String getGPSString(String str)
{

  int count=0;
  char data[249];
  char cammando[]="$GPRMC";
  //String str;

  while(1)
  {
    //Serial.println("Check 2");
    //Serial.println(count);
    count=0;
    while(count !=249)
    {
       
      
      if(Serial.available())
      {
        //Serial.print("Stuck Here 2");
        data[count++]=Serial.read(); // put your main code here, to run repeatedly:
       
      }
//if(count/4==0)
//Serial.println("Check 4");

    }
    //Serial.println("Check 3");

//Serial.println("Stuck Here 3");
str=data;
//Serial.println(str);


    //str=(String)data;
    //Serial.println(str);
    //Serial.println(str);

    if(str!="")
    {
      // Serial.println(str);
      for(int i=0;i<249;i++)
      {
        if(str.substring(i,i+6).equals(cammando))
        {
          int h=i;
          String GPSRMC="";
          int count=0;
          while(str.charAt(h)!='*')
          {
            char data=str.charAt(h++);
            GPSRMC+=data;
            //Serial.println(GPSRMC);

               if(data==',')
               count++;
if(data!=',')
{
               switch(count)
               {
                case 1:
                {
                  //can get time here
                }
                break;
                case 2:
                {
                  validity+=data;
                }
                break;
                case 3:
                {
                  latitude+=data;
                }
                break;
                case 4:
                {
                  latDir+=data;
                  
                }
                break;
                case 5:
                {
                  longitude+=data;
                }
                break;
                case 6:
                {
                  lonDir+=data;
                }
                break;
                case 7:
                {
                  velocity+=data;
                }
                break;
                case 8:
                {
                  course+=data;
                }
                break;
                
               }
}

if(data!='$')
checkSumInt^=(int)data;




            
          }
            checkSum=str.substring(h+1,h+3);
          GPSRMC+=str.substring(h,h+3);
         
        

         String checkSumString=String(checkSumInt,HEX);
         if(checkSumString.length()==1)
        checkSumString="0"+checkSumString;
       // Serial.println("Check Here");
        //Serial.println(checkSumString);
       // Serial.println(checkSum);
          if(checkSumString.equals(checkSum) && validity.equals("A"))
          isValid=true;
          else
          isValid=false;
          //Serial.println(isValid);
          return GPSRMC;
        }

      }
    }

  }
}


  float getDistanceFromLatLonInKm(float lat1,float lon1,float lat2,float lon2)
    {
      float R=6371;
      float dLat=deg2rad(lat2-lat1);
      float dLon=deg2rad(lon2-lon1);
      float a=(sin(dLat/2)*sin(dLat/2))+ (cos(deg2rad(lat1))*cos(deg2rad(lat2))*sin(dLon/2)*sin(dLon/2));
      
      float c=2*atan2(sqrt(a),sqrt(1-a));
      float d=R*c;
      return d;
//        float x1, x2, y1,y2, z1,z2;
//        x1=R*cos(lat1)*cos(lon1);
//        y1=R*cos(lat1)*sin(lon1);
//        z1=R*sin(lat1);
//        x2=R*cos(lat2)*cos(lon2);
//        y2=R*cos(lat2)*sin(lon2);
//        z2=R*sin(lat2);
//        float dis =sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
//        Serial.println(dis);
//        return dis;
    }
    
    float getBearing(float lat1,float lon1,float lat2,float lon2)//in radians
    {
      float X=cos(lat2)*sin(lon2-lon1);
      float Y=cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon2-lon1);
      float B=atan2(X,Y);
      return B;
    }

        float deg2rad(float deg)
        {
          return deg*(PI/180);
        }

void roboCode()
{
  Serial.print(distance);
  Serial.print(" ");
   if(distance>15)
    {
  if(bearing<0)
  bearing=360-abs(bearing);
  
error=bearing-course.toFloat();
  
  if(error<0)
  error=180-error;
  
    if(error<=15)
       {
         left=maxSpeed;
         right=maxSpeed;
       }
       else
       
       if((180-error)>=0)
       {
         //Serial.println("turn right");
         turn="R";
         if(error>180)
         error=error-180;
         //turn right
         left=maxSpeed*(1);
         right=maxSpeed*(1-(error/180));
       }
       else
       if((180-error)<0)
       {
         //Serial.println("turn left");
         turn="L";
          if(error>180)
         error=error-180;
         //turn left
         left=maxSpeed*(1-(error/180));
         right=maxSpeed*(1);
       }
    }
   // else
   // {
      
    //F}
    
    Serial.print(right);
    Serial.print(" ");
    Serial.print(left);
    Serial.print(" ");
    Serial.println(turn);
    Serial.print(" ");
}
