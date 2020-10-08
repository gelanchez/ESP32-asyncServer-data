/**
 * @file index.h
 * @author José Ángel Sánchez (https://github.com/gelanchez)
 * @brief HTML with the main pages.
 * @version 0.0.1
 * @date 2020-09-29
 * @copyright GPL-3.0
 */

#ifndef INDEX_H
#define INDEX_H

/**
 * @brief Main html page.
 */
const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">

<head>
    <title>ESP32-asyncServer-data</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="author" content="José Ángel">
    <meta name="description" content="ESP32 async webserver data sensor, proof of concept">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css"
        integrity="sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z" crossorigin="anonymous">
</head>

<body>
    <h2>ESP32 data</h2>
    <p>Temperature: <span id="temperature">0</span>°C. Illuminance: <span id="illuminance">0</span> lux</p>
    <div class="chart-container" style="position: relative; height:40vh; width:80vw">
        <canvas id="temperatureChart" height="200" width="800" aria-label="Temperature chart" role="img"></canvas>
    </div>
    <br>
    <div class="chart-container" style="position: relative; height:40vh; width:80vw">
        <canvas id="illuminanceChart" height="200" width="800" aria-label="Illuminance chart" role="img"></canvas>
    </div>
    <canvas id="myChart" width="400" height="400"></canvas>

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <!-- Not using the jQuery slim version as it doesn't have AJAX-->
    <script src="https://code.jquery.com/jquery-3.5.1.min.js"
        integrity="sha256-9/aliU8dGd2tb6OSsuzixeV4y/faTqgFtohetphbbj0="
        crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js"
        integrity="sha384-9/reFTGAW83EW2RDu2S0VKaIzap3H66lZH81PoYlFhbGU+6BZp6G7niu735Sk7lN"
        crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"
        integrity="sha384-B4gt1jrGC7Jh4AgTPSdUtOBvfO8shuf57BaghqFfPlYxofvL8/KUEfYiJOMMV+rV"
        crossorigin="anonymous"></script>
    
    <!-- JS -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.3/Chart.min.js" integrity="sha512-s+xg36jbIujB2S2VKfpGmlC3T5V2TF3lY48DX7u2r9XzGzgPsa6wTpOQA7J9iffvdeBN0q9tKzRxVxw1JviZPg==" crossorigin="anonymous"></script>
    <script>
        webSocket = new WebSocket("ws://10.42.0.190/ws");
        webSocket.onmessage = function (event)
        {
            let myObj = JSON.parse(event.data);
            let temp = myObj["temperature"];
            let illum = myObj["illuminance"];
            updateValues(temp, illum);
            updateCharts(temp, illum);
        }

        function updateValues(temperature, illuminance)
        {
            //temperature = Math.floor(Math.random() * 100); // Testing
            //illuminance = Math.floor(Math.random() * 100); // Testing
            document.getElementById("temperature").innerHTML = temperature;
            document.getElementById("illuminance").innerHTML = illuminance;

        }

        function updateCharts(temperature, illuminance)
        {
            
        }
    </script>
</body>

</html>


)=====";

#endif