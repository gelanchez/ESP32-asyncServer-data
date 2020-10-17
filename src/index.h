/**
 * @file index.h
 * @author José Ángel Sánchez (https://github.com/gelanchez)
 * @brief HTML with the main page.
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
    <meta name="description" content="ESP32 async webserver data sensor">
    <link rel="icon" href="data:;base64,iVBORw0KGgo=">
    <style>
        html {text-align: center;}
        #title {
            margin: 0px auto;
            text-decoration: underline;
        }
    </style>
</head>
<body>
    <h2 id="title">ESP32 async server with websockets</h2>
    <p>
        <button id="ledButton" onclick="changeLed()" disabled>Toggle LED</button>
        <canvas id="led" width="50" height="50"></canvas>
        <span><b>Sensors: </b>Temperature: <span id="temperature">0</span>°C. Illuminance: <span id="illuminance">0</span> lx.</span>
    </p>
    
    <div class="chart-container" style="position: relative; width:95vw; margin:auto">
        <canvas id="temperatureChart" width="800" height="200" aria-label="Temperature chart" role="img"></canvas>
    </div>
    <br>
    <div class="chart-container" style="position: relative; width:95vw; margin:auto">
        <canvas id="illuminanceChart" width="800" height="200" aria-label="Illuminance chart" role="img"></canvas>
    </div>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.3/Chart.js" integrity="sha512-QEiC894KVkN9Tsoi6+mKf8HaCLJvyA6QIRzY5KrfINXYuP9NxdIkRQhGq3BZi0J4I7V5SidGM3XUQ5wFiMDuWg==" crossorigin="anonymous"></script>
    <script type = "text/javascript">
        var counter = 0;
        var HTMLbutton = document.getElementById("ledButton");
        var led = false;

        // Draw LED
        var canvasLED = document.getElementById("led");
        var contextLED = canvasLED.getContext("2d");
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
                    borderColor: 'yellow',
                    backgroundColor: 'yellow',
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
            let myObj = JSON.parse(event.data);
            let ledStatus = myObj["ledStatus"];
            let temp = myObj["temperature"];
            let illum = myObj["illuminance"];
            if (led != ledStatus) {
                led = ledStatus;
                updateLed();
            }
            updateValues(temp, illum);
            updateCharts(temp, illum);
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
            if (counter < 100){
                counter++;
            }
            else {
                removeData(temperatureChart);
                removeData(illuminanceChart); 
            }   
        }
        
        //window.setInterval(updateValues, 10); // Update values used when testing
        
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