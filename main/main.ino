/*
   TIVA_HTTPD
   v1.0

   Copyright (c) 2016 Manuel Dionne and Philip Tang

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>
#include <Wire.h>

#include "FillPat.h"
#include "LaunchPad.h"
#include "OrbitBoosterPackDefs.h"
#include "OrbitOled.h"
#include "OrbitOledGrph.h"
#include "OrbitOledChar.h"

#include "file_system.h"

// Constant declarations
#define LOG_BUFF_LEN 16
#define LOG_MAX_LINES 4
#define PROTOCOL "HTTP/1.0"
#define SERVER_NAME "TIVA"
#define SERVER_URL "www.tiva.com"

// Global variable declarations
char orbitLog[LOG_MAX_LINES][LOG_BUFF_LEN] = {{}, {}, {}, {}};
int args = 0;
int error = 0;
int htmlLength = 0;
String connection;
String title;

// Hard coded main page to upload files to server
// This cant be overwritten by the user because its in flash
const String uploadHTML = "<!DOCTYPE html><head><title>TIVA - HOME</title><link rel=\"stylesheet\" href=\"https://dl.dropboxusercontent.com/u/27517534/tiva%20project/Home%20Page/style.css\"><link rel=\"stylesheet\" href=\"https://dl.dropboxusercontent.com/u/27517534/tiva%20project/Home%20Page/dropzone.css\"></head><body class=\"home\"><header class=\"header\"><a class=\"menu-btn\"><span class=\"line\"></span><span class=\"line\"></span><span class=\"line\"></span></a><div style=\"width: 550px; margin: 0 auto 0 auto;\"><center class=\"header-title\" style=\"background-color: rgba(0, 0, 0, 0.15);\">TIVA Project</center></div><div class=\"socials\"><a class=\"social twitter\"><svg version=\"1.1\"></svg></a><a class=\"social instagram\"><svg version=\"1.1\" </svg></a></div></header><nav class=\"menu\"><div><a style=\"font-size: 50px; font-family: 'Open Sans', serif;\">Waterloo</a></div><div class=\"menu-info\" style=\"padding-top: 10px;\"><p style=\"font-family: 'Open Sans',sans-serif;\">This our final first term project for <a href=\"https://uwaterloo.ca/software-engineering\">Software Engineering</a>. Browse around and enjoy!</p></div><div class=\"menu-info\"><p style=\"font-family: 'Open Sans',sans-serif;\">Created by <a href=\"https://twitter.com/AideTechBot\">Manuel Dionne</a> and <a href=\"https://twitter.com/philipwutang\">Philip Tang</a>.</p></div></nav><nav class=\"profile\"><div><a style=\"font-size: 50px; font-family: 'Open Sans', serif;\">Manuel Dionne</a></div><div class=\"profile-info\" style=\"padding-bottom: 10px;\"><p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas maximus magna sit amet risus venenatis mattis. Lorem ipsum dolor sit amet, consectetur adipiscing elit.</p></div></nav><nav class=\"profile2\"><div><a style=\"font-size: 50px; font-family: 'Open Sans', serif;\">Philip Tang</a></div><div class=\"profile-info \" style=\"padding-bottom: 10px;\"><p>Hey there! I'm an 18 year old designer and developer from Toronto. I love<a href=\"https://open.spotify.com/user/philipwutang\">music</a>,<a href=\"http://vsco.co/philiptang/images\">photography</a> and I spend my time working on<a href=\"https://open.spotify.com/user/philipwutang\">side projects</a>.</p><p style=\"font-family: 'Open Sans',sans-serif; font-size: 1.3rem;\">You can try one of these places to find me on the internet:<a href=\"https://twitter.com/philipwutang\">Twitter</a>,<a href=\"https://www.instagram.com/philipwutang\">Instagram</a>, and<a href=\"https://medium.com/@philipwutang\">Medium</a>. Or shoot me an<a target=\"_blank\" href=\"mailto:philipwtang@uwaterloo.ca\">Email</a>.</p><p style=\"font-family: 'Open Sans',sans-serif; font-size: 1.3rem; color: white\">Cheers!</p></div></nav><main class=\"content home\"><article class=\"post\"><a class=\"profile-btn\"><figure><img class=\"post-image\" src=\"https://dl.dropboxusercontent.com/u/27517534/Class%20Photos/13735117_995525227232469_5234807750188189660_o.jpg\"></figure></a><div class=\"post-info\"><h1 class=\"post-title\">Manuel Dionne</h1><h2 class=\"post-subtitle\"><a class>Fullstack Developer</a></h2></div></article><article class=\"post\"><a class=\"philipbtn\"><figure><img class=\"post-image\" src=\"https://scontent-yyz1-1.cdninstagram.com/t51.2885-19/s320x320/14709523_1174591785932081_7071560509224386560_a.jpg\"></figure></a><div class=\"post-info\"><h1 class=\"post-title\">Philip Tang</h1><h2 class=\"post-subtitle\"><a class>Frontend Developer</a></h2></div></article><div style=\"width: 500px; margin: 0 auto 0 auto;\"><form action=\"upload\" method=\"post\" enctype=\"multipart/form-data\">Select a file:<input type=\"file\" name=\"fileToUpload\" id=\"fileToUpload\"><input type=\"submit\" value=\"Upload\" name=\"submit\"></form></div></main><footer class=\"footer\"><nav class=\"pagination\"><div style=\"margin: 0 auto 0 auto;\"><center class=\"bottomText\"><a href=\"directory.html\">Browse</a></center></div></nav></footer><A NAME=\"codeword \"><script src=\"https://dl.dropboxusercontent.com/u/27517534/tiva%20project/Home%20Page/script.js\"></script><script src=\"https://dl.dropboxusercontent.com/u/27517534/tiva%20project/Home%20Page/dropzone.js\"></script></body></html>";

/* HELPER FUNCTIONS */
char *deblank(char *input) {
  char *output = input;
  int i, j;
  for (i = 0, j = 0; i < strlen(input); i++, j++) {
    if (input[i] != ' ') output[j] = input[i];
    else j--;
  }
  output[j] = 0;
  return output;
}

