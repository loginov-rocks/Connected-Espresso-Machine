export class UserInterface {
  /**
   * @param {UserInterfaceDomBuilder} domBuilder
   * @param {Object} options
   */
  constructor(domBuilder, options) {
    this.domBuilder = domBuilder;

    this.containerId = options.containerId;
    this.defaultMakeCoffeeSeconds = options.defaultMakeCoffeeSeconds;
    this.makeCoffeeSecondsStorageKey = options.makeCoffeeSecondsStorageKey;
    this.activeStateClassName = options.activeStateClassName;
  }

  /**
   * @public
   * @param {Object | null} state
   * @returns {void}
   */
  updateState(state) {
    const command = state ? state.command : null;
    this.commandState.textContent = command || 'Unknown';
    this.updateActiveState(this.offButton, command === 'off');
    this.updateActiveState(this.pourWaterButton, command === 'pourWater');
    this.updateActiveState(this.stopPouringWaterButton, command === 'stopPouringWater');
    this.updateActiveState(this.boilButton, command === 'boil');
    this.updateActiveState(this.makeSteamButton, command === 'makeSteam');
    this.updateActiveState(this.coolDownButton, command === 'coolDown');
    this.updateActiveState(this.makeCoffeeButton, command === 'makeCoffee');

    if (state && state.isCommandChanged) {
      // Blink for 1 second if the command changed.
      this.updateActiveState(this.commandState, true);
      setTimeout(() => this.updateActiveState(this.commandState, false), 1000);
    }

    this.updateActiveState(this.doneState, state && state.isDone);

    this.updateActiveState(this.pumpState, state && state.components.pump);

    this.updateActiveState(this.boilerState, state && state.components.boiler);

    const toggleState = state ? state.components.toggleState : null;
    this.updateActiveState(this.toggleMakeSteamState, toggleState === 'makeSteam');
    this.updateActiveState(this.toggleOffState, toggleState === 'off');
    this.updateActiveState(this.toggleBoilState, toggleState === 'boil');
    this.updateActiveState(this.togglePourWaterState, toggleState === 'pourWater');

    // Enable or disable buttons based on the toggle state.
    if (toggleState === null || ['makeSteam', 'boil', 'pourWater'].includes(toggleState)) {
      this.buttons.forEach((button) => button.setAttribute('disabled', true));
    } else {
      this.buttons.forEach((button) => button.removeAttribute('disabled'));
    }
  }

  /**
   * @public
   * @param {(command: string, parameters: Object = {}) => void} listener
   * @returns {void}
   */
  onCommandRequest(listener) {
    this.commandRequestListener = listener;
  }

  /**
   * @public
   * @returns {void}
   */
  build() {
    // Build DOM.
    const container = document.getElementById(this.containerId);
    this.domBuilder.build(container);

    // Get inputs variables from local storage or use defaults.
    const makeCoffeeSeconds = localStorage.getItem(this.makeCoffeeSecondsStorageKey) || this.defaultMakeCoffeeSeconds;

    // Get references to HTML inputs.
    const makeCoffeeSecondsInput = document.getElementById('makeCoffeeSeconds');
    // Label for makeCoffeeSecondsInput.
    const makeCoffeeSecondsLabel = document.getElementById('makeCoffeeSecondsLabel');

    // Update inputs values with initialized variables.
    makeCoffeeSecondsInput.setAttribute('value', makeCoffeeSeconds);

    // Attach listeners to inputs.
    makeCoffeeSecondsInput.onchange = this.handleMakeCoffeeSecondsChange(makeCoffeeSecondsLabel).bind(this);
    // Additional listener to update the label as a mouse moves over the range.
    makeCoffeeSecondsInput.onmousemove = makeCoffeeSecondsInput.onchange;

    // Trigger listeners to react to initialized variables.
    makeCoffeeSecondsInput.onchange({target: {value: makeCoffeeSeconds}});

    // Enable inputs.
    makeCoffeeSecondsInput.removeAttribute('disabled');

    // Get references to HTML buttons.
    this.offButton = document.getElementById('off');
    this.pourWaterButton = document.getElementById('pourWater');
    this.stopPouringWaterButton = document.getElementById('stopPouringWater');
    this.boilButton = document.getElementById('boil');
    this.makeSteamButton = document.getElementById('makeSteam');
    this.coolDownButton = document.getElementById('coolDown');
    this.makeCoffeeButton = document.getElementById('makeCoffee');

    // Attach listeners to buttons.
    this.offButton.onclick = () => this.handleCommand('off');
    this.pourWaterButton.onclick = () => this.handleCommand('pourWater');
    this.stopPouringWaterButton.onclick = () => this.handleCommand('stopPouringWater');
    this.boilButton.onclick = () => this.handleCommand('boil');
    this.makeSteamButton.onclick = () => this.handleCommand('makeSteam');
    this.coolDownButton.onclick = () => this.handleCommand('coolDown');
    this.makeCoffeeButton.onclick = () => {
      this.handleCommand('makeCoffee', {seconds: makeCoffeeSecondsInput.value});
    };

    // Keep references to buttons to react to state updates.
    this.buttons = [
      this.offButton,
      this.pourWaterButton,
      this.stopPouringWaterButton,
      this.boilButton,
      this.makeSteamButton,
      this.coolDownButton,
      this.makeCoffeeButton,
    ];

    // Get and keep references to HTML elements representing various states.
    this.commandState = document.getElementById('commandState');
    this.doneState = document.getElementById('doneState');
    this.pumpState = document.getElementById('pumpState');
    this.boilerState = document.getElementById('boilerState');
    this.toggleMakeSteamState = document.getElementById('toggleMakeSteamState');
    this.toggleOffState = document.getElementById('toggleOffState');
    this.toggleBoilState = document.getElementById('toggleBoilState');
    this.togglePourWaterState = document.getElementById('togglePourWaterState');
  }

  /**
   * @private
   * @param {HTMLElement} element
   * @param {boolean} isActive
   * @returns {void}
   */
  updateActiveState(element, isActive) {
    if (isActive) {
      element.classList.add(this.activeStateClassName);
    } else {
      element.classList.remove(this.activeStateClassName);
    }
  }

  /**
   * @private
   * @param {HTMLElement} labelElement
   * @returns {(event: Event) => void}
   */
  handleMakeCoffeeSecondsChange(labelElement) {
    return (event) => {
      const makeCoffeeSeconds = event.target.value;
      localStorage.setItem(this.makeCoffeeSecondsStorageKey, makeCoffeeSeconds);

      labelElement.textContent = `${makeCoffeeSeconds}"`;
    };
  }

  /**
   * @private
   * @param {string} command
   * @param {Object} [parameters={}]
   * @returns {void}
   */
  handleCommand(command, parameters = {}) {
    if (this.commandRequestListener) {
      this.commandRequestListener(command, parameters);
    }
  }
}
