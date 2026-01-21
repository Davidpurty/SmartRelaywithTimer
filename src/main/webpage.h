#pragma once

const char WEBPAGE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Smart Home Controls</title>

  <style>
    body {
      background: linear-gradient(135deg, #2b2e6b, #5b4db3);
      font-family: Arial, sans-serif;
      color: white;
      text-align: center;
    }

    h2 {
      margin-top: 20px;
    }

    .grid {
      display: grid;
      grid-template-columns: repeat(2, 1fr);
      gap: 20px;
      padding: 20px;
    }

    .card {
      background: rgba(255,255,255,0.15);
      padding: 15px;
      border-radius: 15px;
    }

    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0; left: 0; right: 0; bottom: 0;
      background-color: #999;
      transition: .4s;
      border-radius: 34px;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      transition: .4s;
      border-radius: 50%;
    }

    input:checked + .slider {
      background-color: #4CAF50;
    }

    input:checked + .slider:before {
      transform: translateX(26px);
    }
  </style>
</head>

<body>
  <h2>Smart Home Controls</h2>

  <div class="grid">
    <div class="card">Wi-Fi<br><label class="switch">
      <input type="checkbox" id="wifi"><span class="slider"></span>
    </label></div>

    <div class="card">Bluetooth<br><label class="switch">
      <input type="checkbox" id="bt"><span class="slider"></span>
    </label></div>

    <div class="card">Lights 3<br><label class="switch">
      <input type="checkbox" id="light"><span class="slider"></span>
    </label></div>

    <div class="card">Audio<br><label class="switch">
      <input type="checkbox" id="audio"><span class="slider"></span>
    </label></div>

    <div class="card">Dark Mode<br><label class="switch">
      <input type="checkbox" id="dark"><span class="slider"></span>
    </label></div>

    <div class="card">Lock 6<br><label class="switch">
      <input type="checkbox" id="lock"><span class="slider"></span>
    </label></div>

    <div class="card">Cloud Sync 7<br><label class="switch">
      <input type="checkbox" id="cloud"><span class="slider"></span>
    </label></div>

    <div class="card">Power<br><label class="switch">
      <input type="checkbox" id="power"><span class="slider"></span>
    </label></div>
  </div>

<script>
const controls = ["wifi","bt","light","audio","dark","lock","cloud","power"];

function sendUpdate(id, value) {
  fetch(`/set?key=${id}&value=${value}`);
}

controls.forEach(id => {
  const el = document.getElementById(id);
  el.addEventListener("change", () => {
    sendUpdate(id, el.checked ? 1 : 0);
  });
});

fetch("/state")
  .then(res => res.json())
  .then(data => {
    controls.forEach(id => {
      document.getElementById(id).checked = data[id];
    });
  });
</script>

</body>
</html>
)rawliteral";
