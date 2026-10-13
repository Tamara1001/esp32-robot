"use strict";

const SEND_INTERVAL_MS = 50;
const RECONNECT_MS = 2000;

const statusEl = document.getElementById("status");
const distanceEl = document.getElementById("distance");
const panEl = document.getElementById("panVal");
const tiltEl = document.getElementById("tiltVal");
const joystick = document.getElementById("joystick");
const knob = document.getElementById("knob");

let socket = null;
let pan = 90;
let tilt = 90;
let dirty = false;

function connect() {
  socket = new WebSocket(`ws://${location.host}/ws`);
  socket.onopen = () => setStatus(true);
  socket.onclose = () => {
    setStatus(false);
    setTimeout(connect, RECONNECT_MS);
  };
  socket.onmessage = (e) => {
    if (e.data.startsWith("D:")) {
      const cm = parseFloat(e.data.slice(2));
      distanceEl.textContent = cm < 0 ? "--" : cm.toFixed(1);
    }
  };
}

function setStatus(connected) {
  statusEl.textContent = connected ? "Conectado" : "Desconectado";
  statusEl.className = "status " + (connected ? "on" : "off");
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
  knob.style.left = `${radius + x - knob.offsetWidth / 2}px`;
  knob.style.top = `${radius + y - knob.offsetHeight / 2}px`;

  // X -> pan, Y (up = increase) -> tilt. Map [-radius, radius] to [0, 180].
  pan = Math.round(((x / radius) + 1) * 90);
  tilt = Math.round(((-y / radius) + 1) * 90);
  panEl.textContent = pan;
  tiltEl.textContent = tilt;
  dirty = true;
}

function center() {
  const rect = joystick.getBoundingClientRect();
  updateFromPointer(rect.left + rect.width / 2, rect.top + rect.height / 2);
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
document.getElementById("centerBtn").addEventListener("click", center);

// Throttled sender: avoids flooding the ESP32.
setInterval(() => {
  if (dirty && socket && socket.readyState === WebSocket.OPEN) {
    socket.send(`S:${pan},${tilt}`);
    dirty = false;
  }
}, SEND_INTERVAL_MS);

connect();
