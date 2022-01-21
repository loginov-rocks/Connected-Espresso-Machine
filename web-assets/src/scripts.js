import {Api} from './scripts/Api.js';
import {BusinessLogic} from './scripts/BusinessLogic.js';
import {UserInterface} from './scripts/UserInterface.js';
import {UserInterfaceDomBuilder} from './scripts/UserInterfaceDomBuilder.js';

const api = new Api({
  getStateTimeout: 1000,
});

const businessLogic = new BusinessLogic(api, {
  pollingInterval: 1000,
});

const userInterfaceDomBuilder = new UserInterfaceDomBuilder();

const userInterface = new UserInterface(userInterfaceDomBuilder, {
  containerId: 'app',
  defaultMakeCoffeeSeconds: 30,
  makeCoffeeSecondsStorageKey: 'makeCoffeeSeconds',
  activeStateClassName: 'activeState',
});

businessLogic.onStateUpdate((state) => userInterface.updateState(state));

userInterface.onCommandRequest((command, parameters) => businessLogic.requestCommand(command, parameters));

window.addEventListener('DOMContentLoaded', () => {
  userInterface.build();
  businessLogic.start();
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
