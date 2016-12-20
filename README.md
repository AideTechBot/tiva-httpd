# tiva-httpd
A web server designed for a Tiva C microcontroller equipped with an Orbit Boosterpack
---
# How to use:
## Run the redirect script
To run the redirect script, do:
`sudo python redirect.py [IP]... [PORT]...`
Where IP is the ip you are listening on and where PORT is the port you want to listen with. For example:
`sudo python redirect.py localhost 80`
You can also use the flag -d to set the device you want to listen to.
## Upload the code to your tiva
And that's it! All you have to do now is go to a web browser and try to access the webpage at:
`http://[IP]:[PORT]/`
Using our previous example:
`http://localhost:80/`
Then you can upload your files to the webserver and enjoy :)
## Credits
This project was coded by [Philip Tang](https://twitter.com/philipwutang) and [Manuel Dionne](https://twitter.com/AideTechBot)
