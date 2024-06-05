# Stargazer
Just for fun .

most people are too buys in this world and they dont see up the sky , ignore the true reality . 
i felt the same and wanna make something to solve the problem i faced . 
as a beginner in stargazing its very hard to do it . 
there is no cheap and quick think which can be infomative and fun . 
sterallium and night sky are great , but they are not enough . 

The stargazer is device which uses a green laser to point to any celestial object selected by the user. you just need to install stellarium and select an object . the device will automatically point the lazer to it . dosent matter what your location , time , device orientation is .             (credits-jjrobots) 
Its small , cheap and can unlock the universe for you .

An automatic stargazing device is a tool that helps you easily find and track stars and other celestial objects. It uses motors to move a laser pointer or telescope to the exact position of a star based on your location and the current time. Sensors on the device ensure accurate pointing and tracking, so you can enjoy a clear view of the night sky without manually searching for objects.

i will try to post the updates (as files) . 


@ components -
arduino uno
stepper motor & drivers *3
bmp module 
mpu9250 
lazer pointer 

@ working -

   Input Location:
You start by inputting your location's latitude, longitude, and altitude into the system. This information helps the device understand where you are on Earth.
   
   Calculate Star Position:
The device uses astronomical algorithms to calculate the current position of the star you want to observe. It considers the date, time, and your location to determine the star's azimuth (horizontal angle) and altitude (vertical angle) in the sky.

  Sensor Data:
The stargazer uses an MPU9250 sensor, which has a magnetometer, gyroscope, and accelerometer. These sensors help determine the device's orientation (roll, pitch, and yaw) in real-time.

  Motor Control:
The device has two stepper motors:
One motor adjusts the azimuth (horizontal angle).
The other motor adjusts the altitude (vertical angle).
The motors move the laser pointer or telescope until the sensor readings match the calculated azimuth and altitude for the target star.
(using 3rd motor for z axis will also be great , but its not mandatory) . 

  Automatic Alignment:
The device uses the sensor data to continuously adjust the motors and keep the pointer aligned with the star. This ensures that the star remains in view, even if there are slight movements or changes.



check out this youtube website related to the project 
https://youtu.be/4PSazYw4XaM?si=X3MrnupO3QnW7joi
iit guy vedio link . 
ardunio project website link.




![image](https://github.com/ujwalsahu123/Stargazer-ideation-and-prototype-updates/assets/132041436/5e32b682-6da7-4bfc-b587-126a9c94a41d)






archives :
 
Hi guys , i am ujwal sahu , 21 yrs old , studying bachelors in AI form india   .
i am a beginner in stargazing and i was also making the same project as the startracker (the laser one) that you guys are making . 

I red the ( new version of startracker page) blog and its nice .
I think that the vision is exciting and creating a device which connect the space with a beginner with zero friction and affordability is great , though i think you guys are not headed in the right direction . 

lets talk from the bottom up - There are many many people who want to know about the space and not just fiction movies but really want to know about the reality of this world and who we are (not just the daily life we live ) . people forget that we live on a circular planet and there are millions of those and many other exciting things beyond the sky .  mostly every kid wants to become a astronaut , and they are super excited to explore this mysterious world but most dreams die because there is no one to tell them about these things . i myself when wanted to explore the sky , do stargazing , know about the stars above me - it was very difficult , a telescope is very expensive to buy and then also you have to learn about all the stargazing things . i had a green laser so i used to point to the stars and it made me feel like i have the power to touch the stars using the laser . i explored more and got to know about stellarium app and it was really helpful for me to do stargazing and i pointed my laser to the star using that and then i thought to make it automatic => so a device which can directly point to the star that are above me right now and also give me informoation about that and  hence now i am making that device . 
(tip - for sensor using mpu9250 is better , and all 3 axis matters ( yaw ,pitch ,roll) ).

Now if we take a step back and look then this device sucks ( a laser , are you serious ! , it can be illegal ) . and thats why you guys pivoted and made the starpointer form a laser pointing device to a mount for astrophotography  and thats why i said that you guys were not headed in the right direction . 

I think there is a real gap between the consumer who wants to do stargazing and unlock the universe .  
i know stellarium solves this problem , but still i dont think it connects people and excite them . it doesnt  provide much information and its not frictionless , etc . 
I dont know what to make which can solve this gap , which will be a great product . ( making a goto mount with cheap telescope and a better ai guide app which can walk you though the sky thats above you right now and tell facts and exciting thing about the objects and in the same time pointing the telescope to that object .   like a real human stargazing guide ) .  

I want to deep dive in this problem and i want to make something great .
i want to work with you guys .

please share your thoughts on this . 
Thank you .