/* Main Boosterpack Functions */

// Sets the display text to string on line
void OrbitSetLine(char string[], int line) {
  OrbitOledSetCursor(0, line);
  OrbitOledPutString(string);
}

// Pushes to log on OLED display
void pushToLog(char string[16]) {
  for (int i = 3; i > 0; i--) memcpy(orbitLog[i], orbitLog[i - 1], sizeof(orbitLog[i]));
  memcpy(orbitLog[0], string, sizeof(orbitLog[0]));

  OrbitOledClear();

  for (int i = 0; i < LOG_MAX_LINES; i++) OrbitSetLine(orbitLog[i], i);
}

// Sends files or the 404 error depending on whether or not files are in the directory
void sendFile(String path) {
  if (path.compareTo("/index.html") == 0) {
    pushToLog("sending index");
    sendHeaders(200, "OK", uploadHTML.length(), connection, "-1");
    Serial.print(uploadHTML);
  }

  // Updates directory webpage with links to new files uploaded
  if (path.compareTo("/directory.html") == 0) {
    pushToLog("sending dir");

    String firsthtml = "<!DOCTYPE html><head><title>TIVA - BROWSE</title><link rel=\"stylesheet\" href=\"https://dl.dropboxusercontent.com/u/27517534/tiva%20project/Home%20Page/style.css\"></head><body class=\"home\"><header class=\"header\"><a class=\"menu-btn\"><span class=\"line\"></span><span class=\"line\"></span><span class=\"line\"></span></a><h1 class=\"header-title\"><center>TIVA Project</center></h1><div class=\"socials\"><a class=\"social twitter\"><svg version=\"1.1\"></svg></a><a class=\"social instagram\"><svg version=\"1.1\" </svg></a></div></header><nav class=\"menu\"><div><a style=\"font-size: 50px; font-family: 'Open Sans', serif;\">Waterloo</a></div><div class=\"menu-info \" style=\"padding-top: 10px;\"><p style=\"font-family: 'Open Sans',sans-serif;\">This our final first term project for <a href=\"https://uwaterloo.ca/software-engineering\">Software Engineering</a>. Browse around and enjoy!</p></div><div class=\"menu-info\"><p style=\"font-family: 'Open Sans',sans-serif;\">Created by <a href=\"https://twitter.com/AideTechBot\">Manuel Dionne</a> and <a href=\"https://twitter.com/philipwutang\">Philip Tang</a>.</p></div></nav><main class=\"content home\">";
    String middle = "";

    for (int i = 0; i < totalFiles; i++) {
      middle = middle + "<div style=\"margin: 0 auto 0 auto;\"><center class=\"bottomText\"><a href=\"" + FILE_SYSTEM[i].name + "\" style=\"font-size:50px;\">" + FILE_SYSTEM[i].name + "</a></center></div>";
    }

    String endhtml = "</main><footer class=\"footer\"><nav class=\"pagination\"><div style=\"margin: 0 auto 0 auto;\"><center class=\"bottomText\"><a href=\"index.html \">Home</a></center></div></nav></footer><A NAME=\"codeword\"><script src=\"https://dl.dropboxusercontent.com/u/27517534/tiva%20project/Home%20Page/script.js\"></script></body></html>";
    String upload = firsthtml + middle + endhtml;

    sendHeaders(200, "OK", upload.length(), connection, "-1");
    Serial.print(firsthtml + middle + endhtml);
  }
  else {
    int found = -1;
    String name = path.substring(1, path.length());
    for (int i = 0; i < 128; i++) {
      if (String(FILE_SYSTEM[i].name).compareTo(name) == 0) {
        sendHeaders(200, "OK", (int)FILE_SYSTEM[i].len, connection, "-1");
        // SEND FILE THE USER WANTED
        found = i;
        break;
      }
    }
    if (found >= 0) {
      Serial.print(read_File(name));
    }
    else checkErrors(4);
  }
}

