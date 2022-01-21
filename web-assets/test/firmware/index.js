const cors = require('cors');
const express = require('express');

const WEB_ASSETS_PORT = 3001;
const PORT = 3002;

const app = express();

const state = {
  command: 'off',
  isCommandChanged: false,
  isDone: false,
  makeCoffeeMillisLeft: 0,
  components: {
    pump: false,
    boiler: false,
    boilerTemp: 'cold',
    boilerTargetTemp: 'cold',
    toggleState: 'off',
  },
};

app.use(cors());

app.get('/', (request, response) => {
  response.
      status(200).
      send(`<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8" />
<meta content="width=device-width,initial-scale=1" name="viewport" />
<meta content="#5d4037" name="theme-color" />
<title>Connected Espresso Machine</title>
<script src="http://localhost:${WEB_ASSETS_PORT}/scripts.js" type="module"></script>
<link href="http://localhost:${WEB_ASSETS_PORT}/styles.css" rel="stylesheet" />
</head>
<body>
<div id="app" class="app loading">Loading...</div>
</body>
</html>`);
});

app.get('/state', (request, response) => {
  response.
      send(state);

  // Simple simulation of RestEspressoMachine behavior.
  state.isCommandChanged = false;
});

app.post('/command', (request, response) => {
  // Simple simulation of RestEspressoMachine behavior.
  state.isCommandChanged = true;
  state.command = request.query.command;
  state.components.boiler = ['boil', 'makeSteam', 'makeCoffee'].includes(state.command);
  state.components.pump = ['pourWater', 'makeCoffee'].includes(state.command);

  response.
      status(204).
      send('No Content');
});

app.listen(PORT, () => {
  console.log(`Test API listening at http://localhost:${PORT}`);
});
