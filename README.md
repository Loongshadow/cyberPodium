# cyberPodium
Online classroom system support: students Android apps send messages to teacher, podium PC show these barrages.

## Conponents
![project overview](https://github.com/user-attachments/assets/329873ec-dea2-4eeb-a856-b9277367c584)

### Server
framework: LNMP(Linux-Nginx-MySQL-PHP)  
support:  
1. Maintain the database about courses,student,teacher,online status...
2. Implement the APIs using http.
3. Keep the websocket connections between students and corresponding teacher.

### Teacher Client
language: C++ QT
support:  
course mangement: edit information of the course, edit attendance records of the course
courses list management:  create or drop courses
classing: manage and show the barrages, show the latest attendance records, toolbox just support select student randomly

### Student Client
language: Android Kotlin  
support:  
courses list management:  add courses and drop courses, see oneself attendance records
classing: see oneself latest attendance records, check in by GPS, send barrage to teacher

