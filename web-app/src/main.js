import {Api} from './Api.js';
import {BusinessLogic} from './BusinessLogic.js';
import {UserInterface} from './UserInterface.js';

const api = new Api({
  getStateTimeout: 1000,
});

const businessLogic = new BusinessLogic(api, {
  pollingInterval: 1000,
});

const userInterface = new UserInterface({
  defaultLocalIp: '192.168.1.1',
  defaultHttpPort: '80',
  defaultMakeCoffeeSeconds: 30,
  localIpStorageKey: 'localIp',
  httpPortStorageKey: 'httpPort',
  makeCoffeeSecondsStorageKey: 'makeCoffeeSeconds',
});

businessLogic.onStateUpdate((state) => userInterface.updateState(state));

userInterface.onLocalIpChange((localIp) => businessLogic.setLocalIp(localIp));

userInterface.onHttpPortChange((httpPort) => businessLogic.setHttpPort(httpPort));

userInterface.onRequestCommand((command, parameters) => businessLogic.requestCommand(command, parameters));

window.addEventListener('DOMContentLoaded', () => {
  // Initialize User Interface first to provide initialized variables.
  userInterface.initialize();

  businessLogic.initialize();
});

$(function() {

  function refresh(json) {
    $('.lastCommand').removeClass('lastCommand');
    $('#' + json.lastCommand).addClass('lastCommand');

    if (json.pumpState) {
      $('.currentPump').addClass('working');
    } else {
      $('.currentPump').removeClass('working');
    }

    if (json.boilerState) {
      $('.currentBoiler').addClass('working');
    } else {
      $('.currentBoiler').removeClass('working');
    }

    var currentButton = '';

    switch (json.boilerStage) {
      case 'steam':
        currentButton = 'makeSteam';
        break;
      case 'hot':
        currentButton = 'heatWater';
        break;
      default:
        currentButton = 'coolDown';
        break;
    }

    $('.boiler .current').removeClass('current');
    $('.boiler #' + currentButton).addClass('current');

    var targetButton = '';

    switch (json.boilerTargetStage) {
      case 'steam':
        targetButton = 'makeSteam';
        break;
      case 'hot':
        targetButton = 'heatWater';
        break;
      default:
        targetButton = 'coolDown';
        break;
    }

    $('.boiler .target').removeClass('target');
    $('.boiler #' + targetButton).addClass('target');
  }

});
