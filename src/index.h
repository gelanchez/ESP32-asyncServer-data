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
    <h3>LED</h3>
    <h3>Sensors</h3>
    <p>Temperature: <span id="temperature">0</span>°C. Illuminance: <span id="illuminance">0</span> lx</p>
    
    <div class="chart-container" style="position: relative; height:40vh; width:80vw">
        <canvas id="temperatureChart" width="800" height="200" aria-label="Temperature chart" role="img"></canvas>
    </div>
    <br>
    <div class="chart-container" style="position: relative; height:40vh; width:80vw">
        <canvas id="illuminanceChart" width="800" height="200" aria-label="Illuminance chart" role="img"></canvas>
    </div>

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
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.3/Chart.js" integrity="sha512-QEiC894KVkN9Tsoi6+mKf8HaCLJvyA6QIRzY5KrfINXYuP9NxdIkRQhGq3BZi0J4I7V5SidGM3XUQ5wFiMDuWg==" crossorigin="anonymous"></script>
    <script>

        webSocket = new WebSocket("ws://10.42.0.190/ws");
        webSocket.onmessage = function (event) {
            let myObj = JSON.parse(event.data);
            let temp = myObj["temperature"];
            let illum = myObj["illuminance"];
            updateValues(temp, illum);
            updateCharts(temp, illum);
        }
        
        var counter = 0;
        
        function updateValues(temperature, illuminance) {
            //temperature = Math.floor(Math.random() * 100); // Testing
            //illuminance = Math.floor(Math.random() * 100); // Testing
            document.getElementById("temperature").innerHTML = temperature;
            document.getElementById("illuminance").innerHTML = illuminance;
        }
        
        function updateCharts(temperature, illuminance) {
            let date  = new Date();
            date.getSeconds();
            addData(temperatureChart, 1, [temperature]);
            addData(illuminanceChart, 1, [illuminance]);
            if (counter < 1000){
                counter++;
            }
            else {
                removeData(temperatureChart);
                removeData(illuminanceChart); 
            }   
        }
        
        //window.setInterval(updateValues, 10); // Update values used when testing
        
        var ctxTemp = document.getElementById('temperatureChart').getContext('2d');
        var temperatureChart = new Chart(ctxTemp, {
            type: 'line',
            data: {
                //labels: [1, 2, 3],
                datasets: [{
                    label: 'Temperature',
                    //data: [12, 19, 3],
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
                        display: false,  // this will remove all the x-axis grid lines
                        ticks: {
                            display: false  // this will remove only the label
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
                showLines: false,  // disable for all datasets
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
                        display: false,  // this will remove all the x-axis grid lines
                        ticks: {
                            display: false  // this will remove only the label
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
                showLines: false,  // disable for all datasets
                animation: {
                    duration: 0  // general animation time
                },
                hover: {
                    animationDuration: 0  // duration of animations when hovering an item
                },
                responsiveAnimationDuration: 0  // animation duration after a resize
            }
        });

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