// Sends the headers to the HTTP response
void sendHeaders(int error, String title, int htmlLength, String connection, String location) {
  Serial.print("HTTP/1.0 ");
  Serial.println(error);
  Serial.print(title);

  Serial.print("\r\n");
  Serial.println("Server: micro_httpd");
  Serial.println("text/html; charset=UTF-8");
  Serial.print("Content-Length: ");
  Serial.println(htmlLength);
  Serial.println("Last-Modified: Sun, 4 Nov 2016 16:45:54 EST");
  Serial.print("Connection: ");
  Serial.println(connection);

  if (location != "-1") {
    Serial.print("Location: ");
    Serial.println(location);
  }
  Serial.print("\r\n");
}

// Sends an error page that is default to the webserver
void sendHTMLerror(int error, String title, String text, String location) {
  String output = "<!DOCTYPE html><html lang=\"en\"><head><title>" + title + "</title><link rel=\"stylesheet\" href=\"http://isinvisible.com/dist/css/main.min.css\"><link rel=\"stylesheet\" href=\"//fonts.googleapis.com/css?family=Open+Sans:300,400,600,700&amp;lang=en\" /></head><body class=\"home\"><header class=\"header\"><a class=\"menu-btn\"><span class=\"line\"></span><span class=\"line\"></span><span class=\"line\"></span></a><h1 class=\"header-title\"><center style=\"font-size: 100px; font-family: 'Open Sans', serif;\">SE'XII </center></h1><div class=\"socials\"><a class=\"social twitter\"><svg version=\"1.1\"></svg></a><a class=\"social instagram\"><svg version=\"1.1\" </svg></a></div></header><nav class=\"menu\"><div><a style=\"font-size: 50px; font-family: 'Open Sans', serif;\">Waterloo</a></div><div class=\"menu-info\" style=\"padding-top: 10px;\"><p style=\"font-family: 'Open Sans',sans-serif;\">A profile page for the University of Waterloo's <a href=\"https://uwaterloo.ca/software-engineering/\">Software Engineering</a> Class of 2021. Get in touch with us!</p></div><div class=\"menu-info\"><p style=\"font-family: 'Open Sans',sans-serif;\">Created by <a href=\"https://twitter.com/AideTechBot\">Manuel Dionne</a> and <a href=\"https://twitter.com/philipwutang\">Philip Tang</a>.</p></div></nav><footer class=\"footer\"><div><a style=\"font-size: 300px; font-family: 'Open Sans',sans-serif; color: #FFFFFF;\">" + String(error) + "</a><center style=\"font-size: 20px; font-family: 'Open Sans',sans-serif; color: #FFFFFF\">" + text + "</center></div></footer><script src=\"http://isinvisible.com/dist/js/main.js\"></script></body></html>";
  htmlLength = output.length();

  sendHeaders(error, title, htmlLength, connection, location);

  Serial.println();
  Serial.println(output);
}

// Catches errors and updates the error page
void checkErrors(int args) {
  String text = " ";

  if (args > 0) {
    switch (args) {
      case 1:
        error = 400;
        title = "Bad Request";
        pushToLog("Bad Request");
        text = "Whoa there! That's was a bad request!";
        break;
      case 2:
        error = 401;
        title = "Unauthorized";
        pushToLog("Unauthorized");
        text = "What are you doing! That's unauthorized!";
        break;
      case 3:
        error = 403;
        title = "Forbidden";
        pushToLog("Forbidden");
        text = "Sorry. This page is forbidden.";
        break;
      case 4:
        error = 404;
        title = "Not Found";
        pushToLog("Not Found");
        text = "Whoops! The page you are looking for can not be found.";
        break;
      case 5:
        error = 500;
        title = "Internal Server Error";
        pushToLog("Int. Ser. Error");
        text = "Oh no! Internal server error.";
        break;
      case 6:
        error = 501;
        title = "Not Implemented";
        pushToLog("Not Implemented");
        text = "Guess we did not implement this page yet.";
        break;
      case 7:
        error = 502;
        title = "Bad Gateway";
        pushToLog("Bad Gateway");
        text = "Bad gateway. Sorry.";
        break;
      case 8:
        error = 503;
        title = "Service Unavaliable";
        pushToLog("Ser. Unavaliable");
        text = "Our services are unavaliable";
        break;
      default:
        break;
    }

    sendHTMLerror(error, title, text, "-1");
  }
}

