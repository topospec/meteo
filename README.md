# meteoAR :partly_sunny:

## Weather Station based in Arduino

This weather station was built from scratch, to use it in a astronomic observatory.

![alt text](https://raw.githubusercontent.com/caparrosmatias/meteo/master/images/foto_exterior_less.jpg)

Later a .csv datafile was built with the meteo info to train a model for machine learning experiments (in progress..)

It compiles information from different sensors (LM35, DHT11, BMP180, rain sensor) and save it in a SQL Database via PHP, using the arduino ethernet module.

You can find the sources each in /arduino and /web 

