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
    if (led) {
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
    let date = new Date();
    let timeDislpayed = date.getMinutes().toString().padStart(2, '0') + ":" + date.getSeconds().toString().padStart(2, '0');
    addData(temperatureChart, timeDislpayed, [temperature]);
    addData(illuminanceChart, timeDislpayed, [illuminance]);
    // Remove values from chart after 100 data
    if (counter < 100) {
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