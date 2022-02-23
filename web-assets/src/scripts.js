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
  activeStateClassName: 'active',
});

businessLogic.onStateUpdate((state) => userInterface.updateState(state));

userInterface.onCommandRequest((command, parameters) => businessLogic.requestCommand(command, parameters));

window.addEventListener('DOMContentLoaded', () => {
  userInterface.build();
  businessLogic.start();
});
