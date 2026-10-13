"use strict";

const SEND_INTERVAL_MS = 50;
const RECONNECT_MS = 2000;
const MOCK_SENSOR_MS = 200;
const MAX_DISTANCE_CM = 200; // distance bar full scale
const KEY_STEP_DEG = 5;

// Mock mode lets the UI be developed without hardware: open index.html with ?mock
// (or from file://) and the ESP32 is simulated in the browser.
const MOCK = new URLSearchParams(location.search).has("mock") || location.protocol === "file:";

const statusEl = document.getElementById("status");
const distanceEl = document.getElementById("distance");
const distanceBar = document.getElementById("distanceBar");
const panEl = document.getElementById("panVal");
const tiltEl = document.getElementById("tiltVal");
const panSlider = document.getElementById("panSlider");
const tiltSlider = document.getElementById("tiltSlider");
const joystick = document.getElementById("joystick");
const knob = document.getElementById("knob");

let socket = null;
let pan = 90;
let tilt = 90;
let dirty = false;

function setStatus(connected) {
  statusEl.textContent = connected ? "Conectado" : "Desconectado";
  statusEl.className = "status " + (connected ? "on" : "off");
}

function showDistance(cm) {
  if (cm < 0) {
    distanceEl.textContent = "--";
    distanceBar.style.width = "0%";
    return;
  }
  distanceEl.textContent = cm.toFixed(1);
  const pct = Math.min(cm / MAX_DISTANCE_CM, 1) * 100;
  distanceBar.style.width = `${pct}%`;
  distanceBar.className = cm < 15 ? "danger" : cm < 40 ? "warn" : "";
}

function handleMessage(data) {
  if (data.startsWith("D:")) showDistance(parseFloat(data.slice(2)));
}

function connect() {
  socket = new WebSocket(`ws://${location.host}/ws`);
  socket.onopen = () => setStatus(true);
  socket.onclose = () => {
    setStatus(false);
    setTimeout(connect, RECONNECT_MS);
  };
  socket.onmessage = (e) => handleMessage(e.data);
}

function startMock() {
  document.getElementById("mockBadge").hidden = false;
  setStatus(true);
  let t = 0;
  setInterval(() => {
    t += 0.15;
    const cm = 100 + 80 * Math.sin(t); // simulated obstacle moving back and forth
    handleMessage(`D:${cm.toFixed(1)}`);
  }, MOCK_SENSOR_MS);
}

function send(message) {
  if (MOCK) {
    console.log("[mock] ->", message);
  } else if (socket && socket.readyState === WebSocket.OPEN) {
    socket.send(message);
  }
}

function setServos(newPan, newTilt, source) {
  pan = Math.min(180, Math.max(0, Math.round(newPan)));
  tilt = Math.min(180, Math.max(0, Math.round(newTilt)));
  panEl.textContent = pan;
  tiltEl.textContent = tilt;
  panSlider.value = pan;
  tiltSlider.value = tilt;
  if (source !== "joystick") moveKnob(pan, tilt);
  dirty = true;
}

// Knob position follows the current angles (0..180 -> -radius..radius).
function moveKnob(panDeg, tiltDeg) {
  const radius = joystick.clientWidth / 2;
  const x = (panDeg / 90 - 1) * radius;
  const y = -(tiltDeg / 90 - 1) * radius;
  knob.style.left = `${radius + x - knob.offsetWidth / 2}px`;
  knob.style.top = `${radius + y - knob.offsetHeight / 2}px`;
}

function updateFromPointer(clientX, clientY) {
  const rect = joystick.getBoundingClientRect();
  const radius = rect.width / 2;
  let x = clientX - rect.left - radius;
  let y = clientY - rect.top - radius;
  const dist = Math.hypot(x, y);
  if (dist > radius) {
    x = (x / dist) * radius;
    y = (y / dist) * radius;
  }
  // X -> pan, Y (up = increase) -> tilt.
  setServos(((x / radius) + 1) * 90, ((-y / radius) + 1) * 90, "joystick");
  knob.style.left = `${radius + x - knob.offsetWidth / 2}px`;
  knob.style.top = `${radius + y - knob.offsetHeight / 2}px`;
}

let dragging = false;
joystick.addEventListener("pointerdown", (e) => {
  dragging = true;
  joystick.setPointerCapture(e.pointerId);
  updateFromPointer(e.clientX, e.clientY);
});
joystick.addEventListener("pointermove", (e) => {
  if (dragging) updateFromPointer(e.clientX, e.clientY);
});
joystick.addEventListener("pointerup", () => { dragging = false; });

document.getElementById("centerBtn").addEventListener("click", () => setServos(90, 90));
panSlider.addEventListener("input", () => setServos(panSlider.value, tilt));
tiltSlider.addEventListener("input", () => setServos(pan, tiltSlider.value));

document.addEventListener("keydown", (e) => {
  const moves = {
    ArrowLeft: [-KEY_STEP_DEG, 0],
    ArrowRight: [KEY_STEP_DEG, 0],
    ArrowUp: [0, KEY_STEP_DEG],
    ArrowDown: [0, -KEY_STEP_DEG],
  };
  if (e.code === "Space") {
    e.preventDefault();
    setServos(90, 90);
  } else if (moves[e.code]) {
    e.preventDefault();
    setServos(pan + moves[e.code][0], tilt + moves[e.code][1]);
  }
});

// Throttled sender: avoids flooding the ESP32.
setInterval(() => {
  if (dirty) {
    send(`S:${pan},${tilt}`);
    dirty = false;
  }
}, SEND_INTERVAL_MS);

moveKnob(pan, tilt);
if (MOCK) startMock(); else connect();
