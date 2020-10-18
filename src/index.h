/**
 * @file index.h
 * @author José Ángel Sánchez (https://github.com/gelanchez)
 * @brief HTML with the main page.
 * @version 0.0.2
 * @date 2020-10-17
 * @copyright GPL-3.0
 */

#ifndef INDEX_H
#define INDEX_H

/**
 * @brief main.html, main.js and styles.css in one char array.
 */
const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
    <title>ESP32-asyncServer-data</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="ESP32 async webserver data sensor">
    <meta name="author" content="José Ángel Sánchez">
    <link rel="icon" href="data:;base64,iVBORw0KGgo=">
    <style>
        html {
            text-align: center;
            font-family: Arial, Helvetica, sans-serif;
        }
        #title {
            margin: 0px auto;
            text-decoration: underline;
        }
        #data { margin: 5px auto;}
        #led { margin-bottom: -20px;}
    </style>
</head>
<body>
    <h2 id="title">ESP32 async server with websockets</h2>
    <p id="data">
        <button id="ledButton" onclick="changeLed()" disabled>Toggle LED</button>
        <canvas id="led" width="50" height="50"></canvas>
        <span>SENSORS: Temperature: </span><span id="temperature">0</span><span>°C. Illuminance: </span><span
            id="illuminance">0</span><span>lx.</span>
    </p>

    <div class="chart-container" style="position: relative; width:95vw; margin:auto">
        <canvas id="temperatureChart" width="800" height="200" aria-label="Temperature chart" role="img"></canvas>
    </div>
    <br>
    <div class="chart-container" style="position: relative; width:95vw; margin:auto">
        <canvas id="illuminanceChart" width="800" height="200" aria-label="Illuminance chart" role="img"></canvas>
    </div>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.3/Chart.js"
        integrity="sha512-QEiC894KVkN9Tsoi6+mKf8HaCLJvyA6QIRzY5KrfINXYuP9NxdIkRQhGq3BZi0J4I7V5SidGM3XUQ5wFiMDuWg=="
        crossorigin="anonymous"></script>
    <script type="text/javascript">
        var counter = 0;
        var HTMLbutton = document.getElementById("ledButton");
        var led = false;

        // Draw LED
        var contextLED = document.getElementById("led").getContext("2d");
        contextLED.arc(25, 25, 15, 0, Math.PI * 2, false);
        contextLED.lineWidth = 3;
        contextLED.strokeStyle = "black";
        contextLED.fillStyle = "black";
        contextLED.stroke();
        contextLED.fill();

        var ctxTemp = document.getElementById('temperatureChart').getContext('2d');
        var temperatureChart = new Chart(ctxTemp, {
            type: 'line',
            data: {
                //labels: [1, 2, 3],
                datasets: [{
                    label: 'Temperature',
                    borderColor: 'red',
                    backgroundColor: 'red',
                    borderWidth: 2,
                    pointRadius: 1,
                    fill: false
                }]
            },
            options: {
                legend: {
                    display: false
                },  
                responsive: true,
                scales: {
                    xAxes: [{
						display: true,
                        ticks: {
                            display: true
                        }
                    }],
                    yAxes: [{
                        display: true,
						scaleLabel: {
							display: true,
							labelString: 'Temperature (ºC)'
                        },
                        ticks: {
                            min: 0,
                            max: 40
                        }
					}]
                },
                showLines: true,
                elements: {
                    line: {
                        tension: 0  // disables bezier curves
                    }
                },
                animation: {
                    duration: 0  // general animation time
                },
                hover: {
                    animationDuration: 0  // duration of animations when hovering an item
                },
                responsiveAnimationDuration: 0  // animation duration after a resize
            }
        });

        var ctxIllum = document.getElementById('illuminanceChart').getContext('2d');
        var illuminanceChart = new Chart(ctxIllum, {
            type: 'line',
            data: {
                datasets: [{
                    label: 'Illuminance',
                    borderColor: 'gold',
                    backgroundColor: 'gold',
                    borderWidth: 2,
                    pointRadius: 1,
                    fill: false
                }]
            },
            options: {
                legend: {
                    display: false
                },                
                responsive: true,
                scales: {
                    xAxes: [{
                        display: true,
                        ticks: {
                            display: true
                        }
                    }],
                    yAxes: [{
						display: true,
						scaleLabel: {
							display: true,
							labelString: 'Illuminance (lux)'
						},
                        ticks: {
                            min: 0,
                            max: 10000
                        }
					}]
                },
                showLines: true,
                animation: {
                    duration: 0  // general animation time
                },
                hover: {
                    animationDuration: 0  // duration of animations when hovering an item
                },
                responsiveAnimationDuration: 0  // animation duration after a resize
            }
        });

        var webSocket = new WebSocket("ws://10.42.0.190/ws");
        webSocket.onopen = function (event) {
            HTMLbutton.disabled = false;
        }
        webSocket.onclose = function (event) {
            HTMLbuton.disabled = true;
        }
        webSocket.onmessage = function (event) {
            let jsonObj = JSON.parse(event.data);
            // LED change
            if ((jsonObj["ledStatus"] != undefined) && (jsonObj["ledStatus"] != led)) {
                led = jsonObj["ledStatus"];
                updateLed();
            }
            // Sensors update
            else {
                let temp = jsonObj["temperature"];
                let illum = jsonObj["illuminance"];
                updateValues(temp, illum);
                updateCharts(temp, illum);
            }
        }

        function changeLed() {
            webSocket.send("C");
        }
        
        function updateLed() {
            if (led){
                contextLED.fillStyle = "red";
                contextLED.fill();
            }
            else {
                contextLED.fillStyle = "black";
                contextLED.fill();
            }
        }

        //window.setInterval(updateValues, 10); // Update values used when testing

        function updateValues(temperature, illuminance) {
            //temperature = Math.floor(Math.random() * 100); // Testing
            //illuminance = Math.floor(Math.random() * 100); // Testing
            document.getElementById("temperature").innerHTML = temperature;
            document.getElementById("illuminance").innerHTML = illuminance;
        }
  
        function updateCharts(temperature, illuminance) {
            let date  = new Date();
            let timeDislpayed = date.getMinutes().toString().padStart(2, '0') + ":" + date.getSeconds().toString().padStart(2, '0');
            addData(temperatureChart, timeDislpayed, [temperature]);
            addData(illuminanceChart, timeDislpayed, [illuminance]);
            // Remove values from chart after 100 data
            if (counter < 100){
                counter++;
            }
            else {
                removeData(temperatureChart);
                removeData(illuminanceChart); 
            }   
        }
                
        function addData(chart, label, data) {
            chart.data.labels.push(label);
            chart.data.datasets.forEach((dataset) => {
                dataset.data.push(data);
            });
            chart.update();
        }

        function removeData(chart) {
            chart.data.labels.shift();
            chart.data.datasets.forEach((dataset) => {
                dataset.data.shift();
            });
            chart.update();
        }
    </script>
</body>
</html>

)=====";

#endif