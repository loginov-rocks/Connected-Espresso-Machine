const cors = require('cors');
const express = require('express');

const app = express();
const port = process.env.PORT || 3000;

app.use(cors());

app.get('/', (request, response) => {
  response.
      status(418).
      send('I\'m a teapot');
});

app.get('/state', (request, response) => {
  response.
      send({
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
      });
});

app.post('/command', (request, response) => {
  response.
      status(204).
      send('No Content');
});

app.listen(port, () => {
  console.log(`Test API listening at http://localhost:${port}`);
});