// Main function
void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setModule(0);

  // Initialize all peripherals used and enables the I2C EEPROM communication
  OrbitOledInit();

  //initialize boosterpack LEDs
  GPIOPinTypeGPIOOutput(LED1Port, LED1);
  GPIOPinTypeGPIOOutput(LED2Port, LED2);
  GPIOPinTypeGPIOOutput(LED3Port, LED3);
  GPIOPinTypeGPIOOutput(LED4Port, LED4);

  // Initialises file system
  clearFileSystem(); // Optional to whip the memory of the server
  initFileSystem();
}

// Main loop function
void loop() {
  connection = "Good";

  if (Serial.available() > 0) {
    // Local variable declarations
    char method[10], path[100], protocol[100];
    char lastchar = 0;
    int lastnl = 0, linecount = 0;
    int length = 8;
    int total_read = 0;
    String lines[40];
    char *buf = (char*)malloc(length);

    total_read = Serial.readBytesUntil('\n', buf, length);

    while (length == total_read) {
      length *= 2;
      buf = (char*)realloc(buf, length);
      total_read += Serial.readBytesUntil('\n', buf + total_read, length - total_read);
    }

    pushToLog(buf);

    if (sscanf(buf, "%[^ ] %[^ ] %[^ ]", method, path, protocol) != 3) {
      checkErrors(1); // Send error code 400
      free(buf);
      return;
    }

    // this puts headers into a char array of char arrays so we can use they later
    String header_buf = String(buf);
    
    for (int i = 0; i < header_buf.length(); i++) {
      if (header_buf.charAt(i) ==  'n' && lastchar == '\\') {
        lines[linecount] = header_buf.substring(lastnl, i - 3);
        lastnl = i + 1;
        linecount++;
      }
      lastchar = header_buf.charAt(i);
    }

    // check for what method we are receiving

    // Catches when the user sends a get request
    if (strcasecmp(method, "get") == 0) {
      char *npath = deblank(path);

      if (strcasecmp(npath, "/") == 0) sendFile("/index.html");
      else sendFile(npath);

      free(buf);
      return;
    }

    // Catches when the user sends a post request
    if (strcasecmp(method, "post") == 0) {
      char *npath = deblank(path);

      if (strcasecmp(npath, "/upload") == 0) {
        int filestart = -1, fileend = -1, first = 0;
        String boundary, filename;
        String contents;

        for (int i = 0; i < 50; i++) {
          int index = lines[i].indexOf("boundary=----");

          if (index != -1 && first == 0) {
            boundary = lines[i].substring(index + 13, lines[i].length() - 1);
            first++;
            continue;
          }
          index = lines[i].indexOf(boundary);

          if (index != -1 && filestart < 0 && first == 1) {
            filestart = i + 4;
            filename = lines[i + 1].substring(lines[i + 1].indexOf("filename=\"") + 10, lines[i + 1].length());
            first++;
            continue;
          }

          if (filestart >= 0 && index != -1 && first == 2) {
            fileend = i - 1;
            break;
          }
        }

        for (int i = filestart; i < fileend; i++)
          contents = contents + "\n" + lines[i];
        for (int i = 0; i < contents.length(); i++) {
          if (contents.charAt(i) == 92) {
            pushToLog("hello");
            contents = contents.substring(0, i - 1) + String("m") + contents.substring(i + 1, contents.length() - 1);
          }
        }

        char *name = (char*)malloc(filename.length());
        filename.toCharArray(name, filename.length());
        pushToLog(name);
        
        if (write_File(name, contents, contents.length()) == 0) {
          sendHeaders(302, "Created", String("Uploaded!").length(), "Good", "-1");
          Serial.println("Uploaded!");
          return;
        }
        
        else {
          sendHTMLerror(304, "Not modified", "The file already exists", "index.html");
          return;
        }
      }
      
      else checkErrors(2); // Send error code 402
      free(buf);
      return;
    }
    
    if (strcasecmp(method, "post") != 0 && strcasecmp(method, "get") != 0) {
      checkErrors(6); // Send error code 501
      free(buf);
    }
    return;
  }
}

